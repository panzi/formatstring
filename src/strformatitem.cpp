#include "strformatitem.h"

using namespace formatstring;

StrFormatItem::StrFormatItem(const std::string& str) : m_str(str) {}

void StrFormatItem::apply(std::ostream& out, const Formatters &formatters) const {
    (void)formatters;
    out << m_str;
}
