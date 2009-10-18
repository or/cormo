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

#include "cormo/select_query.h"

#include <string>

#include "cormo/field.h"
#include "cormo/expression.h"
#include "cormo/string.h"

namespace cormo {

using std::string;

SelectQuery &SelectQuery::Distinct() {
  return Distinct();
}


SelectQuery &SelectQuery::Distinct(bool distinct) {
  distinct_ = distinct;
  return *this;
}


SelectQuery &SelectQuery::Limit(int limit) {
  limit_ = limit;
  return *this;
}


SelectQuery &SelectQuery::Offset(int offset) {
  offset_ = offset;
  return *this;
}


SelectQuery &SelectQuery::AddResult(const string &result) {
  return AddResultAs(result, "");
}


SelectQuery &SelectQuery::AddResultAs(const string &result,
                                      const string &alias) {
  if (alias.empty()) {
    results_.push_back(result);
    result_aliases_.push_back(result);
  } else {
    results_.push_back(result + " AS " + alias);
    result_aliases_.push_back(alias);
  }
  return *this;
}


SelectQuery &SelectQuery::AddResult(const Field &result) {
  return AddResult(result.full_name());
}


SelectQuery &SelectQuery::AddResultAs(const Field &result,
                                      const string &alias) {
  return AddResultAs(result.full_name(), alias);
}


void SelectQuery::ClearResults() {
  results_.clear();
  result_aliases_.clear();
}


SelectQuery &SelectQuery::AddSource(const string &source) {
  return AddSourceAs(source, "");
}


SelectQuery &SelectQuery::AddSourceAs(const string &source,
                                      const string &alias) {
  if (alias.empty()) {
    sources_.push_back(source);
    source_aliases_.push_back(source);
  } else {
    sources_.push_back(source + " AS " + alias);
    source_aliases_.push_back(alias);
  }
  return *this;
}


SelectQuery &SelectQuery::LeftJoin(const string &source,
                                   const Field &left,
                                   const Field &right) {
  return LeftJoinAs(source, "", left, right);
}


SelectQuery &SelectQuery::LeftJoinAs(const string &source,
                                     const string &alias,
                                     const Field &left,
                                     const Field &right) {
  return LeftJoinAs(source, alias, left.full_name(), right.full_name());
}


SelectQuery &SelectQuery::LeftJoin(const std::string &source,
                                   const std::string &left,
                                   const std::string &right) {
  return LeftJoinAs(source, "", left, right);
}


SelectQuery &SelectQuery::LeftJoinAs(const std::string &source,
                                     const std::string &alias,
                                     const std::string &left,
                                     const std::string &right) {
  if (alias.empty()) {
    left_joins_.push_back("LEFT JOIN " + source + " ON " +
                            left + " = " + right);
    left_join_aliases_.push_back(source);
  } else {
    left_joins_.push_back("LEFT JOIN " + source + " AS " + alias + " ON " +
                            left + " = " + right);
    left_join_aliases_.push_back(alias);
  }
  return *this;
}


SelectQuery &SelectQuery::Where(const Expression &expression) {
  where_ = And(RawExpression(where_), expression).ToString();
  return *this;
}


SelectQuery &SelectQuery::Where(const string &expression) {
  where_ = And(RawExpression(where_), RawExpression(expression)).ToString();
  return *this;
}


SelectQuery &SelectQuery::GroupBy(const string &group_by) {
  group_by_.push_back(group_by);
  return *this;
}


SelectQuery &SelectQuery::GroupBy(const Field &group_by) {
  group_by_.push_back(group_by.full_name());
  return *this;
}


SelectQuery &SelectQuery::Having(const Expression &having) {
    having_ = having.ToString();
    return *this;
}


SelectQuery &SelectQuery::Having(const string &having) {
    having_ = having;
    return *this;
}


SelectQuery &SelectQuery::OrderBy(const string &order_by) {
  return OrderByAsc(order_by);
}


SelectQuery &SelectQuery::OrderBy(const Field &order_by) {
  return OrderBy(order_by.full_name());
}


SelectQuery &SelectQuery::OrderByAsc(const string &order_by) {
  order_by_.push_back(order_by);
  return *this;
}


SelectQuery &SelectQuery::OrderByAsc(const Field &order_by) {
  return OrderByAsc(order_by.full_name());
}


SelectQuery &SelectQuery::OrderByDesc(const string &order_by) {
  string tmp = order_by + " DESC";
  order_by_.push_back(tmp);
  return *this;
}


SelectQuery &SelectQuery::OrderByDesc(const Field &order_by) {
  return OrderByDesc(order_by.full_name());
}


string SelectQuery::ToString() const {
  string query = "SELECT ";
  if (distinct_) { query += "DISTINCT "; }
  query += results_.Join(",");
  query += " FROM ";
  query += sources_.Join(",");
  query += " ";
  if (left_joins_.size() > 0) {
    query += left_joins_.Join(" ");
  }
  if (!where_.empty() &&
      where_ != Expression::kTrue) { query += " WHERE " + where_; }
  if (group_by_.size() > 0) { query += " GROUP BY " + group_by_.Join(","); }
  if (having_ != "") { query += " HAVING " + having_; }
  if (order_by_.size() > 0) { query += " ORDER BY " + order_by_.Join(","); }
  if (limit_) { query += " LIMIT " + cormo::ToString(limit_); }
  if (offset_) { query += " OFFSET " + cormo::ToString(offset_); }

  return query;
}

}  // namespace cormo
