#include "formatstring/formatvalue.h"

using namespace formatstring;

template<> const char* const impl::names::TRUE_LOWER  = "true";
template<> const char* const impl::names::TRUE_UPPER  = "TRUE";
template<> const char* const impl::names::FALSE_LOWER = "false";
template<> const char* const impl::names::FALSE_UPPER = "FALSE";

#ifdef FORMATSTRING_CHAR16_SUPPORT
template<> const char16_t* const impl::u16names::TRUE_LOWER  = u"true";
template<> const char16_t* const impl::u16names::TRUE_UPPER  = u"TRUE";
template<> const char16_t* const impl::u16names::FALSE_LOWER = u"false";
template<> const char16_t* const impl::u16names::FALSE_UPPER = u"FALSE";
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
template<> const char32_t* const impl::u32names::TRUE_LOWER  = U"true";
template<> const char32_t* const impl::u32names::TRUE_UPPER  = U"TRUE";
template<> const char32_t* const impl::u32names::FALSE_LOWER = U"false";
template<> const char32_t* const impl::u32names::FALSE_UPPER = U"FALSE";
#endif

template<> const wchar_t* const impl::wnames::TRUE_LOWER  = L"true";
template<> const wchar_t* const impl::wnames::TRUE_UPPER  = L"TRUE";
template<> const wchar_t* const impl::wnames::FALSE_LOWER = L"false";
template<> const wchar_t* const impl::wnames::FALSE_UPPER = L"FALSE";

template<typename Char>
struct group_thousands : public std::numpunct<Char> {
    std::string do_grouping() const { return "\03"; }
    Char do_thousands_sep() const { return ','; }
    Char do_decimal_point() const { return '.'; }
};

template<typename Char>
struct no_grouping : public group_thousands<Char> {
    std::string do_grouping() const { return ""; }
};

template void repr_char<char>(std::ostream& out, char value);
template void repr_char<wchar_t>(std::wostream& out, wchar_t value);

template void repr_string<char>(std::ostream& out, const char* value);
template void repr_string<wchar_t>(std::wostream& out, const wchar_t* value);

template void format_string<char>(std::ostream& out, const char value[], const FormatSpec& spec);
template void format_string<wchar_t>(std::wostream& out, const wchar_t value[], const WFormatSpec& spec);

template void format_float<char,float>(std::ostream& out, float value, const FormatSpec& spec);
template void format_float<wchar_t,float>(std::wostream& out, float value, const WFormatSpec& spec);

template void format_float<char,double>(std::ostream& out, double value, const FormatSpec& spec);
template void format_float<wchar_t,double>(std::wostream& out, double value, const WFormatSpec& spec);

template void format_integer<char,char>(std::ostream& out, char value, const FormatSpec& spec);
template void format_integer<char,short>(std::ostream& out, short value, const FormatSpec& spec);
template void format_integer<char,int>(std::ostream& out, int value, const FormatSpec& spec);
template void format_integer<char,long>(std::ostream& out, long value, const FormatSpec& spec);
template void format_integer<char,long long>(std::ostream& out, long long value, const FormatSpec& spec);

template void format_integer<char,unsigned char>(std::ostream& out, unsigned char value, const FormatSpec& spec);
template void format_integer<char,unsigned short>(std::ostream& out, unsigned short value, const FormatSpec& spec);
template void format_integer<char,unsigned int>(std::ostream& out, unsigned int value, const FormatSpec& spec);
template void format_integer<char,unsigned long>(std::ostream& out, unsigned long value, const FormatSpec& spec);
template void format_integer<char,unsigned long long>(std::ostream& out, unsigned long long value, const FormatSpec& spec);

template void format_integer<wchar_t,wchar_t>(std::wostream& out, wchar_t value, const WFormatSpec& spec);

template void format_integer<wchar_t,char>(std::wostream& out, char value, const WFormatSpec& spec);
template void format_integer<wchar_t,short>(std::wostream& out, short value, const WFormatSpec& spec);
template void format_integer<wchar_t,int>(std::wostream& out, int value, const WFormatSpec& spec);
template void format_integer<wchar_t,long>(std::wostream& out, long value, const WFormatSpec& spec);
template void format_integer<wchar_t,long long>(std::wostream& out, long long value, const WFormatSpec& spec);

template void format_integer<wchar_t,unsigned char>(std::wostream& out, unsigned char value, const WFormatSpec& spec);
template void format_integer<wchar_t,unsigned short>(std::wostream& out, unsigned short value, const WFormatSpec& spec);
template void format_integer<wchar_t,unsigned int>(std::wostream& out, unsigned int value, const WFormatSpec& spec);
template void format_integer<wchar_t,unsigned long>(std::wostream& out, unsigned long value, const WFormatSpec& spec);
template void format_integer<wchar_t,unsigned long long>(std::wostream& out, unsigned long long value, const WFormatSpec& spec);

const std::locale impl::basic_grouping<char>::non_grouping_locale(std::locale(), new no_grouping<char>());
const std::locale impl::basic_grouping<char>::thousands_grouping_locale(std::locale(), new group_thousands<char>());

