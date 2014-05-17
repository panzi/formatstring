#ifndef FORMATSTRING_VALUEFORMATITEM_H
#define FORMATSTRING_VALUEFORMATITEM_H

#include "formatstring/formatitem.h"
#include "formatstring/formatspec.h"
#include "formatstring/conversion.h"
#include "formatstring/exceptions.h"

#include <cstdint>

namespace formatstring {

    template<typename Char>
    class BasicValueFormatItem : public BasicFormatItem<Char> {
    public:
        typedef Char char_type;

        BasicValueFormatItem(std::size_t index, Conversion conv, const BasicFormatSpec<Char>& spec) :
            m_index(index), m_conv(conv), m_spec(spec) {}

        virtual void apply(std::basic_ostream<Char>& out, const typename BasicFormatter<Char>::List& formatters) const {
            if (m_index >= formatters.size()) {
                throw InvalidFormatArgumentException(m_index);
            }
            formatters[m_index]->format(out, m_conv, m_spec);
        }

    private:
        std::size_t           m_index;
        Conversion            m_conv;
        BasicFormatSpec<Char> m_spec;
    };

    typedef BasicValueFormatItem<char> ValueFormatItem;
}

#endif // FORMATSTRING_VALUEFORMATITEM_H
