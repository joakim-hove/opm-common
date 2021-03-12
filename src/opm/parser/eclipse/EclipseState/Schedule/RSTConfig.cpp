/*
  Copyright 2021 Equinor ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <optional>
#include <fmt/format.h>

#include <opm/parser/eclipse/Parser/ErrorGuard.hpp>
#include <opm/parser/eclipse/Parser/ParseContext.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/RSTConfig.hpp>
#include <opm/parser/eclipse/Parser/ParserKeywords/R.hpp>
#include <opm/parser/eclipse/Utility/Functional.hpp>
#include <opm/common/utility/OpmInputError.hpp>


namespace Opm {

namespace {

inline bool is_int( const std::string& x ) {
    auto is_digit = []( char c ) { return std::isdigit( c ); };

    return !x.empty()
        && ( x.front() == '-' || is_digit( x.front() ) )
        && std::all_of( x.begin() + 1, x.end(), is_digit );
}

constexpr const char* RSTIntegerKeywords[] = { "BASIC",      //  1
                                               "FLOWS",      //  2
                                               "FIP",        //  3
                                               "POT",        //  4
                                               "PBPD",       //  5
                                               "FREQ",       //  6
                                               "PRES",       //  7
                                               "VISC",       //  8
                                               "DEN",        //  9
                                               "DRAIN",      // 10
                                               "KRO",        // 11
                                               "KRW",        // 12
                                               "KRG",        // 13
                                               "PORO",       // 14
                                               "NOGRAD",     // 15
                                               "NORST",      // 16 NORST - not supported
                                               "SAVE",       // 17
                                               "SFREQ",      // 18 SFREQ=?? - not supported
                                               "ALLPROPS",   // 19
                                               "ROCKC",      // 20
                                               "SGTRAP",     // 21
                                               "",           // 22 - Blank - ignored.
                                               "RSSAT",      // 23
                                               "RVSAT",      // 24
                                               "GIMULT",     // 25
                                               "SURFBLK",    // 26
                                               "",           // 27 - PCOW, PCOG, special cased
                                               "STREAM",     // 28 STREAM=?? - not supported
                                               "RK",         // 29
                                               "VELOCITY",   // 30
                                               "COMPRESS" }; // 31

bool is_RPTRST_mnemonic( const std::string& kw ) {
    /* all eclipse 100 keywords we want to not simply ignore. The list is
     * sorted, so we can use binary_search for log(n) lookup. It is important
     * that the list is sorted, but these are all the keywords listed in the
     * manual and unlikely to change at all
     */
    static constexpr const char* valid[] = {
        "ACIP",     "ACIS",     "ALLPROPS", "BASIC",  "BG",       "BO",
        "BW",       "CELLINDX", "COMPRESS", "CONV",   "DEN",      "DRAIN",
        "DRAINAGE", "DYNREG",   "FIP",      "FLORES", "FLOWS",    "FREQ",
        "GIMULT",   "HYDH",     "HYDHFW",   "KRG",    "KRO",      "KRW",
        "NOGRAD",   "NORST",    "NPMREB",   "PBPD",   "PCOG",     "PCOW",
        "PERMREDN", "POIS",     "PORO",     "PORV",   "POT",      "PRES",
        "RFIP",     "RK",       "ROCKC",    "RPORV",  "RSSAT",    "RVSAT",
        "SAVE",     "SDENO",    "SFIP",     "SFREQ",  "SGTRAP",   "SIGM_MOD",
        "STREAM",   "SURFBLK",  "TRAS",     "VELGAS", "VELOCITY", "VELOIL",
        "VELWAT",   "VISC",
    };

    return std::binary_search( std::begin( valid ), std::end( valid ), kw );
}

