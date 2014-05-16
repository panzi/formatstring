#ifndef FORMATSTRING_FORMATTEDVALUE_H
#define FORMATSTRING_FORMATTEDVALUE_H

#include "formatstring/formatter.h"
#include "formatstring/formatspec.h"
#include "formatstring/formatvalue.h"

#include <sstream>

namespace formatstring {

    class FormattedValue {
    public:
        template<typename T>
        FormattedValue(const T& value, Conversion conv, const FormatSpec& spec) :
            m_formatter(make_formatter(value)), m_conv(conv), m_spec(spec) {}

        FormattedValue(Formatter* formatter, Conversion conv = NoConv, const FormatSpec& spec = FormatSpec::DEFAULT) :
            m_formatter(formatter), m_conv(conv), m_spec(spec) {}

        FormattedValue(FormattedValue&& other) :
            m_formatter(std::move(other.m_formatter)), m_conv(other.m_conv), m_spec(other.m_spec) {}

        inline void format(std::ostream& out) const {
            m_formatter->format(out, m_conv, m_spec);
        }

        inline operator std::string () const {
            std::stringstream out;
            format(out);
            return out.str();
        }

        inline FormattedValue& align(FormatSpec::Alignment alignment) {
            m_spec.alignment = alignment;
            return *this;
        }

        inline FormattedValue& left() {
            m_spec.alignment = FormatSpec::Left;
            return *this;
        }

        inline FormattedValue& right() {
            m_spec.alignment = FormatSpec::Right;
            return *this;
        }

        inline FormattedValue& afterSign() {
            m_spec.alignment = FormatSpec::AfterSign;
            return *this;
        }

        inline FormattedValue& center() {
            m_spec.alignment = FormatSpec::Center;
            return *this;
        }

        inline FormattedValue& type(FormatSpec::Type type) {
            m_spec.type = type;
            return *this;
        }

        inline FormattedValue& bin() {
            m_spec.type = FormatSpec::Bin;
            return *this;
        }

        inline FormattedValue& character() {
            m_spec.type = FormatSpec::Character;
            return *this;
        }

        inline FormattedValue& dec() {
            m_spec.type = FormatSpec::Dec;
            return *this;
        }

        inline FormattedValue& oct() {
            m_spec.type = FormatSpec::Oct;
            return *this;
        }

        inline FormattedValue& hex() {
            m_spec.type = FormatSpec::Hex;
            return *this;
        }

        inline FormattedValue& exp() {
            m_spec.type = FormatSpec::Exp;
            return *this;
        }

        inline FormattedValue& fixed() {
            m_spec.type = FormatSpec::Fixed;
            return *this;
        }

        inline FormattedValue& general() {
            m_spec.type = FormatSpec::General;
            return *this;
        }

        inline FormattedValue& precentage() {
            m_spec.type = FormatSpec::Percentage;
            return *this;
        }

        inline FormattedValue& str() {
            m_spec.type = FormatSpec::String;
            return *this;
        }

        inline FormattedValue& sign(FormatSpec::Sign sign) {
            m_spec.sign = sign;
            return *this;
        }

        inline FormattedValue& fill(char fill, std::size_t width) {
            m_spec.fill  = fill;
            m_spec.width = width;
            return *this;
        }

        inline FormattedValue& fill(char fill) {
            m_spec.fill = fill;
            return *this;
        }

        inline FormattedValue& width(std::size_t width) {
            m_spec.width = width;
            return *this;
        }

        inline FormattedValue& alt(bool alternate = true) {
            m_spec.alternate = alternate;
            return *this;
        }

        inline FormattedValue& upper() {
            m_spec.upperCase = true;
            return *this;
        }

        inline FormattedValue& lower() {
            m_spec.upperCase = false;
            return *this;
        }

        inline FormattedValue& thoudsandsSeperator(bool thoudsandsSeperator = true) {
            m_spec.thoudsandsSeperator = thoudsandsSeperator;
            return *this;
        }

