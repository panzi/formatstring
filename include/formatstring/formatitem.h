#ifndef FORMATSTRING_FORMATITEM_H
#define FORMATSTRING_FORMATITEM_H
#pragma once

#include "formatstring/formatter.h"

#include <iosfwd>
#include <vector>
#include <memory>

namespace formatstring {

    template<typename Char>
    class BasicFormatter;

    template<typename Char>
    class BasicFormatItem {
    public:
        typedef Char char_type;
        typedef std::vector< std::unique_ptr< BasicFormatItem<Char> > > List;

        virtual ~BasicFormatItem() {}
        virtual void apply(std::basic_ostream<Char>& out, const typename BasicFormatter<Char>::List& formatters) const = 0;
    };

    typedef BasicFormatItem<char> FormatItem;
    typedef BasicFormatItem<wchar_t> WFormatItem;
    typedef FormatItem::List FormatItems;

#ifdef FORMATSTRING_CHAR16_SUPPORT
    typedef BasicFormatItem<char16_t> U16FormatItem;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    typedef BasicFormatItem<char32_t> U32FormatItem;
#endif
}

#endif // FORMATSTRING_FORMATITEM_H
