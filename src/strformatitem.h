#ifndef FORMATSTRING_STRFORMATITEM_H
#define FORMATSTRING_STRFORMATITEM_H
#pragma once

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
    typedef BasicStrFormatItem<wchar_t> WStrFormatItem;

    // ---- extern template instantiations ----
    extern template class BasicStrFormatItem<char>;
    extern template class BasicStrFormatItem<wchar_t>;

#ifdef FORMATSTRING_CHAR16_SUPPORT
    typedef BasicStrFormatItem<char16_t> U16StrFormatItem;
    extern template class BasicStrFormatItem<char16_t>;
#endif

#ifdef FORMATSTRING_CHAR16_SUPPORT
    typedef BasicStrFormatItem<char16_t> U32StrFormatItem;
    extern template class BasicStrFormatItem<char32_t>;
#endif
}

#endif // FORMATSTRING_STRFORMATITEM_H
