#include "formatstring/formatvalue.h"

namespace formatstring {
    namespace impl {
        template<typename Char>
        struct basic_grouping;

        template<>
        struct basic_grouping<char> {
            static const std::locale non_grouping_locale;
            static const std::locale thousands_grouping_locale;
        };

#ifdef FORMATSTRING_CHAR16_SUPPORT
        template<>
        struct basic_grouping<char16_t> {
            static const std::locale non_grouping_locale;
            static const std::locale thousands_grouping_locale;
        };
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
        template<>
        struct basic_grouping<char32_t> {
            static const std::locale non_grouping_locale;
            static const std::locale thousands_grouping_locale;
        };
#endif

        template<>
        struct basic_grouping<wchar_t> {
            static const std::locale non_grouping_locale;
            static const std::locale thousands_grouping_locale;
        };

        template<typename Char>
        struct basic_names {
            typedef Char char_type;

            static const Char* const TRUE_LOWER;
            static const Char* const TRUE_UPPER;

            static const Char* const FALSE_LOWER;
            static const Char* const FALSE_UPPER;
        };

        typedef basic_names<char>     names;

#ifdef FORMATSTRING_CHAR16_SUPPORT
        typedef basic_names<char16_t> u16names;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
        typedef basic_names<char32_t> u32names;
#endif

        typedef basic_names<wchar_t>  wnames;

        template<typename Char>
        inline void fill(std::basic_ostream<Char>& out, Char fill, std::size_t width) {
            for (; width > 0; -- width) { out.put(fill); }
        }

        template<typename Char>
        void sepfill(std::basic_ostream<Char>& out, std::size_t width, std::size_t numlen) {
            std::size_t place = width + numlen;
            // (x % 4) == (x & 3)
            if ((place & 3) == 0) {
                out.put('0');
            }
            for (; width > 0; -- width, -- place) {
                if ((place & 3) == 0) {
                    out.put(',');
                }
                else {
                    out.put('0');
                }
            }
        }

        template<typename Char>
        struct repr_char {
            static inline void write_prefix(std::basic_ostream<Char>& out) {
                (void)out;
            }
        };

        template<>
        struct repr_char<wchar_t> {
            static inline void write_prefix(std::wostream& out) {
                out.put('L');
            }
        };

#ifdef FORMATSTRING_CHAR16_SUPPORT
        template<>
        struct repr_char<char16_t> {
            static inline void write_prefix(std::basic_ostream<char16_t>& out) {
                out.put('u');
            }
        };
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
        template<>
        struct repr_char<char32_t> {
            static inline void write_prefix(std::basic_ostream<char32_t>& out) {
                out.put('U');
            }
        };
#endif
    }
}

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

template<typename Char>
void formatstring::repr_char(std::basic_ostream<Char>& out, Char value) {
    impl::repr_char<Char>::write_prefix(out);
    out.put('\'');
    switch (value) {
    case '\0': out.put('\\'); out.put('0'); break;
    case '\a': out.put('\\'); out.put('a'); break;
    case '\b': out.put('\\'); out.put('b'); break;
    case '\t': out.put('\\'); out.put('t'); break;
    case '\n': out.put('\\'); out.put('n'); break;
    case '\v': out.put('\\'); out.put('v'); break;
    case '\f': out.put('\\'); out.put('f'); break;
    case '\r': out.put('\\'); out.put('r'); break;
    case '\'': out.put('\\'); out.put('\''); break;
    case '\\': out.put('\\'); out.put('\\'); break;
    default:   out.put(value); break;
    }
    out.put('\'');
}

