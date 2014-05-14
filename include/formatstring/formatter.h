#ifndef FORMATSTRING_FORMATTER_H
#define FORMATSTRING_FORMATTER_H

#include <iosfwd>
#include <memory>
#include <vector>

#include "formatstring/formatvalue.h"

namespace formatstring {

    class Format;
    class FormatSpec;
    class Formatter;

    typedef std::vector< std::unique_ptr<Formatter> > Formatters;

    class Formatter {
    public:
        virtual ~Formatter() {}
        virtual void format(std::ostream& out, const FormatSpec& spec) const = 0;
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

        virtual void format(std::ostream& out, const FormatSpec& spec) const {
            format_value(out, value, spec);
        }

        virtual ValueFormatter<T>* clone() const {
            return new ValueFormatter<T>(value);
        }

        const value_type value;
    };

    typedef ValueFormatter<bool>  BoolFormatter;

    typedef ValueFormatter<char>  CharFormatter;
    typedef ValueFormatter<short> ShortFormatter;
    typedef ValueFormatter<int>   IntFormatter;
    typedef ValueFormatter<long>  LongFormatter;

    typedef ValueFormatter<unsigned char>  UCharFormatter;
    typedef ValueFormatter<unsigned short> UShortFormatter;
    typedef ValueFormatter<unsigned int>   UIntFormatter;
    typedef ValueFormatter<unsigned long>  ULongFormatter;

    typedef ValueFormatter<float>  FloatFormatter;
    typedef ValueFormatter<double> DoubleFormatter;

    typedef ValueFormatter<std::string> StringFormatter;
    typedef ValueFormatter<const char*> CStringFormatter;

    template<typename First, typename... Rest>
    inline void Formatter::extend(Formatters& formatters, const First& first, const Rest&... rest) {
        formatters.emplace_back(new ValueFormatter<First>(first));
        extend(formatters, rest...);
    }

    template<typename... Rest>
    inline void Formatter::extend(Formatters& formatters, const char first[], const Rest&... rest) {
        formatters.emplace_back(new CStringFormatter(first));
        extend(formatters, rest...);
    }

    template<typename Last>
    inline void Formatter::extend(Formatters& formatters, const Last& last) {
        formatters.emplace_back(new ValueFormatter<Last>(last));
    }

    inline void Formatter::extend(Formatters& formatters, const char last[]) {
        formatters.emplace_back(new CStringFormatter(last));
    }
}

#endif // FORMATSTRING_FORMATTER_H
