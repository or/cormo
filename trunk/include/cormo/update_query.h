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

#ifndef CORMO_INCLUDE_CORMO_UPDATE_QUERY_H_
#define CORMO_INCLUDE_CORMO_UPDATE_QUERY_H_

#include <string>

#include "cormo/constants.h"
#include "cormo/record.h"
#include "cormo/string_list.h"

namespace cormo {

class Expression;
class Field;

class UpdateQuery {
 public:
  explicit UpdateQuery(const std::string &table_name)
      : table_name_(table_name) {}

  UpdateQuery &Add(const Field &field, const RecordValue &value);
  UpdateQuery &Add(const Field &field, const std::string &value);
  UpdateQuery &Where(const Expression &expression);
  UpdateQuery &Where(const std::string &expression);

  std::string ToString() const;

 private:
  std::string table_name_;
  StringList set_pairs_;
  std::string where_;

  // I want the default copy constructor here, so no dummy.
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_UPDATE_QUERY_H_
