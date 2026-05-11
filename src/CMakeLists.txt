add_library(
        core
        parser.cpp
        file_io.cpp
        ../include/file_io.h
)

target_include_directories(core PUBLIC ../include)

add_executable(app
        main.cpp
)

set_target_properties(app PROPERTIES ENABLE_EXPORTS OFF)

target_link_libraries(app PRIVATE core)

if(MINGW)
endif()
