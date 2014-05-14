#ifndef FORMATSTRING_FORMATVALUE_H
#define FORMATSTRING_FORMATVALUE_H

#include <iosfwd>

namespace formatstring {

    class FormatSpec;

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
}

#endif // FORMATSTRING_FORMATVALUE_H
