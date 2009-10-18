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
     : database_(*database), cursor_(cursor), done_(false), got_row_(false) {
    operator++();
  }
  ~Cursor() { delete cursor_; }

  Cursor<T> &operator++() {
    if (done_) {
      return *this;
    }

    current_row_ = cursor_->FetchOne();
    if (current_row_.size() == 0) {
      done_ = true;
      got_row_ = false;
    } else {
      got_row_ = true;
    }

    return *this;
  }

  T operator*() {
    T obj(&database_);
    if (!got_row_) {
      return obj;
    }

    obj.Init(current_row_);
    return obj;
  }

  std::vector<T> dump() {
    std::vector<T> result;
    for ( ; !done_; operator++()) {
      result.push_back(operator*());
    }
    return result;
  }

 private:
  Database &database_;
  Backend::Cursor *cursor_;
  bool done_;
  bool got_row_;
  Record current_row_;
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_CURSOR_H_