template<typename Char>
void formatstring::repr_string(std::basic_ostream<Char>& out, const Char* value) {
    impl::repr_char<Char>::write_prefix(out);
    out.put('"');
    for (; *value; ++ value) {
        Char ch = *value;
        switch (ch) {
        case '\0': out.put('\\'); out.put('0'); break;
        case '\a': out.put('\\'); out.put('a'); break;
        case '\b': out.put('\\'); out.put('b'); break;
        case '\t': out.put('\\'); out.put('t'); break;
        case '\n': out.put('\\'); out.put('n'); break;
        case '\v': out.put('\\'); out.put('v'); break;
        case '\f': out.put('\\'); out.put('f'); break;
        case '\r': out.put('\\'); out.put('r'); break;
        case '"':  out.put('\\'); out.put('"'); break;
        case '?':
            // prevent trigraphs from being interpreted inside string literals
            out.put('?');
            if (*(value + 1) == '?') {
                out.put('\\');
            }
            break;

        case '\\': out.put('\\'); out.put('\\'); break;
        default:   out.put(ch); break;
        }
    }
    out.put('"');
}

template<typename Char>
void formatstring::format_bool(std::basic_ostream<Char>& out, bool value, const BasicFormatSpec<Char>& spec) {
    if (spec.isNumberType()) {
        format_integer<Char,unsigned int>(out, value ? 1 : 0, spec);
    }
    else {
        const Char* str = spec.upperCase ?
                    (value ? impl::basic_names<Char>::TRUE_UPPER : impl::basic_names<Char>::FALSE_UPPER) :
                    (value ? impl::basic_names<Char>::TRUE_LOWER : impl::basic_names<Char>::FALSE_LOWER);
        BasicFormatSpec<Char> strspec = spec;
        strspec.type = BasicFormatSpec<Char>::String;
        format_string(out, str, strspec);
    }
}

template<typename Char>
void formatstring::repr_bool(std::basic_ostream<Char>& out, bool value) {
    const Char* str = value ? impl::basic_names<Char>::TRUE_LOWER : impl::basic_names<Char>::FALSE_LOWER;
    out.write(str, std::char_traits<Char>::length(str));
}

template<typename Char, typename Int, typename UInt = typename std::make_unsigned<Int>::type>
void formatstring::format_integer(std::basic_ostream<Char>& out, Int value, const BasicFormatSpec<Char>& spec) {
    typedef BasicFormatSpec<Char> Spec;

    if (spec.type == Spec::Character) {
        Char str[2] = {(Char)value, 0};
        Spec strspec = spec;
        strspec.type = Spec::String;
        format_string(out, str, strspec);
        return;
    }
    else if (spec.isFloatType()) {
        format_float<Char,double>(out, value, spec);
        return;
    }

    bool negative = value < 0;
    UInt abs = negative ? -value : value;
    std::basic_string<Char> prefix;

    switch (spec.sign) {
    case Spec::NegativeOnly:
    case Spec::DefaultSign:
        if (negative) {
            prefix += (Char)'-';
        }
        break;

    case Spec::Always:
        prefix += negative ? (Char)'-' : (Char)'+';
        break;

    case Spec::SpaceForPositive:
        prefix += negative ? (Char)'-' : (Char)' ';
        break;
    }

    std::basic_ostringstream<Char> buffer;

    buffer.imbue(spec.thoudsandsSeperator ? impl::basic_grouping<Char>::thousands_grouping_locale : impl::basic_grouping<Char>::non_grouping_locale);

    switch (spec.type) {
    case Spec::Generic:
    case Spec::Dec:
    case Spec::String:
        buffer << abs;
        break;

    case Spec::Bin:
        if (spec.alternate) {
            prefix += (Char)'0';
            prefix += spec.upperCase ? (Char)'B' : (Char)'b';
        }
        if (abs == 0) {
            buffer.put('0');
        }
        else {
            UInt bit = (UInt)1 << ((sizeof(abs) * 8) - 1);
            while ((abs & bit) == 0) {
                bit >>= 1;
            }

            while (bit != 0) {
                buffer.put((abs & bit) ? '1' : '0');
                bit >>= 1;
            }
        }
        break;

    case Spec::Oct:
        if (spec.alternate) {
            prefix += (Char)'0';
            prefix += spec.upperCase ? (Char)'O' : (Char)'o';
        }
        buffer.setf(std::ios::oct, std::ios::basefield);
        buffer << abs;
        break;

    case Spec::Hex:
        if (spec.alternate) {
            prefix += (Char)'0';
            prefix += spec.upperCase ? (Char)'X' : (Char)'x';
        }
        buffer.setf(std::ios::hex, std::ios::basefield);
        if (spec.upperCase) {
            buffer.setf(std::ios::uppercase);
        }
        buffer << abs;
        break;

    default:
        break;
    }

    std::basic_string<Char> num = buffer.str();
    typename std::basic_string<Char>::size_type length = prefix.size() + num.size();

    if (length < spec.width) {
        std::size_t padding = spec.width - length;
        switch (spec.alignment) {
        case Spec::Left:
            out.write(prefix.c_str(), prefix.size());
            out.write(num.c_str(), num.size());
            impl::fill(out, spec.fill, padding);
            break;

        case Spec::Right:
            impl::fill(out, spec.fill, padding);
            out.write(prefix.c_str(), prefix.size());
            out.write(num.c_str(), num.size());
            break;

        case Spec::Center:
        {
            std::size_t before = padding / 2;
            impl::fill(out, spec.fill, before);
            out.write(prefix.c_str(), prefix.size());
            out.write(num.c_str(), num.size());
            impl::fill(out, spec.fill, padding - before);
            break;
        }

        case Spec::AfterSign:
        case Spec::DefaultAlignment:
            out.write(prefix.c_str(), prefix.size());
            if (spec.thoudsandsSeperator && spec.fill == '0') {
                impl::sepfill(out, padding, num.size());
            }
            else {
                impl::fill(out, spec.fill, padding);
            }
            out.write(num.c_str(), num.size());
            break;
        }
    }
    else {
        out.write(prefix.c_str(), prefix.size());
        out.write(num.c_str(), num.size());
    }
}

