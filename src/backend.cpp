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

#include "cormo/backend.h"

#include <string>

#include "cormo/postgresql.h"

namespace cormo {

using std::string;

Backend *Backend::CreateBackend(const string &type,
                                const string &conn_info) {
  Backend* backend = NULL;

  if (type == "postgresql") {
    backend = new PostgreSQL(conn_info);
  } else {
    throw Error("No backend '" + type + "' available.");
  }

  return backend;
}

}  // namespace cormo
