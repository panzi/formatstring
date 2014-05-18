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

#include "formatstring/config.h"
#include "formatstring/export.h"
#include "formatstring/formatspec.h"

namespace formatstring {

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, bool value);

#ifdef FORMATSTRING_CHAR16_SUPPORT
    inline void repr_value(std::basic_ostream<char16_t>& out, char16_t value);
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    inline void repr_value(std::basic_ostream<char32_t>& out, char32_t value);
#endif

    inline void repr_value(std::wostream& out, wchar_t value);

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, bool value);

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

    template<typename Char> void format_bool(std::basic_ostream<Char>& out, bool value, const BasicFormatSpec<Char>& spec);
    template<typename Char> void format_char(std::basic_ostream<Char>& out, Char value, const BasicFormatSpec<Char>& spec);

    template<typename Char, typename Int, typename UInt = typename std::make_unsigned<Int>::type>
    void format_integer(std::basic_ostream<Char>& out, Int value, const BasicFormatSpec<Char>& spec);

    template<typename Char, typename Float>
    void format_float(std::basic_ostream<Char>& out, Float value, const BasicFormatSpec<Char>& spec);

    template<typename Char> void format_string(std::basic_ostream<Char>& out, const Char value[], const BasicFormatSpec<Char>& spec);

    template<typename Char> inline void format_value(std::basic_ostream<Char>& out, bool value, const BasicFormatSpec<Char>& spec);

