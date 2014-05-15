#ifndef FORMATSTRING_FORMATVALUE_H
#define FORMATSTRING_FORMATVALUE_H

#include <iosfwd>
#include <sstream>
#include <tuple>

#include "formatstring/formatspec.h"

namespace formatstring {

    class FormattedValue;

    inline void repr_value(std::ostream& out, bool value) { out << (value ? "true" : "false"); }

    void repr_value(std::ostream& out, char value);
    inline void repr_value(std::ostream& out, short value) { out << value; }
    inline void repr_value(std::ostream& out, int value) { out << value; }
    inline void repr_value(std::ostream& out, long value) { out << value; }

    inline void repr_value(std::ostream& out, unsigned char value) { repr_value(out, (char)value); }
    inline void repr_value(std::ostream& out, unsigned short value) { out << value; }
    inline void repr_value(std::ostream& out, unsigned int value) { out << value; }
    inline void repr_value(std::ostream& out, unsigned long value) { out << value; }

    inline void repr_value(std::ostream& out, float value) { out << value; }
    inline void repr_value(std::ostream& out, double value) { out << value; }

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
        static inline void format(std::ostream& out, const std::tuple<Args...>& value) {
            format_tail<N-1,Args...>::format(out, value);
            out.write(", ", 2);
            repr_value(out, std::get<N-1>(value));
        }
    };

    template<typename... Args>
    struct format_tail<0, Args...> {
        static inline void format(std::ostream& out, const std::tuple<Args...>& value) {
            (void)out;
            (void)value;
        }
    };

    template<typename... Args>
    struct format_tail<1, Args...> {
        static inline void format(std::ostream& out, const std::tuple<Args...>& value) {
            repr_value(out, std::get<0>(value));
        }
    };

    template<std::size_t N, typename... Args>
    struct format_tuple {
        static inline void format(std::ostream& out, const std::tuple<Args...>& value) {
            format_tail<std::tuple_size< std::tuple<Args...> >::value, Args...>::format(out, value);
        }
    };

    template<typename... Args>
    struct format_tuple<1, Args...> {
        static inline void format(std::ostream& out, const std::tuple<Args...>& value) {
            repr_value(out, std::get<0>(value));
            out.put(',');
        }
    };

    template<typename... Args>
    void format_value(std::ostream& out, const std::tuple<Args...>& value, const FormatSpec& spec, char left = '(', char right = ')') {
        std::stringstream buffer;

        buffer.put(left);
        format_tuple<std::tuple_size< std::tuple<Args...> >::value, Args...>::format(buffer, value);
        buffer.put(right);

        format_value(out, buffer.str(), spec);
    }

    template<typename Iter>
    void format_slice(std::ostream& out, Iter begin, Iter end, const FormatSpec& spec, char left = '[', char right = ']') {
        std::stringstream buffer;

        buffer.put(left);
        if (begin != end) {
            repr_value(buffer, *begin);

            for (++ begin; begin != end; ++ begin) {
                buffer.write(", ", 2);
                repr_value(buffer, *begin);
            }
        }
        buffer.put(right);

        format_value(out, buffer.str(), spec);
    }

    template<typename... Args>
    void repr_value(std::ostream& out, const std::tuple<Args...>& value) {
        format_value(out, value, FormatSpec());
    }
}

#endif // FORMATSTRING_FORMATVALUE_H
