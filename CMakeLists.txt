cmake_minimum_required(VERSION 2.8)

project(formatstring)

set(FORMATSTRING_MAJOR_VERSION 1)
set(FORMATSTRING_MINOR_VERSION 0)
set(FORMATSTRING_PATCH_VERSION 0)

option(WITH_EXAMPLES "Build examples." OFF)

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

check_cxx_source_compiles("
#include <ios>
int main() {
	std::cout.setf(std::ios::hexfloat, std::ios::floatfield);
	return 0;
}
" FORMATSTRING_HEXFLOAT_SUPPORT)

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
	message(FATAL_ERROR "No wide string (std::wstring, wchar_t, ...) detected.")
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

if(WITH_EXAMPLES)
	add_subdirectory(examples)
endif()

add_subdirectory(src)

# uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
