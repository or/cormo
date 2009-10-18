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
  Database(const std::string &type, const std::string &conn_info)
      : debug_(false),
        backend_(NULL),
        backend_type_(type),
        conn_info_(conn_info) {}

  ~Database() { Reset(); }

  Records Query(const std::string &query);

  template <class T>
  Cursor<T> cursor(const std::string &query) {
    if (debug_) {
      fprintf(stderr, "%s\n", query.c_str());
    }
    return Cursor<T>(this, backend_->CreateCursor(query));
  }

  void Connect();
  void Reset();
  void Insert(const std::string &table,
              const StringList &fields,
              const Record &values);
  void Delete(const std::string &table, const Expression &e);
  void Begin();
  void Commit();
  void Rollback();
  bool in_transaction() const;

  bool got_error() const { return last_error_ != ""; }
  std::string last_error() {
    std::string tmp = last_error_;
    if (debug_) {
      fprintf(stderr, "Error: %s\n", tmp.c_str());
    }
    last_error_ = "";
    return tmp;
  }

  void set_debug(bool debug) { debug_ = debug; }

 private:
  void set_last_error(const std::string &error) { last_error_ = error; }

  bool debug_;
  Backend *backend_;
  std::string backend_type_;
  std::string conn_info_;
  std::string last_error_;

  DISALLOW_COPY_AND_ASSIGN(Database);
};

}  // namespade cormo

#endif  // CORMO_INCLUDE_CORMO_DATABASE_H_
