import os

env = Environment()
env.Append(CCFLAGS = '-Wall -g')
env.Append(CPPPATH = ['example/include',
                      'include',
                      '/usr/include/postgresql/'])

os.system('generator/generate_model_code.py --source example/src --include example/include --project cormo --models example/models.py --header example/header.txt')

env.Program('build/cormo', Glob('example/src/*.cpp') +
                           Glob('example/src/library/*.cpp') +
                           Glob('example/src/model/*.cpp') +
                           Glob('example/src/model/generated/*.cpp') +
                           Glob('src/*.cpp') +
                           Glob('src/library/*.cpp'),
            LIBS=['pq'])

os.system('./run_cpplint')