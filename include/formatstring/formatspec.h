#ifndef FORMATSTRING_FORMATSPEC_H
#define FORMATSTRING_FORMATSPEC_H

#include <cstdint>
#include <string>

namespace formatstring {

    struct FormatSpec {

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

        char      fill;
        Alignment alignment;
        Sign      sign;
        bool      alternate;
        int       width;
        bool      thoudsandsSeperator;
        int       precision;
        Type      type;
        bool      upperCase;

        FormatSpec(const char* spec);
        FormatSpec(const std::string& spec);

        FormatSpec(const FormatSpec& other) = default;

        inline FormatSpec() :
            fill(' '), alignment(DefaultAlignment), sign(DefaultSign), alternate(false), width(0),
            thoudsandsSeperator(false), precision(12), type(Generic), upperCase(false) {}

        FormatSpec& operator= (const FormatSpec& other) = default;

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
