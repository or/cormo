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

import re

Boolean  = 'Boolean'
Integer  = 'Integer'
String   = 'String'
DateTime = 'DateTime'
Types = [Boolean, Integer, String, DateTime, '__FK__']

class Field(object):
  def __init__(self, type, field=None):
    if type not in Types:
      raise Exception("'%s' is not a valid Field type." % (type))

    self.type = type
    self.field = field

  def f_type(self):
    return 'kType' + self.type


class ForeignKey(Field):
  def __init__(self, model_name, field=None):
    super(ForeignKey, self).__init__('__FK__', field)
    self.model_name = model_name
    self.file_name = re.sub(r'(.)(?=[A-Z])', r'\1_', self.model_name).lower()


class Model(object):
  __fields__ = {}
  __field_order__ = []
  __foreign_keys__ = {}
  __foreign_key_order__ = []

  table_name = ''

  id = Field(Integer)
