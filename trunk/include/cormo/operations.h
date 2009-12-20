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

#ifndef CORMO_INCLUDE_CORMO_OPERATIONS_H_
#define CORMO_INCLUDE_CORMO_OPERATIONS_H_

#include <vector>

#include "cormo/data_source.h"
#include "cormo/expression.h"
#include "cormo/value.h"

namespace cormo {

class Database;
class Expression;

template <class T>
DataSource<T> Select(Database *database) {
  DataSource<T> data_source(database);
  T::FillDataSource(&data_source);
  return data_source;
}


template <class T>
DataSource<T> Select(Database *database, const Expression &expression) {
  return Select<T>(database).Filter(expression);
}


template <class T>
T Get(Database *database, const Expression &expression) {
  DataSource<T> data_source(database);
  T::FillDataSource(&data_source);
  std::vector<T> objs;
  Error error = data_source.Filter(expression).Limit(2).GetAll(&objs);
  if (error.occurred()) {
    throw error;
  }
  if (objs.size() == 2) {
    throw typename T::NotUnique();
  } else if (objs.size() == 0) {
    throw typename T::DoesNotExist();
  }

  return objs[0];
}

template <class T>
T Get(Database *database, int id) {
  return Get<T>(database, T::Id == id);
}

template <class T>
T Get(Database *database, const Value<int> &id) {
  return Get<T>(database, T::Id == id);
}

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_OPERATIONS_H_
