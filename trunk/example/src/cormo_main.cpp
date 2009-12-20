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

#include <cstdio>
#include <vector>

#include "cormo/cormo.h"
#include "cormo/database.h"
#include "cormo/data_source.h"
#include "model/test1.h"
#include "model/test2.h"

using std::vector;
using cormo::model::Test1;
using cormo::model::Test2;
using cormo::Select;
using cormo::DataSource;

int main(int argc, char *argv[]) {
  cormo::Database db(cormo::Backend::kPostgresql,
                     "host=localhost;"
                       "user=cormo_user;"
                       "password=cormo_password;"
                       "database=cormo_test");
  db.set_debug(true);

  cormo::Error error = db.Connect();
  if (error.occurred()) {
    fprintf(stderr, "Error: %s\n", error.message().c_str());
    return 1;
  }

  DataSource<Test1> test = Select<Test1>(&db).
                                Filter(Test1::Some_int < 0).OrderBy(Test1::Id);
  vector<Test1> objs;
  test.GetAll(&objs);
  for (size_t i = 0; i < objs.size(); ++i) {
    printf("name: ");
    if (objs[i].name().is_null()) {
      printf("NULL\n");
    } else {
      printf("'%s'\n", objs[i].name().get().c_str());
    }

    printf("xyz: ");
    if (objs[i].xyz().is_null()) {
      printf("NULL\n");
    } else {
      printf("'%s'\n", objs[i].xyz().get() ? "true" : "false");
    }

    printf("some_int: ");
    if (objs[i].some_int().is_null()) {
      printf("NULL\n");
    } else {
      printf("'%d'\n", objs[i].some_int().get());
    }

    printf("test2: ");
    if (objs[i].test2().is_null()) {
      printf("NULL\n");
    } else {
      printf("'%s'\n", objs[i].test2().get().name().get().c_str());
    }
    printf("\n");
  }

  if (!objs.size()) {
    printf("none found... adding one.\n");
    Test1 stuff(&db);
    stuff.set_xyz(true);
    objs.push_back(stuff);
  }

  Test1 new_obj(&db);
  new_obj.set_name("Cool.");
  new_obj.set_some_int(-1);
  new_obj.set_some_int(2 * -new_obj.some_int());
  new_obj.some_int()++;
  ++new_obj.some_int();
  error = new_obj.Save();
  new_obj.some_int() -= 2 * new_obj.id();
  new_obj.set_xyz(objs[0].xyz());
  error = new_obj.Save();
  if (error.occurred()) {
    fprintf(stderr, "Error: %s\n", error.message().c_str());
    return 1;
  }
  printf("saved object's id: %d\n", new_obj.id().get());
  objs[0].set_name(objs[0].name() + "_");
  error = objs[0].Save();
  if (error.occurred()) {
    fprintf(stderr, "Error: %s\n", error.message().c_str());
    return 1;
  }

  if (objs.size() > 1) {
    error = objs[1].Delete();
    if (error.occurred()) {
      fprintf(stderr, "Error: %s\n", error.message().c_str());
      return 1;
    }
    Test2 new_obj2(&db);
    new_obj2.set_name("FIRLEFANZ!");
    objs[1].set_test2(new_obj2);
    error = objs[1].Save();
    if (error.occurred()) {
      fprintf(stderr, "Error: %s\n", error.message().c_str());
      return 1;
    }
  }

  return 0;
}