template<typename Float>
inline std::string format_hexfloat(Float value, const FormatSpec& spec) {
    const char *fmt = std::is_same<Float,long double>::value ?
            (spec.upperCase ? "%*.*LA" : "%*.*La") :
            (spec.upperCase ? "%*.*A" : "%*.*a");
    char buf[64];
    int count = std::snprintf(buf, sizeof(buf), fmt, spec.width, spec.precision, value);
    if (count < 2 || count >= sizeof(buf)) {
        throw std::runtime_error("unexpected snprintf fail while processing hexfloat format");
    }
    return buf;
}

template<typename Float>
inline std::wstring format_hexfloat(Float value, const WFormatSpec& spec) {
    const wchar_t *fmt = std::is_same<Float,long double>::value ?
            (spec.upperCase ? L"%*.*LA" : L"%*.*La") :
            (spec.upperCase ? L"%*.*A" : L"%*.*a");
    wchar_t buf[64];
    int count = std::swprintf(buf, sizeof(buf)/sizeof(wchar_t), fmt, spec.width, spec.precision, value);
    if (count < 2 || count >= sizeof(buf)/sizeof(wchar_t)) {
        throw std::runtime_error("unexpected printf fail while processing hexfloat format");
    }
    return buf;
}

#ifdef FORMATSTRING_CHAR16_SUPPORT
template<typename Float>
inline std::basic_string<char16_t> format_hexfloat(Float value, const U16FormatSpec& spec) {
    (void)value;
    (void)spec;
    throw std::runtime_error("STL implementation does not support std::ios::hexfloat.");
}
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
template<typename Float>
inline std::basic_string<char32_t> format_hexfloat(Float value, const U32FormatSpec& spec) {
    (void)value;
    (void)spec;
    throw std::runtime_error("STL implementation does not support std::ios::hexfloat.");
}
#endif

