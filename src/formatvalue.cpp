#include "formatstring/formatvalue.h"
#include "formatstring/formatspec.h"

#include <stdexcept>
#include <iomanip>
#include <cstring>
#include <iostream>
#include <locale>
#include <sstream>

using namespace formatstring;

struct group_thousands : public std::numpunct<char> {
    std::string do_grouping() const { return "\03"; }
    char do_thousands_sep() const { return ','; }
    char do_decimal_point() const { return '.'; }
};

struct no_grouping : public group_thousands {
    std::string do_grouping() const { return ""; }
};

template<typename Int>
struct make_unsigned;

template<> struct make_unsigned<char>  { typedef unsigned char  type; };
template<> struct make_unsigned<short> { typedef unsigned short type; };
template<> struct make_unsigned<int>   { typedef unsigned int   type; };
template<> struct make_unsigned<long>  { typedef unsigned long  type; };

template<> struct make_unsigned<unsigned char>  { typedef unsigned char  type; };
template<> struct make_unsigned<unsigned short> { typedef unsigned short type; };
template<> struct make_unsigned<unsigned int>   { typedef unsigned int   type; };
template<> struct make_unsigned<unsigned long>  { typedef unsigned long  type; };

static inline void fill(std::ostream& out, char fill, std::size_t width) {
    for (; width > 0; -- width) { out.put(fill); }
}

template<typename Int, typename UInt = typename make_unsigned<Int>::type>
static inline void format_integer(std::ostream& out, Int value, const FormatSpec& spec);

template<typename Float>
static inline void format_float(std::ostream& out, Float value, const FormatSpec& spec);

static inline void format_string(std::ostream& out, const char value[], const FormatSpec& spec);