#ifdef FORMATSTRING_CHAR16_SUPPORT
template void repr_char<char16_t>(std::basic_ostream<char16_t>& out, char16_t value);
template void repr_string<char16_t>(std::basic_stream<char16_t>& out, const char16_t* value);
template void format_string<char16_t>(std::basic_ostream<char16_t>& out, const char16_t value[], const U16FormatSpec& spec);
template void format_float<char16_t,float>(std::basic_ostream<char16_t>& out, float value, const U16FormatSpec& spec);
template void format_float<char16_t,double>(std::basic_ostream<char16_t>& out, double value, const U16FormatSpec& spec);

template void format_integer<char16_t,char16_t>(std::basic_ostream<char16_t>& out, char16_t value, const U16FormatSpec& spec);

template void format_integer<char16_t,char>(std::basic_ostream<char16_t>& out, char value, const U16FormatSpec& spec);
template void format_integer<char16_t,short>(std::basic_ostream<char16_t>& out, short value, const U16FormatSpec& spec);
template void format_integer<char16_t,int>(std::basic_ostream<char16_t>& out, int value, const U16FormatSpec& spec);
template void format_integer<char16_t,long>(std::basic_ostream<char16_t>& out, long value, const U16FormatSpec& spec);
template void format_integer<char16_t,long long>(std::basic_ostream<char16_t>& out, long long value, const U16FormatSpec& spec);

template void format_integer<char16_t,unsigned char>(std::basic_ostream<char16_t>& out, unsigned char value, const U16FormatSpec& spec);
template void format_integer<char16_t,unsigned short>(std::basic_ostream<char16_t>& out, unsigned short value, const U16FormatSpec& spec);
template void format_integer<char16_t,unsigned int>(std::basic_ostream<char16_t>& out, unsigned int value, const U16FormatSpec& spec);
template void format_integer<char16_t,unsigned long>(std::basic_ostream<char16_t>& out, unsigned long value, const U16FormatSpec& spec);
template void format_integer<char16_t,unsigned long long>(std::basic_ostream<char16_t>& out, unsigned long long value, const U16FormatSpec& spec);

const std::locale impl::basic_grouping<char16_t>::non_grouping_locale(std::locale(), new no_grouping<char16_t>());
const std::locale impl::basic_grouping<char16_t>::thousands_grouping_locale(std::locale(), new group_thousands<char16_t>());
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
template void repr_char<char32_t>(std::basic_ostream<char32_t>& out, char32_t value);
template void repr_string<char32_t>(std::basic_stream<char32_t>& out, const char32_t* value);
template void format_string<char32_t>(std::basic_ostream<char32_t>& out, const char32_t value[], const U32FormatSpec& spec);
template void format_float<char32_t,float>(std::basic_ostream<char32_t>& out, float value, const U32FormatSpec& spec);
template void format_float<char32_t,double>(std::basic_ostream<char32_t>& out, double value, const U32FormatSpec& spec);

template void format_integer<char32_t,char32_t>(std::basic_ostream<char32_t>& out, char32_t value, const U32FormatSpec& spec);

template void format_integer<char32_t,char>(std::basic_ostream<char16_t>& out, char value, const U32FormatSpec& spec);
template void format_integer<char32_t,short>(std::basic_ostream<char32_t>& out, short value, const U32FormatSpec& spec);
template void format_integer<char32_t,int>(std::basic_ostream<char32_t>& out, int value, const U32FormatSpec& spec);
template void format_integer<char32_t,long>(std::basic_ostream<char32_t>& out, long value, const U32FormatSpec& spec);
template void format_integer<char32_t,long long>(std::basic_ostream<char32_t>& out, long long value, const U32FormatSpec& spec);

template void format_integer<char32_t,unsigned char>(std::basic_ostream<char32_t>& out, unsigned char value, const U32FormatSpec& spec);
template void format_integer<char32_t,unsigned short>(std::basic_ostream<char32_t>& out, unsigned short value, const U32FormatSpec& spec);
template void format_integer<char32_t,unsigned int>(std::basic_ostream<char32_t>& out, unsigned int value, const U32FormatSpec& spec);
template void format_integer<char32_t,unsigned long>(std::basic_ostream<char32_t>& out, unsigned long value, const U32FormatSpec& spec);
template void format_integer<char32_t,unsigned long long>(std::basic_ostream<char32_t>& out, unsigned long long value, const U32FormatSpec& spec);

const std::locale impl::basic_grouping<char32_t>::non_grouping_locale(std::locale(), new no_grouping<char32_t>());
const std::locale impl::basic_grouping<char32_t>::thousands_grouping_locale(std::locale(), new group_thousands<char32_t>());
#endif

const std::locale impl::basic_grouping<wchar_t>::non_grouping_locale(std::locale(), new no_grouping<wchar_t>());
const std::locale impl::basic_grouping<wchar_t>::thousands_grouping_locale(std::locale(), new group_thousands<wchar_t>());
