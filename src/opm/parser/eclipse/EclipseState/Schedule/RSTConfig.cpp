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

#include <opm/parser/eclipse/EclipseState/Schedule/RSTConfig.hpp>
#include <opm/parser/eclipse/Parser/ParserKeywords/R.hpp>


namespace Opm {


bool RSTConfig::operator==(const RSTConfig& other) const {
    return true;
}

RSTConfig::RSTConfig(const SOLUTIONSection& solution_section)
{
    if (solution_section.hasKeyword<ParserKeywords::RPTRST>()) {

    }

    if (solution_section.hasKeyword<ParserKeywords::RPTSOL>()) {

    }
}

RSTConfig::RSTConfig(const DeckKeyword& keyword) {
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


