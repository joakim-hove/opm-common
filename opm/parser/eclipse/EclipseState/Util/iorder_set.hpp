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

#ifndef OPM_IORDER_SET_HPP
#define OPM_IORDER_SET_HPP

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

namespace Opm {


/*
  Small class which implements an insert-ordered set. The set has an erase()
  method which can be used to remove elements, otherwise the elements in the
  container are immutable.
*/

template <typename T>
class iorder_set {

    using storage_type = typename std::vector<T>;
    using index_type = typename std::unordered_set<T>;
    using const_iter_type = typename storage_type::const_iterator;

private:
    index_type index;
    storage_type data;

public:

    std::size_t size() const {
        return this->index.size();
    }

    bool empty() const {
        return (this->size() == 0);
    }

<<<<<<< HEAD
    std::size_t count(const T& value) const {
        return this->index.count(value);
    }

    bool contains(const T& value) const {
=======
    std::size_t count(const T& value) {
        return this->index.count(value);
    }

    bool contains(const T& value) {
>>>>>>> Add insert ordered set
        return (this->count(value) != 0);
    }

    bool insert(const T& value) {
        if (this->contains(value))
            return false;

        this->index.insert(value);
        this->data.push_back(value);
        return true;
    }

    std::size_t erase(const T& value) {
        if (!this->contains(value))
            return 0;

        this->index.erase(value);
        auto data_iter = std::find(this->data.begin(), this->data.end(), value);
        this->data.erase(data_iter);
        return 1;
    }

    const_iter_type begin() const {
        return this->data.begin();
    }

    const_iter_type end() const {
        return this->data.end();
    }

    const T& operator[](std::size_t i) const {
        if (i >= this->size())
            throw std::invalid_argument("Invalid index in []");

        return this->data[i];
    }

    const std::vector<T>& vector() const {
        return this->data;
    };
};
}

#endif
