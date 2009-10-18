# Copyright (C) 2009 Oliver Runge (thi@thialfihar.org)
#
# This file is part of cormo.
#
# cormo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# cormo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with cormo.  If not, see <http://www.gnu.org/licenses/>.

from model import *

class Test1(Model):
    table_name = 'cormo_test1'

    name     = Field(String)
    xyz      = Field(Boolean)
    some_int = Field(Integer)
    test2    = ForeignKey('Test2')

class Test2(Model):
    table_name = 'cormo_test2'

    name     = Field(String)


"""
CREATE TABLE cormo_test2
(
  id serial NOT NULL,
  "name" character varying(80),
  CONSTRAINT cormo_test2_pkey PRIMARY KEY (id)
);

CREATE TABLE cormo_test1
(
  id serial NOT NULL,
  "name" character varying(80),
  "xyz" boolean,
  "some_int" integer,
  "test2_id" integer,
  CONSTRAINT cormo_test1_pkey PRIMARY KEY (id),
  CONSTRAINT cormo_test1_test2_fkey FOREIGN KEY (test2_id)
      REFERENCES cormo_test2 (id)
);
"""