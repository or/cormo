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

#ifndef CORMO_INCLUDE_CORMO_CURSOR_H_
#define CORMO_INCLUDE_CORMO_CURSOR_H_

#include <vector>

#include "cormo/backend.h"
#include "cormo/database.h"

namespace cormo {

template <class T>
class Cursor {
 public:
  Cursor(Database *database, Backend::Cursor *cursor)
     : database_(*database), cursor_(cursor), done_(false) {
    Next();
  }
  ~Cursor() {
    delete cursor_;
  }

  Error Next() {
    if (done_) {
      return Success();
    }

    Error error = cursor_->FetchOne(&current_row_);
    if (error.occurred()) {
      return error;
    }
    if (current_row_.size() == 0) {
      done_ = true;
    }

    return Success();
  }

  Error Get(T *obj) {
    if (current_row_.size() == 0) {
      return Success();
    }

    T tmp_obj(&database_);
    tmp_obj.Init(current_row_);
    *obj = tmp_obj;

    return Success();
  }

  Error Dump(std::vector<T> *result) {
    result->clear();
    while (!done_) {
      T obj(&database_);
      Error error = Get(&obj);
      if (error.occurred()) {
        return error;
      }
      result->push_back(obj);
      error = Next();
      if (error.occurred()) {
        return error;
      }
    }
    return Success();
  }

 private:
  Database &database_;
  Backend::Cursor *cursor_;
  bool done_;
  Record current_row_;
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_CURSOR_H_
