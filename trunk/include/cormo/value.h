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

#ifndef CORMO_INCLUDE_CORMO_VALUE_H_
#define CORMO_INCLUDE_CORMO_VALUE_H_

#include <cassert>
#include <cstddef>

#include <string>

#include "cormo/constants.h"
#include "cormo/date_time.h"

namespace cormo {

namespace value_types {
  typedef bool Boolean;
  typedef int Integer;
  typedef std::string String;
  typedef cormo::DateTime DateTime;
}

template <class T>
class Value {
 public:
  Value() : value_(NULL) {}
  explicit Value(const T &value) : value_(new T(value)) {}
  Value(const Value<T> &value) : value_(NULL) {  // NOLINT
    *this = value;
  }
  ~Value() { reset(); }

  void reset() {
    if (is_null()) {
      return;
    }

    delete value_;
    value_ = NULL;
  }

  bool is_null() const {
    return value_ == NULL;
  }

  const T &get() const {
    assert(!is_null());
    return *value_;
  }

  T &get() {
    assert(!is_null());
    return *value_;
  }

  const T *operator*() const {
    return value_;
  }

  T *operator*() {
    return value_;
  }

  const T *operator->() const {
    return value_;
  }

  T *operator->() {
    return value_;
  }

  Value &operator=(const Value<T> &value) {
    reset();
    if (!value.is_null()) {
      value_ = new T(value.get());
    }
    return *this;
  }

  Value &operator=(const T &value) {
    reset();
    value_ = new T(value);
    return *this;
  }

  // !x
  T operator!() const {
    return !get();
  }

  // +x
  T operator+() const {
    return +get();
  }

  // -x
  T operator-() const {
    return -get();
  }

  // ~x
  T operator~() const {
    return ~get();
  }

  // ++x
  Value &operator++() {
    assert(!is_null());
    ++(*value_);
    return *this;
  }

  // x++
  Value &operator++(int dummy_for_postfix) {
    assert(!is_null());
    ++(*value_);
    return *this;
  }

  // --x
  Value &operator--() {
    assert(!is_null());
    --(*value_);
    return *this;
  }

  // x--
  Value &operator--(int dummy_for_postfix) {
    assert(!is_null());
    --(*value_);
    return *this;
  }

  // x+=
  template <class S>
  Value &operator+=(const S &value) {
    assert(!is_null());
    *value_ = *value_ + value;
    return *this;
  }

  // x-=
  template <class S>
  Value &operator-=(const S &value) {
    assert(!is_null());
    *value_ = *value_ - value;
    return *this;
  }

  // x*=
  template <class S>
  Value &operator*=(const S &value) {
    assert(!is_null());
    *value_ = *value_ * value;
    return *this;
  }

  // x/=
  template <class S>
  Value &operator/=(const S &value) {
    assert(!is_null());
    *value_ = *value_ / value;
    return *this;
  }

  // x%=
  template <class S>
  Value &operator%=(const S &value) {
    assert(!is_null());
    *value_ = *value_ % value;
    return *this;
  }

  // x<<=
  template <class S>
  Value &operator<<=(const S &value) {  // NOLINT
    assert(!is_null());
    *value_ = *value_ << value;
    return *this;
  }

  // x>>=
  template <class S>
  Value &operator>>=(const S &value) {  // NOLINT
    assert(!is_null());
    *value_ = *value_ >> value;
    return *this;
  }

  // x&=
  template <class S>
  Value &operator&=(const S &value) {
    assert(!is_null());
    *value_ = *value_ & value;
    return *this;
  }

  // x|=
  template <class S>
  Value &operator|=(const S &value) {
    assert(!is_null());
    *value_ = *value_ | value;
    return *this;
  }

  // x^=
  template <class S>
  Value &operator^=(const S &value) {
    assert(!is_null());
    *value_ = *value_ ^ value;
    return *this;
  }