template<typename Int, typename UInt = typename make_unsigned<Int>::type>
static inline void format_integer(std::ostream& out, Int value, const FormatSpec& spec) {
    if (spec.type == FormatSpec::Character) {
        char str[2] = {(char)value, 0};
        FormatSpec strspec = spec;
        strspec.type = FormatSpec::String;
        format_string(out, str, strspec);
        return;
    }
    else if (spec.isDecimalType()) {
        format_float<double>(out, value, spec);
        return;
    }

    bool negative = value < 0;
    UInt abs = negative ? -value : value;
    std::string prefix;

    switch (spec.sign) {
    case FormatSpec::NegativeOnly:
    case FormatSpec::DefaultSign:
        if (negative) {
            prefix += '-';
        }
        break;

    case FormatSpec::Always:
        prefix += negative ? '-' : '+';
        break;

    case FormatSpec::SpaceForPositive:
        prefix += negative ? '-' : ' ';
        break;
    }

    std::stringstream buffer;
    std::locale loc(std::locale(), spec.thoudsandsSeperator ? new group_thousands() : new no_grouping());

    buffer.imbue(loc);

    switch (spec.type) {
    case FormatSpec::Generic:
    case FormatSpec::Dec:
    case FormatSpec::String:
        buffer << abs;
        break;

    case FormatSpec::Bin:
        if (spec.alternate) {
            prefix += spec.upperCase ? "0B" : "0b";
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

    case FormatSpec::Oct:
        if (spec.alternate) {
            prefix.append(spec.upperCase ? "0O" : "0o");
        }
        buffer.setf(std::ios::oct, std::ios::basefield);
        buffer << abs;
        break;

    case FormatSpec::Hex:
        if (spec.alternate) {
            prefix.append(spec.upperCase ? "0X" : "0x");
        }
        buffer.setf(std::ios::hex, std::ios::basefield);
        buffer << abs;
        break;

    default:
        break;
    }

    std::string num = buffer.str();
    std::string::size_type length = prefix.length() + num.length();

    if (length < spec.width) {
        std::size_t padding = spec.width - length;
        switch (spec.alignment) {
        case FormatSpec::Left:
            out.write(prefix.c_str(), prefix.size());
            out.write(num.c_str(), num.size());
            fill(out, spec.fill, padding);
            break;

        case FormatSpec::Right:
            fill(out, spec.fill, padding);
            out.write(prefix.c_str(), prefix.size());
            out.write(num.c_str(), num.size());
            break;

        case FormatSpec::Center:
        {
            std::size_t before = padding / 2;
            fill(out, spec.fill, before);
            out.write(prefix.c_str(), prefix.size());
            out.write(num.c_str(), num.size());
            fill(out, spec.fill, padding - before);
            break;
        }

        case FormatSpec::AfterSign:
        case FormatSpec::DefaultAlignment:
            out.write(prefix.c_str(), prefix.size());
            fill(out, spec.fill, padding);
            out.write(num.c_str(), num.size());
            break;
        }
    }
    else {
        out.write(prefix.c_str(), prefix.size());
        out.write(num.c_str(), num.size());
    }
}

template<typename Float>
static inline void format_float(std::ostream& out, Float value, const FormatSpec& spec) {
    if (!spec.isDecimalType() && spec.type != FormatSpec::Generic) {
        throw std::invalid_argument("Cannot use floating point numbers with non-decimal format specifier.");
    }

    // TODO
    (void)out;
    (void)value;
}

static inline void format_string(std::ostream& out, const char value[], const FormatSpec& spec) {
    if (spec.sign != FormatSpec::DefaultSign) {
        throw std::invalid_argument("Sign not allowed with string or character");
    }

    if (spec.thoudsandsSeperator) {
        throw std::invalid_argument("Cannot specify ',' for string");
    }

    if (spec.alternate && spec.type != FormatSpec::Character) {
        throw std::invalid_argument("Alternate form (#) not allowed in string format specifier");
    }

    switch (spec.type) {
    case FormatSpec::Generic:
    case FormatSpec::String:
        break;

    default:
        throw std::invalid_argument("Invalid format specifier for string or character");
    }

    std::size_t length = std::strlen(value);
    if (spec.width > 0 && length < (std::size_t)spec.width) {
        std::size_t padding = spec.width - length;
        switch (spec.alignment) {
        case FormatSpec::AfterSign:
            throw std::invalid_argument("'=' alignment not allowed in string or character format specifier");

        case FormatSpec::DefaultAlignment:
        case FormatSpec::Left:
            out.write(value, length);
            fill(out, spec.fill, padding);
            break;

        case FormatSpec::Right:
            fill(out, spec.fill, padding);
            out.write(value, length);
            break;

        case FormatSpec::Center:
            std::size_t before = padding / 2;
            fill(out, spec.fill, before);
            out.write(value, length);
            fill(out, spec.fill, padding - before);
            break;
        }
    }
    else {
        out.write(value, length);
    }
}

void formatstring::format_value(std::ostream& out, bool value, const FormatSpec& spec) {
    if (spec.isNumberType()) {
        format_integer<unsigned int>(out, value ? 1 : 0, spec);
    }
    else {
        const char *str = spec.upperCase ?
                    (value ? "TRUE" : "FALSE") :
                    (value ? "true" : "false");
        FormatSpec strspec = spec;
        strspec.type = FormatSpec::String;
        format_string(out, str, strspec);
    }
}

void formatstring::format_value(std::ostream& out, char value, const FormatSpec& spec) {
    if (spec.type == FormatSpec::Generic || spec.isStringType()) {
        char str[2] = { value, 0 };
        FormatSpec strspec = spec;
        strspec.type = FormatSpec::String;
        format_string(out, str, strspec);
    }
    else {
        format_integer<int>(out, value, spec);
    }
}

void formatstring::format_value(std::ostream& out, short value, const FormatSpec& spec) {
    format_integer<short>(out, value, spec);
}

void formatstring::format_value(std::ostream& out, int value, const FormatSpec& spec) {
    format_integer<int>(out, value, spec);
}

void formatstring::format_value(std::ostream& out, long value, const FormatSpec& spec) {
    format_integer<long>(out, value, spec);
}

void formatstring::format_value(std::ostream& out, unsigned char value, const FormatSpec& spec) {
    if (spec.type == FormatSpec::Generic || spec.isStringType()) {
        char str[2] = { (char)value, 0 };
        FormatSpec strspec = spec;
        strspec.type = FormatSpec::String;
        format_string(out, str, strspec);
    }
    else {
        format_integer<unsigned int>(out, value, spec);
    }
}

void formatstring::format_value(std::ostream& out, unsigned short value, const FormatSpec& spec) {
    format_integer<unsigned short>(out, value, spec);
}

void formatstring::format_value(std::ostream& out, unsigned int value, const FormatSpec& spec) {
    format_integer<unsigned int>(out, value, spec);
}

void formatstring::format_value(std::ostream& out, unsigned long value, const FormatSpec& spec) {
    format_integer<unsigned long>(out, value, spec);
}

void formatstring::format_value(std::ostream& out, float value, const FormatSpec& spec) {
    format_float<float>(out, value, spec);
}

void formatstring::format_value(std::ostream& out, double value, const FormatSpec& spec) {
    format_float<double>(out, value, spec);
}

void formatstring::format_value(std::ostream& out, const std::string& str, const FormatSpec& spec) {
    format_string(out, str.c_str(), spec);
}

void formatstring::format_value(std::ostream& out, const char* str, const FormatSpec& spec) {
    format_string(out, str, spec);
}

void formatstring::repr_value(std::ostream& out, const std::string& value) {
    repr_value(out, value.c_str());
}

void formatstring::repr_value(std::ostream& out, const char* value) {
    out.put('"');
    for (; *value; ++ value) {
        char ch = *value;
        switch (ch) {
        case '\0': // 0x00
            out.write("\\0",2);
            break;

        case '\a': // 0x07
            out.write("\\a",2);
            break;

        case '\b': // 0x08
            out.write("\\b",2);
            break;

        case '\t': // 0x09
            out.write("\\t",2);
            break;

        case '\n': // 0x0a
            out.write("\\n",2);
            break;

        case '\v': // 0x0b
            out.write("\\v",2);
            break;

        case '\f': // 0x0c
            out.write("\\f",2);
            break;

        case '\r': // 0x0d
            out.write("\\r",2);
            break;

        case '"': // 0x22
            out.write("\\\"",2);
            break;

        case '?': // 0x3f
            out.write("\\?",2);
            break;

        case '\\': // 0x5c
            out.write("\\\\",2);
            break;

        default:
            out.put(ch);
            break;
        }
    }
    out.put('"');
}
