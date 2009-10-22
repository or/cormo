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

#include "cormo/string_list.h"

#include <malloc.h>
#include <cassert>
#include <cstring>

#include <string>
#include <vector>

namespace cormo {

using std::string;
using std::vector;

StringList::StringList(const string &str, const string &delimiter) {
  assert(delimiter != "");

  char *buf = strdup(str.c_str());
  char *ptr = buf;
  int len = delimiter.size();
  vector<char*> pointers;
  pointers.push_back(ptr);
  while ((ptr = strstr(ptr, delimiter.c_str()))) {
    *ptr = '\0';
    ptr += len;
    pointers.push_back(ptr);
  }
  for (vector<char*>::iterator i = pointers.begin();
       i != pointers.end();
       ++i) {
    push_back(string(*i));
  }

  if (buf) {
    free(buf);
  }
}


string StringList::Join(const string &delimiter) const {
  string result;
  for (const_iterator i = begin(); i != end(); ++i) {
    if (i != begin()) {
       result += delimiter;
    }
    result += *i;
  }

  return result;
}

}  // namespace cormo
