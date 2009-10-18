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
  class Cursor {
  public:
    virtual ~Cursor() {}
    virtual Record FetchOne() = 0;
  };

  class Result {
  public:
    virtual ~Result() {}
    virtual std::size_t num_fields() const = 0;
    virtual std::size_t num_records() const = 0;
    virtual std::vector<std::string> fields() const = 0;
    virtual Records records() const = 0;
  };

  static Backend *CreateBackend(const std::string &type,
                                const std::string &conn_info);

  virtual ~Backend() {}

  virtual void Connect() {}

  virtual void Begin() {}
  virtual void Commit() {}
  virtual void Rollback() {}
  virtual bool in_transaction() const { return false; }

  virtual bool got_error() const { return last_error_ != ""; }
  virtual std::string last_error() {
    std::string tmp = last_error_;
    last_error_ = "";
    return tmp;
  }

  virtual Result *Execute(const std::string &query) = 0;
  virtual Cursor *CreateCursor(const std::string &query) = 0;

 protected:
  Backend() {}

  void set_last_error(const std::string &error) { last_error_ = error; }

  std::string last_error_;

 private:

  DISALLOW_COPY_AND_ASSIGN(Backend);
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_BACKEND_H_
