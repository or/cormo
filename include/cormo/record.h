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

#ifndef CORMO_INCLUDE_CORMO_RECORD_H_
#define CORMO_INCLUDE_CORMO_RECORD_H_

#include <vector>

#include "cormo/constants.h"
#include "cormo/value.h"

namespace cormo {

class Database;
typedef Value<value_types::String> RecordValue;

class Record : public std::vector<RecordValue> {
 public:
  Record() : std::vector<RecordValue>() {}
};

typedef std::vector<Record> Records;

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_RECORD_H_
