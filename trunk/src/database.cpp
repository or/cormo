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

#include "cormo/database.h"

#include <stdio.h>

#include <string>

#include "cormo/expression.h"
#include "cormo/select_query.h"
#include "cormo/string_list.h"
#include "cormo/string.h"

namespace cormo {

using std::string;

Error Database::Connect() {
  if (debug_) {
    fprintf(stderr, "connect;\n");
  }
  Reset();
  Error error = Backend::CreateBackend(backend_type_, conn_info_, &backend_);
  if (error.occurred()) {
    return error;
  }

  error = backend_->Connect();
  if (error.occurred()) {
    return error;
  }

  return Success();
}


void Database::Reset() {
  if (backend_) {
    delete backend_;
    backend_ = NULL;
  }
}


Error Database::Begin() {
  if (debug_) {
    fprintf(stderr, "begin;\n");
  }
  assert(backend_ != NULL);
  return backend_->Begin();
}


Error Database::Commit() {
  if (debug_) {
    fprintf(stderr, "commit;\n");
  }
  assert(backend_ != NULL);
  return backend_->Commit();
}


Error Database::Rollback() {
  if (debug_) {
    fprintf(stderr, "rollback;\n");
  }
  assert(backend_ != NULL);
  return backend_->Rollback();
}


bool Database::in_transaction() const {
  assert(backend_ != NULL);
  return backend_->in_transaction();
}


Error Database::Query(const string &query, Records *records) {
  if (debug_) {
    fprintf(stderr, "%s\n", query.c_str());
  }
  Backend::Result *result;
  Error error = backend_->Execute(query, &result);
  if (error.occurred()) {
    return error;
  }

  if (records) {
    // only do this if records is not NULL, otherwise we assume the records
    // are not wanted anyway
    *records = result->records();
  }
  // but free the result
  delete result;

  return Success();
}


Error Database::Query(const string &query) {
  return Query(query, NULL);
}


Error Database::Insert(const string &table,
                       const StringList &fields,
                       const Record &record) {
  string command = "INSERT INTO ";
  command += table;
  command += " (";
  command += fields.Join(",");
  command += ") VALUES (";

  for (size_t i = 0; i < record.size(); ++i) {
    if (i != 0) {
      command += ",";
    }
    command += EscapeSql(record[i]);
  }
  command += ")";

  return Query(command);
}


Error Database::Delete(const string &table,
                        const Expression &where) {
  return Query("DELETE FROM " + table + " WHERE " + where.ToString());
}

}  // namespace cormo
