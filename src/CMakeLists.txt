add_library(
        core
        parser.cpp
)

add_executable(app
        main.cpp
        parser.cpp
)

target_include_directories(core PUBLIC ../include)