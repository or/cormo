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

#include <auto_ptr.h>
#include <string>

#include "cormo/expression.h"
#include "cormo/select_query.h"
#include "cormo/string_list.h"
#include "cormo/string.h"

namespace cormo {

using std::auto_ptr;
using std::string;

void Database::Connect() {
  if (debug_) {
    fprintf(stderr, "connect;\n");
  }
  Reset();
  backend_ = Backend::CreateBackend(backend_type_, conn_info_);
  if (backend_ == NULL) {
    set_last_error("Backend creation failed.");
    return;
  }
  backend_->Connect();
  if (backend_->got_error()) {
    set_last_error(backend_->last_error());
    return;
  }
}


void Database::Reset() {
  if (backend_) {
    delete backend_;
  }
}


void Database::Begin() {
  if (debug_) {
    fprintf(stderr, "begin;\n");
  }
  assert(backend_ != NULL);
  backend_->Begin();
  if (backend_->got_error()) {
    set_last_error(backend_->last_error());
    return;
  }
}


void Database::Commit() {
  if (debug_) {
    fprintf(stderr, "commit;\n");
  }
  assert(backend_ != NULL);
  backend_->Commit();
  if (backend_->got_error()) {
    set_last_error(backend_->last_error());
    return;
  }
}


void Database::Rollback() {
  if (debug_) {
    fprintf(stderr, "rollback;\n");
  }
  assert(backend_ != NULL);
  backend_->Rollback();
  if (backend_->got_error()) {
    set_last_error(backend_->last_error());
    return;
  }
}


bool Database::in_transaction() const {
  assert(backend_ != NULL);
  return backend_->in_transaction();
}


Records Database::Query(const string &query) {
  if (debug_) {
    fprintf(stderr, "%s\n", query.c_str());
  }
  auto_ptr<Backend::Result> result(backend_->Execute(query));
  if (backend_->got_error()) {
    set_last_error(backend_->last_error());
    return Records();
  }

  return result->records();
}


void Database::Insert(const string &table,
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

  Query(command);
  if (backend_->got_error()) {
    set_last_error(backend_->last_error());
    return;
  }
}


void Database::Delete(const string &table,
                      const Expression &where) {
  Query("DELETE FROM " + table + " WHERE " + where.ToString());
  if (backend_->got_error()) {
    set_last_error(backend_->last_error());
    return;
  }
}

}  // namespace cormo
