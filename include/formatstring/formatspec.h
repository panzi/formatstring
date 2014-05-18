#ifndef FORMATSTRING_FORMATSPEC_H
#define FORMATSTRING_FORMATSPEC_H
#pragma once

#include <cstdint>
#include <string>

namespace formatstring {

    template<typename Char>
    struct BasicFormatSpec;

    typedef BasicFormatSpec<char> FormatSpec;
    typedef BasicFormatSpec<char16_t> U16FormatSpec;
    typedef BasicFormatSpec<char32_t> U32FormatSpec;
    typedef BasicFormatSpec<wchar_t> WFormatSpec;

    void parse_spec(const char* str, FormatSpec* spec);
    void parse_spec(const char16_t* str, U16FormatSpec* spec);
    void parse_spec(const char32_t* str, U32FormatSpec* spec);
    void parse_spec(const wchar_t* str, WFormatSpec* spec);

    template<typename Char>
    struct BasicFormatSpec {
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

        BasicFormatSpec(const char_type* spec) : BasicFormatSpec() {
            parse_spec(spec, this);
        }

        BasicFormatSpec(const std::basic_string<char_type>& spec) : BasicFormatSpec(spec.c_str()) {}

        BasicFormatSpec(const self_type& other) = default;

        inline BasicFormatSpec() :
            fill(' '), alignment(DefaultAlignment), sign(DefaultSign), alternate(false), width(0),
            thoudsandsSeperator(false), precision(DEFAULT_PRECISION), type(Generic), upperCase(false) {}

        self_type& operator= (const self_type& other) = default;

        self_type& operator= (const std::basic_string<Char>& spec) {
            *this = self_type::DEFAULT;
            parse_spec(spec.c_str(), this);
            return *this;
        }

        self_type& operator= (const Char* spec) {
            *this = self_type::DEFAULT;
            parse_spec(spec, this);
            return *this;
        }

        inline bool isNumberType() const {
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
                return true;

            default:
                return false;
            }
        }

        inline bool isIntegerType() const {
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

        inline bool isDecimalType() const {
            switch (type) {
            case Exp:
            case Fixed:
            case General:
            case Percentage:
                return true;

            default:
                return false;
            }
        }

        inline bool isStringType() const {
            switch (type) {
            case Character:
            case String:
                return true;

            default:
                return false;
            }
        }
    };
}

#endif // FORMATSTRING_FORMATSPEC_H
