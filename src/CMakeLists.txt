include(GenerateExportHeader)

configure_file(
	../include/formatstring/config.h.in
	"${CMAKE_CURRENT_BINARY_DIR}/../include/formatstring/config.h"
	@ONLY)

add_compiler_export_flags()
add_library(${FORMATSTRING_NAME} SHARED
	config.cpp
	format.cpp
	formatspec.cpp
	formattedvalue.cpp
	formatvalue.cpp
	exceptions.cpp
	strformatitem.cpp
	valueformatitem.cpp
	
	strformatitem.h
	valueformatitem.h

	../include/formatstring.h
	../include/formatstring/conversion.h
	../include/formatstring/format.h
	../include/formatstring/formatitem.h
	../include/formatstring/formatspec.h
	../include/formatstring/formatter.h
	../include/formatstring/format_traits_fwd.h
	../include/formatstring/format_traits.h
	../include/formatstring/formattedvalue.h
	../include/formatstring/formatvalue.h
	../include/formatstring/exceptions.h)

generate_export_header(${FORMATSTRING_NAME}
	EXPORT_MACRO_NAME FORMATSTRING_EXPORT
	EXPORT_FILE_NAME ../include/formatstring/export.h
	STATIC_DEFINE FORMATSTRING_STATIC_LIB)

install(TARGETS ${FORMATSTRING_NAME} LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

install(FILES ../include/formatstring.h	DESTINATION "include/${FORMATSTRING_NAME}")
install(FILES

	../include/formatstring/conversion.h
	../include/formatstring/format.h
	../include/formatstring/formatitem.h
	../include/formatstring/formatspec.h
	../include/formatstring/formatter.h
	../include/formatstring/format_traits.h
	../include/formatstring/formattedvalue.h
	../include/formatstring/formatvalue.h
	../include/formatstring/exceptions.h

	"${CMAKE_CURRENT_BINARY_DIR}/../include/formatstring/config.h"
	"${CMAKE_CURRENT_BINARY_DIR}/../include/formatstring/export.h"

	DESTINATION "include/${FORMATSTRING_NAME}/formatstring")
