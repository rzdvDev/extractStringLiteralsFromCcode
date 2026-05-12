include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
        test_utils.cpp
        test_utils.h
        currentChar_tests.cpp
        advance_tests.cpp
        startsWith_tests.cpp
        skipUntil_tests.cpp
        main_func_tests.cpp)

target_link_libraries(unit_tests
        GTest::gtest
        GTest::gtest_main
        core
)

if(MINGW)
endif()

include(GoogleTest)
gtest_discover_tests(unit_tests)