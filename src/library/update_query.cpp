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

#include "cormo/update_query.h"

#include <string>

#include "cormo/expression.h"
#include "cormo/field.h"
#include "cormo/record.h"
#include "cormo/string.h"
#include "cormo/value.h"

namespace cormo {

using std::string;

UpdateQuery &UpdateQuery::Add(const Field &field, const RecordValue &value) {
  set_pairs_.push_back(field.name() + "=" + EscapeSql(value));
  return *this;
}


UpdateQuery &UpdateQuery::Add(const Field &field, const string &value) {
  set_pairs_.push_back(field.name() + "=" + EscapeSql(value));
  return *this;
}


UpdateQuery &UpdateQuery::Where(const Expression &expression) {
  where_ = And(RawExpression(where_), expression).ToString();
  return *this;
}


UpdateQuery &UpdateQuery::Where(const string &expression) {
  where_ = And(RawExpression(where_), RawExpression(expression)).ToString();
  return *this;
}


string UpdateQuery::ToString() const {
  string query = "UPDATE ";
  query += table_name_;
  query += " SET ";
  query += set_pairs_.Join(",");
  if (!where_.empty()) {
    query += " WHERE ";
    query += where_;
  }

  return query;
}

}
