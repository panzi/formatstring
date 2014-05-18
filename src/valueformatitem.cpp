#include "valueformatitem.h"

using namespace formatstring;

template class BasicValueFormatItem<char>;
template class BasicValueFormatItem<wchar_t>;

#ifdef FORMATSTRING_CHAR16_SUPPORT
template class BasicValueFormatItem<char16_t>;
#endif

#ifdef FORMATSTRING_CHAR16_SUPPORT
template class BasicValueFormatItem<char32_t>;
#endif
