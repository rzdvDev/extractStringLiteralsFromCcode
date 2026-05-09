cmake_minimum_required(VERSION 4.2)
project(extractStringLiteralsFromCcode)

set(CMAKE_CXX_STANDARD 20)

add_executable(extractStringLiteralsFromCcode main.cpp
        include/parser.h
        parser.cpp
        tests/test_utils.cpp
        tests/test_utils.h
        tests/currentChar_tests.cpp
        tests/Advance_tests.cpp
        tests/startsWith_tests.cpp
        tests/skipUntil_tests.cpp
        tests/main_func_tests.cpp)