template<typename Char, typename Float>
void formatstring::format_float(std::basic_ostream<Char>& out, Float value, const BasicFormatSpec<Char>& spec) {
    typedef BasicFormatSpec<Char> Spec;

    if (!spec.isFloatType() && spec.type != Spec::Generic) {
        throw std::invalid_argument("Cannot use floating point numbers with non-decimal format specifier.");
    }

    bool negative = std::signbit(value);
    Float abs = negative ? -value : value;
    std::basic_string<Char> prefix;

    switch (spec.sign) {
    case Spec::NegativeOnly:
    case Spec::DefaultSign:
        if (negative) {
            prefix += (Char)'-';
        }
        break;

    case Spec::Always:
        prefix += negative ? (Char)'-' : (Char)'+';
        break;

    case Spec::SpaceForPositive:
        prefix += negative ? (Char)'-' : (Char)' ';
        break;
    }

    std::basic_string<Char> num;

    if (std::isnan(abs)) {
        if (spec.upperCase) {
            Char buffer[] = {'N', 'A', 'N', 0};
            num = buffer;
        }
        else {
            Char buffer[] = {'n', 'a', 'n', 0};
            num = buffer;
        }

        if (spec.type == Spec::Percentage) {
            num += (Char)'%';
        }
    }
    else if (std::isinf(abs)) {
        if (spec.upperCase) {
            Char buffer[] = {'I', 'N', 'F', 0};
            num = buffer;
        }
        else {
            Char buffer[] = {'i', 'n', 'f', 0};
            num = buffer;
        }

        if (spec.type == Spec::Percentage) {
            num += (Char)'%';
        }
    }
#if !defined(FORMATSTRING_IOS_HEXFLOAT_SUPPORT) && defined(FORMATSTRING_PRINTF_HEXFLOAT_SUPPORT)
    else if (spec.type == Spec::HexFloat) {
        num = format_hexfloat(abs, spec);
    }
#endif
    else {
        std::basic_ostringstream<Char> buffer;

        buffer.imbue(spec.thoudsandsSeperator ? impl::basic_grouping<Char>::thousands_grouping_locale : impl::basic_grouping<Char>::non_grouping_locale);

        if (spec.upperCase) {
            buffer.setf(std::ios::uppercase);
        }

        switch (spec.type) {
        case Spec::Exp:
            buffer.setf(std::ios::scientific, std::ios::floatfield);
            buffer.precision(spec.precision);
            buffer << abs;
            break;

        case Spec::Fixed:
            buffer.setf(std::ios::fixed, std::ios::floatfield);
            buffer.precision(spec.precision);
            buffer << abs;
            break;

        case Spec::Generic:
        case Spec::General:
        {
            // XXX: not 100% correct. it doesn't skip trailing 0s after the decimal point
            int exponent = std::log10(abs);
            int precision = spec.precision < 1 ? 1 : spec.precision;
            if (-4 <= exponent && exponent < precision) {
                buffer.setf(std::ios::fixed, std::ios::floatfield);
                precision = precision - 1 - exponent;
            }
            else {
                buffer.setf(std::ios::scientific, std::ios::floatfield);
                precision = precision - 1;
            }
            buffer.precision(precision);
            buffer << abs;
            break;
        }
        case Spec::Percentage:
            buffer.setf(std::ios::fixed, std::ios::floatfield);
            buffer.precision(spec.precision);
            buffer << (abs * 100);
            buffer.put('%');
            break;

        case Spec::HexFloat:
#ifdef FORMATSTRING_IOS_HEXFLOAT_SUPPORT
            buffer.setf(std::ios::hexfloat, std::ios::floatfield);
            buffer.precision(spec.precision);
            buffer << abs;
            break;
#else
            throw std::runtime_error("STL implementation does not support std::ios::hexfloat.");
#endif

        default:
            break;
        }

        num = buffer.str();
    }

    typename std::basic_string<Char>::size_type length = prefix.size() + num.size();

    if (length < spec.width) {
        std::size_t padding = spec.width - length;
        switch (spec.alignment) {
        case Spec::Left:
            out.write(prefix.c_str(), prefix.size());
            out.write(num.c_str(), num.size());
            impl::fill(out, spec.fill, padding);
            break;

        case Spec::Right:
            impl::fill(out, spec.fill, padding);
            out.write(prefix.c_str(), prefix.size());
            out.write(num.c_str(), num.size());
            break;

        case Spec::Center:
        {
            std::size_t before = padding / 2;
            impl::fill(out, spec.fill, before);
            out.write(prefix.c_str(), prefix.size());
            out.write(num.c_str(), num.size());
            impl::fill(out, spec.fill, padding - before);
            break;
        }

        case Spec::AfterSign:
        case Spec::DefaultAlignment:
            out.write(prefix.c_str(), prefix.size());
            if (spec.thoudsandsSeperator && spec.fill == '0' && std::isfinite(abs)) {
                Char chars[] = { (Char)'.', (Char)'e', 0 };
                if (spec.upperCase) {
                    chars[1] = (Char)'E';
                }
                std::size_t pos = num.find_first_of(chars);
                impl::sepfill(out, padding, pos == std::basic_string<Char>::npos ? num.size() : pos);
            }
            else {
                impl::fill(out, spec.fill, padding);
            }
            out.write(num.c_str(), num.size());
            break;
        }
    }
    else {
        out.write(prefix.c_str(), prefix.size());
        out.write(num.c_str(), num.size());
    }
}

