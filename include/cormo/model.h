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

#ifndef CORMO_INCLUDE_CORMO_MODEL_H_
#define CORMO_INCLUDE_CORMO_MODEL_H_

#include "cormo/constants.h"
#include "cormo/database.h"

namespace cormo {

class StringList;
class Record;

class Model {
 public:
  explicit Model(Database *database)
      : database_(database), is_initialized_(false) {}
  Model() : database_(NULL), is_initialized_(false) {}
  virtual ~Model() {}

  virtual void Save() = 0;
  virtual void Init(const Record &record) = 0;
  bool is_initialized() const { return is_initialized_; }

 protected:
  virtual void FillFieldsAndValues(StringList *fields,
                                   Record *record) = 0;

  Database *database_;
  bool is_initialized_;
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_MODEL_H_
