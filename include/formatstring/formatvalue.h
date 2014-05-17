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
    inline void repr_value(std::ostream& out, long long value) { out << value; }

    inline void repr_value(std::ostream& out, unsigned char value) { repr_value(out, (char)value); }
    inline void repr_value(std::ostream& out, unsigned short value) { out << value; }
    inline void repr_value(std::ostream& out, unsigned int value) { out << value; }
    inline void repr_value(std::ostream& out, unsigned long value) { out << value; }
    inline void repr_value(std::ostream& out, unsigned long long value) { out << value; }

    inline void repr_value(std::ostream& out, float value) { out << value; }
    inline void repr_value(std::ostream& out, double value) { out << value; }

    void repr_value(std::ostream& out, const std::string& value);
    void repr_value(std::ostream& out, const char* value);

    template<typename... Args>
    void repr_value(std::ostream& out, const std::tuple<Args...>& value);

    template<typename First, typename Second>
    void repr_value(std::ostream& out, const std::pair<First,Second>& value);

    template<typename Iter>
    void repr_slice(std::ostream& out, Iter begin, Iter end, char left = '[', char right = ']');

    template<typename Iter>
    void repr_map(std::ostream& out, Iter begin, Iter end, char left = '{', char right = '}');

    void format_value(std::ostream& out, bool value, const FormatSpec& spec);

    void format_value(std::ostream& out, char value, const FormatSpec& spec);
    void format_value(std::ostream& out, short value, const FormatSpec& spec);
    void format_value(std::ostream& out, int value, const FormatSpec& spec);
    void format_value(std::ostream& out, long value, const FormatSpec& spec);
    void format_value(std::ostream& out, long long value, const FormatSpec& spec);

    void format_value(std::ostream& out, unsigned char value, const FormatSpec& spec);
    void format_value(std::ostream& out, unsigned short value, const FormatSpec& spec);
    void format_value(std::ostream& out, unsigned int value, const FormatSpec& spec);
    void format_value(std::ostream& out, unsigned long value, const FormatSpec& spec);
    void format_value(std::ostream& out, unsigned long long value, const FormatSpec& spec);

    void format_value(std::ostream& out, float value, const FormatSpec& spec);
    void format_value(std::ostream& out, double value, const FormatSpec& spec);

    void format_value(std::ostream& out, const std::string& str, const FormatSpec& spec);
    void format_value(std::ostream& out, const char* str, const FormatSpec& spec);

    template<typename Iter>
    void format_slice(std::ostream& out, Iter begin, Iter end, const FormatSpec& spec, char left = '[', char right = ']');

    template<typename T>
    void format_value_fallback(std::ostream& out, const T& value, const FormatSpec& spec);

    template<typename Iter>
    void format_map(std::ostream& out, Iter begin, Iter end, const FormatSpec& spec, char left = '{', char right = '}');

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
    void format_value(std::ostream& out, const std::tuple<Args...>& value, const FormatSpec& spec) {
        std::stringstream buffer;

        repr_value(buffer, value);

        format_value(out, buffer.str(), spec);
    }

    template<typename First, typename Second>
    void format_value(std::ostream& out, const std::pair<First,Second>& value, const FormatSpec& spec) {
        std::stringstream buffer;

        repr_value(buffer, value);

        format_value(out, buffer.str(), spec);
    }

    template<typename Iter>
    void format_slice(std::ostream& out, Iter begin, Iter end, const FormatSpec& spec, char left = '[', char right = ']') {
        std::stringstream buffer;

        repr_slice(buffer, begin, end, left, right);

        format_value(out, buffer.str(), spec);
    }

    template<typename Iter>
    void format_map(std::ostream& out, Iter begin, Iter end, const FormatSpec& spec, char left = '{', char right = '}') {
        std::stringstream buffer;

        repr_map(buffer, begin, end, left, right);

        format_value(out, buffer.str(), spec);
    }

    template<typename T>
    void format_value_fallback(std::ostream& out, const T& value, const FormatSpec& spec) {
        (void)spec;
        out << value;
    }

    template<typename... Args>
    void repr_value(std::ostream& out, const std::tuple<Args...>& value) {
        out.put('(');
        format_tuple<std::tuple_size< std::tuple<Args...> >::value, Args...>::format(out, value);
        out.put(')');
    }

    template<typename First, typename Second>
    void repr_value(std::ostream& out, const std::pair<First,Second>& value) {
        out.put('(');
        repr_value(out, value.first);
        out.write(", ", 2);
        repr_value(out, value.second);
        out.put(')');
    }

    template<typename Iter>
    void repr_slice(std::ostream& out, Iter begin, Iter end, char left = '[', char right = ']') {
        out.put(left);
        if (begin != end) {
            repr_value(out, *begin);

            for (++ begin; begin != end; ++ begin) {
                out.write(", ", 2);
                repr_value(out, *begin);
            }
        }
        out.put(right);
    }

    template<typename K, typename V>
    void repr_map_item(std::ostream& out, const std::pair<K,V>& item) {
        repr_value(out, item.first);
        out.write(": ",2);
        repr_value(out, item.second);
    }

    template<typename Iter>
    void repr_map(std::ostream& out, Iter begin, Iter end, char left = '{', char right = '}') {
        out.put(left);
        if (begin != end) {
            repr_map_item(out, *begin);

            for (++ begin; begin != end; ++ begin) {
                out.write(", ", 2);
                repr_map_item(out, *begin);
            }
        }
        out.put(right);
    }

    template<typename T>
    void repr_value_fallback(std::ostream& out, const T& value) {
        out << value;
    }
}

#endif // FORMATSTRING_FORMATVALUE_H
