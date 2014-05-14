#ifndef FORMATSTRING_FORMATTEDVALUE_H
#define FORMATSTRING_FORMATTEDVALUE_H

#include "formatstring/formatter.h"
#include "formatstring/formatspec.h"
#include "formatstring/formatvalue.h"

#include <sstream>

namespace formatstring {

    template<typename T>
    class FormattedValue : public ValueFormatter<T> {
    public:
        typedef T value_type;
        typedef ValueFormatter<T> super_type;
        typedef FormattedValue<T> self_type;

        FormattedValue(const value_type& value, const FormatSpec& spec) : super_type(value), m_spec(spec) {}

        inline void format(std::ostream& out) const {
            format_value(out, this->value, m_spec);
        }

        inline operator std::string () const {
            std::stringstream out;
            format(out);
            return out.str();
        }

        inline self_type& align(FormatSpec::Alignment alignment) {
            m_spec.alignment = alignment;
            return *this;
        }

        inline self_type& left() {
            m_spec.alignment = FormatSpec::Left;
            return *this;
        }

        inline self_type& right() {
            m_spec.alignment = FormatSpec::Right;
            return *this;
        }

        inline self_type& afterSign() {
            m_spec.alignment = FormatSpec::AfterSign;
            return *this;
        }

        inline self_type& center() {
            m_spec.alignment = FormatSpec::Center;
            return *this;
        }

        inline self_type& type(FormatSpec::Type type) {
            m_spec.type = type;
            return *this;
        }

        inline self_type& bin() {
            m_spec.type = FormatSpec::Bin;
            return *this;
        }

        inline self_type& character() {
            m_spec.type = FormatSpec::Character;
            return *this;
        }

        inline self_type& dec() {
            m_spec.type = FormatSpec::Dec;
            return *this;
        }

        inline self_type& oct() {
            m_spec.type = FormatSpec::Oct;
            return *this;
        }

        inline self_type& hex() {
            m_spec.type = FormatSpec::Hex;
            return *this;
        }

        inline self_type& exp() {
            m_spec.type = FormatSpec::Exp;
            return *this;
        }

        inline self_type& fixed() {
            m_spec.type = FormatSpec::Fixed;
            return *this;
        }

        inline self_type& general() {
            m_spec.type = FormatSpec::General;
            return *this;
        }

        inline self_type& precentage() {
            m_spec.type = FormatSpec::Percentage;
            return *this;
        }

        inline self_type& hexFloat() {
            m_spec.type = FormatSpec::HexFloat;
            return *this;
        }

        inline self_type& str() {
            m_spec.type = FormatSpec::String;
            return *this;
        }

        inline self_type& sign(FormatSpec::Sign sign) {
            m_spec.sign = sign;
            return *this;
        }

        inline self_type& fill(char fill, std::size_t width) {
            m_spec.fill = fill;
            m_spec.width = width;
            return *this;
        }

        inline self_type& fill(char fill) {
            m_spec.fill = fill;
            return *this;
        }

        inline self_type& width(std::size_t width) {
            m_spec.width = width;
            return *this;
        }

        inline self_type& alt(bool alternate = true) {
            m_spec.alternate = alternate;
            return *this;
        }

        inline self_type& upper() {
            m_spec.upperCase = true;
            return *this;
        }

        inline self_type& lower() {
            m_spec.upperCase = false;
            return *this;
        }

        inline self_type& thoudsandsSeperator(bool thoudsandsSeperator = true) {
            m_spec.thoudsandsSeperator = thoudsandsSeperator;
            return *this;
        }

        inline self_type& spec(const FormatSpec& spec) {
            m_spec = spec;
            return *this;
        }

        inline self_type& spec(const std::string& spec) {
            m_spec = FormatSpec(spec);
            return *this;
        }

        inline self_type& spec(const char* spec) {
            m_spec = FormatSpec(spec);
            return *this;
        }

        inline const FormatSpec& spec() const {
            return m_spec;
        }

        inline std::string string() const {
            return (std::string)*this;
        }

    private:
        FormatSpec m_spec;
    };

    typedef FormattedValue<const char*> FormattedUtf8;

    template<typename T>
    inline FormattedValue<T> val(const T& value) {
        return FormattedValue<T>(value, FormatSpec());
    }

    inline FormattedUtf8 val(const char str[]) {
        return FormattedUtf8(str, FormatSpec());
    }

    template<typename T>
    inline auto val(const T& value, const char* spec) {
        return val(value).spec(spec);
    }

    template<typename T>
    inline auto val(const T& value, const std::string& spec) {
        return val(value).spec(spec);
    }

    template<typename T>
    inline auto val(const T& value, const FormatSpec& spec) {
        return val(value).spec(spec);
    }

    template<typename T>
    inline auto val(const T& value, FormatSpec::Alignment alignment) {
        return val(value).align(alignment);
    }

    template<typename T>
    inline auto val(const T& value, FormatSpec::Type type) {
        return val(value).type(type);
    }

    template<typename T>
    inline auto val(const T& value, FormatSpec::Sign sign) {
        return val(value).sign(sign);
    }

    template<typename T>
    inline auto val(const T& value, char fill, std::size_t width) {
        return val(value).fill(fill, width);
    }

    template<typename NumberType>
    inline auto bin(NumberType value) {
        return val(value).bin();
    }

    template<typename NumberType>
    inline auto dec(NumberType value) {
        return val(value).dec();
    }

    template<typename NumberType>
    inline auto oct(NumberType value) {
        return val(value).oct();
    }

    template<typename NumberType>
    inline auto hex(NumberType value) {
        return val(value).hex();
    }

    template<typename OStream, typename Value>
    OStream& operator << (OStream& out, const FormattedValue<Value>& value) {
        value.format(out);
        return out;
    }
}

#endif // FORMATSTRING_FORMATTEDVALUE_H
