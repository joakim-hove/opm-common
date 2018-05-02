/*
  Copyright (c) 2017 Statoil ASA
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
#ifndef RESTART_VALUE_HPP
#define RESTART_VALUE_HPP

#include <string>
#include <map>
#include <vector>

namespace Opm {


    class RestartKey {
    public:

        std::string key;
        UnitSystem::measure dim;
        bool required;

        RestartKey( const std::string& _key, UnitSystem::measure _dim)
            : key(_key),
              dim(_dim),
              required(true)
        {}


        RestartKey( const std::string& _key, UnitSystem::measure _dim, bool _required)
            : key(_key),
              dim(_dim),
              required(_required)
        {}

    };


    /*
      A simple struct - the only purpose is to facilitate return by value from the
      RestartIO::load( ) function.
    */


    struct RestartValue {

        using extra_vector = std::vector<std::pair<RestartKey, std::vector<double>>>;

        data::Solution solution;
        data::Wells wells;
        extra_vector extra = {};

        RestartValue(data::Solution sol, data::Wells wells_arg, extra_vector extra_arg) :
            solution(std::move(sol)),
            wells(std::move(wells_arg)),
            extra(std::move(extra_arg))
        {
        }


        RestartValue(data::Solution sol, data::Wells wells_arg) :
            solution(std::move(sol)),
            wells(std::move(wells_arg))
        {
        }



        bool has_extra(const std::string& key) const {
            const auto iter = std::find_if(this->extra.begin(), this->extra.end(), [&](std::pair<RestartKey, std::vector<double>> pair) {return (pair.first.key == key);});
            return  (iter != this->extra.end());
        }

        void add_extra(const std::string& key, UnitSystem::measure dimension, std::vector<double> data) {
            if (key.size() > 8)
                throw std::runtime_error("The keys used for Eclipse output must be maximum 8 characters long.");

            if (this->has_extra(key))
                throw std::runtime_error("The keys in the extra vector must be unique.");

            if (this->solution.has(key))
                throw std::runtime_error("The key is already present in the solution section.");

            this->extra.push_back( std::make_pair(RestartKey(key, dimension), data));
        }

        void add_extra(const std::string& key, const std::vector<double>& data) {
            this->add_extra(key, UnitSystem::measure::identity, data);
        }

    };

}


#endif