template<typename Char>
void formatstring::format_string(std::basic_ostream<Char>& out, const Char value[], const BasicFormatSpec<Char>& spec) {
    typedef BasicFormatSpec<Char> Spec;

    if (spec.sign != Spec::DefaultSign) {
        throw std::invalid_argument("Sign not allowed with string or character");
    }

    if (spec.thoudsandsSeperator) {
        throw std::invalid_argument("Cannot specify ',' for string");
    }

    if (spec.alternate && spec.type != Spec::Character) {
        throw std::invalid_argument("Alternate form (#) not allowed in string format specifier");
    }

    switch (spec.type) {
    case Spec::Generic:
    case Spec::String:
        break;

    default:
        throw std::invalid_argument("Invalid format specifier for string or character");
    }

    std::size_t length = std::char_traits<Char>::length(value);
    if (spec.width > 0 && length < (std::size_t)spec.width) {
        std::size_t padding = spec.width - length;
        switch (spec.alignment) {
        case Spec::AfterSign:
            throw std::invalid_argument("'=' alignment not allowed in string or character format specifier");

        case Spec::DefaultAlignment:
        case Spec::Left:
            out.write(value, length);
            impl::fill(out, spec.fill, padding);
            break;

        case Spec::Right:
            impl::fill(out, spec.fill, padding);
            out.write(value, length);
            break;

        case Spec::Center:
            std::size_t before = padding / 2;
            impl::fill(out, spec.fill, before);
            out.write(value, length);
            impl::fill(out, spec.fill, padding - before);
            break;
        }
    }
    else {
        out.write(value, length);
    }
}

template<typename Char>
void formatstring::format_int_char(std::basic_ostream<Char>& out, typename std::char_traits<Char>::int_type value, const BasicFormatSpec<Char>& spec) {
    if (spec.type == BasicFormatSpec<Char>::Generic || spec.isStringType()) {
        Char str[2] = { (Char)value, 0 };
        BasicFormatSpec<Char> strspec = spec;
        strspec.type = BasicFormatSpec<Char>::String;
        format_string(out, str, strspec);
    }
    else {
        format_integer(out, value, spec);
    }
}

template void repr_bool<char>(std::ostream& out, bool value);
template void repr_bool<wchar_t>(std::wostream& out, bool value);

template void repr_char<char>(std::ostream& out, char value);
template void repr_char<wchar_t>(std::wostream& out, wchar_t value);

