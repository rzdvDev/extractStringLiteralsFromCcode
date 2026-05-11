cmake_minimum_required(VERSION 4.2)
project(extractStringLiteralsFromCcode)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
set(CMAKE_EXECUTABLE_ENABLE_EXPORTS OFF)

if(MINGW)
	get_filename_component(MINGW_RUNTIME_DLL_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
	set(MINGW_RUNTIME_DLLS
			"${MINGW_RUNTIME_DLL_DIR}/libgcc_s_seh-1.dll"
			"${MINGW_RUNTIME_DLL_DIR}/libstdc++-6.dll"
			"${MINGW_RUNTIME_DLL_DIR}/libwinpthread-1.dll")
	foreach(dll_path IN LISTS MINGW_RUNTIME_DLLS)
		if(EXISTS "${dll_path}")
			file(COPY "${dll_path}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/src")
			file(COPY "${dll_path}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/tests")
		endif()
	endforeach()
endif()


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

