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

#ifndef CORMO_INCLUDE_CORMO_EXPRESSION_H_
#define CORMO_INCLUDE_CORMO_EXPRESSION_H_

#include <string>

#include "cormo/constants.h"
#include "cormo/field.h"
#include "cormo/record.h"
#include "cormo/string_list.h"
#include "cormo/string.h"
#include "cormo/value.h"

// TODO(thi): some better unification of values, NULL, how is NULL handled?

namespace cormo {

class SelectQuery;

class Expression {
 public:
  virtual ~Expression() {}

  virtual std::string ToString() const { return kTrue; }

  const StringList &extra_tables() const;

  static const char *kTrue;
  static const char *kFalse;

 protected:
  Expression() {}
};


class RawExpression : public Expression {
 public:
  explicit RawExpression(const std::string &raw_expression)
      : raw_(raw_expression) {}

  virtual std::string ToString() const {
    if (raw_.empty()) {
      return kTrue;
    }
    return raw_;
  }

 private:
  std::string raw_;
};


class Not : public Expression {
 public:
  explicit Not(const Expression &e) : e_(e) {}

  virtual std::string ToString() const {
    return "NOT (" + e_.ToString() + ")";
  }

 private:
  const Expression &e_;
};


class Connective : public Expression {
 public:
  virtual ~Connective() {}
  virtual std::string ToString() const {
    return "(" + e1_.ToString() + ") " + op_ + " (" + e2_.ToString() + ")";
  }

 protected:
  Connective(const Expression &e1, const std::string &op,
             const Expression &e2)
      : e1_(e1), e2_(e2), op_(op) {}

  const Expression &e1_, &e2_;

 private:
  std::string op_;
};


class And : public Connective {
 public:
  And(const Expression &e1, const Expression &e2)
      : Connective(e1, "AND", e2) {}
};


class Or : public Connective {
 public:
  Or(const Expression &e1, const Expression &e2)
      : Connective(e1, "OR", e2) {}
};


class Operator : public Expression {
 public:
  virtual std::string ToString() const {
    // TODO(thi): special cases for NULL in the case of '==' and '!='?
    return field_.full_name() + " " +
             op_ + " " +
             (escape_ ? EscapeSql(value_) : value_.get());
  }

 protected:
  Operator(const Field &field,
           const std::string &op,
           const std::string &value)
      : field_(field), op_(op), value_(value), escape_(true) {}

  template <class T>
  Operator(const Field &field,
           const std::string &op,
           const Value<T> &value)
      : field_(field), op_(op),
        value_(ConvertTo<RecordValue>(value)), escape_(true) {}

  Operator(const Field &field,
           const std::string &op,
           const Field &field2)
      : field_(field), op_(op), value_(field2.full_name()), escape_(false) {}

 private:
  const Field &field_;
  std::string op_;
  RecordValue value_;
  bool escape_;
};


class Equal : public Operator {
 public:
  Equal(const Field &field, const std::string &value)
      : Operator(field, "=", value) {}
  template <class T>
  Equal(const Field &field, const Value<T> &value)
      : Operator(field, "=", value) {}
  Equal(const Field &field, const Field &field2)
      : Operator(field, "=", field2) {}
};


class NotEqual : public Operator {
 public:
  NotEqual(const Field &field, const std::string &value)
      : Operator(field, "<>", value) {}
  template <class T>
  NotEqual(const Field &field, const Value<T> &value)
      : Operator(field, "<>", value) {}
  NotEqual(const Field &field, const Field &field2)
      : Operator(field, "<>", field2) {}
};


class GreaterThan : public Operator {
 public:
  GreaterThan(const Field &field, const std::string &value)
      : Operator(field, ">", value) {}
  template <class T>
  GreaterThan(const Field &field, const Value<T> &value)
      : Operator(field, ">", value) {}
  GreaterThan(const Field &field, const Field &field2)
      : Operator(field, ">", field2) {}
};


class GreaterThanOrEqual : public Operator {
 public:
  GreaterThanOrEqual(const Field &field, const std::string &value)
      : Operator(field, ">=", value) {}
  template <class T>
  GreaterThanOrEqual(const Field &field, const Value<T> &value)
      : Operator(field, ">=", value) {}
  GreaterThanOrEqual(const Field &field, const Field &field2)
      : Operator(field, ">=", field2) {}
};


class LowerThan : public Operator {
 public:
  LowerThan(const Field &field, const std::string &value)
      : Operator(field, "<", value) {}
  template <class T>
  LowerThan(const Field &field, const Value<T> &value)
      : Operator(field, "<", value) {}
  LowerThan(const Field &field, const Field &field2)
      : Operator(field, "<", field2) {}
};


class LowerThanOrEqual : public Operator {
 public:
  LowerThanOrEqual(const Field &field, const std::string &value)
      : Operator(field, "<=", value) {}
  template <class T>
  LowerThanOrEqual(const Field &field, const Value<T> &value)
      : Operator(field, "<=", value) {}
  LowerThanOrEqual(const Field &field, const Field &field2)
      : Operator(field, "<=", field2) {}
};


class Like : public Operator {
 public:
  Like(const Field &field, const std::string &value)
      : Operator(field, "LIKE", value) {}
};

class LikeNoCase : public Operator {
 public:
  LikeNoCase(const Field &field, const std::string &value)
      : Operator(field, "ILIKE", value) {}
};


template <class T>
Equal operator==(const Field &f1, const T &f2) {
  return Equal(f1, ConvertTo<RecordValue>(f2));
}


template <class T>
NotEqual operator!=(const Field &f1, const T &f2) {
  return NotEqual(f1, ConvertTo<RecordValue>(f2));
}


template <class T>
GreaterThan operator>(const Field &f1, const T &f2) {
  return GreaterThan(f1, ConvertTo<RecordValue>(f2));
}


template <class T>
GreaterThanOrEqual operator>=(const Field &f1, const T &f2) {
  return GreaterThanOrEqual(f1, ConvertTo<RecordValue>(f2));
}


template <class T>
LowerThan operator<(const Field &f1, const T &f2) {
  return LowerThan(f1, ConvertTo<RecordValue>(f2));
}

template <class T>
LowerThanOrEqual operator<=(const Field &f1, const T &f2) {
  return LowerThanOrEqual(f1, ConvertTo<RecordValue>(f2));
}


}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_EXPRESSION_H_
