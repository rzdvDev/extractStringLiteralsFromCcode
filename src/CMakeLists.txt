add_library(
        core
        parser.cpp
        file_io.cpp
        ../include/file_io.h
)

add_executable(app
        main.cpp
        parser.cpp
        file_io.cpp
        ../include/file_io.h
)

target_include_directories(core PUBLIC ../include)