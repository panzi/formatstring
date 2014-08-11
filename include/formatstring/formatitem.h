#ifndef FORMATSTRING_FORMATITEM_H
#define FORMATSTRING_FORMATITEM_H
#pragma once

#include "formatstring/formatter.h"

#include <iosfwd>
#include <vector>
#include <memory>

namespace formatstring {

    template<typename Char>
    class BasicFormatItem {
    public:
        typedef Char char_type;

        virtual ~BasicFormatItem() {}
        virtual void apply(std::basic_ostream<Char>& out, const BasicFormatters<Char>& formatters) const = 0;
    };

    template<typename Char>
    using BasicFormatItems = std::vector< std::unique_ptr< BasicFormatItem<Char> > >;

    typedef BasicFormatItem<char> FormatItem;
    typedef BasicFormatItem<wchar_t> WFormatItem;
    typedef BasicFormatItems<char> FormatItems;
    typedef BasicFormatItems<wchar_t> WFormatItems;

#ifdef FORMATSTRING_CHAR16_SUPPORT
    typedef BasicFormatItem<char16_t> U16FormatItem;
    typedef BasicFormatItems<char16_t> U16FormatItems;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    typedef BasicFormatItem<char32_t> U32FormatItem;
    typedef BasicFormatItems<char32_t> U32FormatItems;
#endif
}

#endif // FORMATSTRING_FORMATITEM_H
