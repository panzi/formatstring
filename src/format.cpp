#include "formatstring/format.h"
#include "formatstring/formatspec.h"
#include "formatstring/exceptions.h"

#include "strformatitem.h"
#include "valueformatitem.h"

using namespace formatstring;

static std::string::const_iterator parse_size(std::string::const_iterator it, std::string::const_iterator end, std::size_t* numberptr) {
    std::size_t number = 0;

    // TODO: handle integer overflow
    for (; it != end; ++ it) {
        char ch = *it;
        if (ch < '0' || ch > '9') {
            break;
        }
        number *= 10;
        number += ch - '0';
    }

    *numberptr = number;

    return it;
}

static std::string::const_iterator parse_spec(const std::string& fmt, std::string::const_iterator it, FormatSpec* spec) {
    std::string::const_iterator end = fmt.end();

    if (it == end) {
        return it;
    }

    bool precision = false;
    bool fill = false;

    switch (*(it + 1)) {
    case '<':
        spec->alignment = FormatSpec::Left;
        spec->fill = *it;
        fill = true;
        it += 2;
        break;

    case '>':
        spec->alignment = FormatSpec::Right;
        spec->fill = *it;
        fill = true;
        it += 2;
        break;

    case '=':
        spec->alignment = FormatSpec::AfterSign;
        spec->fill = *it;
        fill = true;
        it += 2;
        break;

    case '^':
        spec->alignment = FormatSpec::Center;
        spec->fill = *it;
        fill = true;
        it += 2;
        break;
    }

    switch (*it) {
    case '+':
        spec->sign = FormatSpec::Always;
        ++ it;
        break;

    case '-':
        spec->sign = FormatSpec::NegativeOnly;
        ++ it;
        break;

    case ' ':
        spec->sign = FormatSpec::SpaceForPositive;
        ++ it;
        break;
    }

    if (*it == '#') {
        spec->alternate = true;
        ++ it;
    }

    if (*it == '0') {
        if (!fill) {
            spec->fill = '0';
        }
        ++ it;
    }

    std::size_t size = 0;
    std::string::const_iterator next = parse_size(it, end, &size);
    if (next != it) {
        spec->width = size;
        it = next;
    }

    if (*it == ',') {
        spec->thoudsandsSeperator = true;
        ++ it;
    }

    if (*it == '.') {
        ++ it;
        if (it == fmt.end()) {
            throw InvalidFormatStringException(fmt, it - fmt.begin(), "expected number");
        }
        next = parse_size(it, end, &size);
        if (next != it) {
            spec->precision = size;
            it = next;
        }
        precision = true;
    }

    char type = *it;
    switch (type) {
    case 'b':
    case 'B':
        spec->type = FormatSpec::Bin;
        spec->upperCase = type == 'B';
        ++ it;
        break;

    case 'c':
        spec->type = FormatSpec::Character;
        ++ it;
        break;

    case 'd':
        spec->type = FormatSpec::Dec;
        ++ it;
        break;

    case 'e':
    case 'E':
        spec->type = FormatSpec::Exp;
        spec->upperCase = type == 'E';
        if (!precision) {
            spec->precision = 6;
        }
        ++ it;
        break;

    case 'f':
    case 'F':
        spec->type = FormatSpec::Fixed;
        spec->upperCase = type == 'F';
        if (!precision) {
            spec->precision = 6;
        }
        ++ it;
        break;

    case 'g':
    case 'G':
        spec->type = FormatSpec::General;
        spec->upperCase = type == 'G';
        if (!precision) {
            spec->precision = 6;
        }
        ++ it;
        break;

    /*
    case 'n':
        optionsptr->type = FormatOptions::LocaleAwareNumber;
        ++ it;
        break;
    */

    case 'o':
    case 'O':
        spec->type = FormatSpec::Oct;
        spec->upperCase = type == 'O';
        ++ it;
        break;

    case 's':
        spec->type = FormatSpec::String;
        ++ it;
        break;

    case 'x':
    case 'X':
        spec->type = FormatSpec::Hex;
        spec->upperCase = type == 'X';
        ++ it;
        break;

    case '%':
        spec->type = FormatSpec::Percentage;
        ++ it;
        break;

    case 'a':
    case 'A':
        spec->type = FormatSpec::HexFloat;
        spec->upperCase = type == 'A';
        ++ it;
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

    if (precision && ((!spec->isDecimalType() && spec->type != FormatSpec::Generic) || spec->type == FormatSpec::HexFloat)) {
        std::string msg = "Cannot specify '.' with '";
        msg += (char)type;
        msg += "'.";
        throw std::invalid_argument(msg);
    }

    return it;
}

Format::Format(const std::string& fmt) : m_fmt() {
    // Format string similar to Python, but a bit more limited:
    // https://docs.python.org/3/library/string.html#format-string-syntax
    //
    // And added "a" and "A" format specifier from C99 format strings.
    //
    // replacement_field ::=  "{" [arg_index] [":" format_spec] "}"
    // arg_index         ::=  integer
    // format_spec       ::=  [[fill]align][sign][#][0][width][,][.precision][type]
    // fill              ::=  <any character>
    // align             ::=  "<" | ">" | "=" | "^"
    // sign              ::=  "+" | "-" | " "
    // width             ::=  integer
    // precision         ::=  integer
    // type              ::=  "b" | "c" | "d" | "e" | "E" | "f" | "F" | "g" | "G" | "n" | "o" | "s" | "x" | "X" | "%" | "a" | "A"

    std::size_t currentIndex = 0;
    std::stringbuf buffer;
    std::string::const_iterator it = fmt.begin();
    std::string::const_iterator end = fmt.end();

    while (it != end) {
        char cp = *it;

        switch (cp) {
        case '{':
            ++ it;
            cp = *it;
            if (cp == '{') {
                buffer.sputc(cp);
            }
            else {
                if (buffer.in_avail() > 0) {
                    m_fmt.emplace_back(new StrFormatItem(buffer.str()));
                    buffer.str("");
                }

                // parse format
                std::size_t index = currentIndex;
                FormatSpec options;

                if (cp == ':') {
                    ++ it;
                    it = parse_spec(fmt, it, &options);
                    cp = *it;
                    ++ currentIndex;
                }
                else if (cp != '}') {
                    it = parse_size(it, end, &index);
                    cp = *it;
                    if (cp == ':') {
                        ++ it;
                        it = parse_spec(fmt, it, &options);
                        cp = *it;
                    }
                }
                else {
                    ++ currentIndex;
                }

                if (cp != '}') {
                    throw InvalidFormatStringException(fmt, it - fmt.begin(), "expected '}'");
                }

                m_fmt.emplace_back(new ValueFormatItem(index, options));
            }
            break;

        case '}':
            ++ it;
            if (*it == '}') {
                buffer.sputc(cp);
            }
            else {
                throw InvalidFormatStringException(fmt, it - fmt.begin(), "expected '}'");
            }
            break;

        default:
            buffer.sputc(cp);
            break;
        }
        ++ it;
    }

    if (buffer.in_avail() > 0) {
        m_fmt.emplace_back(new StrFormatItem(buffer.str()));
    }
}

Format::Format(const Format& other) {
    for (auto& item : other.m_fmt) {
        m_fmt.emplace_back(item->clone());
    }
}

void Format::apply(std::ostream& out, const Formatters& formatters) const {
    for (auto& item : m_fmt) {
        item->apply(out, formatters);
    }
}

FormatSpec::FormatSpec(const std::string& spec) : FormatSpec() {
    std::string::const_iterator it = parse_spec(spec, spec.begin(), this);
    if (it != spec.end()) {
        throw InvalidFormatStringException(spec, it - spec.begin(), "expected end of string");
    }
}

BoundFormat::BoundFormat(const Format& fmt, const Formatters& formatters) : m_format(fmt) {
    for (auto& formatter : formatters) {
        m_formatters.emplace_back(formatter->clone());
    }
}

BoundFormat::BoundFormat(const std::string& fmt, const Formatters& formatters) : m_format(fmt) {
    for (auto& formatter : formatters) {
        m_formatters.emplace_back(formatter->clone());
    }
}

BoundFormat::BoundFormat(const BoundFormat& other) : m_format(other.m_format) {
    for (auto& formatter : other.m_formatters) {
        m_formatters.emplace_back(formatter->clone());
    }
}
