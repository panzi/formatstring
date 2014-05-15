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

    template<typename T>
    class GenericFormatter : public Formatter {
    public:
        typedef T value_type;

        GenericFormatter(const value_type& value) : value(value) {}

        virtual void format(std::ostream& out, Conversion conv, const FormatSpec& spec) const {
            (void)conv;
            std::stringstream buffer;
            buffer << value;
            format_value(out, buffer.str(), spec);
        }

        virtual GenericFormatter<T>* clone() const {
            return new GenericFormatter<T>(value);
        }

        const value_type value;
    };

    template<typename Iter>
    class SliceFormatter : public Formatter {
    public:
        typedef Iter iterator_type;
        typedef typename std::iterator_traits<iterator_type>::value_type value_type;

        SliceFormatter(iterator_type begin, iterator_type end) :
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

    inline ValueFormatter<bool>* make_formatter(bool value) { return new ValueFormatter<bool>(value); }

    inline ValueFormatter<char>*  make_formatter(char  value) { return new ValueFormatter<char>(value); }
    inline ValueFormatter<short>* make_formatter(short value) { return new ValueFormatter<short>(value); }
    inline ValueFormatter<int>*   make_formatter(int   value) { return new ValueFormatter<int>(value); }
    inline ValueFormatter<long>*  make_formatter(long  value) { return new ValueFormatter<long>(value); }

    inline ValueFormatter<unsigned char>*  make_formatter(unsigned char  value) { return new ValueFormatter<unsigned char>(value); }
    inline ValueFormatter<unsigned short>* make_formatter(unsigned short value) { return new ValueFormatter<unsigned short>(value); }
    inline ValueFormatter<unsigned int>*   make_formatter(unsigned int   value) { return new ValueFormatter<unsigned int>(value); }
    inline ValueFormatter<unsigned long>*  make_formatter(unsigned long  value) { return new ValueFormatter<unsigned long>(value); }

    inline ValueFormatter<float>*  make_formatter(float  value) { return new ValueFormatter<float>(value); }
    inline ValueFormatter<double>* make_formatter(double value) { return new ValueFormatter<double>(value); }

    inline ValueFormatter<const char*>* make_formatter(const char value[]) {
        return new ValueFormatter<const char*>(value);
    }

    inline ValueFormatter<std::string>* make_formatter(const std::string& value) {
        return new ValueFormatter<std::string>(value);
    }

    template<typename T>
    inline GenericFormatter<T>* make_formatter(const T& value) {
        return new GenericFormatter<T>(value);
    }

    template<typename T>
    inline SliceFormatter<typename std::vector<T>::const_iterator>* make_formatter(const std::vector<T>& value) {
        return new SliceFormatter<typename std::vector<T>::const_iterator>(value.begin(), value.end());
    }

    template<typename T, std::size_t N>
    inline SliceFormatter<typename std::array<T,N>::const_iterator>* make_formatter(const std::array<T,N>& value) {
        return new SliceFormatter<typename std::array<T,N>::const_iterator>(value.begin(), value.end());
    }

    template<typename... Args>
    inline ValueFormatter< std::tuple<Args...> >* make_formatter(const std::tuple<Args...>& value) {
        return new ValueFormatter< std::tuple<Args...> >(value);
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
