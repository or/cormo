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

#ifndef CORMO_INCLUDE_CORMO_DATABASE_H_
#define CORMO_INCLUDE_CORMO_DATABASE_H_

#include <cstdio>
#include <exception>
#include <string>

#include "cormo/backend.h"
#include "cormo/constants.h"
#include "cormo/cursor.h"
#include "cormo/error.h"

namespace cormo {

class Expression;
class StringList;

class Database {
 public:
  Database(Backend::Types type, const std::string &conn_info)
      : debug_(false),
        backend_(NULL),
        backend_type_(type),
        conn_info_(conn_info) {}

  ~Database() { Reset(); }

  Error Query(const std::string &query, Records *records);
  Error Query(const std::string &query);

  template <class T>
  Error CreateCursor(const std::string &query, Cursor<T> **cursor) {
    if (debug_) {
      fprintf(stderr, "%s\n", query.c_str());
    }
    Backend::Cursor *tmp_cursor;
    Error error = backend_->CreateCursor(query, &tmp_cursor);
    if (error.occurred()) {
      return error;
    }
    *cursor = new Cursor<T>(this, tmp_cursor);
    return Success();
  }

  Error Connect();
  void Reset();
  Error Insert(const std::string &table,
               const StringList &fields,
               const Record &values);
  Error Delete(const std::string &table, const Expression &e);
  Error Begin();
  Error Commit();
  Error Rollback();
  bool in_transaction() const;

  void set_debug(bool debug) { debug_ = debug; }

 private:

  bool debug_;
  Backend *backend_;
  Backend::Types backend_type_;
  std::string conn_info_;

  DISALLOW_COPY_AND_ASSIGN(Database);
};

}  // namespade cormo

#endif  // CORMO_INCLUDE_CORMO_DATABASE_H_
