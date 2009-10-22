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

#include "cormo/value.h"

#include <cstdlib>
#include <string>

#include "cormo/date_time.h"
#include "cormo/string.h"

namespace cormo {

using std::string;

template <>
string ConvertTo<string>(int value) {
  return ToString(value);
}


template <>
float ConvertTo<float>(int value) {
  return value;
}


template <>
bool ConvertTo<bool>(int value) {
  return value;
}


template <>
string ConvertTo<string>(float value) {
  return ToString(value);
}


template <>
string ConvertTo<string>(bool value) {
  return ToString(value);
}


template <>
string ConvertTo<string>(const DateTime &value) {
  return value.ToString();
}


template <>
int ConvertTo<int>(const string &value) {
  return strtol(value.c_str(), NULL, 10);
}


template <>
bool ConvertTo<bool>(const string &value) {
  return ConvertTo<int>(value);
}


template <>
float ConvertTo<float>(const string &value) {
  return strtof(value.c_str(), NULL);
}


template <>
DateTime ConvertTo<DateTime>(const string &value) {
  return DateTime(value);
}


template <>
string ConvertTo<string>(const string &value) {
  return value;
}


template <>
Value<value_types::Integer> ConvertTo(const Value<value_types::String> &value) {
  if (value.is_null()) {
    return Value<value_types::Integer>();
  }
  return Value<value_types::Integer>(
                              ConvertTo<value_types::Integer>(value.get()));
}


template <>
Value<value_types::Boolean> ConvertTo(const Value<value_types::String> &value) {
  if (value.is_null()) {
    return Value<value_types::Boolean>();
  }
  return Value<value_types::Boolean>(
                              ConvertTo<value_types::Boolean>(value.get()));
}


template <>
Value<value_types::DateTime>
    ConvertTo(const Value<value_types::String> &value) {
  if (value.is_null()) {
    return Value<value_types::DateTime>();
  }
  return Value<value_types::DateTime>(
                              ConvertTo<value_types::DateTime>(value.get()));
}


template <>
Value<value_types::String> ConvertTo(const Value<value_types::String> &value) {
  return value;
}


template <>
Value<value_types::String> ConvertTo(bool value) {
  return Value<value_types::String>(ConvertTo<value_types::String>(value));
}


template <>
Value<value_types::String> ConvertTo(int value) {
  return Value<value_types::String>(ConvertTo<value_types::String>(value));
}


template <>
Value<value_types::String> ConvertTo(const Value<value_types::Boolean> &value) {
  if (value.is_null()) {
    return Value<value_types::String>();
  }
  return Value<value_types::String>(
                               ConvertTo<value_types::String>(value.get()));
}


template <>
Value<value_types::String> ConvertTo(const Value<value_types::Integer> &value) {
  if (value.is_null()) {
    return Value<value_types::String>();
  }
  return Value<value_types::String>(
                               ConvertTo<value_types::String>(value.get()));
}


template <>
Value<value_types::String>
  ConvertTo(const Value<value_types::DateTime> &value) {
  if (value.is_null()) {
    return Value<value_types::String>();
  }
  return Value<value_types::String>(
                               ConvertTo<value_types::String>(value.get()));
}


}  // namespace cormo
