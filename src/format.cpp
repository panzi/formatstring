#include "formatstring/format.h"
#include "formatstring/formatspec.h"
#include "formatstring/exceptions.h"

#include "strformatitem.h"
#include "valueformatitem.h"

using namespace formatstring;

static const char* parse_size(const char* ptr, std::size_t* numberptr) {
    std::size_t number = 0;

    // TODO: handle integer overflow
    for (; *ptr; ++ ptr) {
        char ch = *ptr;
        if (ch < '0' || ch > '9') {
            break;
        }
        number *= 10;
        number += ch - '0';
    }

    *numberptr = number;

    return ptr;
}

static const char* parse_spec(const char* fmt, const char* ptr, FormatSpec* spec) {
    if (!*ptr) {
        return ptr;
    }

    bool precision = false;
    bool fill = false;

    switch (*(ptr + 1)) {
    case '<':
        spec->alignment = FormatSpec::Left;
        spec->fill = *ptr;
        fill = true;
        ptr += 2;
        break;

    case '>':
        spec->alignment = FormatSpec::Right;
        spec->fill = *ptr;
        fill = true;
        ptr += 2;
        break;

    case '=':
        spec->alignment = FormatSpec::AfterSign;
        spec->fill = *ptr;
        fill = true;
        ptr += 2;
        break;

    case '^':
        spec->alignment = FormatSpec::Center;
        spec->fill = *ptr;
        fill = true;
        ptr += 2;
        break;
    }

    switch (*ptr) {
    case '+':
        spec->sign = FormatSpec::Always;
        ++ ptr;
        break;

    case '-':
        spec->sign = FormatSpec::NegativeOnly;
        ++ ptr;
        break;

    case ' ':
        spec->sign = FormatSpec::SpaceForPositive;
        ++ ptr;
        break;
    }

    if (*ptr == '#') {
        spec->alternate = true;
        ++ ptr;
    }

    if (*ptr == '0') {
        if (!fill) {
            spec->fill = '0';
        }
        ++ ptr;
    }

    std::size_t size = 0;
    const char* next = parse_size(ptr, &size);
    if (next != ptr) {
        spec->width = size;
        ptr = next;
    }

    if (*ptr == ',') {
        spec->thoudsandsSeperator = true;
        ++ ptr;
    }

    if (*ptr == '.') {
        ++ ptr;
        if (!*ptr) {
            throw InvalidFormatStringException(fmt, ptr - fmt, "expected number");
        }
        next = parse_size(ptr, &size);
        if (next != ptr) {
            spec->precision = size;
            ptr = next;
        }
        precision = true;
    }

    char type = *ptr;
    switch (type) {
    case 'b':
    case 'B':
        spec->type = FormatSpec::Bin;
        spec->upperCase = type == 'B';
        ++ ptr;
        break;

    case 'c':
        spec->type = FormatSpec::Character;
        ++ ptr;
        break;

    case 'd':
        spec->type = FormatSpec::Dec;
        ++ ptr;
        break;

    case 'e':
    case 'E':
        spec->type = FormatSpec::Exp;
        spec->upperCase = type == 'E';
        if (!precision) {
            spec->precision = 6;
        }
        ++ ptr;
        break;

    case 'f':
    case 'F':
        spec->type = FormatSpec::Fixed;
        spec->upperCase = type == 'F';
        if (!precision) {
            spec->precision = 6;
        }
        ++ ptr;
        break;

    case 'g':
    case 'G':
        spec->type = FormatSpec::General;
        spec->upperCase = type == 'G';
        if (!precision) {
            spec->precision = 6;
        }
        ++ ptr;
        break;

    /*
    case 'n':
        optionsptr->type = FormatOptions::LocaleAwareNumber;
        ++ ptr;
        break;
    */

    case 'o':
    case 'O':
        spec->type = FormatSpec::Oct;
        spec->upperCase = type == 'O';
        ++ ptr;
        break;

    case 's':
        spec->type = FormatSpec::String;
        ++ ptr;
        break;

    case 'x':
    case 'X':
        spec->type = FormatSpec::Hex;
        spec->upperCase = type == 'X';
        ++ ptr;
        break;

    case '%':
        spec->type = FormatSpec::Percentage;
        ++ ptr;
        break;
    }

    if (spec->thoudsandsSeperator &&
            spec->type != FormatSpec::Generic &&
            spec->type != FormatSpec::Dec &&
            spec->type != FormatSpec::Exp &&
            spec->type != FormatSpec::Fixed &&
            spec->type != FormatSpec::General &&
            spec->type != FormatSpec::Percentage) {
        std::string msg = "Cannot specify ',' with '";
        msg += (char)type;
        msg += "'.";
        throw std::invalid_argument(msg);
    }

    if (spec->alternate && spec->type == FormatSpec::String) {
        throw std::invalid_argument("Alternate form (#) not allowed in string format specifier");
    }

    if (precision && (!spec->isDecimalType() && spec->type != FormatSpec::Generic)) {
        std::string msg = "Cannot specify '.' with '";
        msg += (char)type;
        msg += "'.";
        throw std::invalid_argument(msg);
    }

    return ptr;
}

