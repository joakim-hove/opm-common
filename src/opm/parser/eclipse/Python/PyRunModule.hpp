/*
  Copyright 2020 Equinor ASA.

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
#ifndef EMBEDDED_PYTHON
error BUG: The PyRunModule.hpp header should *not* be included in a configuration without EMBEDDED_PYTHON
#endif

#ifndef OPM_PY_RUN_MODULE
#define OPM_PY_RUN_MODULE

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;

#include <memory>
#include <string>
#include <opm/parser/eclipse/Python/Python.hpp>

namespace Opm {

class EclipseState;
class Schedule;
class SummaryState;


class  PyRunModule {
public:
    PyRunModule(std::shared_ptr<const Python> python, const std::string& fname);

    bool run(EclipseState& ecl_state, Schedule& sched, std::size_t report_step, SummaryState& st);

private:
    py::object run_function = py::none();
    std::shared_ptr<const Python> python_handle;
    py::module module;
    py::module opm_embedded;
    py::dict storage;
};

}
#endif
