#ifndef FORMATSTRING_VALUEFORMATITEM_H
#define FORMATSTRING_VALUEFORMATITEM_H

#include "formatstring/formatitem.h"
#include "formatstring/formatspec.h"

#include <cstdint>

namespace formatstring {

    class ValueFormatItem : public FormatItem {
    public:
        ValueFormatItem(std::size_t index, const FormatSpec& spec);

        virtual void apply(std::ostream& out, const Formatters& formatters) const;
        virtual ValueFormatItem* clone() const;

    private:
        std::size_t m_index;
        FormatSpec  m_spec;
    };

}

#endif // FORMATSTRING_VALUEFORMATITEM_H