inline std::map< std::string, int >
RPTRST_integer( const std::vector< int >& ints ) {
    const size_t PCO_index = 26;
    const size_t BASIC_index = 0;

    std::map< std::string, int > mnemonics;
    const size_t size = std::min( ints.size(), sizeof( RSTIntegerKeywords ) );

    /* fun with special cases. Eclipse seems to ignore the BASIC=0,
     * interpreting it as sort-of "don't modify". Handle this by *not*
     * adding/updating the integer list sourced BASIC mnemonic, should it be
     * zero. I'm not sure if this applies to other mnemonics, but the eclipse
     * manual indicates that any zero here should disable the output.
     *
     * See https://github.com/OPM/opm-parser/issues/886 for reference
     */
    if( size > 0 && ints[ BASIC_index ] != 0 )
        mnemonics[ RSTIntegerKeywords[ BASIC_index ] ] = ints[ BASIC_index ];

    for( size_t i = 1; i < std::min( size, PCO_index ); ++i )
        mnemonics[ RSTIntegerKeywords[ i ] ] = ints[ i ];

    for( size_t i = PCO_index + 1; i < size; ++i )
        mnemonics[ RSTIntegerKeywords[ i ] ] = ints[ i ];

    /* item 27 (index 26) sets both PCOW and PCOG, so we special case it */
    if( ints.size() >= PCO_index ) {
        mnemonics[ "PCOW" ] = ints[ PCO_index ];
        mnemonics[ "PCOG" ] = ints[ PCO_index ];
    }

    return mnemonics;
}

template< typename F, typename G >
inline std::map< std::string, int > RPT( const DeckKeyword& keyword,
                                         const ParseContext& parseContext,
                                         ErrorGuard& errors,
                                         F is_mnemonic,
                                         G integer_mnemonic ) {

    std::vector<std::string> items;
    const auto& deck_items = keyword.getStringData();
    const auto ints = std::any_of( deck_items.begin(), deck_items.end(), is_int );
    const auto strs = !std::all_of( deck_items.begin(), deck_items.end(), is_int );

    /* if any of the values are pure integers we assume this is meant to be the
     * slash-terminated list of integers way of configuring. If integers and
     * non-integers are mixed, this is an error; however if the error mode
     * RPT_MIXED_STYLE is permissive we try some desperate heuristics to
     * interpret this as list of mnemonics. See the the documentation of the
     * RPT_MIXED_STYLE error handler for more details.
     */
    auto stoi = []( const std::string& str ) { return std::stoi( str ); };
    if( !strs )
        return integer_mnemonic( fun::map( stoi, deck_items ) );


    if (ints && strs) {
        const auto& location = keyword.location();
        std::string msg = "Error in keyword {keyword}, mixing mnemonics and integers is not allowed\n"
                          "In {file} line {line}.";
        parseContext.handleError(ParseContext::RPT_MIXED_STYLE, msg, location, errors);

        std::vector<std::string> stack;
        for (size_t index=0; index < deck_items.size(); index++) {
            if (is_int(deck_items[index])) {

                if (stack.size() < 2)
                    throw OpmInputError("Problem processing {keyword}\nIn {file} line {line}.", location);

                if (stack.back() == "=") {
                    stack.pop_back();
                    std::string mnemonic = stack.back();
                    stack.pop_back();

                    items.insert(items.begin(), stack.begin(), stack.end());
                    stack.clear();
                    items.push_back( mnemonic + "=" + deck_items[index]);
                } else
                    throw OpmInputError("Problem processing {keyword}\nIn {file} line {line}.", location);

            } else
                stack.push_back(deck_items[index]);
        }
        items.insert(items.begin(), stack.begin(), stack.end());
    } else
        items = deck_items;

    std::map< std::string, int > mnemonics;
    for( const auto& mnemonic : items ) {
        const auto sep_pos = mnemonic.find_first_of( "= " );

        std::string base = mnemonic.substr( 0, sep_pos );
        if( !is_mnemonic( base ) ) {
            std::string msg_fmt = fmt::format("Error in keyword {{keyword}}, unrecognized mnemonic {}\nIn {{file}} line {{line}}.", base);
            parseContext.handleError(ParseContext::RPT_UNKNOWN_MNEMONIC, msg_fmt, keyword.location(), errors);
            continue;
        }

        int val = 1;
        if (sep_pos != std::string::npos) {
            const auto value_pos = mnemonic.find_first_not_of("= ", sep_pos);
            if (value_pos != std::string::npos)
                val = std::stoi(mnemonic.substr(value_pos));
        }

        mnemonics.emplace( base, val );
    }

    return mnemonics;
}

inline void expand_RPTRST_mnemonics(std::map< std::string, int >& mnemonics) {
    const auto allprops_iter = mnemonics.find( "ALLPROPS");
    if (allprops_iter != mnemonics.end()) {
        const auto value = allprops_iter->second;
        mnemonics.erase( allprops_iter );

        for (const auto& kw : {"BG","BO","BW","KRG","KRO","KRW","VOIL","VGAS","VWAT","DEN"})
            mnemonics[kw] = value;
    }
}

