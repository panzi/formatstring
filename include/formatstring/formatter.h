#ifndef FORMATSTRING_FORMATTER_H
#define FORMATSTRING_FORMATTER_H

#include <iosfwd>
#include <memory>
#include <vector>
#include <list>
#include <array>
#include <tuple>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "formatstring/format.h"
#include "formatstring/formatvalue.h"
#include "formatstring/conversion.h"
#include "formatstring/formatspec.h"

namespace formatstring {

    template<typename Char>
    class BasicFormatter {
    public:
        typedef Char char_type;
        typedef std::vector< std::unique_ptr< BasicFormatter<char_type> > > List;

        virtual ~BasicFormatter() {}
        virtual void format(std::basic_ostream<char_type>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const = 0;
    };

    typedef BasicFormatter<char> Formatter;
    typedef Formatter::List Formatters;

    template<typename Char, typename First, typename... Rest>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const First& first, const Rest&... rest);

    template<typename Char, typename... Rest>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const char first[], const Rest&... rest);

    template<typename Char, typename Last>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const Last& last);

    template<typename Char>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const char last[]);

    template<typename Char, typename T,
             void _format(std::basic_ostream<Char>& out, T value, const BasicFormatSpec<Char>& spec) = format_value,
             void _repr(std::basic_ostream<Char>& out, T value) = repr_value>
    class ValueFormatter : public BasicFormatter<Char> {
    public:
        typedef Char char_type;
        typedef T value_type;

        ValueFormatter(const value_type& value) : value(value) {}

        virtual void format(std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const {
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
        }

    private:
        const value_type value;
    };

    template<typename Char, typename T, typename Ptr = const T*,
             void _format(std::basic_ostream<Char>& out, const T& value, const BasicFormatSpec<Char>& spec) = format_value,
             void _repr(std::basic_ostream<Char>& out, const T& value) = repr_value>
    class PtrFormatter : public BasicFormatter<Char> {
    public:
        typedef Char char_type;
        typedef T value_type;
        typedef Ptr ptr_type;

        PtrFormatter(ptr_type ptr) : ptr(ptr) {}

        virtual void format(std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const {
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
        }

    private:
        ptr_type ptr;
    };

    template<typename Char,typename T>
    class FallbackFormatter : public PtrFormatter<Char,T,const T*,format_value_fallback,repr_value_fallback> {
    public:
        typedef Char char_type;
        typedef PtrFormatter<Char,T,const T*,format_value_fallback,repr_value_fallback> super_type;
        typedef typename super_type::value_type value_type;
        typedef typename super_type::ptr_type ptr_type;

        FallbackFormatter(ptr_type ptr) : super_type(ptr) {}
    };

    template<typename Char, typename Iter, Char left = '[', Char right = ']',
             void _format(std::basic_ostream<Char>& out, Iter begin, Iter end, const BasicFormatSpec<Char>& spec, Char left, Char right) = format_slice,
             void _repr(std::basic_ostream<Char>& out, Iter begin, Iter end, Char left, Char right) = repr_slice>
    class SliceFormatter : public BasicFormatter<Char> {
    public:
        typedef Char char_type;
        typedef Iter iterator_type;
        typedef typename std::iterator_traits<iterator_type>::value_type value_type;

        SliceFormatter(iterator_type begin, iterator_type end) :
            begin(begin), end(end) {}

        virtual void format(std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const {
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
        }

        const iterator_type begin;
        const iterator_type end;
    };
}

// include is here because of circular dependency
#include "formatstring/format_traits.h"

// ==== unpack_formatters ====
template<typename Char, typename First, typename... Rest>
inline void formatstring::unpack_formatters(typename BasicFormatter<Char>::List& formatters, const First& first, const Rest&... rest) {
    formatters.emplace_back(format_traits<Char,First>::make_formatter(first));
    unpack_formatters<Char, Rest...>(formatters, rest...);
}

template<typename Char, typename Last>
inline void formatstring::unpack_formatters(typename BasicFormatter<Char>::List& formatters, const Last& last) {
    formatters.emplace_back(format_traits<Char,Last>::make_formatter(last));
}

#endif // FORMATSTRING_FORMATTER_H
