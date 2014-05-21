#ifndef FORMATSTRING_FORMATTEDVALUE_H
#define FORMATSTRING_FORMATTEDVALUE_H
#pragma once

#include "formatstring/config.h"
#include "formatstring/export.h"
#include "formatstring/format.h"
#include "formatstring/formatter.h"
#include "formatstring/formatspec.h"
#include "formatstring/formatvalue.h"

#include <sstream>

namespace formatstring {

    template<typename Char>
    class BasicFormattedValue;

    typedef BasicFormattedValue<char>     FormattedValue;

#ifdef FORMATSTRING_CHAR16_SUPPORT
    typedef BasicFormattedValue<char16_t> U16FormattedValue;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    typedef BasicFormattedValue<char32_t> U32FormattedValue;
#endif

    typedef BasicFormattedValue<wchar_t>  WFormattedValue;

    template<typename Char>
    class BasicFormattedValue {
    public:
        typedef Char char_type;
        typedef BasicFormattedValue<Char> self_type;
        typedef BasicFormatSpec<Char> spec_type;

        template<typename T>
        BasicFormattedValue(const T& value, Conversion conv, const spec_type& spec) :
            m_formatter(format_traits<Char,T>::make_formatter(value)), m_conv(conv), m_spec(spec) {}

        BasicFormattedValue(BasicFormatter<Char> formatter, Conversion conv = NoConv, const spec_type& spec = spec_type::DEFAULT) :
            m_formatter(formatter), m_conv(conv), m_spec(spec) {}

        BasicFormattedValue(BasicFormattedValue<Char>&& other) :
            m_formatter(std::move(other.m_formatter)), m_conv(other.m_conv), m_spec(other.m_spec) {}

        inline void format(std::basic_ostream<Char>& out) const {
            m_formatter(out, m_conv, m_spec);
        }

        inline operator std::basic_string<Char> () const {
            std::basic_ostringstream<Char> out;
            format(out);
            return out.str();
        }

        inline self_type& align(typename spec_type::Alignment alignment) {
            m_spec.alignment = alignment;
            return *this;
        }

        inline self_type& left() {
            m_spec.alignment = spec_type::Left;
            return *this;
        }

        inline self_type& right() {
            m_spec.alignment = spec_type::Right;
            return *this;
        }

        inline self_type& afterSign() {
            m_spec.alignment = spec_type::AfterSign;
            return *this;
        }

        inline self_type& center() {
            m_spec.alignment = spec_type::Center;
            return *this;
        }

        inline self_type& type(typename spec_type::Type type) {
            m_spec.type = type;
            return *this;
        }

        inline self_type& bin() {
            m_spec.type = spec_type::Bin;
            return *this;
        }

        inline self_type& character() {
            m_spec.type = spec_type::Character;
            return *this;
        }

        inline self_type& dec() {
            m_spec.type = spec_type::Dec;
            return *this;
        }

        inline self_type& oct() {
            m_spec.type = spec_type::Oct;
            return *this;
        }

        inline self_type& hex() {
            m_spec.type = spec_type::Hex;
            return *this;
        }

        inline self_type& exp() {
            m_spec.type = spec_type::Exp;
            return *this;
        }

        inline self_type& fixed() {
            m_spec.type = spec_type::Fixed;
            return *this;
        }

        inline self_type& general() {
            m_spec.type = spec_type::General;
            return *this;
        }

        inline self_type& precentage() {
            m_spec.type = spec_type::Percentage;
            return *this;
        }

        inline self_type& str() {
            m_spec.type = spec_type::String;
            return *this;
        }

        inline self_type& sign(typename spec_type::Sign sign) {
            m_spec.sign = sign;
            return *this;
        }

        inline self_type& fill(char fill, int width) {
            m_spec.fill  = fill;
            m_spec.width = width;
            return *this;
        }

        inline self_type& fill(char fill) {
            m_spec.fill = fill;
            return *this;
        }

        inline self_type& width(int width) {
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

        inline self_type& tdssep(bool thoudsandsSeperator = true) {
            m_spec.thoudsandsSeperator = thoudsandsSeperator;
            return *this;
        }

        inline self_type& spec(const spec_type& spec) {
            m_spec = spec;
            return *this;
        }

        inline self_type& spec(const std::basic_string<Char>& spec) {
            m_spec = spec;
            return *this;
        }

        inline self_type& spec(const Char* spec) {
            m_spec = spec;
            return *this;
        }

        inline const spec_type& spec() const {
            return m_spec;
        }

        inline self_type& conversion(Conversion conv) {
            m_conv = conv;
            return *this;
        }

        inline self_type& repr() {
            m_conv = ReprConv;
            return *this;
        }

        inline Conversion conversion() const {
            return m_conv;
        }

        inline std::basic_string<Char> string() const {
            return (std::basic_string<Char>)*this;
        }

    private:
        BasicFormatter<Char>  m_formatter;
        Conversion            m_conv;
        BasicFormatSpec<Char> m_spec;
    };