std::optional<int> extract(std::map<std::string, int>& mnemonics, const std::string& key) {
    auto iter = mnemonics.find(key);
    if (iter == mnemonics.end())
        return {};

    int value = iter->second;
    mnemonics.erase(iter);
    return value;
}


inline std::pair< std::map< std::string, int >, std::pair<std::optional<int>, std::optional<int>>>
RPTRST( const DeckKeyword& keyword, const ParseContext& parseContext, ErrorGuard& errors) {
    auto mnemonics = RPT( keyword, parseContext, errors, is_RPTRST_mnemonic, RPTRST_integer );
    std::optional<int> basic = extract(mnemonics, "BASIC");
    std::optional<int> freq  = extract(mnemonics, "FREQ");

    expand_RPTRST_mnemonics( mnemonics );
    return {mnemonics, { basic, freq }};
}


template <typename T>
void update_optional(std::optional<T>& target, const std::optional<T>& src) {
    if (src.has_value())
        target = src;
}


}

// The handleRPTSOL() function is only invoked from the constructor which uses
// the SOLUTION section, and the only information actually extracted is whether
// to write the initial restart file.

void RSTConfig::handleRPTSOL( const DeckKeyword& keyword) {
    const auto& record = keyword.getRecord(0);
    const auto& item = record.getItem(0);
    for (const auto& mnemonic : item.getData<std::string>()) {
        auto mnemonic_RESTART_pos = mnemonic.find("RESTART=");
        if (mnemonic_RESTART_pos != std::string::npos) {
            std::string restart_no = mnemonic.substr(mnemonic_RESTART_pos + 8, mnemonic.size());
            auto restart = std::strtoul(restart_no.c_str(), nullptr, 10);
            this->write_rst_file = (restart > 1);
            return;
        }
    }


    /* If no RESTART mnemonic is found, either it is not present or we might
       have an old data set containing integer controls instead of mnemonics.
       Restart integer switch is integer control nr 7 */

    if (item.data_size() >= 7) {
        const std::string& integer_control = item.get<std::string>(6);
        auto restart = std::strtoul(integer_control.c_str(), nullptr, 10);
        this->write_rst_file = (restart > 1);
        return;
    }
}

bool RSTConfig::operator==(const RSTConfig& other) const {
    return true;
}

RSTConfig::RSTConfig(const SOLUTIONSection& solution_section, const ParseContext& parseContext, ErrorGuard& errors)
{
    this->write_rst_file = false;
    if (solution_section.hasKeyword<ParserKeywords::RPTRST>()) {
        // Guessing on eclipse rules for write of initial RESTART file (at time 0):
        // Write of initial restart file is (due to the eclipse reference manual)
        // governed by RPTSOL RESTART in solution section,
        // if RPTSOL RESTART > 1 initial restart file is written.
        // but - due to initial restart file written from Eclipse
        // for data where RPTSOL RESTART not set - guessing that
        // when RPTRST is set in SOLUTION (no basic though...) -> write inital restart.
        this->write_rst_file = true;
        const auto& keyword = solution_section.getKeyword<ParserKeywords::RPTRST>();
        const auto& [mnemonics, basic_freq] = RPTRST(keyword, parseContext, errors);

        update_optional(this->basic, basic_freq.first);
        update_optional(this->freq, basic_freq.second);

        for (const auto& [kw,num] : mnemonics)
            this->keywords[kw] = num;
    }

    if (solution_section.hasKeyword<ParserKeywords::RPTSOL>()) {
        const auto& keyword = solution_section.getKeyword<ParserKeywords::RPTSOL>();
        this->handleRPTSOL(keyword);
    }
}


void RSTConfig::update(const DeckKeyword& keyword, const ParseContext& parseContext, ErrorGuard& errors) {
    if (keyword.name() == ParserKeywords::RPTRST::keywordName) {

    } else if (keyword.name() == ParserKeywords::RPTSCHED::keywordName) {

    } else
        throw std::logic_error("The RSTConfig object can only use RPTRST and RPTSCHED keywords");
}


RSTConfig RSTConfig::serializeObject() {
    RSTConfig rst_config;

    return rst_config;
}

}


