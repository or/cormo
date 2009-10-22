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

#include "cormo/postgresql.h"

#include <string>
#include <vector>

#include "cormo/string.h"
#include "cormo/string_list.h"

namespace cormo {

using std::string;
using std::vector;

vector<string> PostgreSQL::Result::fields() const {
  vector<string> r;
  r.reserve(num_fields());
  for (size_t i = 0; i < num_fields(); ++i) {
    r.push_back(PQfname(result_, i));
  }

  return r;
}


Records PostgreSQL::Result::records() const {
  Records recs;
  recs.resize(num_records());
  for (size_t i = 0; i < num_records(); ++i) {
    recs[i].reserve(num_fields());
    for (size_t j = 0; j < num_fields(); ++j) {
      if (PQgetisnull(result_, i, j)) {
        recs[i].push_back(RecordValue());
      } else {
        recs[i].push_back(RecordValue(PQgetvalue(result_, i, j)));
      }
    }
  }

  return recs;
}


size_t PostgreSQL::Cursor::cache_size_ = 30;
int PostgreSQL::Cursor::sid_ = 1;

PostgreSQL::Cursor::Cursor(PostgreSQL *postgresql,
                           const string &query)
    : postgresql_(*postgresql),
      name_("cursor" + ToString(sid_++)),
      cache_pos_(0),
      new_transaction_(false) {
  if (!postgresql_.in_transaction()) {
    postgresql_.Begin();
    new_transaction_ = true;
  }
  delete postgresql_.Execute("DECLARE \"" + name_ + "\" CURSOR FOR " + query);
}


Record PostgreSQL::Cursor::FetchOne() {
  if (cache_.size() == 0 ||
      cache_pos_ >= cache_.size()) {
    cache_pos_ = 0;
    Backend::Result *r = postgresql_.Execute("FETCH " + ToString(cache_size_) +
                                               " FROM " + name_ + ";");
    cache_ = r->records();
    delete r;
  }

  if (cache_pos_ >= cache_.size()) {
    return Record();
  }

  return cache_[cache_pos_++];
}


void PostgreSQL::Connect() {
  StringList params(connection_info_, ";");
  string new_connection_info = "";
  for (size_t i = 0; i < params.size(); ++i) {
    StringList param(params[i], "=");
    if (param.size() == 1) {
      continue;
    }

    if (param[0] == "host") {
      new_connection_info += "host=" + param[1] + " ";
    } else if (param[0] == "database") {
      new_connection_info += "dbname=" + param[1] + " ";
    } else if (param[0] == "password") {
      new_connection_info += "password=" + param[1] + " ";
    } else if (param[0] == "user") {
      new_connection_info += "user=" + param[1] + " ";
    }
  }

  connection_ = PQconnectdb(new_connection_info.c_str());
  if (PQstatus(connection_) != CONNECTION_OK) {
    string message = "PostgreSQL connection " + connection_info_ + " failed: " +
                       string(PQerrorMessage(connection_));
    set_last_error(message);
    throw Error(message);
    return;
  }
}


void PostgreSQL::Begin() {
  if (!transaction_) {
    delete Execute("BEGIN;");
    transaction_ = true;
  }
}


void PostgreSQL::Commit() {
  if (transaction_) {
    delete Execute("COMMIT;");
    transaction_ = false;
  }
}


void PostgreSQL::Rollback() {
  if (transaction_) {
    delete Execute("ROLLBACK;");
    transaction_ = false;
  }
}


Backend::Result *PostgreSQL::Execute(const string &query) {
  string tmp = query;
  tmp += ";";
  PGresult *result = PQexec(connection_, tmp.c_str());

  if (PQresultStatus(result) != PGRES_TUPLES_OK &&
      PQresultStatus(result) != PGRES_COMMAND_OK) {
    PQclear(result);
    string message = "Query: " + query + " failed: " +
                       string(PQerrorMessage(connection_));
    set_last_error(message);
    throw Error(message);
    return NULL;
  }

  return new Result(result);
}


Backend::Cursor *PostgreSQL::CreateCursor(const string &query) {
  return new Cursor(this, query + ";");
}

}  // namespace cormo
