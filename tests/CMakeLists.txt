include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests
        test_utils.cpp
        test_utils.h
        currentChar_tests.cpp
        Advance_tests.cpp
        startsWith_tests.cpp
        skipUntil_tests.cpp
        main_func_tests.cpp)

target_link_libraries(unit_tests
        gtest
        gtest_main
        core
)

include(GoogleTest)
gtest_discover_tests(unit_tests)