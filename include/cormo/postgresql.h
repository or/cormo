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

#ifndef CORMO_INCLUDE_CORMO_POSTGRESQL_H_
#define CORMO_INCLUDE_CORMO_POSTGRESQL_H_

#include <libpq-fe.h>

#include <cstddef>
#include <string>
#include <vector>

#include "cormo/constants.h"
#include "cormo/backend.h"

namespace cormo {

class PostgreSQL : public Backend {
 public:
  class Result : public Backend::Result {
   public:
    explicit Result(PGresult *result) : result_(result) {}
    virtual ~Result() { PQclear(result_); }
    virtual size_t num_fields() const { return PQnfields(result_); }
    virtual size_t num_records() const { return PQntuples(result_); }
    virtual std::vector<std::string> fields() const;
    virtual Records records() const;

   private:
    PGresult *result_;

    DISALLOW_COPY_AND_ASSIGN(Result);
  };

  class Cursor : public Backend::Cursor {
   public:
    Cursor(PostgreSQL *postgresql, const std::string &query);
    virtual ~Cursor() {
      delete postgresql_.Execute("CLOSE " + name_ + ";");
      if (new_transaction_) {
        postgresql_.Commit();
      }
    }

    virtual Record FetchOne();

  private:
    static size_t cache_size_;
    static int sid_;

    PostgreSQL &postgresql_;
    std::string name_;
    Records cache_;
    size_t cache_pos_;
    bool new_transaction_;
  };

  explicit PostgreSQL(const std::string &connection_info)
      : connection_(NULL),
        connection_info_(connection_info),
        transaction_(false) {}

  virtual ~PostgreSQL() {
    if (connection_) {
      if (transaction_) {
        Commit();
      }
      PQfinish(connection_);
    }
  }

  virtual void Connect();

  virtual void Begin();
  virtual void Commit();
  virtual void Rollback();
  virtual bool in_transaction() const { return transaction_; }

  virtual Backend::Result *Execute(const std::string &query);
  virtual Backend::Cursor *CreateCursor(const std::string &query);

 private:
  PGconn *connection_;
  std::string connection_info_;
  bool transaction_;

  DISALLOW_COPY_AND_ASSIGN(PostgreSQL);
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_POSTGRESQL_H_
