#include "formatstring/formatspec.h"

using namespace formatstring;

template class BasicFormatSpec<char>;
template<> const FormatSpec FormatSpec::DEFAULT = FormatSpec();

#ifdef FORMATSTRING_CHAR16_SUPPORT
template class BasicFormatSpec<char16_t>;
template<> const U16FormatSpec U16FormatSpec::DEFAULT = U16FormatSpec();
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
template class BasicFormatSpec<char32_t>;
template<> const U32FormatSpec U32FormatSpec::DEFAULT = U32FormatSpec();
#endif

template class BasicFormatSpec<wchar_t>;
template<> const WFormatSpec WFormatSpec::DEFAULT = WFormatSpec();