Format::Format(const std::string& fmt) : Format(fmt.c_str()) {}

Format::Format(const char* fmt) : m_fmt(std::make_shared<FormatItems>()) {
    // Format string similar to Python, but a bit more limited:
    // https://docs.python.org/3/library/string.html#format-string-syntax
    //
    // And added "a" and "A" format specifier from C99 format strings.
    //
    // replacement_field ::=  "{" [arg_index] ["!" conversion] [":" format_spec] "}"
    // arg_index         ::=  integer
    // conversion        ::=  "r" | "s"
    // format_spec       ::=  [[fill]align][sign][#][0][width][,][.precision][type]
    // fill              ::=  <any character>
    // align             ::=  "<" | ">" | "=" | "^"
    // sign              ::=  "+" | "-" | " "
    // width             ::=  integer
    // precision         ::=  integer
    // type              ::=  "b" | "c" | "d" | "e" | "E" | "f" | "F" | "g" | "G" | "n" | "o" | "s" | "x" | "X" | "%" | "a" | "A"

    std::size_t currentIndex = 0;
    std::stringbuf buffer;
    const char* ptr = fmt;

    while (*ptr) {
        char ch = *ptr;

        switch (ch) {
        case '{':
            ++ ptr;
            ch = *ptr;
            if (ch == '{') {
                buffer.sputc(ch);
            }
            else {
                if (buffer.in_avail() > 0) {
                    m_fmt->emplace_back(new StrFormatItem(buffer.str()));
                    buffer.str("");
                }

                // parse format
                std::size_t index = currentIndex;
                FormatSpec spec;
                Conversion conv = NoConv;

                if (ch >= '0' && ch <= '9') {
                    ptr = parse_size(ptr, &index);
                    ch = *ptr;
                }
                else {
                    ++ currentIndex;
                }

                if (ch == '!') {
                    ++ ptr;
                    ch = *ptr;
                    if (ch == 'r') {
                        conv = ReprConv;
                    }
                    else if (ch == 's') {
                        conv = StrConv;
                    }
                    else {
                        throw InvalidFormatStringException(fmt, ptr - fmt, "expected 'r' or 's'");
                    }
                    ++ ptr;
                    ch = *ptr;
                }

                if (ch == ':') {
                    ++ ptr;
                    ptr = parse_spec(fmt, ptr, &spec);
                    ch = *ptr;
                }

                if (ch != '}') {
                    throw InvalidFormatStringException(fmt, ptr - fmt, "expected '}'");
                }

                m_fmt->emplace_back(new ValueFormatItem(index, conv, spec));
            }
            break;

        case '}':
            ++ ptr;
            if (*ptr == '}') {
                buffer.sputc(ch);
            }
            else {
                throw InvalidFormatStringException(fmt, ptr - fmt, "expected '}'");
            }
            break;

        default:
            buffer.sputc(ch);
            break;
        }
        ++ ptr;
    }

    if (buffer.in_avail() > 0) {
        m_fmt->emplace_back(new StrFormatItem(buffer.str()));
    }
}

Format::Format(const Format& other) : m_fmt(other.m_fmt) {}

void Format::apply(std::ostream& out, const Formatters& formatters) const {
    for (auto& item : *m_fmt) {
        item->apply(out, formatters);
    }
}

FormatSpec::FormatSpec(const char* spec) : FormatSpec() {
    const char* ptr = parse_spec(spec, spec, this);
    if (*ptr) {
        throw InvalidFormatStringException(spec, ptr - spec, "expected end of string");
    }
}

FormatSpec::FormatSpec(const std::string& spec) : FormatSpec(spec.c_str()) {}

BoundFormat::BoundFormat(const BoundFormat& other) : m_format(other.m_format), m_formatters(other.m_formatters) {}
