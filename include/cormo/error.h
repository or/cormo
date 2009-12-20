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

#ifndef CORMO_INCLUDE_CORMO_ERROR_H_
#define CORMO_INCLUDE_CORMO_ERROR_H_

#include <exception>
#include <string>

namespace cormo {

class Error : public std::exception {
 public:
  static const unsigned int ID = 0xffffffff;
  static const unsigned int UNINITIALIZED_ID = 0xfffffffe;
  static const unsigned int SUCCESS_ID = 0x00000000;

  Error() : id_(UNINITIALIZED_ID), message_("") {}
  explicit Error(const std::string &message) : id_(ID), message_(message) {}
  Error(unsigned int id, const std::string &message) : id_(id),
                                                       message_(message) {}
  ~Error() throw() {}

  const std::string &message() const { return message_; }
  unsigned int id() const { return id_; }
  bool occurred() const { return id() != SUCCESS_ID; }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  unsigned int id_;
  std::string message_;
};

class Success : public Error {
 public:
  static const unsigned int ID = Error::SUCCESS_ID;
  Success() : Error(ID, "") {}
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_ERROR_H_
