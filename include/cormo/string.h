/* Copyright (C) 2009 Oliver Runge (thi@thialfihar.org)
 *
 * This file is part of cormo.
 *
 * cormo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cormo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cormo.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CORMO_INCLUDE_CORMO_STRING_H_
#define CORMO_INCLUDE_CORMO_STRING_H_

#include <sstream>
#include <string>

#include "cormo/constants.h"
#include "cormo/record.h"

namespace cormo {

template <class T>
std::string ToString(const T a) {
  std::ostringstream os;
  os << a;
  return os.str();
}

std::string Replace(const std::string &str,
                    const std::string &what,
                    const std::string &with);
std::string EscapeSql(const std::string &str);
std::string EscapeSql(const RecordValue &str);

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_STRING_H_
