/*
  Copyright 2021 Equinor ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RST_UDQ
#define RST_UDQ

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include <opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQEnums.hpp>

namespace Opm {

namespace RestartIO {

struct RstHeader;

class RstUDQ {
public:
    struct RstDefine {
        RstDefine(const std::string& expression_arg, UDQUpdate status_arg);

        std::string expression;
        UDQUpdate status;
        std::vector<std::pair<std::string, double>> values;
        std::vector<std::pair<std::string, double>> well_values;
        std::vector<std::pair<std::string, double>> group_values;
        std::optional<double> field_value;
    };

    struct RstAssign {
        void update_value(const std::string& name, double new_value);

        std::optional<double> value;
        std::unordered_set<std::string> selector;
    };


    RstUDQ(const std::string& name_arg,
           const std::string& unit_arg,
           const std::string& define_arg,
           UDQUpdate status_arg);

    RstUDQ(const std::string& name_arg,
           const std::string& unit_arg);

    void add_value(const std::string& wgname, double value);
    void add_well_value(const std::string& wname, double value);
    void add_group_value(const std::string& wname, double value);
    void add_field_value(double value);
    void update_assign(double value);

    bool is_define() const;
    double assign_value() const;
    const std::unordered_set<std::string>& assign_selector() const;
    const std::string& expression() const;

    // Common properties
    std::string name;
    std::string unit;
    UDQVarType var_type;


    // DEFINE properties
    std::vector<std::pair<std::string, double>> well_values;
    std::vector<std::pair<std::string, double>> group_values;
    std::optional<double> field_value;

    //// ASSIGN properties
    //std::unordered_set<std::string> assign_selector;
    //std::optional<double> assign_value;
private:
    std::variant<std::monostate, RstDefine, RstAssign> data;
};


}
}




#endif
