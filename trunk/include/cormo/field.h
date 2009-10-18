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

#ifndef CORMO_INCLUDE_CORMO_FIELD_H_
#define CORMO_INCLUDE_CORMO_FIELD_H_

#include <string>

#include "cormo/constants.h"

namespace cormo {

class Field {
 public:
  enum TypeEnum {kTypeBoolean,
                 kTypeInteger,
                 kTypeString,
                 kTypeDateTime};

  Field(const std::string &table,
        const std::string &name, TypeEnum type)
      : table_(table), name_(name), type_(type) {}

  const std::string &table() const { return table_; }
  const std::string &name() const { return name_; }
  TypeEnum type() const { return type_; }

  std::string full_name() const {
    return table_ + "." + name_;
  }

 private:
  std::string table_;
  std::string name_;
  TypeEnum type_;

  DISALLOW_COPY_AND_ASSIGN(Field);
};

}

#endif  // CORMO_INCLUDE_CORMO_FIELD_H_
