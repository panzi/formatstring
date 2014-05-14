#include "formatstring/formatspec.h"
#include "formatstring/exceptions.h"

#include <sstream>

using namespace formatstring;

InvalidFormatArgumentException::InvalidFormatArgumentException(std::size_t index) :
    std::out_of_range(message(index)), m_index(index) {}

std::string InvalidFormatArgumentException::message(std::size_t index) {
    std::stringstream buf;
    buf << "Invalid format argument index: " << index;
    return buf.str();
}

InvalidFormatStringException::InvalidFormatStringException(const std::string& str, std::size_t pos, const char* what) :
    std::invalid_argument(message(str, pos, what)), m_str(str), m_pos(pos) {}

std::string InvalidFormatStringException::message(const std::string& str, std::size_t pos, const char* what) {
    std::stringstream buf;
    buf << "Invalid format string: " << what << " at position " << pos << ": " << str;
    return buf.str();
}
