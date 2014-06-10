cmake_minimum_required(VERSION 2.8)

project(formatstring)

set(FORMATSTRING_MAJOR_VERSION 0)
set(FORMATSTRING_MINOR_VERSION 9)
set(FORMATSTRING_PATCH_VERSION 0)
set(FORMATSTRING_NAME "formatstring${FORMATSTRING_MAJOR_VERSION}${FORMATSTRING_MINOR_VERSION}")
set(FORMATSTRING_VERSION "${FORMATSTRING_MAJOR_VERSION}.${FORMATSTRING_MINOR_VERSION}.${FORMATSTRING_PATCH_VERSION}")

option(WITH_EXAMPLES "Build examples." OFF)
option(WITH_TESTS "Build tests." OFF)

if(MSVC)
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	# Update if necessary
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -pedantic -Werror -O3")
endif()

include(CheckCXXSourceCompiles)
include(CheckCXXSourceRuns)

check_cxx_source_compiles("
#include <ios>
int main() {
	std::cout.setf(std::ios::hexfloat, std::ios::floatfield);
	return 0;
}
" FORMATSTRING_IOS_HEXFLOAT_SUPPORT)

check_cxx_source_runs("
#include <cstdio>
#include <cstring>
int main() {
	wchar_t buf[64];
	if (std::swprintf(buf, sizeof(buf), L\"%.3La\", (long double)1200.0) < 0) return 1;
	return std::wcscmp(buf, L\"0x9.600p+7\");
}
" FORMATSTRING_PRINTF_HEXFLOAT_SUPPORT)

check_cxx_source_compiles("
#include <iostream>
#include <sstream>
#include <locale>

int main() {
	std::wstringstream buf;
	buf.imbue(std::locale(std::locale(), new std::numpunct<wchar_t>()));
	buf << L\"test\" << L'\\\\n';
	return 0;
}
" FORMATSTRING_WCHAR_SUPPORT)

if(NOT(FORMATSTRING_WCHAR_SUPPORT))
	message(FATAL_ERROR "No wide string support (std::wstring, wchar_t, std::numpunct<wchar_t>, ...) detected.")
endif()

check_cxx_source_compiles("
#include <iostream>
#include <sstream>
#include <locale>

int main() {
	std::basic_stringstream<char16_t> buf;
	buf.imbue(std::locale(std::locale(), new std::numpunct<char16_t>()));
	buf << u\"test\" << u'\\\\u000a';
	return 0;
}
" FORMATSTRING_CHAR16_SUPPORT)

check_cxx_source_compiles("
#include <iostream>
#include <sstream>
#include <locale>

int main() {
	std::basic_stringstream<char32_t> buf;
	buf.imbue(std::locale(std::locale(), new std::numpunct<char32_t>()));
	buf << U\"test\" << U'\\\\U0000000a';
	return 0;
}
" FORMATSTRING_CHAR32_SUPPORT)

include_directories(include "${CMAKE_CURRENT_BINARY_DIR}/include")

# from libpng's CMakeFile.txt
macro(create_symlink SRC_FILE DEST_FILE)
	file(REMOVE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${DEST_FILE})
	if(WIN32 AND NOT CYGWIN AND NOT MSYS)
		add_custom_command(
			OUTPUT ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${DEST_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${DEST_FILE}
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SRC_FILE}" ${CMAKE_CURRENT_BINARY_DIR}/${DEST_FILE}
			DEPENDS ${PNG_LIB_TARGETS})
		add_custom_target(${DEST_FILE}_COPY ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${DEST_FILE})
	else()
		get_filename_component(LINK_TARGET "${SRC_FILE}" NAME)
		execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${LINK_TARGET}" ${DEST_FILE} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
	endif()
endmacro()

# for CMAKE_INSTALL_LIBDIR:
include(GNUInstallDirs)

if(NOT WIN32 OR CYGWIN OR MINGW)
	configure_file(formatstring.pc.in
		${CMAKE_CURRENT_BINARY_DIR}/${FORMATSTRING_NAME}.pc @ONLY)

	create_symlink(${FORMATSTRING_NAME}.pc formatstring.pc)

	install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/${FORMATSTRING_NAME}.pc
		${CMAKE_CURRENT_BINARY_DIR}/formatstring.pc
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

if(WITH_EXAMPLES)
	add_subdirectory(examples)
endif()

add_subdirectory(src)

if(WITH_TESTS)
	add_subdirectory(test)
endif()

# uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
