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

#include <math.h>
#include <opm/common/util/numeric/cmp.hpp>

/**
   Ahhh - the joys of comparing floating point numbers ....

   http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
*/


namespace Opm {
  
  bool double_equal(double value1 , double value2 , double abs_epsilon , double rel_epsilon) {
    bool equal = true;
    double diff = fabs(value1 - value2);
    if (diff > abs_epsilon) {
      double sum = fabs(value1) + fabs(value2);
    
      if (diff > sum * rel_epsilon) {
	equal = false;
      }
    }
    return equal;
  }


  bool double_equal(double value1 , double value2) {
    const double abs_epsilon = 1e-8;
    const double rel_epsilon = 1e-5;

    return double_equal( value1 , value2 , abs_epsilon , rel_epsilon );
  }

}
