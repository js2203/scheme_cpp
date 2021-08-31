cmake_minimum_required(VERSION 3.19)
project(scheme_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(scheme_cpp main.cpp memory.cpp memory.h scheme_basics.cpp scheme_basics.h repl.cpp repl.h parser.cpp parser.h scheme_getter.cpp scheme_getter.h environment.cpp environment.h trampoline.cpp trampoline.h evaluate.cpp evaluate.h scheme_builtInFunc.cpp scheme_builtInFunc.h scheme_syntax.cpp scheme_syntax.h init.cpp init.h)