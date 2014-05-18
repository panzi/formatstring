#ifndef FORMATSTRING_FORMATVALUE_H
#define FORMATSTRING_FORMATVALUE_H
#pragma once

#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <locale>
#include <tuple>
#include <type_traits>
#include <string>
#include <sstream>
#include <cmath>

#include "formatstring/formatspec.h"

namespace formatstring {

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, bool value);

    inline void repr_value(std::basic_ostream<char16_t>& out, char16_t value);
    inline void repr_value(std::basic_ostream<char32_t>& out, char32_t value);
    inline void repr_value(std::wostream& out, wchar_t value);

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, char      value);
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, short     value);
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, int       value);
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, long      value);
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, long long value);

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, unsigned char      value);
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, unsigned short     value);
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, unsigned int       value);
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, unsigned long      value);
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, unsigned long long value);

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, float  value);
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, double value);

    template<typename Char> void repr_value(std::basic_ostream<Char>& out, const std::basic_string<Char>& value);
    template<typename Char> void repr_value(std::basic_ostream<Char>& out, const Char* value);

    template<typename Char, typename... Args>
    void repr_value(std::basic_ostream<Char>& out, const std::tuple<Args...>& value);

    template<typename Char, typename First, typename Second>
    void repr_value(std::basic_ostream<Char>& out, const std::pair<First,Second>& value);

    template<typename Char, typename Iter>
    void repr_slice(std::basic_ostream<Char>& out, Iter begin, Iter end, Char left = '[', Char right = ']');

    template<typename Char, typename Iter>
    void repr_map(std::basic_ostream<Char>& out, Iter begin, Iter end, Char left = '{', Char right = '}');

    template<typename Char> void format_value(std::basic_ostream<Char>& out, bool value, const BasicFormatSpec<Char>& spec);

    inline void format_value(std::basic_ostream<char16_t>& out, char16_t value, const U16FormatSpec& spec);
    inline void format_value(std::basic_ostream<char32_t>& out, char32_t value, const U32FormatSpec& spec);
    inline void format_value(std::wostream& out, wchar_t value, const WFormatSpec& spec);

    template<typename Char> void format_value(std::basic_ostream<Char>& out, char      value, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_value(std::basic_ostream<Char>& out, short     value, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_value(std::basic_ostream<Char>& out, int       value, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_value(std::basic_ostream<Char>& out, long      value, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_value(std::basic_ostream<Char>& out, long long value, const BasicFormatSpec<Char>& spec);

    template<typename Char> void format_value(std::basic_ostream<Char>& out, unsigned char      value, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_value(std::basic_ostream<Char>& out, unsigned short     value, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_value(std::basic_ostream<Char>& out, unsigned int       value, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_value(std::basic_ostream<Char>& out, unsigned long      value, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_value(std::basic_ostream<Char>& out, unsigned long long value, const BasicFormatSpec<Char>& spec);

    template<typename Char> void format_value(std::basic_ostream<Char>& out, float  value, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_value(std::basic_ostream<Char>& out, double value, const BasicFormatSpec<Char>& spec);

    template<typename Char> void format_value(std::basic_ostream<Char>& out, const std::basic_string<Char>& str, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_value(std::basic_ostream<Char>& out, const Char* str, const BasicFormatSpec<Char>& spec);

    template<typename Char, typename... Args>
    void format_value(std::basic_ostream<Char>& out, const std::tuple<Args...>& value, const BasicFormatSpec<Char>& spec);

    template<typename Char, typename First, typename Second>
    void format_value(std::basic_ostream<Char>& out, const std::pair<First,Second>& value, const FormatSpec& spec);

    template<typename Char, typename Iter>
    void format_slice(std::basic_ostream<Char>& out, Iter begin, Iter end, const BasicFormatSpec<Char>& spec, Char left = '[', Char right = ']');

    template<typename Char, typename Iter>
    void format_map(std::basic_ostream<Char>& out, Iter begin, Iter end, const BasicFormatSpec<Char>& spec, Char left = '{', Char right = '}');

    template<typename Char, typename T>
    void format_value_fallback(std::basic_ostream<Char>& out, const T& value, const BasicFormatSpec<Char>& spec);

    // ---- tempalte definitions and implementation details ------------------------------------------------------------

    namespace impl {
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
        struct basic_names {
            typedef Char char_type;

            static const Char* const TRUE_LOWER;
            static const Char* const TRUE_UPPER;

            static const Char* const FALSE_LOWER;
            static const Char* const FALSE_UPPER;
        };

        typedef basic_names<char>     names;
        typedef basic_names<char16_t> u16names;
        typedef basic_names<char32_t> u32names;
        typedef basic_names<wchar_t>  wnames;

        template<typename Char>
        inline void fill(std::basic_ostream<Char>& out, Char fill, std::size_t width) {
            for (; width > 0; -- width) { out.put(fill); }
        }
    }

    template<typename Char, typename Int, typename UInt = typename std::make_unsigned<Int>::type>
    void format_integer(std::basic_ostream<Char>& out, Int value, const BasicFormatSpec<Char>& spec);

    template<typename Char, typename Float>
    void format_float(std::basic_ostream<Char>& out, Float value, const BasicFormatSpec<Char>& spec);

    template<typename Char>
    void format_string(std::basic_ostream<Char>& out, const Char value[], const BasicFormatSpec<Char>& spec);

    template<typename Char, typename Int, typename UInt = typename std::make_unsigned<Int>::type>
    void format_integer(std::basic_ostream<Char>& out, Int value, const BasicFormatSpec<Char>& spec) {
        typedef BasicFormatSpec<Char> Spec;

        if (spec.type == Spec::Character) {
            Char str[2] = {(Char)value, 0};
            Spec strspec = spec;
            strspec.type = Spec::String;
            format_string(out, str, strspec);
            return;
        }
        else if (spec.isDecimalType()) {
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
        std::locale loc(std::locale(), spec.thoudsandsSeperator ? new impl::group_thousands<Char>() : new impl::no_grouping<Char>());

        buffer.imbue(loc);

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
                UInt bit = 1 << ((sizeof(abs) * 8) - 1);
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
        typename std::basic_string<Char>::size_type length = prefix.length() + num.length();

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
                impl::fill(out, spec.fill, padding);
                out.write(num.c_str(), num.size());
                break;
            }
        }
        else {
            out.write(prefix.c_str(), prefix.size());
            out.write(num.c_str(), num.size());
        }
    }

    template<typename Char, typename Float>
    void format_float(std::basic_ostream<Char>& out, Float value, const BasicFormatSpec<Char>& spec) {
        typedef BasicFormatSpec<Char> Spec;

        if (!spec.isDecimalType() && spec.type != Spec::Generic) {
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

        std::basic_ostringstream<Char> buffer;
        std::locale loc(std::locale(), spec.thoudsandsSeperator ? new impl::group_thousands<Char>() : new impl::no_grouping<Char>());

        buffer.imbue(loc);

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
            int exponent = std::log10(abs);
            int precision = spec.precision < 1 ? 1 : spec.precision;
            if (-4 <= exponent && exponent < precision) {
                buffer.setf(std::ios::fixed, std::ios::floatfield);
                buffer.precision(precision - 1 - exponent);
            }
            else {
                buffer.setf(std::ios::scientific, std::ios::floatfield);
                buffer.precision(precision - 1);
            }
            buffer << abs;
            break;
        }
        case Spec::Percentage:
            buffer.setf(std::ios::fixed, std::ios::floatfield);
            buffer.precision(spec.precision);
            buffer << (abs * 100);
            buffer.put('%');
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
                impl::fill(out, spec.fill, padding);
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
    void format_string(std::basic_ostream<Char>& out, const Char value[], const BasicFormatSpec<Char>& spec) {
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
    void format_char(std::basic_ostream<Char>& out, Char value, const BasicFormatSpec<Char>& spec) {
        if (spec.type == BasicFormatSpec<Char>::Generic || spec.isStringType()) {
            Char str[2] = { value, 0 };
            BasicFormatSpec<Char> strspec = spec;
            strspec.type = BasicFormatSpec<Char>::String;
            format_string(out, str, strspec);
        }
        else {
            format_integer(out, value, spec);
        }
    }

    namespace impl {
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

        template<>
        struct repr_char<char16_t> {
            static inline void write_prefix(std::basic_ostream<char16_t>& out) {
                out.put('u');
            }
        };

        template<>
        struct repr_char<char32_t> {
            static inline void write_prefix(std::basic_ostream<char32_t>& out) {
                out.put('U');
            }
        };
    }

    template<typename Char>
    void repr_char(std::basic_ostream<Char>& out, Char value) {
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
    void repr_string(std::basic_ostream<Char>& out, const Char* value) {
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

    // ---- format_value impl ----

    template<typename Char>
    void format_value(std::basic_ostream<Char>& out, bool value, const BasicFormatSpec<Char>& spec) {
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

    inline void format_value(std::basic_ostream<char16_t>& out, char16_t value, const U16FormatSpec& spec) { format_char(out, value, spec); }
    inline void format_value(std::basic_ostream<char32_t>& out, char32_t value, const U32FormatSpec& spec) { format_char(out, value, spec); }
    inline void format_value(std::wostream& out, wchar_t value, const WFormatSpec& spec) { format_char(out, value, spec); }

    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, Char value, const BasicFormatSpec<Char>& spec) { format_char(out, value, spec); }

    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, char      value, const BasicFormatSpec<Char>& spec) { format_char(out, (Char)value, spec); }
    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, short     value, const BasicFormatSpec<Char>& spec) { format_integer(out, value, spec); }
    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, int       value, const BasicFormatSpec<Char>& spec) { format_integer(out, value, spec); }
    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, long      value, const BasicFormatSpec<Char>& spec) { format_integer(out, value, spec); }
    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, long long value, const BasicFormatSpec<Char>& spec) { format_integer(out, value, spec); }

    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, unsigned char      value, const BasicFormatSpec<Char>& spec) { format_char(out, (Char)value, spec); }
    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, unsigned short     value, const BasicFormatSpec<Char>& spec) { format_integer(out, value, spec); }
    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, unsigned int       value, const BasicFormatSpec<Char>& spec) { format_integer(out, value, spec); }
    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, unsigned long      value, const BasicFormatSpec<Char>& spec) { format_integer(out, value, spec); }
    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, unsigned long long value, const BasicFormatSpec<Char>& spec) { format_integer(out, value, spec); }

    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, float  value, const BasicFormatSpec<Char>& spec) { format_float(out, value, spec); }
    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, double value, const BasicFormatSpec<Char>& spec) { format_float(out, value, spec); }

    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, const std::basic_string<Char>& str, const BasicFormatSpec<Char>& spec) { format_string(out, str.c_str(), spec); }
    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, const Char* str, const BasicFormatSpec<Char>& spec) { format_string(out, str, spec); }

    // --- repr_value impl ----

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, bool value) {
        const Char* str = value ? impl::basic_names<Char>::TRUE_LOWER : impl::basic_names<Char>::FALSE_LOWER;
        out.write(str, std::char_traits<Char>::length(str));
    }

    inline void repr_value(std::basic_ostream<char16_t>& out, char16_t value) { repr_char(out, value); }
    inline void repr_value(std::basic_ostream<char32_t>& out, char32_t value) { repr_char(out, value); }
    inline void repr_value(std::wostream& out, wchar_t value) { repr_char(out, value); }

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, char      value) { repr_char(out, (Char)value); }
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, short     value) { out << value; }
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, int       value) { out << value; }
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, long      value) { out << value; }
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, long long value) { out << value; }

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, unsigned char      value) { repr_char(out, (Char)value); }
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, unsigned short     value) { out << value; }
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, unsigned int       value) { out << value; }
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, unsigned long      value) { out << value; }
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, unsigned long long value) { out << value; }

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, float  value) { out << value; }
    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, double value) { out << value; }

    template<typename Char> void repr_value(std::basic_ostream<Char>& out, const std::basic_string<Char>& value) { repr_string(out, value.c_str()); }
    template<typename Char> void repr_value(std::basic_ostream<Char>& out, const Char* value) { repr_string(out, value); }

    namespace impl {
        template<typename Char, std::size_t N, typename... Args>
        struct format_tail {
            static inline void format(std::basic_ostream<Char>& out, const std::tuple<Args...>& value) {
                format_tail<Char,N-1,Args...>::format(out, value);
                out.put(',');
                out.put(' ');
                repr_value(out, std::get<N-1>(value));
            }
        };

        template<typename Char, typename... Args>
        struct format_tail<Char, 0, Args...> {
            static inline void format(std::basic_ostream<Char>& out, const std::tuple<Args...>& value) {
                (void)out;
                (void)value;
            }
        };

        template<typename Char, typename... Args>
        struct format_tail<Char, 1, Args...> {
            static inline void format(std::basic_ostream<Char>& out, const std::tuple<Args...>& value) {
                repr_value(out, std::get<0>(value));
            }
        };

        template<typename Char, std::size_t N, typename... Args>
        struct format_tuple {
            static inline void format(std::basic_ostream<Char>& out, const std::tuple<Args...>& value) {
                format_tail< Char, std::tuple_size< std::tuple<Args...> >::value, Args...>::format(out, value);
            }
        };

        template<typename Char, typename... Args>
        struct format_tuple<Char, 1, Args...> {
            static inline void format(std::basic_ostream<Char>& out, const std::tuple<Args...>& value) {
                repr_value(out, std::get<0>(value));
                out.put(',');
            }
        };
    }

    // --- format_value for complex types ----

    template<typename Char, typename... Args>
    void format_value(std::basic_ostream<Char>& out, const std::tuple<Args...>& value, const BasicFormatSpec<Char>& spec) {
        std::basic_ostringstream<Char> buffer;

        repr_value(buffer, value);

        format_value(out, buffer.str(), spec);
    }

    template<typename Char, typename First, typename Second>
    void format_value(std::basic_ostream<Char>& out, const std::pair<First,Second>& value, const FormatSpec& spec) {
        std::basic_ostringstream<Char> buffer;

        repr_value(buffer, value);

        format_value(out, buffer.str(), spec);
    }

    template<typename Char, typename Iter>
    void format_slice(std::basic_ostream<Char>& out, Iter begin, Iter end, const BasicFormatSpec<Char>& spec, Char left = '[', Char right = ']') {
        std::basic_ostringstream<Char> buffer;

        repr_slice(buffer, begin, end, left, right);

        format_value(out, buffer.str(), spec);
    }

    template<typename Char, typename Iter>
    void format_map(std::basic_ostream<Char>& out, Iter begin, Iter end, const BasicFormatSpec<Char>& spec, Char left = '{', Char right = '}') {
        std::basic_ostringstream<Char> buffer;

        repr_map(buffer, begin, end, left, right);

        format_value(out, buffer.str(), spec);
    }

    template<typename Char, typename T>
    void format_value_fallback(std::basic_ostream<Char>& out, const T& value, const BasicFormatSpec<Char>& spec) {
        std::basic_ostringstream<Char> buffer;
        buffer << value;
        format_value(out, buffer.str(), spec);
    }

    // --- repr_value for complex types ----

    template<typename Char, typename... Args>
    void repr_value(std::basic_ostream<Char>& out, const std::tuple<Args...>& value) {
        out.put('(');
        impl::format_tuple<Char,std::tuple_size< std::tuple<Args...> >::value, Args...>::format(out, value);
        out.put(')');
    }

    template<typename Char, typename First, typename Second>
    void repr_value(std::basic_ostream<Char>& out, const std::pair<First,Second>& value) {
        out.put('(');
        repr_value(out, value.first);
        out.put(',');
        out.put(' ');
        repr_value(out, value.second);
        out.put(')');
    }

    template<typename Char, typename Iter>
    void repr_slice(std::basic_ostream<Char>& out, Iter begin, Iter end, Char left = '[', Char right = ']') {
        out.put(left);
        if (begin != end) {
            repr_value(out, *begin);

            for (++ begin; begin != end; ++ begin) {
                out.put(',');
                out.put(' ');
                repr_value(out, *begin);
            }
        }
        out.put(right);
    }

    template<typename Char, typename K, typename V>
    void repr_map_item(std::basic_ostream<Char>& out, const std::pair<K,V>& item) {
        repr_value(out, item.first);
        out.put(':');
        out.put(' ');
        repr_value(out, item.second);
    }

    template<typename Char, typename Iter>
    void repr_map(std::basic_ostream<Char>& out, Iter begin, Iter end, Char left = '{', Char right = '}') {
        out.put(left);
        if (begin != end) {
            repr_map_item(out, *begin);

            for (++ begin; begin != end; ++ begin) {
                out.put(',');
                out.put(' ');
                repr_map_item(out, *begin);
            }
        }
        out.put(right);
    }

    template<typename Char, typename T>
    void repr_value_fallback(std::basic_ostream<Char>& out, const T& value) {
        out << value;
    }
}

#endif // FORMATSTRING_FORMATVALUE_H
