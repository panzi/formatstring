#ifndef FORMATSTRING_FORMATSPEC_H
#define FORMATSTRING_FORMATSPEC_H
#pragma once

#include "formatstring/config.h"
#include "formatstring/export.h"

#include <cstdint>
#include <string>

namespace formatstring {

    template<typename Char>
    struct BasicFormatSpec;

    typedef BasicFormatSpec<char> FormatSpec;

#ifdef FORMATSTRING_CHAR16_SUPPORT
    typedef BasicFormatSpec<char16_t> U16FormatSpec;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    typedef BasicFormatSpec<char32_t> U32FormatSpec;
#endif

    typedef BasicFormatSpec<wchar_t> WFormatSpec;

    template<typename Char>
    BasicFormatSpec<Char> parse_spec(const Char* str);

    template<typename Char>
    struct FORMATSTRING_EXPORT BasicFormatSpec {
        typedef Char char_type;
        typedef BasicFormatSpec<char_type> self_type;

        static const self_type DEFAULT;
        static const int DEFAULT_PRECISION = 12;

        enum Alignment {
            DefaultAlignment,
            Left,
            Right,
            Center,
            AfterSign
        };

        enum Type {
            // generic type:
            Generic,

            // integer types:
            Bin,
            Character, // is string-ish though
            Dec,
            Oct,
            Hex,

            // decimal types:
            Exp,
            Fixed,
            General,
            Percentage,
            HexFloat,

            String

            // TODO:
            // LocaleAwareNumber
        };

        enum Sign {
            DefaultSign,
            Always,
            NegativeOnly,
            SpaceForPositive
        };

        char_type fill;
        Alignment alignment;
        Sign      sign;
        bool      alternate;
        int       width;
        bool      thoudsandsSeperator;
        int       precision;
        Type      type;
        bool      upperCase;

        inline BasicFormatSpec(const char_type* spec) : BasicFormatSpec(std::move(parse_spec(spec))) {}

        inline BasicFormatSpec(const std::basic_string<char_type>& spec) : BasicFormatSpec(spec.c_str()) {}

        BasicFormatSpec(const self_type& other) = default;

        inline BasicFormatSpec(
                char_type fill = ' ',
                Alignment alignment = DefaultAlignment,
                Sign      sign = DefaultSign,
                bool      alternate = false,
                int       width = 0,
                bool      thoudsandsSeperator = false,
                int       precision = DEFAULT_PRECISION,
                Type      type = Generic,
                bool      upperCase = false) noexcept :
            fill(fill), alignment(alignment), sign(sign), alternate(alternate), width(width),
            thoudsandsSeperator(thoudsandsSeperator), precision(precision), type(type), upperCase(upperCase) {}

        self_type& operator= (const self_type& other) = default;

        inline self_type& operator= (const std::basic_string<Char>& spec) {
            *this = parse_spec(spec.c_str());
            return *this;
        }

        inline self_type& operator= (const Char* spec) {
            *this = parse_spec(spec);
            return *this;
        }

        inline bool equals(const self_type& other) const noexcept {
            return fill == other.fill && alignment == other.alignment &&
                   sign == other.sign && alternate == other.alternate &&
                   width == other.width && thoudsandsSeperator == other.thoudsandsSeperator &&
                   precision == other.precision && type == other.type &&
                   upperCase == other.upperCase;
        }

        inline bool isNumberType() const noexcept {
            switch (type) {
            case Bin:
            case Character:
            case Dec:
            case Oct:
            case Hex:
            case Exp:
            case Fixed:
            case General:
            case Percentage:
            case HexFloat:
                return true;

            default:
                return false;
            }
        }

        inline bool isIntegerType() const noexcept {
            switch (type) {
            case Bin:
            case Character:
            case Dec:
            case Oct:
            case Hex:
                return true;

            default:
                return false;
            }
        }

        inline bool isFloatType() const noexcept {
            switch (type) {
            case Exp:
            case Fixed:
            case General:
            case Percentage:
            case HexFloat:
                return true;

            default:
                return false;
            }
        }

        inline bool isStringType() const noexcept {
            switch (type) {
            case Character:
            case String:
                return true;

            default:
                return false;
            }
        }
    };

    template<typename Char>
    inline bool operator==(const BasicFormatSpec<Char>& lhs, const BasicFormatSpec<Char>& rhs) noexcept {
        return lhs.equals(rhs);
    }

    template<typename Char>
    inline bool operator!=(const BasicFormatSpec<Char>& lhs, const BasicFormatSpec<Char>& rhs) noexcept {
        return !lhs.equals(rhs);
    }

    // ---- extern template instantiations ----
    extern template FORMATSTRING_EXPORT FormatSpec parse_spec<char>(const char* str);

#ifdef FORMATSTRING_CHAR16_SUPPORT
    extern template FORMATSTRING_EXPORT U16FormatSpec parse_spec<char16_t>(const char16_t* str);
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    extern template FORMATSTRING_EXPORT U32FormatSpec parse_spec<char32_t>(const char32_t* str);
#endif

    extern template FORMATSTRING_EXPORT WFormatSpec parse_spec<wchar_t>(const wchar_t* str);

    extern template class FORMATSTRING_EXPORT BasicFormatSpec<char>;
    extern template class FORMATSTRING_EXPORT BasicFormatSpec<wchar_t>;

#ifdef FORMATSTRING_CHAR16_SUPPORT
    extern template class FORMATSTRING_EXPORT BasicFormatSpec<char16_t>;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    extern template class FORMATSTRING_EXPORT BasicFormatSpec<char32_t>;
#endif
}

#endif // FORMATSTRING_FORMATSPEC_H
