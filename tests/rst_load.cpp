/*
  Copyright 2020 Equinor ASA

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
#include <iostream>
#include <vector>
#include <unordered_map>


#include <iostream>

#include <opm/io/eclipse/rst/state.hpp>
#include <opm/io/eclipse/ERst.hpp>
#include <opm/parser/eclipse/Units/UnitSystem.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/Well/Well.hpp>

#include <opm/parser/eclipse/Parser/Parser.hpp>
#include <opm/parser/eclipse/Parser/ParseContext.hpp>
#include <opm/parser/eclipse/Parser/ErrorGuard.hpp>
#include <opm/parser/eclipse/Deck/Deck.hpp>
#include <opm/parser/eclipse/EclipseState/EclipseState.hpp>
#include <opm/parser/eclipse/EclipseState/EclipseState.hpp>
#include <opm/parser/eclipse/EclipseState/SummaryConfig/SummaryConfig.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/Schedule.hpp>

#include <opm/common/OpmLog/OpmLog.hpp>
#include <opm/common/OpmLog/StreamLog.hpp>
#include <opm/common/OpmLog/LogUtil.hpp>

void initLogging() {
    std::shared_ptr<Opm::StreamLog> cout_log = std::make_shared<Opm::StreamLog>(std::cout, Opm::Log::DefaultMessageTypes);
    Opm::OpmLog::addBackend( "COUT" , cout_log);
}


int main(int , char ** argv) {
    initLogging();
    std::string deck_file = argv[1];
    Opm::ParseContext parseContext;
    Opm::ErrorGuard errors;
    Opm::Parser parser;

    auto deck = parser.parseFile(deck_file, parseContext, errors);
    Opm::EclipseState ecl_state( deck);
    const auto& init_config = ecl_state.getInitConfig();
    int report_step = init_config.getRestartStep();
    const auto& rst_filename = ecl_state.getIOConfig().getRestartFileName( init_config.getRestartRootName(), report_step, false );
    Opm::EclIO::ERst rst_file(rst_filename);

    std::cout << "Loading restart information from: " << rst_filename << ":" << report_step << std::endl;
    const auto& rst_state = Opm::RestartIO::RstState::load(rst_file, report_step);
    Opm::Schedule sched(deck, ecl_state, &rst_state);
}
