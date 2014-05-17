#include "formatstring/formatspec.h"
#include "formatstring/exceptions.h"

#include <sstream>

using namespace formatstring;

InvalidFormatArgumentException::InvalidFormatArgumentException(std::size_t index) :
    std::out_of_range(message(index)), m_index(index) {}

std::string InvalidFormatArgumentException::message(std::size_t index) {
    std::ostringstream buf;
    buf << "Invalid format argument index: " << index;
    return buf.str();
}

InvalidFormatStringException::InvalidFormatStringException(std::size_t pos, const char* what) :
    std::invalid_argument(message(pos, what)), m_pos(pos) {}

std::string InvalidFormatStringException::message(std::size_t pos, const char* what) {
    std::ostringstream buf;
    buf << "Invalid format string: " << what << " at position " << pos;
    return buf.str();
}
