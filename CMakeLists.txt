cmake_minimum_required(VERSION 4.2)
project(extractStringLiteralsFromCcode)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests)

option(BUILD_DOCS "Generate API documentation with Doxygen" ON)

if(BUILD_DOCS)
	find_package(Doxygen QUIET)

	if(DOXYGEN_FOUND)
		set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
		set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

		add_custom_target(docs
				COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
				WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
				COMMENT "Generating API documentation with Doxygen"
				VERBATIM)
	else()
		message(STATUS "Doxygen was not found. The 'docs' target will not be available.")
	endif()
endif()

