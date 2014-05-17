#ifndef FORMATSTRING_EXCEPTIONS_H
#define FORMATSTRING_EXCEPTIONS_H

#include <stdexcept>
#include <string>
#include <cstdint>

namespace formatstring {

    class InvalidFormatStringException : public std::invalid_argument {
    public:
        InvalidFormatStringException(std::string::size_type pos, const char* what);

        std::string::size_type position() const { return m_pos; }

    private:
        std::string message(std::string::size_type pos, const char* what);

        std::string::size_type m_pos;
    };

    class InvalidFormatArgumentException : public std::out_of_range {
    public:
        InvalidFormatArgumentException(std::size_t index);

        std::size_t index() const { return m_index; }

    private:
        std::size_t m_index;

        static std::string message(std::size_t index);
    };

}

#endif // FORMATSTRING_EXCEPTIONS_H
