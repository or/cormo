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

#ifndef CORMO_INCLUDE_CORMO_SELECT_QUERY_H_
#define CORMO_INCLUDE_CORMO_SELECT_QUERY_H_

#include <string>

#include "cormo/constants.h"
#include "cormo/string_list.h"

namespace cormo {

class Expression;
class Field;

class SelectQuery {
 public:
  SelectQuery() : distinct_(false), limit_(0), offset_(0) {}

  SelectQuery &Distinct(bool distinct);
  SelectQuery &Distinct();
  SelectQuery &Limit(int limit);
  SelectQuery &Offset(int offset);
  SelectQuery &AddResult(const std::string &result);
  SelectQuery &AddResultAs(const std::string &result,
                           const std::string &alias);
  SelectQuery &AddResult(const Field &result);
  SelectQuery &AddResultAs(const Field &result,
                           const std::string &alias);
  SelectQuery &AddSource(const std::string &source);
  SelectQuery &AddSourceAs(const std::string &source,
                           const std::string &alias);
  SelectQuery &LeftJoin(const std::string &source,
                        const Field &left,
                        const Field &right);
  SelectQuery &LeftJoinAs(const std::string &source,
                          const std::string &alias,
                          const Field &left,
                          const Field &right);
  SelectQuery &LeftJoin(const std::string &source,
                        const std::string &left,
                        const std::string &right);
  SelectQuery &LeftJoinAs(const std::string &source,
                          const std::string &alias,
                          const std::string &left,
                          const std::string &right);
  SelectQuery &Where(const Expression &expression);
  SelectQuery &Where(const std::string &expression);
  SelectQuery &GroupBy(const std::string &group_by);
  SelectQuery &GroupBy(const Field &group_by);
  SelectQuery &Having(const Expression &having);
  SelectQuery &Having(const std::string &having);
  SelectQuery &OrderBy(const std::string &order_by);
  SelectQuery &OrderBy(const Field &order_by);
  SelectQuery &OrderByAsc(const std::string &order_by);
  SelectQuery &OrderByAsc(const Field &order_by);
  SelectQuery &OrderByDesc(const std::string &order_by);
  SelectQuery &OrderByDesc(const Field &order_by);

  void ClearResults();

  std::string ToString() const;

 private:
  bool distinct_;
  int limit_;
  int offset_;
  StringList results_;
  StringList result_aliases_;
  StringList sources_;
  StringList source_aliases_;
  StringList left_joins_;
  StringList left_join_aliases_;
  std::string where_;
  std::string having_;
  StringList group_by_;
  StringList order_by_;

  // I want the default copy constructor here, so no dummy.
};

}  // namespace cormo

#endif  // CORMO_INCLUDE_CORMO_SELECT_QUERY_H_