template void repr_string<char>(std::ostream& out, const char* value);
template void repr_string<wchar_t>(std::wostream& out, const wchar_t* value);

template void format_bool<char>(std::ostream& out, bool value, const FormatSpec& spec);
template void format_bool<wchar_t>(std::wostream& out, bool value, const WFormatSpec& spec);

template void format_int_char<char>(std::ostream& out, std::char_traits<char>::int_type value, const FormatSpec& spec);
template void format_int_char<wchar_t>(std::wostream& out, std::char_traits<wchar_t>::int_type value, const WFormatSpec& spec);

template void format_string<char>(std::ostream& out, const char value[], const FormatSpec& spec);
template void format_string<wchar_t>(std::wostream& out, const wchar_t value[], const WFormatSpec& spec);

template void format_float<char,float>(std::ostream& out, float value, const FormatSpec& spec);
template void format_float<wchar_t,float>(std::wostream& out, float value, const WFormatSpec& spec);

template void format_float<char,double>(std::ostream& out, double value, const FormatSpec& spec);
template void format_float<wchar_t,double>(std::wostream& out, double value, const WFormatSpec& spec);

template void format_float<char,long double>(std::ostream& out, long double value, const FormatSpec& spec);
template void format_float<wchar_t,long double>(std::wostream& out, long double value, const WFormatSpec& spec);

template void format_integer<char,char>(std::ostream& out, char value, const FormatSpec& spec);
template void format_integer<char,short>(std::ostream& out, short value, const FormatSpec& spec);
template void format_integer<char,int>(std::ostream& out, int value, const FormatSpec& spec);
template void format_integer<char,long>(std::ostream& out, long value, const FormatSpec& spec);
template void format_integer<char,long long>(std::ostream& out, long long value, const FormatSpec& spec);

template void format_integer<char,signed char>(std::ostream& out, signed char value, const FormatSpec& spec);
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

template void format_integer<wchar_t,signed char>(std::wostream& out, signed char value, const WFormatSpec& spec);
template void format_integer<wchar_t,unsigned char>(std::wostream& out, unsigned char value, const WFormatSpec& spec);
template void format_integer<wchar_t,unsigned short>(std::wostream& out, unsigned short value, const WFormatSpec& spec);
template void format_integer<wchar_t,unsigned int>(std::wostream& out, unsigned int value, const WFormatSpec& spec);
template void format_integer<wchar_t,unsigned long>(std::wostream& out, unsigned long value, const WFormatSpec& spec);
template void format_integer<wchar_t,unsigned long long>(std::wostream& out, unsigned long long value, const WFormatSpec& spec);

const std::locale impl::basic_grouping<char>::non_grouping_locale(std::locale(), new no_grouping<char>());
const std::locale impl::basic_grouping<char>::thousands_grouping_locale(std::locale(), new group_thousands<char>());

#ifdef FORMATSTRING_CHAR16_SUPPORT
template void repr_bool<char16_t>(std::basic_ostream<char16_t>& out, bool value);
template void repr_char<char16_t>(std::basic_ostream<char16_t>& out, char16_t value);
template void repr_string<char16_t>(std::basic_stream<char16_t>& out, const char16_t* value);

template void format_bool<char16_t>(std::basic_stream<char16_t>& out, bool value, const U16FormatSpec& spec);

template void format_int_char<char16_t>(std::basic_ostream<char16_t>& out, std::char_traits<char16_t>::int_type value, const U16FormatSpec& spec);
template void format_string<char16_t>(std::basic_ostream<char16_t>& out, const char16_t value[], const U16FormatSpec& spec);

template void format_float<char16_t,float>(std::basic_ostream<char16_t>& out, float value, const U16FormatSpec& spec);
template void format_float<char16_t,double>(std::basic_ostream<char16_t>& out, double value, const U16FormatSpec& spec);
template void format_float<char16_t,long double>(std::basic_ostream<char16_t>& out, long double value, const U16FormatSpec& spec);

