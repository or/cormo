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

#ifndef CORMO_INCLUDE_CORMO_DATA_SOURCE_H_
#define CORMO_INCLUDE_CORMO_DATA_SOURCE_H_

#include <string>
#include <vector>

#include "cormo/constants.h"
#include "cormo/database.h"
#include "cormo/select_query.h"

namespace cormo {

class SelectQuery;
class Field;

template <class T>
class DataSource {
 public:
  explicit DataSource(Database *database) : database_(*database) {}

  const Database &database() const { return database_; }
  const SelectQuery &query() const { return query_; }
  size_t count() const;

  DataSource<T> &AddSource(const std::string &table) {
    query_.AddSource(table);
    return *this;
  }

  DataSource<T> &AddLeftJoin(const std::string &table,
                             const Field &left,
                             const Field &right) {
    query_.LeftJoin(table, left, right);
    return *this;
  }

  DataSource<T> &AddResult(const Field &field) {
    query_.AddResult(field);
    return *this;
  }

  DataSource<T> &AddResult(const std::string &field) {
    query_.AddResult(field);
    return *this;
  }

  DataSource<T> &Distinct(bool distinct) {
    query_.Distinct(distinct);
    return *this;
  }

  DataSource<T> &Distinct() {
    query_.Distinct();
    return *this;
  }

  DataSource<T> &Limit(int limit) {
    query_.Limit(limit);
    return *this;
  }

  DataSource<T> &Offset(int offset) {
    query_.Offset(offset);
    return *this;
  }

  template <class S>
  DataSource<T> &Filter(const S &expression) {
    query_.Where(expression);
    return *this;
  }

  template <class S>
  DataSource<T> &OrderBy(const S &order_by) {
    query_.OrderBy(order_by);
    return *this;
  }

  Error CreateCursor(Cursor<T> **cursor) {
    return database_.CreateCursor(query_.ToString(), cursor);
  }

  Error GetOne(T *obj) {
    Cursor<T> *cursor;
    Error error = CreateCursor(&cursor);
    if (error.occurred()) {
      return error;
    }

    error = cursor->Get(obj);
    if (error.occurred()) {
      return error;
    }

    if (!obj->is_initialized()) {
      return T::DoesNotExist();
    }

    delete cursor;

    return Success();
  }

  Error GetAll(std::vector<T> *result) {
    Cursor<T> *cursor;
    Error error = CreateCursor(&cursor);
    if (error.occurred()) {
      return error;
    }
    error = cursor->Dump(result);
    delete cursor;
    return error;
  }

 private:
  Database &database_;
  SelectQuery query_;

  // I want the default copy constructor here, so no dummy.
};

}

#endif  // CORMO_INCLUDE_CORMO_DATA_SOURCE_H_