        inline FormattedValue& spec(const FormatSpec& spec) {
            m_spec = spec;
            return *this;
        }

        inline FormattedValue& spec(const std::string& spec) {
            m_spec = FormatSpec(spec);
            return *this;
        }

        inline FormattedValue& spec(const char* spec) {
            m_spec = FormatSpec(spec);
            return *this;
        }

        inline const FormatSpec& spec() const {
            return m_spec;
        }

        inline FormattedValue& conversion(Conversion conv) {
            m_conv = conv;
            return *this;
        }

        inline FormattedValue& repr() {
            m_conv = ReprConv;
            return *this;
        }

        inline Conversion conversion() const {
            return m_conv;
        }

        inline std::string string() const {
            return (std::string)*this;
        }

    private:
        std::unique_ptr<Formatter> m_formatter;
        Conversion m_conv;
        FormatSpec m_spec;
    };

    template<typename T>
    inline FormattedValue val(const T& value) {
        return FormattedValue(value,NoConv,FormatSpec::DEFAULT);
    }

    template<typename Iter>
    inline FormattedValue slice(Iter begin, Iter end) {
        return new SliceFormatter<Iter>(begin, end);
    }

    inline FormattedValue val(const char str[]) {
        return FormattedValue(str,NoConv,FormatSpec::DEFAULT);
    }

    template<typename T>
    inline FormattedValue val(const T& value, const char* spec) {
        FormattedValue fmt = val(value);
        fmt.spec(spec);
        return fmt;
    }

    template<typename T>
    inline FormattedValue val(const T& value, const std::string& spec) {
        FormattedValue fmt = val(value);
        fmt.spec(spec);
        return fmt;
    }

    template<typename T>
    inline FormattedValue val(const T& value, const FormatSpec& spec) {
        FormattedValue fmt = val(value);
        fmt.spec(spec);
        return fmt;
    }

    template<typename T>
    inline FormattedValue val(const T& value, FormatSpec::Alignment alignment) {
        FormattedValue fmt = val(value);
        fmt.align(alignment);
        return fmt;
    }

    template<typename T>
    inline FormattedValue val(const T& value, FormatSpec::Type type) {
        FormattedValue fmt = val(value);
        fmt.type(type);
        return fmt;
    }

    template<typename T>
    inline FormattedValue val(const T& value, FormatSpec::Sign sign) {
        FormattedValue fmt = val(value);
        fmt.sign(sign);
        return fmt;
    }

    template<typename T>
    inline FormattedValue val(const T& value, char fill, std::size_t width) {
        FormattedValue fmt = val(value);
        fmt.fill(fill, width);
        return fmt;
    }

    template<typename NumberType>
    inline FormattedValue bin(NumberType value) {
        FormattedValue fmt = val(value);
        fmt.bin();
        return fmt;
    }

    template<typename NumberType>
    inline FormattedValue dec(NumberType value) {
        FormattedValue fmt = val(value);
        fmt.dec();
        return fmt;
    }

    template<typename NumberType>
    inline FormattedValue oct(NumberType value) {
        FormattedValue fmt = val(value);
        fmt.oct();
        return fmt;
    }

    template<typename NumberType>
    inline FormattedValue hex(NumberType value) {
        FormattedValue fmt = val(value);
        fmt.hex();
        return fmt;
    }

    template<typename T>
    inline FormattedValue str(T value) {
        FormattedValue fmt = val(value);
        fmt.str();
        return fmt;
    }

    template<typename T>
    inline FormattedValue repr(const T& value) {
        FormattedValue fmt = val(value);
        fmt.repr();
        return fmt;
    }

    template<typename OStream>
    OStream& operator << (OStream& out, const FormattedValue& value) {
        value.format(out);
        return out;
    }
}

#endif // FORMATSTRING_FORMATTEDVALUE_H
