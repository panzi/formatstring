#ifndef FORMATSTRING_FORMATITEM_H
#define FORMATSTRING_FORMATITEM_H

#include "formatstring/formatter.h"

#include <iosfwd>

namespace formatstring {

    template<typename Char>
    class BasicFormatItem {
    public:
        typedef Char char_type;
        typedef std::vector< std::unique_ptr< BasicFormatItem<Char> > > List;

        virtual ~BasicFormatItem() {}
        virtual void apply(std::basic_ostream<Char>& out, const typename BasicFormatter<Char>::List& formatters) const = 0;
    };

    typedef BasicFormatItem<char> FormatItem;
    typedef FormatItem::List FormatItems;
}

#endif // UNISTR_FORMATITEM_H
