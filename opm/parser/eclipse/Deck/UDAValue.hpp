/*
  Copyright 2019 Equinor ASA.

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

#ifndef UDA_VALUE_HPP
#define UDA_VALUE_HPP

#include <stdexcept>
#include <vector>
#include <string>
#include <iosfwd>

#include <opm/parser/eclipse/Units/Dimension.hpp>

namespace Opm {

class UDAValue {
public:
    UDAValue();
    explicit UDAValue(double);
    explicit UDAValue(const std::string&);
    explicit UDAValue(const Dimension& dim);
    UDAValue(double data, const Dimension& dim);
    UDAValue(const std::string& data, const Dimension& dim);

    /*
      The get<double>() and get<std::string>() methods will throw an
      exception if the internal type and the template parameter disagree.
    */

    template<typename T>
    T get() const;

    /*
      The getSI() can only be called for numerical values.
    */
    double getSI() const;
    bool zero() const;

    template<typename T>
    bool is() const;

    void assert_numeric() const;
    void assert_numeric(const std::string& error_msg) const;
    const Dimension& get_dim() const;

    bool operator==(const UDAValue& other) const;
    bool operator!=(const UDAValue& other) const;
    UDAValue& operator=(double value);
    UDAValue& operator=(const std::string& value);

    bool is_numeric() { return numeric_value; }

    template<class Serializer>
    void serializeOp(Serializer& serializer)
    {
        serializer(numeric_value);
        serializer(double_value);
        serializer(string_value);
        dim.serializeOp(serializer);
    }

private:
    bool numeric_value;
    double double_value;
    std::string string_value;

    Dimension dim;
};

std::ostream& operator<<( std::ostream& stream, const UDAValue& uda_value );
}



#endif
