#!/usr/bin/python -W ignore::DeprecationWarning

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

import imp
import inspect
import os
import re
import sys
from optparse import OptionParser

from Cheetah.Template import Template

import model

base_path = '/'.join(sys.argv[0].split('/')[:-1]) + '/'

parser = OptionParser()
parser.add_option("-s", "--source", help="source directory", default="./src/")
parser.add_option("-i", "--include", help="include directory", default="./include/")
parser.add_option("-m", "--models", help="models file to load", default=None)
parser.add_option("-H", "--header", help="header", default=None)
parser.add_option("-p", "--project", help="project name for which to generate the models", default='cormo')
parser.add_option("-n", "--namespace", help="namespace for the models to live in (inside cormo::model)", default=None)

(options, args) = parser.parse_args()
if not options.source.endswith('/'):
  options.source += '/'
if not options.include.endswith('/'):
  options.include += '/'
if not options.models:
  import models
else:
  models = imp.load_source('models', options.models)

if options.header:
    header_comment = '/* '
    for line in file(options.header, 'r'):
        header_comment += line + ' * '

    if header_comment and header_comment[-1] == ' ':
        header_comment = header_comment[:-1]
    header_comment += '/\n'
    options.header = header_comment

classes = {}

id_field = model.Model.id
id_field.name = 'id'

for k in models.__dict__:
  m = models.__dict__[k]
  if inspect.isclass(m) and issubclass(m, model.Model) and m != model.Model:
    field_map = {}
    fields = []
    foreign_key_map = {}
    foreign_keys = []
    for pk in m.__dict__:
      p = m.__dict__[pk]
      if isinstance(p, model.ForeignKey):
        if not p.field:
          p.field = pk + '_id'
        p.name = pk
        foreign_key_map[pk] = p
        foreign_keys.append(p)
      elif isinstance(p, model.Field):
        p.name = pk
        if not p.field:
          p.field = p.name
        field_map[pk] = p
        fields.append(p)

    # for now -always- add the id field in first place, will result in
    # compile errors if the model had one as well
    field_map['id'] = id_field
    fields = [id_field] + fields

    m.__field_map__ = field_map
    m.__fields__ = fields
    m.__foreign_key_map__ = foreign_key_map
    m.__foreign_keys__ = foreign_keys
    m.name = k
    m.file_name = re.sub(r'(.)(?=[A-Z])', r'\1_', m.name).lower()
    m.name_base = k + 'Base'
    m.file_name_base = m.file_name + '_base'
    classes[k] = m

# check foreign keys
for model_name in classes:
  m = classes[model_name]
  for p in m.__foreign_keys__:
    if p.model_name not in classes:
      sys.stderr.write("Error:models: ForeignKey for model '%s', which was never defined\n" % p.model_name)
      sys.exit(1)

testing = True
create_check = False
for model_name in classes:
  model = classes[model_name]
  context = {'generator': 'cormo',
             'model': model,
             'models': classes.values(),
             'project': options.project,
             'own_namespace': options.namespace,
             'header': options.header,
             'model_namespace': 'model' + ('::' + options.namespace if options.namespace else ''),
             'full_namespace': 'cormo::model' + ('::' + options.namespace if options.namespace else '')}

  files = [('class_base.h-template', options.include + 'model/generated/' + model.file_name_base + '.h', True),
           ('class_base.cpp-template', options.source + 'model/generated/' + model.file_name_base + '.cpp', True),
           ('class.h-template', options.include + 'model/' + model.file_name + '.h', False),
           ('class.cpp-template', options.source + 'model/' + model.file_name + '.cpp', False),
           ('include_all.h-template', options.include + 'model/cormo_model%s.h' % ('_' + options.namespace if options.namespace else '',), True),
          ]
  for (template, file_name, always) in files:
    context['relative_path'] = '/'.join(re.sub(r'^./', '', file_name).split('/')[:-1])
    context['file_name'] = file_name.split('/')[-1]

    header_template = file(base_path + 'templates/' + template, 'r').read()
    t = Template(header_template, searchList=[context, model])
    file_path = '/'.join(file_name.split('/')[:-1])
    try:
        os.makedirs(file_path)
    except OSError, e:
        # path already existed... fair enough
        pass
    if always or not os.path.exists(file_name) or testing:
      print "generating %s..." % (file_name)
      file(file_name, 'w').write(str(t))
    elif not always and create_check:
      file(file_name + '.check', 'w').write(str(t))
