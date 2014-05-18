#include "formatstring/formattedvalue.h"

using namespace formatstring;

template class BasicFormattedValue<char>;

#ifdef FORMATSTRING_CHAR16_SUPPORT
template class BasicFormattedValue<char16_t>;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
template class BasicFormattedValue<char32_t>;
#endif

template class BasicFormattedValue<wchar_t>;