    // ---- extern template instantiations ----
    extern template class FORMATSTRING_EXPORT BasicFormattedValue<char>;

#ifdef FORMATSTRING_CHAR16_SUPPORT
    extern template class FORMATSTRING_EXPORT BasicFormattedValue<char16_t>;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    extern template class FORMATSTRING_EXPORT BasicFormattedValue<char32_t>;
#endif

    extern template class FORMATSTRING_EXPORT BasicFormattedValue<wchar_t>;

    // ---- helper functions ----
    template<typename Char,typename T>
    inline BasicFormattedValue<Char> val(const T& value) {
        return BasicFormattedValue<Char>(value,NoConv,BasicFormatSpec<Char>::DEFAULT);
    }

    template<typename T> inline FormattedValue    val(   const T& value) { return val<char,T>(value); }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename T> inline U16FormattedValue u16val(const T& value) { return val<char16_t,T>(value); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename T> inline U32FormattedValue u32val(const T& value) { return val<char32_t,T>(value); }
#endif

    template<typename T> inline WFormattedValue   wval(  const T& value) { return val<wchar_t,T>(value); }

    template<typename Char>
    inline BasicFormattedValue<Char> val(const Char str[]) {
        return BasicFormattedValue<Char>(str,NoConv,BasicFormatSpec<Char>::DEFAULT);
    }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    inline U16FormattedValue u16val(const char16_t str[]) { return val<char16_t>(str); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    inline U32FormattedValue u32val(const char32_t str[]) { return val<char32_t>(str); }
#endif

    inline WFormattedValue   wval(  const wchar_t  str[]) { return val<wchar_t>(str); }

    template<typename Char,typename T>
    inline BasicFormattedValue<Char> val(const T& value, const Char* spec) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.spec(spec);
        return fmt;
    }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename T> inline U16FormattedValue u16val(const T& value, const char16_t* spec) { return val<char16_t,T>(value,spec); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename T> inline U32FormattedValue u32val(const T& value, const char32_t* spec) { return val<char32_t,T>(value,spec); }
#endif

    template<typename T> inline WFormattedValue   wval(  const T& value, const wchar_t*  spec) { return val<wchar_t,T>(value,spec); }

    template<typename Char,typename T>
    inline BasicFormattedValue<Char> val(const T& value, const std::basic_string<Char>& spec) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.spec(spec);
        return fmt;
    }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename T> inline U16FormattedValue u16val(const T& value, const std::u16string& spec) { return val<char16_t,T>(value, spec); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename T> inline U32FormattedValue u32val(const T& value, const std::u32string& spec) { return val<char32_t,T>(value, spec); }
#endif

    template<typename T> inline WFormattedValue   wval(  const T& value, const std::wstring&   spec) { return val<wchar_t,T>(value, spec); }

    template<typename Char,typename T>
    inline BasicFormattedValue<Char> val(const T& value, const BasicFormatSpec<Char>& spec) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.spec(spec);
        return fmt;
    }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename T> inline U16FormattedValue u16val(const T& value, const U16FormatSpec& spec) { return val<char16_t,T>(value, spec); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename T> inline U32FormattedValue u32val(const T& value, const U32FormatSpec& spec) { return val<char32_t,T>(value, spec); }
#endif

    template<typename T> inline WFormattedValue   wval(  const T& value, const WFormatSpec&   spec) { return val<wchar_t,T>(value, spec); }

    template<typename Char,typename T>
    inline BasicFormattedValue<Char> val(const T& value, typename BasicFormatSpec<Char>::Alignment alignment) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.align(alignment);
        return fmt;
    }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename T> inline U16FormattedValue u16val(const T& value, U16FormatSpec::Alignment alignment) { return val<char16_t,T>(value, alignment); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename T> inline U32FormattedValue u32val(const T& value, U32FormatSpec::Alignment alignment) { return val<char32_t,T>(value, alignment); }
#endif

    template<typename T> inline WFormattedValue   wval(  const T& value, WFormatSpec::Alignment   alignment) { return val<wchar_t,T>(value, alignment); }

    template<typename Char,typename T>
    inline BasicFormattedValue<Char> val(const T& value, typename BasicFormatSpec<Char>::Type type) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.type(type);
        return fmt;
    }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename T> inline U16FormattedValue wval(const T& value, U16FormatSpec::Type type) { return val<char16_t,T>(value, type); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename T> inline U32FormattedValue wval(const T& value, U32FormatSpec::Type type) { return val<char32_t,T>(value, type); }
#endif

    template<typename T> inline WFormattedValue   wval(const T& value, WFormatSpec::Type   type) { return val<wchar_t,T>(value, type); }

    template<typename Char,typename T>
    inline BasicFormattedValue<Char> val(const T& value, typename BasicFormatSpec<Char>::Sign sign) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.sign(sign);
        return fmt;
    }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename T> inline U16FormattedValue u16val(const T& value, U16FormatSpec::Sign sign) { return val<char16_t,T>(value, sign); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename T> inline U32FormattedValue u32val(const T& value, U32FormatSpec::Sign sign) { return val<char32_t,T>(value, sign); }
#endif

    template<typename T> inline WFormattedValue   wval(  const T& value, WFormatSpec::Sign   sign) { return val<wchar_t,T>(value, sign); }

    template<typename Char,typename T>
    inline BasicFormattedValue<Char> val(const T& value, Char fill, int width) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.fill(fill, width);
        return fmt;
    }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename T> inline U16FormattedValue u16val(const T& value, char16_t fill, int width) { return val<char16_t,T>(value, fill, width); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename T> inline U32FormattedValue u32val(const T& value, char32_t fill, int width) { return val<char32_t,T>(value, fill, width); }
#endif

    template<typename T> inline WFormattedValue   wval(  const T& value, wchar_t  fill, int width) { return val<wchar_t,T>(value, fill, width); }

    template<typename Char,typename Iter>
    inline BasicFormattedValue<Char> slice(Iter begin, Iter end) {
        return make_slice_formatter<Char,Iter>(begin, end);
    }

    template<typename Iter> inline FormattedValue    slice(  Iter begin, Iter end) { return slice<char,Iter>(begin, end); }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename Iter> inline U16FormattedValue u16slice(Iter begin, Iter end) { return slice<char16_t,Iter>(begin, end); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename Iter> inline U32FormattedValue u32slice(Iter begin, Iter end) { return slice<char32_t,Iter>(begin, end); }
#endif

    template<typename Iter> inline WFormattedValue   wslice(  Iter begin, Iter end) { return slice<wchar_t,Iter>(begin, end); }

    template<typename Char,typename Number>
    inline BasicFormattedValue<Char> bin(Number value) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.bin();
        return fmt;
    }

    template<typename Number> inline FormattedValue    bin(   Number value) { return bin<char,Number>(value); }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename Number> inline U16FormattedValue u16bin(Number value) { return bin<char16_t,Number>(value); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename Number> inline U32FormattedValue u32bin(Number value) { return bin<char32_t,Number>(value); }
#endif

    template<typename Number> inline WFormattedValue   wbin(  Number value) { return bin<wchar_t,Number>(value); }

    template<typename Char,typename Number>
    inline BasicFormattedValue<Char> dec(Number value) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.dec();
        return fmt;
    }

    template<typename Number> inline FormattedValue    dec(   Number value) { return dec<char,Number>(value); }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename Number> inline U16FormattedValue u16dec(Number value) { return dec<char16_t,Number>(value); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename Number> inline U32FormattedValue u32dec(Number value) { return dec<char32_t,Number>(value); }
#endif

    template<typename Number> inline WFormattedValue   wdec(  Number value) { return dec<wchar_t,Number>(value); }

    template<typename Char,typename Number>
    inline BasicFormattedValue<Char> oct(Number value) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.oct();
        return fmt;
    }

    template<typename Number> inline FormattedValue    oct(   Number value) { return oct<char,Number>(value); }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename Number> inline U16FormattedValue u16oct(Number value) { return oct<char16_t,Number>(value); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename Number> inline U32FormattedValue u32oct(Number value) { return oct<char32_t,Number>(value); }
#endif

    template<typename Number> inline WFormattedValue   woct(  Number value) { return oct<wchar_t,Number>(value); }

    template<typename Char,typename Number>
    inline BasicFormattedValue<Char> hex(Number value) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.hex();
        return fmt;
    }

    template<typename Number> inline FormattedValue    hex(   Number value) { return hex<char,Number>(value); }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename Number> inline U16FormattedValue u16hex(Number value) { return hex<char16_t,Number>(value); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename Number> inline U32FormattedValue u32hex(Number value) { return hex<char32_t,Number>(value); }
#endif

    template<typename Number> inline WFormattedValue   whex(  Number value) { return hex<wchar_t,Number>(value); }

    template<typename Char,typename T>
    inline BasicFormattedValue<Char> str(const T& value) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.str();
        return fmt;
    }

    template<typename T> inline FormattedValue    str(   const T& value) { return str<char,T>(value); }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename T> inline U16FormattedValue u16str(const T& value) { return str<char16_t,T>(value); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename T> inline U32FormattedValue u32str(const T& value) { return str<char32_t,T>(value); }
#endif

    template<typename T> inline WFormattedValue   wstr(  const T& value) { return str<wchar_t,T>(value); }

    template<typename Char,typename T>
    inline BasicFormattedValue<Char> repr(const T& value) {
        BasicFormattedValue<Char> fmt = val(value);
        fmt.repr();
        return fmt;
    }

    template<typename T> inline FormattedValue    repr(   const T& value) { return repr<char,T>(value); }

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<typename T> inline U16FormattedValue u16repr(const T& value) { return repr<char16_t,T>(value); }
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<typename T> inline U32FormattedValue u32repr(const T& value) { return repr<char32_t,T>(value); }
#endif

    template<typename T> inline WFormattedValue   wrepr(  const T& value) { return repr<wchar_t,T>(value); }

    template<typename Char,typename OStream>
    OStream& operator << (OStream& out, const BasicFormattedValue<Char>& value) {
        value.format(out);
        return out;
    }
}

#endif // FORMATSTRING_FORMATTEDVALUE_H
