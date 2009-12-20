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

#ifndef CORMO_INCLUDE_CORMO_BACKEND_H_
#define CORMO_INCLUDE_CORMO_BACKEND_H_

#include <exception>
#include <string>
#include <vector>

#include "cormo/constants.h"
#include "cormo/record.h"
#include "cormo/error.h"

namespace cormo {

class Backend {
 public:
  enum Types { kPostgresql = 1 };

  class Result {
  public:
    virtual ~Result() {}
    virtual std::size_t num_fields() const = 0;
    virtual std::size_t num_records() const = 0;
    virtual std::vector<std::string> fields() const = 0;
    virtual Records records() const = 0;
  };

  class Cursor {
  public:
    virtual ~Cursor() {}
    virtual Error Init(const std::string &query) = 0;
    virtual Error FetchOne(Record *record) = 0;
  };

  static Error CreateBackend(Types type,
                             const std::string &conn_info,
                             Backend **backend);

  virtual ~Backend() {}

  virtual Error Connect() { return Success(); }

  virtual Error Begin() { return Success(); }
  virtual Error Commit() { return Success(); }
  virtual Error Rollback() { return Success(); }
  virtual bool in_transaction() const { return false; }

  virtual Error Execute(const std::string &query, Result **result) = 0;
  virtual Error Execute(const std::string &query) = 0;
  virtual Error CreateCursor(const std::string &query, Cursor **cursor) = 0;

 protected:
  Backend() {}

 private:

  DISALLOW_COPY_AND_ASSIGN(Backend);
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_BACKEND_H_