template void format_integer<char16_t,char16_t>(std::basic_ostream<char16_t>& out, char16_t value, const U16FormatSpec& spec);

template void format_integer<char16_t,char>(std::basic_ostream<char16_t>& out, char value, const U16FormatSpec& spec);
template void format_integer<char16_t,short>(std::basic_ostream<char16_t>& out, short value, const U16FormatSpec& spec);
template void format_integer<char16_t,int>(std::basic_ostream<char16_t>& out, int value, const U16FormatSpec& spec);
template void format_integer<char16_t,long>(std::basic_ostream<char16_t>& out, long value, const U16FormatSpec& spec);
template void format_integer<char16_t,long long>(std::basic_ostream<char16_t>& out, long long value, const U16FormatSpec& spec);

template void format_integer<char16_t,signed char>(std::basic_ostream<char16_t>& out, signed char value, const U16FormatSpec& spec);
template void format_integer<char16_t,unsigned char>(std::basic_ostream<char16_t>& out, unsigned char value, const U16FormatSpec& spec);
template void format_integer<char16_t,unsigned short>(std::basic_ostream<char16_t>& out, unsigned short value, const U16FormatSpec& spec);
template void format_integer<char16_t,unsigned int>(std::basic_ostream<char16_t>& out, unsigned int value, const U16FormatSpec& spec);
template void format_integer<char16_t,unsigned long>(std::basic_ostream<char16_t>& out, unsigned long value, const U16FormatSpec& spec);
template void format_integer<char16_t,unsigned long long>(std::basic_ostream<char16_t>& out, unsigned long long value, const U16FormatSpec& spec);

const std::locale impl::basic_grouping<char16_t>::non_grouping_locale(std::locale(), new no_grouping<char16_t>());
const std::locale impl::basic_grouping<char16_t>::thousands_grouping_locale(std::locale(), new group_thousands<char16_t>());
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
template void repr_bool<char32_t>(std::basic_ostream<char32_t>& out, bool value);
template void repr_char<char32_t>(std::basic_ostream<char32_t>& out, char32_t value);
template void repr_string<char32_t>(std::basic_stream<char32_t>& out, const char32_t* value);

template void format_bool<char32_t>(std::basic_stream<char32_t>& out, bool value, const U32FormatSpec& spec);

template void format_int_char<char32_t>(std::basic_ostream<char32_t>& out, std::char_traits<char32_t>::int_type value, const U32FormatSpec& spec);
template void format_string<char32_t>(std::basic_ostream<char32_t>& out, const char32_t value[], const U32FormatSpec& spec);

template void format_float<char32_t,float>(std::basic_ostream<char32_t>& out, float value, const U32FormatSpec& spec);
template void format_float<char32_t,double>(std::basic_ostream<char32_t>& out, double value, const U32FormatSpec& spec);
template void format_float<char32_t,long double>(std::basic_ostream<char32_t>& out, long double value, const U32FormatSpec& spec);

template void format_integer<char32_t,char32_t>(std::basic_ostream<char32_t>& out, char32_t value, const U32FormatSpec& spec);

template void format_integer<char32_t,char>(std::basic_ostream<char16_t>& out, char value, const U32FormatSpec& spec);
template void format_integer<char32_t,short>(std::basic_ostream<char32_t>& out, short value, const U32FormatSpec& spec);
template void format_integer<char32_t,int>(std::basic_ostream<char32_t>& out, int value, const U32FormatSpec& spec);
template void format_integer<char32_t,long>(std::basic_ostream<char32_t>& out, long value, const U32FormatSpec& spec);
template void format_integer<char32_t,long long>(std::basic_ostream<char32_t>& out, long long value, const U32FormatSpec& spec);

template void format_integer<char32_t,signed char>(std::basic_ostream<char32_t>& out, signed char value, const U32FormatSpec& spec);
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
