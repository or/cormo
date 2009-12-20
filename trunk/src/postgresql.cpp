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
#include <stdio.h>
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


PostgreSQL::Cursor::Cursor(PostgreSQL *postgresql)
    : postgresql_(*postgresql),
      name_("cursor" + ToString(sid_++)),
      cache_pos_(0),
      new_transaction_(false) {
  // nothing else to do
}


Error PostgreSQL::Cursor::Init(const string &query) {
  Error error;
  if (!postgresql_.in_transaction()) {
    error = postgresql_.Begin();
    if (error.occurred()) {
      return error;
    }
    new_transaction_ = true;
  }
  error = postgresql_.Execute("DECLARE \"" + name_ + "\" CURSOR FOR " + query);
  if (error.occurred()) {
    return error;
  }

  return Success();
}


Error PostgreSQL::Cursor::FetchOne(Record *record) {
  if (cache_.size() == 0 ||
      cache_pos_ >= cache_.size()) {
    cache_pos_ = 0;
    Backend::Result *result;
    Error error = postgresql_.Execute("FETCH " + ToString(cache_size_) +
                                      " FROM " + name_ + ";", &result);
    if (error.occurred()) {
      return error;
    }
    cache_ = result->records();
    delete result;
  }

  if (cache_pos_ >= cache_.size()) {
    *record = Record();
  } else {
    *record = cache_[cache_pos_++];
  }

  return Success();
}


Error PostgreSQL::Connect() {
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
    return Error(message);
  }

  return Success();
}


Error PostgreSQL::Begin() {
  if (!transaction_) {
    Error error = Execute("BEGIN;");
    if (error.occurred()) {
      return error;
    }
    transaction_ = true;
  }
  return Success();
}


Error PostgreSQL::Commit() {
  if (transaction_) {
    Error error = Execute("COMMIT;");
    if (error.occurred()) {
      return error;
    }
    transaction_ = false;
  }
  return Success();
}


Error PostgreSQL::Rollback() {
  if (transaction_) {
    Error error = Execute("ROLLBACK;");
    if (error.occurred()) {
      return error;
    }
    transaction_ = false;
  }
  return Success();
}


Error PostgreSQL::Execute(const string &query, Backend::Result **result) {
  string tmp = query;
  tmp += ";";
  PGresult *raw_result = PQexec(connection_, tmp.c_str());

  if (PQresultStatus(raw_result) != PGRES_TUPLES_OK &&
      PQresultStatus(raw_result) != PGRES_COMMAND_OK) {
    PQclear(raw_result);
    string message = "Query: " + query + " failed: " +
                       string(PQerrorMessage(connection_));
    return Error(message);
  }

  *result = new Result(raw_result);
  return Success();
}


Error PostgreSQL::Execute(const string &query) {
  Backend::Result *result;
  Error error = Execute(query, &result);
  if (error.occurred()) {
    return error;
  }

  // don't want the result, so free it
  delete result;
  return Success();
}



Error PostgreSQL::CreateCursor(const string &query, Backend::Cursor **cursor) {
  *cursor = new Cursor(this);
  Error error = (*cursor)->Init(query + ";");
  if (error.occurred()) {
    delete *cursor;
    *cursor = NULL;
    return error;
  }
  return Success();
}

}  // namespace cormo
