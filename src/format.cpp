#include "formatstring/format.h"
#include "formatstring/formatspec.h"
#include "formatstring/exceptions.h"

#include "strformatitem.h"
#include "valueformatitem.h"

using namespace formatstring;

template<typename Char>
static const Char* parse_size(const Char* ptr, std::size_t* numberptr) {
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

template<typename Char>
static const Char* parse_spec_internal(const Char* fmt, const Char* ptr, BasicFormatSpec<Char>* spec) {
    typedef BasicFormatSpec<Char> Spec;

    if (!*ptr) {
        return ptr;
    }

    bool precision = false;
    bool fill = false;

    switch (*(ptr + 1)) {
    case '<':
        spec->alignment = Spec::Left;
        spec->fill = *ptr;
        fill = true;
        ptr += 2;
        break;

    case '>':
        spec->alignment = Spec::Right;
        spec->fill = *ptr;
        fill = true;
        ptr += 2;
        break;

    case '=':
        spec->alignment = Spec::AfterSign;
        spec->fill = *ptr;
        fill = true;
        ptr += 2;
        break;

    case '^':
        spec->alignment = Spec::Center;
        spec->fill = *ptr;
        fill = true;
        ptr += 2;
        break;
    }

    switch (*ptr) {
    case '+':
        spec->sign = Spec::Always;
        ++ ptr;
        break;

    case '-':
        spec->sign = Spec::NegativeOnly;
        ++ ptr;
        break;

    case ' ':
        spec->sign = Spec::SpaceForPositive;
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
    const Char* next = parse_size(ptr, &size);
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
            throw InvalidFormatStringException(ptr - fmt, "expected number");
        }
        next = parse_size(ptr, &size);
        if (next != ptr) {
            spec->precision = size;
            ptr = next;
        }
        precision = true;
    }

    Char type = *ptr;
    switch (type) {
    case 'b':
    case 'B':
        spec->type = Spec::Bin;
        spec->upperCase = type == 'B';
        ++ ptr;
        break;

    case 'c':
        spec->type = Spec::Character;
        ++ ptr;
        break;

    case 'd':
        spec->type = Spec::Dec;
        ++ ptr;
        break;

    case 'e':
    case 'E':
        spec->type = Spec::Exp;
        spec->upperCase = type == 'E';
        if (!precision) {
            spec->precision = 6;
        }
        ++ ptr;
        break;

    case 'f':
    case 'F':
        spec->type = Spec::Fixed;
        spec->upperCase = type == 'F';
        if (!precision) {
            spec->precision = 6;
        }
        ++ ptr;
        break;

    case 'g':
    case 'G':
        spec->type = Spec::General;
        spec->upperCase = type == 'G';
        if (!precision) {
            spec->precision = 6;
        }
        ++ ptr;
        break;

    /*
    case 'n':
    case 'N':
        optionsptr->type = Spec::LocaleAwareNumber;
        spec->upperCase = type == 'N';
        ++ ptr;
        break;
    */

    case 'o':
    case 'O':
        spec->type = Spec::Oct;
        spec->upperCase = type == 'O';
        ++ ptr;
        break;

    case 's':
    case 'S':
        spec->type = Spec::String;
        spec->upperCase = type == 'S';
        ++ ptr;
        break;

    case 'x':
    case 'X':
        spec->type = Spec::Hex;
        spec->upperCase = type == 'X';
        ++ ptr;
        break;

    case '%':
        spec->type = Spec::Percentage;
        ++ ptr;
        break;
    }

    if (spec->thoudsandsSeperator &&
            spec->type != Spec::Generic &&
            spec->type != Spec::Dec &&
            spec->type != Spec::Exp &&
            spec->type != Spec::Fixed &&
            spec->type != Spec::General &&
            spec->type != Spec::Percentage) {
        std::string msg = "Cannot specify ',' with '";
        msg += (char)type;
        msg += "'.";
        throw std::invalid_argument(msg);
    }

    if (spec->alternate && spec->type == Spec::String) {
        throw std::invalid_argument("Alternate form (#) not allowed in string format specifier");
    }

    if (precision && (!spec->isDecimalType() && spec->type != Spec::Generic)) {
        std::string msg = "Cannot specify '.' with '";
        msg += (char)type;
        msg += "'.";
        throw std::invalid_argument(msg);
    }

    return ptr;
}

