/*
  Copyright 2016 Statoil ASA.

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

#ifndef COMMON_UTIL_NUMERIC_CMP
#define COMMON_UTIL_NUMERIC_CMP

#include <opm/common/util/numeric/cmp.hpp>

namespace Opm {

  bool double_equal(double value1 , double value2);
  bool double_equal(double value1 , double value2 , double abs_epsilon , double rel_epsilon);

  
  //int opm_memcmp_double(const double * p1 , const double *p2 , size_t num_elements);

}

#endif