 private:
  T *value_;

};

// a + b
template <class S, class T>
S operator+(const Value<S> &a, const Value<T> &b) {
  return a.get() + b.get();
}
template <class S, class T>
S operator+(const Value<S> &a, const T &b) {
  return a.get() + b;
}
template <class S, class T>
S operator+(const S &a, const Value<T> &b) {
  return a + b.get();
}

// a - b
template <class S, class T>
S operator-(const Value<S> &a, const Value<T> &b) {
  return a.get() - b.get();
}
template <class S, class T>
S operator-(const Value<S> &a, const T &b) {
  return a.get() - b;
}
template <class S, class T>
S operator-(const S &a, const Value<T> &b) {
  return a - b.get();
}

// a * b
template <class S, class T>
S operator*(const Value<S> &a, const Value<T> &b) {
  return a.get() * b.get();
}
template <class S, class T>
S operator*(const Value<S> &a, const T &b) {
  return a.get() * b;
}
template <class S, class T>
S operator*(const S &a, const Value<T> &b) {
  return a * b.get();
}

// a / b
template <class S, class T>
S operator/(const Value<S> &a, const Value<T> &b) {
  return a.get() / b.get();
}
template <class S, class T>
S operator/(const Value<S> &a, const T &b) {
  return a.get() / b;
}
template <class S, class T>
S operator/(const S &a, const Value<T> &b) {
  return a / b.get();
}

// a % b
template <class S, class T>
S operator%(const Value<S> &a, const Value<T> &b) {
  return a.get() % b.get();
}
template <class S, class T>
S operator%(const Value<S> &a, const T &b) {
  return a.get() % b;
}
template <class S, class T>
S operator%(const S &a, const Value<T> &b) {
  return a % b.get();
}

// a << b
template <class S, class T>
S operator<<(const Value<S> &a, const Value<T> &b) {
  return a.get() << b.get();
}
template <class S, class T>
S operator<<(const Value<S> &a, const T &b) {
  return a.get() << b;
}
template <class S, class T>
S operator<<(const S &a, const Value<T> &b) {
  return a << b.get();
}

// a >> b
template <class S, class T>
S operator>>(const Value<S> &a, const Value<T> &b) {
  return a.get() >> b.get();
}
template <class S, class T>
S operator>>(const Value<S> &a, const T &b) {
  return a.get() >> b;
}
template <class S, class T>
S operator>>(const S &a, const Value<T> &b) {
  return a >> b.get();
}

// a & b
template <class S, class T>
S operator&(const Value<S> &a, const Value<T> &b) {
  return a.get() & b.get();
}
template <class S, class T>
S operator&(const Value<S> &a, const T &b) {
  return a.get() & b;
}
template <class S, class T>
S operator&(const S &a, const Value<T> &b) {
  return a & b.get();
}

// a | b
template <class S, class T>
S operator|(const Value<S> &a, const Value<T> &b) {
  return a.get() | b.get();
}
template <class S, class T>
S operator|(const Value<S> &a, const T &b) {
  return a.get() | b;
}
template <class S, class T>
S operator|(const S &a, const Value<T> &b) {
  return a | b.get();
}

// a ^ b
template <class S, class T>
S operator^(const Value<S> &a, const Value<T> &b) {
  return a.get() ^ b.get();
}
template <class S, class T>
S operator^(const Value<S> &a, const T &b) {
  return a.get() ^ b;
}
template <class S, class T>
S operator^(const S &a, const Value<T> &b) {
  return a ^ b.get();
}

// a < b
template <class S, class T>
bool operator<(const Value<S> &a, const Value<T> &b) {
  return a.get() < b.get();
}
template <class S, class T>
bool operator<(const Value<S> &a, const T &b) {
  return a.get() < b;
}
/*template <class S, class T>
bool operator<(const S &a, const Value<T> &b) {
  return a < b.get();
}*/

// a <= b
template <class S, class T>
bool operator<=(const Value<S> &a, const Value<T> &b) {
  return a.get() <= b.get();
}
template <class S, class T>
bool operator<=(const Value<S> &a, const T &b) {
  return a.get() <= b;
}

// a > b
template <class S, class T>
bool operator>(const Value<S> &a, const Value<T> &b) {
  return a.get() > b.get();
}
template <class S, class T>
bool operator>(const Value<S> &a, const T &b) {
  return a.get() > b;
}

// a >= b
template <class S, class T>
bool operator>=(const Value<S> &a, const Value<T> &b) {
  return a.get() >= b.get();
}
template <class S, class T>
bool operator>=(const Value<S> &a, const T &b) {
  return a.get() >= b;
}

// a == b
template <class S, class T>
bool operator==(const Value<S> &a, const Value<T> &b) {
  return a.get() == b.get();
}
template <class S, class T>
bool operator==(const Value<S> &a, const T &b) {
  return a.get() == b;
}

// a != b
template <class S, class T>
bool operator!=(const Value<S> &a, const Value<T> &b) {
  return a.get() != b.get();
}
template <class S, class T>
bool operator!=(const Value<S> &a, const T &b) {
  return a.get() != b;
}

// a && b
template <class S, class T>
bool operator&&(const Value<S> &a, const Value<T> &b) {
  return a.get() && b.get();
}
template <class S, class T>
bool operator&&(const Value<S> &a, const T &b) {
  return a.get() && b;
}

// a || b
template <class S, class T>
bool operator||(const Value<S> &a, const Value<T> &b) {
  return a.get() || b.get();
}
template <class S, class T>
bool operator||(const Value<S> &a, const T &b) {
  return a.get() || b;
}

template <class T>
T ConvertTo(int value);

template <class T>
T ConvertTo(float value);

template <class T>
T ConvertTo(bool value);

template <class T>
T ConvertTo(const DateTime &value);

template <class T>
T ConvertTo(const std::string &value);

template <class T>
T ConvertTo(const Value<value_types::String> &value);

template <class T>
T ConvertTo(const Value<value_types::Integer> &value);

template <class T>
T ConvertTo(const Value<value_types::Boolean> &value);

template <class T>
T ConvertTo(const Value<value_types::DateTime> &value);

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_VALUE_H_
