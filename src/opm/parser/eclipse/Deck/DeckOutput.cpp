/*
  Copyright 2017 Statoil ASA.

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

#include <ostream>

#include <opm/common/OpmLog/OpmLog.hpp>
#include <opm/parser/eclipse/Deck/DeckOutput.hpp>
#include <opm/parser/eclipse/Deck/UDAValue.hpp>
#include <opm/parser/eclipse/Units/UnitSystem.hpp>


namespace Opm {

    DeckOutput::DeckOutput( std::ostream& s, int precision, const UnitSystem* output_units_arg) :
        os( s ),
        default_count( 0 ),
        row_count( 0 ),
        record_on( false ),
        org_precision( os.precision(precision) ),
        m_output_units(output_units_arg)
    {}

    DeckOutput::~DeckOutput() {
        this->set_precision(this->org_precision);
    }

    const UnitSystem* DeckOutput::output_units() const {
        return this->m_output_units;
    }

    void DeckOutput::set_precision(int precision) {
        this->os.precision(precision);
    }


    void DeckOutput::endl() {
        this->os << std::endl;
    }

    void DeckOutput::write_string(const std::string& s) {
        this->os << s;
    }


    template <typename T>
    void DeckOutput::write( const T& value ) {
        if (default_count > 0) {
            write_sep( );

            os << default_count << "*";
            default_count = 0;
            row_count++;
        }

        write_sep( );
        write_value( value );
        row_count++;
    }

    template <>
    void DeckOutput::write_value( const std::string& value ) {
        this->os << "'" << value << "'";
    }

    template <>
    void DeckOutput::write_value( const int& value ) {
        this->os << value;
    }

    template <>
    void DeckOutput::write_value( const double& value ) {
        this->os << value;
    }

    template <>
    void DeckOutput::write_value( const UDAValue& value ) {
        if (value.is<double>()) {
            double si_value = value.get<double>();
            const auto& dim = value.get_dim();
            double deck_value = dim.convertSiToRaw(si_value);
            this->write_value(deck_value);
        }
        else
            this->write_value(value.get<std::string>());
    }

    void DeckOutput::stash_default( ) {
        this->default_count++;
    }


    void DeckOutput::start_keyword(const std::string& kw) {
        if (this->m_output_units) {
            if (kw == "FILEUNIT" || kw == "GRIDUNIT")
                OpmLog::warning("The content of the " + kw + " keywords is not updated when doing unit conversion");
        }

        this->os << kw << std::endl;
    }


    void DeckOutput::end_keyword(bool add_slash) {
        if (add_slash)
            this->os << "/" << std::endl;
    }


    void DeckOutput::write_sep( ) {
        if (record_on) {
            if ((row_count > 0) && ((row_count % columns) == 0))
                split_record();
        }

        if (row_count > 0)
            os << item_sep;
        else if (record_on)
            os << record_indent;
    }

    void DeckOutput::start_record( ) {
        this->default_count = 0;
        this->row_count = 0;
        this->record_on = true;
    }


    void DeckOutput::split_record() {
        this->os << std::endl;
        this->row_count = 0;
    }


    void DeckOutput::end_record( ) {
        this->os << " /" << std::endl;
        this->record_on = false;
    }


    template void DeckOutput::write( const int& value);
    template void DeckOutput::write( const double& value);
    template void DeckOutput::write( const std::string& value);
    template void DeckOutput::write( const UDAValue& value);
}
