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

#include "cormo/expression.h"

#include "cormo/select_query.h"

namespace cormo {

const char *Expression::kTrue = "True";
const char *Expression::kFalse = "False";

And operator&&(const Expression &e1, const Expression &e2) {
  return And(e1, e2);
}


Or operator||(const Expression &e1, const Expression &e2) {
  return Or(e1, e2);
}


Not operator!(const Expression &e) {
  return Not(e);
}


Equal operator==(const Field &f1, const Field &f2) {
  return Equal(f1, f2);
}


NotEqual operator!=(const Field &f1, const Field &f2) {
  return NotEqual(f1, f2);
}


GreaterThan operator>(const Field &f1, const Field &f2) {
  return GreaterThan(f1, f2);
}


GreaterThanOrEqual operator>=(const Field &f1, const Field &f2) {
  return GreaterThanOrEqual(f1, f2);
}


LowerThan operator<(const Field &f1, const Field &f2) {
  return LowerThan(f1, f2);
}


LowerThanOrEqual operator<=(const Field &f1, const Field &f2) {
  return LowerThanOrEqual(f1, f2);
}


}  // namespace cormo

