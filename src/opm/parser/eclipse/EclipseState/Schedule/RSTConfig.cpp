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

#include <opm/parser/eclipse/Parser/ErrorGuard.hpp>
#include <opm/parser/eclipse/Parser/ParseContext.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/RSTConfig.hpp>
#include <opm/parser/eclipse/Parser/ParserKeywords/R.hpp>


namespace Opm {

namespace {

struct RSTBasic {
    std::optional<int> basic;
    std::optional<int> frequency;
};


//inline std::pair< std::map< std::string, int >, RSTBasic>
//RPTRST( const DeckKeyword& keyword, const ParseContext& parseContext, ErrorGuard& errors, RestartSchedule prev, size_t step ) {
//    auto mnemonics = RPT( keyword, parseContext, errors, is_RPTRST_mnemonic, RPTRST_integer );
//
//    const bool has_freq  = mnemonics.find( "FREQ" )  != mnemonics.end();
//    const bool has_basic = mnemonics.find( "BASIC" ) != mnemonics.end();
//
//    expand_RPTRST_mnemonics( mnemonics );
//
//    if( !has_freq && !has_basic ) return { std::move( mnemonics ), {} };
//
//    const auto basic = has_basic ? mnemonics.at( "BASIC" ) : prev.basic;
//    const auto freq  = has_freq  ? mnemonics.at( "FREQ"  ) : prev.frequency;
//
//    return { std::move( mnemonics ), { step, basic, freq } };
//}



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


