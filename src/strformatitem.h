#ifndef FORMATSTRING_STRFORMATITEM_H
#define FORMATSTRING_STRFORMATITEM_H

#include "formatstring/formatitem.h"

namespace formatstring {

    template<typename Char>
    class BasicStrFormatItem : public BasicFormatItem<Char> {
    public:
        typedef Char char_type;

        BasicStrFormatItem(const Char* str) : m_str(str) {}
        BasicStrFormatItem(const std::basic_string<Char>& str) : m_str(str) {}

        virtual void apply(std::basic_ostream<Char>& out, const typename BasicFormatter<Char>::List& formatters) const {
            (void)formatters;
            out << m_str;
        }

    private:
        std::basic_string<Char> m_str;
    };

    typedef BasicStrFormatItem<char> StrFormatItem;

}

#endif // FORMATSTRING_STRFORMATITEM_H
