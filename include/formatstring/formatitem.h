#ifndef FORMATSTRING_FORMATITEM_H
#define FORMATSTRING_FORMATITEM_H

#include "formatstring/formatter.h"

#include <iosfwd>

namespace formatstring {

    class FormatItem {
    public:
        virtual ~FormatItem() {}
        virtual void apply(std::ostream& out, const Formatters& formatters) const = 0;
        virtual FormatItem* clone() const = 0;
    };

    typedef std::vector< std::unique_ptr<FormatItem> > FormatItems;
}

#endif // UNISTR_FORMATITEM_H
