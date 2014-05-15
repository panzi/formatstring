add_library(formatstring
	format.cpp
	strformatitem.cpp
	valueformatitem.cpp
	formatvalue.cpp
	exceptions.cpp
	
	strformatitem.h
	valueformatitem.h

	../include/formatstring.h
	../include/formatstring/conversion.h
	../include/formatstring/format.h
	../include/formatstring/formatitem.h
	../include/formatstring/formatspec.h
	../include/formatstring/formatter.h
	../include/formatstring/formattedvalue.h
	../include/formatstring/formatvalue.h
	../include/formatstring/exceptions.h)