#ifdef FORMATSTRING_CHAR16_SUPPORT
    inline void format_value(std::basic_ostream<char16_t>& out, char16_t value, const U16FormatSpec& spec);
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    inline void format_value(std::basic_ostream<char32_t>& out, char32_t value, const U32FormatSpec& spec);
#endif

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

    template<typename Char, typename Int, typename UInt = typename std::make_unsigned<Int>::type>
    void format_integer(std::basic_ostream<Char>& out, Int value, const BasicFormatSpec<Char>& spec);

    template<typename Char, typename Float>
    void format_float(std::basic_ostream<Char>& out, Float value, const BasicFormatSpec<Char>& spec);

    template<typename Char>
    void format_string(std::basic_ostream<Char>& out, const Char value[], const BasicFormatSpec<Char>& spec);

    template<typename Char>
    void repr_char(std::basic_ostream<Char>& out, Char value);

    template<typename Char>
    void repr_string(std::basic_ostream<Char>& out, const Char* value);

    // ---- format_value impl ----
    template<typename Char>
    inline void format_value(std::basic_ostream<Char>& out, bool value, const BasicFormatSpec<Char>& spec) { format_bool(out, value, spec); }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    inline void format_value(std::basic_ostream<char16_t>& out, char16_t value, const U16FormatSpec& spec) { format_char(out, value, spec); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    inline void format_value(std::basic_ostream<char32_t>& out, char32_t value, const U32FormatSpec& spec) { format_char(out, value, spec); }
#endif

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
    template<typename Char> void repr_bool(std::basic_ostream<Char>& out, bool value);

    template<typename Char> inline void repr_value(std::basic_ostream<Char>& out, bool value) { repr_bool(out, value); }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    inline void repr_value(std::basic_ostream<char16_t>& out, char16_t value) { repr_char(out, value); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    inline void repr_value(std::basic_ostream<char32_t>& out, char32_t value) { repr_char(out, value); }
#endif

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

    // ---- extern template instantiations ----
    extern template FORMATSTRING_EXPORT void repr_bool<char>(std::ostream& out, bool value);
    extern template FORMATSTRING_EXPORT void repr_bool<wchar_t>(std::wostream& out, bool value);

    extern template FORMATSTRING_EXPORT void repr_char<char>(std::ostream& out, char value);
    extern template FORMATSTRING_EXPORT void repr_char<wchar_t>(std::wostream& out, wchar_t value);

    extern template FORMATSTRING_EXPORT void repr_string<char>(std::ostream& out, const char* value);
    extern template FORMATSTRING_EXPORT void repr_string<wchar_t>(std::wostream& out, const wchar_t* value);

    extern template FORMATSTRING_EXPORT void format_bool<char>(std::ostream& out, bool value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_bool<wchar_t>(std::wostream& out, bool value, const WFormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_char<char>(std::ostream& out, char value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_char<wchar_t>(std::wostream& out, wchar_t value, const WFormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_string<char>(std::ostream& out, const char value[], const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_string<wchar_t>(std::wostream& out, const wchar_t value[], const WFormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_float<char,float>(std::ostream& out, float value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_float<wchar_t,float>(std::wostream& out, float value, const WFormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_float<char,double>(std::ostream& out, double value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_float<wchar_t,double>(std::wostream& out, double value, const WFormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<char,char>(std::ostream& out, char value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char,short>(std::ostream& out, short value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char,int>(std::ostream& out, int value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char,long>(std::ostream& out, long value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char,long long>(std::ostream& out, long long value, const FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<char,unsigned char>(std::ostream& out, unsigned char value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char,unsigned short>(std::ostream& out, unsigned short value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char,unsigned int>(std::ostream& out, unsigned int value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char,unsigned long>(std::ostream& out, unsigned long value, const FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char,unsigned long long>(std::ostream& out, unsigned long long value, const FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,wchar_t>(std::wostream& out, wchar_t value, const WFormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,char>(std::wostream& out, char value, const WFormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,short>(std::wostream& out, short value, const WFormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,int>(std::wostream& out, int value, const WFormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,long>(std::wostream& out, long value, const WFormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,long long>(std::wostream& out, long long value, const WFormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,unsigned char>(std::wostream& out, unsigned char value, const WFormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,unsigned short>(std::wostream& out, unsigned short value, const WFormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,unsigned int>(std::wostream& out, unsigned int value, const WFormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,unsigned long>(std::wostream& out, unsigned long value, const WFormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<wchar_t,unsigned long long>(std::wostream& out, unsigned long long value, const WFormatSpec& spec);

#ifdef FORMATSTRING_CHAR16_SUPPORT
    extern template FORMATSTRING_EXPORT void repr_bool<char16_t>(std::basic_ostream<char16_t>& out, bool value);
    extern template FORMATSTRING_EXPORT void repr_char<char16_t>(std::basic_ostream<char16_t>& out, char16_t value);
    extern template FORMATSTRING_EXPORT void repr_string<char16_t>(std::basic_stream<char16_t>& out, const char16_t* value);

    extern template FORMATSTRING_EXPORT void format_bool<char16_t>(std::basic_stream<char16_t>& out, bool value, const U16FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_char<char16_t>(std::basic_ostream<char16_t>& out, char16_t value, const U16FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_string<char16_t>(std::basic_ostream<char16_t>& out, const char16_t value[], const U16FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_float<char16_t,float>(std::basic_ostream<char16_t>& out, float value, const U16FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_float<char16_t,double>(std::basic_ostream<char16_t>& out, double value, const U16FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<char16_t,char16_t>(std::basic_ostream<char16_t>& out, char16_t value, const U16FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<char16_t,char>(std::basic_ostream<char16_t>& out, char value, const U16FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char16_t,short>(std::basic_ostream<char16_t>& out, short value, const U16FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char16_t,int>(std::basic_ostream<char16_t>& out, int value, const U16FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char16_t,long>(std::basic_ostream<char16_t>& out, long value, const U16FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char16_t,long long>(std::basic_ostream<char16_t>& out, long long value, const U16FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<char16_t,unsigned char>(std::basic_ostream<char16_t>& out, unsigned char value, const U16FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char16_t,unsigned short>(std::basic_ostream<char16_t>& out, unsigned short value, const U16FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char16_t,unsigned int>(std::basic_ostream<char16_t>& out, unsigned int value, const U16FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char16_t,unsigned long>(std::basic_ostream<char16_t>& out, unsigned long value, const U16FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char16_t,unsigned long long>(std::basic_ostream<char16_t>& out, unsigned long long value, const U16FormatSpec& spec);
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    extern template FORMATSTRING_EXPORT void repr_bool<char32_t>(std::basic_ostream<char32_t>& out, bool value);
    extern template FORMATSTRING_EXPORT void repr_char<char32_t>(std::basic_ostream<char32_t>& out, char32_t value);
    extern template FORMATSTRING_EXPORT void repr_string<char32_t>(std::basic_stream<char32_t>& out, const char32_t* value);

    extern template FORMATSTRING_EXPORT void format_bool<char32_t>(std::basic_stream<char32_t>& out, bool value, const U32FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_char<char32_t>(std::basic_ostream<char32_t>& out, char32_t value, const U32FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_string<char32_t>(std::basic_ostream<char32_t>& out, const char32_t value[], const U32FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_float<char32_t,float>(std::basic_ostream<char32_t>& out, float value, const U32FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_float<char32_t,double>(std::basic_ostream<char32_t>& out, double value, const U32FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<char32_t,char32_t>(std::basic_ostream<char32_t>& out, char32_t value, const U32FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<char32_t,char>(std::basic_ostream<char16_t>& out, char value, const U32FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char32_t,short>(std::basic_ostream<char32_t>& out, short value, const U32FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char32_t,int>(std::basic_ostream<char32_t>& out, int value, const U32FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char32_t,long>(std::basic_ostream<char32_t>& out, long value, const U32FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char32_t,long long>(std::basic_ostream<char32_t>& out, long long value, const U32FormatSpec& spec);

    extern template FORMATSTRING_EXPORT void format_integer<char32_t,unsigned char>(std::basic_ostream<char32_t>& out, unsigned char value, const U32FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char32_t,unsigned short>(std::basic_ostream<char32_t>& out, unsigned short value, const U32FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char32_t,unsigned int>(std::basic_ostream<char32_t>& out, unsigned int value, const U32FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char32_t,unsigned long>(std::basic_ostream<char32_t>& out, unsigned long value, const U32FormatSpec& spec);
    extern template FORMATSTRING_EXPORT void format_integer<char32_t,unsigned long long>(std::basic_ostream<char32_t>& out, unsigned long long value, const U32FormatSpec& spec);
#endif
}

#endif // FORMATSTRING_FORMATVALUE_H
