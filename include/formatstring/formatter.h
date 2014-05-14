#ifndef FORMATSTRING_FORMATTER_H
#define FORMATSTRING_FORMATTER_H

#include <iosfwd>
#include <memory>
#include <vector>
#include <array>
#include <tuple>

#include "formatstring/formatvalue.h"
#include "formatstring/conversion.h"

namespace formatstring {

    class Format;
    class FormatSpec;
    class Formatter;

    typedef std::vector< std::unique_ptr<Formatter> > Formatters;

    class Formatter {
    public:
        virtual ~Formatter() {}
        virtual void format(std::ostream& out, Conversion conv, const FormatSpec& spec) const = 0;
        virtual Formatter* clone() const = 0;

        template<typename First, typename... Rest>
        inline static void extend(Formatters& formatters, const First& first, const Rest&... rest);

        template<typename... Rest>
        inline static void extend(Formatters& formatters, const char first[], const Rest&... rest);

        template<typename Last>
        inline static void extend(Formatters& formatters, const Last& last);

        inline static void extend(Formatters& formatters, const char last[]);
    };

    template<typename T>
    class ValueFormatter : public Formatter {
    public:
        typedef T value_type;

        ValueFormatter(const value_type& value) : value(value) {}

        virtual void format(std::ostream& out, Conversion conv, const FormatSpec& spec) const {
            switch (conv) {
            case ReprConv:
            {
                std::stringstream buffer;
                repr_value(buffer, value);
                format_value(out, buffer.str(), spec);
                break;
            }

            case StrConv:
            {
                std::stringstream buffer;
                format_value(buffer, value, FormatSpec());
                format_value(out, buffer.str(), spec);
                break;
            }
            default:
                format_value(out, value, spec);
                break;
            }
        }

        virtual ValueFormatter<T>* clone() const {
            return new ValueFormatter<T>(value);
        }

        const value_type value;
    };

    template<typename Iter>
    class SliceFormatter : public Formatter {
    public:
        typedef Iter iterator_type;
        typedef typename std::iterator_traits<Iter>::value_type value_type;

        SliceFormatter(Iter begin, Iter end) :
            begin(begin), end(end) {}

        virtual void format(std::ostream& out, Conversion conv, const FormatSpec& spec) const {
            switch (conv) {
            case ReprConv:
            case StrConv:
            {
                std::stringstream buffer;
                format_slice(buffer, begin, end, FormatSpec());
                format_value(out, buffer.str(), spec);
                break;
            }
            default:
                format_slice(out, begin, end, spec);
                break;
            }
        }

        virtual SliceFormatter<Iter>* clone() const {
            return new SliceFormatter<Iter>(begin, end);
        }

        const iterator_type begin;
        const iterator_type end;
    };

    template<typename T>
    inline ValueFormatter<T>* make_formatter(const T& value) {
        return new ValueFormatter<T>(value);
    }

    inline ValueFormatter<const char*>* make_formatter(const char value[]) {
        return new ValueFormatter<const char*>(value);
    }

    template<typename T>
    inline SliceFormatter<typename std::vector<T>::const_iterator>* make_formatter(const std::vector<T>& value) {
        return new SliceFormatter<typename std::vector<T>::const_iterator>(value.begin(), value.end());
    }

    template<typename T, std::size_t N>
    inline SliceFormatter<typename std::array<T,N>::const_iterator>* make_formatter(const std::array<T,N>& value) {
        return new SliceFormatter<typename std::array<T,N>::const_iterator>(value.begin(), value.end());
    }

    template<typename First, typename... Rest>
    inline void Formatter::extend(Formatters& formatters, const First& first, const Rest&... rest) {
        formatters.emplace_back(make_formatter(first));
        extend(formatters, rest...);
    }

    template<typename... Rest>
    inline void Formatter::extend(Formatters& formatters, const char first[], const Rest&... rest) {
        formatters.emplace_back(make_formatter(first));
        extend(formatters, rest...);
    }

    template<typename Last>
    inline void Formatter::extend(Formatters& formatters, const Last& last) {
        formatters.emplace_back(make_formatter(last));
    }

    inline void Formatter::extend(Formatters& formatters, const char last[]) {
        formatters.emplace_back(make_formatter(last));
    }
}

#endif // FORMATSTRING_FORMATTER_H