template<typename Char>
static void parse_format_internal(const Char* fmt, typename BasicFormatItem<Char>::List* items) {
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

    const std::basic_string<Char> empty;
    std::size_t currentIndex = 0;
    std::basic_stringbuf<Char> buffer;
    const Char* ptr = fmt;

    while (*ptr) {
        Char ch = *ptr;

        switch (ch) {
        case '{':
            ++ ptr;
            ch = *ptr;
            if (ch == '{') {
                buffer.sputc(ch);
            }
            else {
                if (buffer.in_avail() > 0) {
                    items->emplace_back(new BasicStrFormatItem<Char>(buffer.str()));
                    buffer.str(empty);
                }

                // parse format
                std::size_t index = currentIndex;
                BasicFormatSpec<Char> spec;
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
                        throw InvalidFormatStringException(ptr - fmt, "expected 'r' or 's'");
                    }
                    ++ ptr;
                    ch = *ptr;
                }

                if (ch == ':') {
                    ++ ptr;
                    ptr = parse_spec_internal(fmt, ptr, &spec);
                    ch = *ptr;
                }

                if (ch != '}') {
                    throw InvalidFormatStringException(ptr - fmt, "expected '}'");
                }

                items->emplace_back(new BasicValueFormatItem<Char>(index, conv, spec));
            }
            break;

        case '}':
            ++ ptr;
            if (*ptr == '}') {
                buffer.sputc(ch);
            }
            else {
                throw InvalidFormatStringException(ptr - fmt, "expected '}'");
            }
            break;

        default:
            buffer.sputc(ch);
            break;
        }
        ++ ptr;
    }

    if (buffer.in_avail() > 0) {
        items->emplace_back(new BasicStrFormatItem<Char>(buffer.str()));
    }
}

void formatstring::parse_format(const char* fmt, BasicFormatItem<char>::List *items) {
    parse_format_internal(fmt, items);
}

#ifdef FORMATSTRING_CHAR16_SUPPORT
void formatstring::parse_format(const char16_t* fmt, BasicFormatItem<char16_t>::List *items) {
    parse_format_internal(fmt, items);
}
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
void formatstring::parse_format(const char32_t* fmt, BasicFormatItem<char32_t>::List *items) {
    parse_format_internal(fmt, items);
}
#endif

void formatstring::parse_format(const wchar_t* fmt, BasicFormatItem<wchar_t>::List *items) {
    parse_format_internal(fmt, items);
}

template<typename Char>
static inline void parse_spec_templ(const Char* str, BasicFormatSpec<Char>* spec) {
    const Char* ptr = parse_spec_internal(str, str, spec);
    if (*ptr) {
        throw InvalidFormatStringException(ptr - str, "expected end of string");
    }
}

void formatstring::parse_spec(const char* str, FormatSpec* spec) {
    parse_spec_templ(str, spec);
}

#ifdef FORMATSTRING_CHAR16_SUPPORT
void formatstring::parse_spec(const char16_t* str, U16FormatSpec* spec) {
    parse_spec_templ(str, spec);
}
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
void formatstring::parse_spec(const char32_t* str, U32FormatSpec* spec) {
    parse_spec_templ(str, spec);
}
#endif

void formatstring::parse_spec(const wchar_t* str, WFormatSpec* spec) {
    parse_spec_templ(str, spec);
}

template class BasicFormat<char>;
template class BasicBoundFormat<char>;

#ifdef FORMATSTRING_CHAR16_SUPPORT
template class BasicFormat<char16_t>;
template class BasicBoundFormat<char16_t>;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
template class BasicFormat<char32_t>;
template class BasicBoundFormat<char32_t>;
#endif

template class BasicFormat<wchar_t>;
template class BasicBoundFormat<wchar_t>;
