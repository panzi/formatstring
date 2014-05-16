#ifndef FORMATSTRING_STRFORMATITEM_H
#define FORMATSTRING_STRFORMATITEM_H

#include "formatstring/formatitem.h"

namespace formatstring {

    class StrFormatItem : public FormatItem {
    public:
        StrFormatItem(const std::string& str);

        virtual void apply(std::ostream& out, const Formatters& formatters) const;

    private:
        std::string m_str;
    };

}

#endif // FORMATSTRING_STRFORMATITEM_H
