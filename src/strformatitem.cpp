#include "strformatitem.h"

using namespace formatstring;

template class BasicStrFormatItem<char>;
template class BasicStrFormatItem<wchar_t>;

#ifdef FORMATSTRING_CHAR16_SUPPORT
template class BasicStrFormatItem<char16_t>;
#endif

#ifdef FORMATSTRING_CHAR16_SUPPORT
template class BasicStrFormatItem<char32_t>;
#endif
