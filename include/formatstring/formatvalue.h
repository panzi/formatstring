#ifndef FORMATSTRING_FORMATVALUE_H
#define FORMATSTRING_FORMATVALUE_H

#include <iosfwd>
#include <sstream>
#include <tuple>

#include "formatstring/formatspec.h"

namespace formatstring {

    class FormatSpec;

    template<typename T>
    void repr_value(std::ostream& out, T value);

    void repr_value(std::ostream& out, const std::string& value);
    void repr_value(std::ostream& out, const char* value);

    void format_value(std::ostream& out, bool value, const FormatSpec& spec);

    void format_value(std::ostream& out, char value, const FormatSpec& spec);
    void format_value(std::ostream& out, short value, const FormatSpec& spec);
    void format_value(std::ostream& out, int value, const FormatSpec& spec);
    void format_value(std::ostream& out, long value, const FormatSpec& spec);

    void format_value(std::ostream& out, unsigned char value, const FormatSpec& spec);
    void format_value(std::ostream& out, unsigned short value, const FormatSpec& spec);
    void format_value(std::ostream& out, unsigned int value, const FormatSpec& spec);
    void format_value(std::ostream& out, unsigned long value, const FormatSpec& spec);

    void format_value(std::ostream& out, float value, const FormatSpec& spec);
    void format_value(std::ostream& out, double value, const FormatSpec& spec);

    void format_value(std::ostream& out, const std::string& str, const FormatSpec& spec);
    void format_value(std::ostream& out, const char* str, const FormatSpec& spec);

    template<std::size_t N, typename... Args>
    struct format_tail {
        static inline void format(std::ostream& out, const std::tuple<Args...>& value, const FormatSpec& spec) {
            format_tail<N-1,Args...>::format(out, value, spec);
            out.write(", ", 2);
            format_value(out, std::get<N-1>(value), spec);
        }
    };

    template<typename... Args>
    struct format_tail<0, Args...> {
        static inline void format(std::ostream& out, const std::tuple<Args...>& value, const FormatSpec& spec) {
            (void)out;
            (void)value;
            (void)spec;
        }
    };

    template<typename... Args>
    struct format_tail<1, Args...> {
        static inline void format(std::ostream& out, const std::tuple<Args...>& value, const FormatSpec& spec) {
            format_value(out, std::get<0>(value), spec);
        }
    };

    template<typename... Args>
    void format_value(std::ostream& out, const std::tuple<Args...>& value, const FormatSpec& spec, char left = '(', char right = ')') {
        std::stringstream buffer;
        FormatSpec valspec;

        // only copy what makes sense:
        valspec.upperCase = spec.upperCase;
        valspec.alternate = spec.alternate;
        valspec.precision = spec.precision;
        valspec.sign      = spec.sign;
        valspec.type      = spec.type;

        buffer.put(left);
        format_tail<std::tuple_size< std::tuple<Args...> >::value, Args...>::format(buffer, value, valspec);
        buffer.put(right);

        FormatSpec strspec;
        strspec.fill      = spec.fill;
        strspec.alignment = spec.alignment;
        strspec.width     = spec.width;
        format_value(out, buffer.str(), strspec);
    }

    template<typename Iter>
    void format_slice(std::ostream& out, Iter begin, Iter end, const FormatSpec& spec, char left = '[', char right = ']') {
        std::stringstream buffer;

        buffer.put(left);
        if (begin != end) {
            FormatSpec valspec;

            // only copy what makes sense:
            valspec.upperCase = spec.upperCase;
            valspec.alternate = spec.alternate;
            valspec.precision = spec.precision;
            valspec.sign      = spec.sign;
            valspec.type      = spec.type;

            format_value(buffer, *begin, valspec);

            for (++ begin; begin != end; ++ begin) {
                buffer.write(", ", 2);
                format_value(buffer, *begin, valspec);
            }
        }
        buffer.put(right);

        FormatSpec strspec;
        strspec.fill      = spec.fill;
        strspec.alignment = spec.alignment;
        strspec.width     = spec.width;
        format_value(out, buffer.str(), strspec);
    }

    template<typename T>
    void repr_value(std::ostream& out, T value) {
        format_value(out, value, FormatSpec());
    }
}

#endif // FORMATSTRING_FORMATVALUE_H
