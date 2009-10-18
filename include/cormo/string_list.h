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

#ifndef CORMO_INCLUDE_CORMO_STRING_LIST_H_
#define CORMO_INCLUDE_CORMO_STRING_LIST_H_

#include <string>
#include <vector>

#include "cormo/constants.h"

namespace cormo {

class StringList : public std::vector<std::string> {
 public:
  StringList() : std::vector<std::string>() {}
  StringList(const std::string &str, const std::string &delimiter);

  std::string Join(const std::string &delimiter) const;
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_STRING_LIST_H_
