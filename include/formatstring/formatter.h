#ifndef FORMATSTRING_FORMATTER_H
#define FORMATSTRING_FORMATTER_H
#pragma once

#include <iosfwd>
#include <memory>
#include <vector>
#include <functional>

#include "formatstring/config.h"
#include "formatstring/formatvalue.h"
#include "formatstring/conversion.h"
#include "formatstring/formatspec.h"
#include "formatstring/format_traits_fwd.h"

namespace formatstring {

    template<typename Char>
    using BasicFormatter = std::function<void(std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec)>;

    template<typename Char>
    using BasicFormatters = std::vector< BasicFormatter<Char> >;

    typedef BasicFormatter<char> Formatter;
    typedef BasicFormatters<char> Formatters;

    typedef BasicFormatter<wchar_t> WFormatter;
    typedef BasicFormatters<wchar_t> WFormatters;

#ifdef FORMATSTRING_CHAR16_SUPPORT
    typedef BasicFormatter<char16_t> U16Formatter;
    typedef BasicFormatters<char16_t> U16Formatters;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    typedef BasicFormatter<char32_t> U16Formatter;
    typedef BasicFormatters<char32_t> U16Formatters;
#endif

    template<typename Char, typename First, typename... Rest>
    inline void unpack_formatters(BasicFormatters<Char>& formatters, const First& first, const Rest&... rest);

    template<typename Char, typename Last>
    inline void unpack_formatters(BasicFormatters<Char>& formatters, const Last& last);

    template<typename Char, typename First, typename... Rest>
    inline void unpack_formatters(BasicFormatters<Char>& formatters, const First& first, const Rest&... rest) {
        formatters.emplace_back(format_traits<Char,First>::make_formatter(first));
        unpack_formatters<Char, Rest...>(formatters, rest...);
    }

    template<typename Char, typename Last>
    inline void unpack_formatters(BasicFormatters<Char>& formatters, const Last& last) {
        formatters.emplace_back(format_traits<Char,Last>::make_formatter(last));
    }

    template<typename Char, typename T,
             void _format(std::basic_ostream<Char>& out, T value, const BasicFormatSpec<Char>& spec) = format_value,
             void _repr(std::basic_ostream<Char>& out, T value) = repr_value>
    BasicFormatter<Char> make_value_formatter(T value) {
        return [value](std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) {
            switch (conv) {
            case ReprConv:
            {
                std::basic_ostringstream<Char> buffer;
                _repr(buffer, value);
                format_value(out, buffer.str(), spec);
                break;
            }
            case StrConv:
            {
                std::basic_ostringstream<Char> buffer;
                _format(buffer, value, BasicFormatSpec<Char>::DEFAULT);
                format_value(out, buffer.str(), spec);
                break;
            }
            default:
                _format(out, value, spec);
                break;
            }
        };
    }

    template<typename Char, typename T, typename Ptr = const T*,
             void _format(std::basic_ostream<Char>& out, const T& value, const BasicFormatSpec<Char>& spec) = format_value,
             void _repr(std::basic_ostream<Char>& out, const T& value) = repr_value>
    BasicFormatter<Char> make_ptr_formatter(Ptr ptr) {
        return [ptr](std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) {
            switch (conv) {
            case ReprConv:
            {
                std::basic_ostringstream<Char> buffer;
                _repr(buffer, *ptr);
                format_value(out, buffer.str(), spec);
                break;
            }
            case StrConv:
            {
                std::basic_ostringstream<Char> buffer;
                _format(buffer, *ptr, BasicFormatSpec<Char>::DEFAULT);
                format_value(out, buffer.str(), spec);
                break;
            }
            default:
                _format(out, *ptr, spec);
                break;
            }
        };
    }

    template<typename Char, typename T>
    inline BasicFormatter<Char> make_fallback_formatter(const T* ptr) {
        return make_ptr_formatter<Char,T,const T*,format_value_fallback,repr_value_fallback>(ptr);
    }

    template<typename Char, typename Iter, Char left = '[', Char right = ']',
             void _format(std::basic_ostream<Char>& out, Iter begin, Iter end, const BasicFormatSpec<Char>& spec, Char left, Char right) = format_slice,
             void _repr(std::basic_ostream<Char>& out, Iter begin, Iter end, Char left, Char right) = repr_slice>
    BasicFormatter<Char> make_slice_formatter(Iter begin, Iter end) {
        return [begin, end](std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) {
            switch (conv) {
            case ReprConv:
            {
                std::basic_ostringstream<Char> buffer;
                _repr(buffer, begin, end, left, right);
                format_value(out, buffer.str(), spec);
                break;
            }
            case StrConv:
            {
                std::basic_ostringstream<Char> buffer;
                _format(buffer, begin, end, BasicFormatSpec<Char>::DEFAULT, left, right);
                format_value(out, buffer.str(), spec);
                break;
            }
            default:
                _format(out, begin, end, spec, left, right);
                break;
            }
        };
    }
}

#endif // FORMATSTRING_FORMATTER_H
