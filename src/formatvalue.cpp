#include "formatstring/formatvalue.h"

using namespace formatstring;

template<> const char* impl::names::TRUE_LOWER  = "true";
template<> const char* impl::names::TRUE_UPPER  = "TRUE";
template<> const char* impl::names::FALSE_LOWER = "false";
template<> const char* impl::names::FALSE_UPPER = "FALSE";

template<> const wchar_t* impl::wnames::TRUE_LOWER  = L"true";
template<> const wchar_t* impl::wnames::TRUE_UPPER  = L"TRUE";
template<> const wchar_t* impl::wnames::FALSE_LOWER = L"false";
template<> const wchar_t* impl::wnames::FALSE_UPPER = L"FALSE";

template<> const char16_t* impl::u16names::TRUE_LOWER  = u"true";
template<> const char16_t* impl::u16names::TRUE_UPPER  = u"TRUE";
template<> const char16_t* impl::u16names::FALSE_LOWER = u"false";
template<> const char16_t* impl::u16names::FALSE_UPPER = u"FALSE";

template<> const char32_t* impl::u32names::TRUE_LOWER  = U"true";
template<> const char32_t* impl::u32names::TRUE_UPPER  = U"TRUE";
template<> const char32_t* impl::u32names::FALSE_LOWER = U"false";
template<> const char32_t* impl::u32names::FALSE_UPPER = U"FALSE";
