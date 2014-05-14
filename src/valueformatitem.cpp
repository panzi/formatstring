#include "formatstring/exceptions.h"

#include "valueformatitem.h"

using namespace formatstring;

ValueFormatItem::ValueFormatItem(std::size_t index, const FormatSpec& spec) :
    m_index(index), m_spec(spec) {}

void ValueFormatItem::apply(std::ostream &out, const Formatters &formatters) const {
    if (m_index >= formatters.size()) {
        throw InvalidFormatArgumentException(m_index);
    }
    formatters[m_index]->format(out, m_spec);
}

ValueFormatItem* ValueFormatItem::clone() const {
    return new ValueFormatItem(m_index, m_spec);
}
