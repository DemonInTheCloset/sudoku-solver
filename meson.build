project('Sudoku Solver', 'cpp', default_options : ['cpp_std=c++20'])

compiler_flags = ['-Wshadow', '-Wconversion', '-Wsign-conversion', '-Wnull-dereference', '-Wold-style-cast',
  '-Wmisleading-indentation', '-Wdouble-promotion']
add_global_arguments(compiler_flags, language : 'cpp')

gtest_dep = dependency('gtest')
src_dir = include_directories('src')

sudoku_solver_test = executable('parser_test', 'test/test.cpp', dependencies : gtest_dep, include_directories : src_dir)
# executable('parser', 'src/main.cpp')

test('sudoku_solver_test', sudoku_solver_test)
