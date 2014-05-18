#ifndef FORMATSTRING_FORMAT_H
#define FORMATSTRING_FORMAT_H
#pragma once

#include <string>
#include <iosfwd>
#include <sstream>

#include "formatstring/formatter.h"
#include "formatstring/formatitem.h"

namespace formatstring {

    template<typename Char>
    class BasicFormat;

    typedef BasicFormat<char> Format;
    typedef BasicFormat<char16_t> U16Format;
    typedef BasicFormat<char32_t> U32Format;
    typedef BasicFormat<wchar_t> WFormat;

    template<typename Char>
    class BasicBoundFormat;

    typedef BasicBoundFormat<char> BoundFormat;

    void parse_format(const char* fmt, BasicFormatItem<char>::List *items);
    void parse_format(const char16_t* fmt, BasicFormatItem<char16_t>::List *items);
    void parse_format(const char32_t* fmt, BasicFormatItem<char32_t>::List *items);
    void parse_format(const wchar_t* fmt, BasicFormatItem<wchar_t>::List *items);

    template<typename Char>
    class BasicFormat {
    public:
        typedef Char char_type;

        BasicFormat(const Char* fmt) : m_fmt(std::make_shared<typename BasicFormatItem<Char>::List>()) {
            parse_format(fmt, &*m_fmt);
        }

        BasicFormat(const std::basic_string<Char>& fmt) : BasicFormat(fmt.c_str()) {}
        BasicFormat(const BasicFormat<Char>& other) : m_fmt(other.m_fmt) {}
        BasicFormat(BasicFormat<Char>&& rhs) : m_fmt(std::move(rhs.m_fmt)) {}

        template<typename... Args>
        void format(std::basic_ostream<Char>& out, const Args&... args) const {
            typename BasicFormatter<Char>::List formatters;
            unpack_formatters(formatters, args...);
            apply(out, formatters);
        }

        template<typename... Args>
        inline BasicBoundFormat<Char> bind(const Args&... args) const;

        template<typename... Args>
        inline BasicBoundFormat<Char> operator () (const Args&... args) const;

        void apply(std::basic_ostream<Char>& out, const typename BasicFormatter<Char>::List& formatters) const {
            for (auto& item : *m_fmt) {
                item->apply(out, formatters);
            }
        }

    private:
        std::shared_ptr<typename BasicFormatItem<Char>::List> m_fmt;
    };

    template<typename Char>
    class BasicBoundFormat {
    public:
        typedef Char char_type;

        BasicBoundFormat(BasicBoundFormat<Char>&& rhs) :
            m_format(std::move(rhs.m_format)), m_formatters(std::move(rhs.m_formatters)) {}

        BasicBoundFormat(const BasicBoundFormat<Char>& other) :
            m_format(other.m_format), m_formatters(other.m_formatters) {}

        template<typename... Args>
        BasicBoundFormat(const BasicFormat<Char>& format, const Args&... args) :
            m_format(format), m_formatters(std::make_shared<typename BasicFormatter<Char>::List>()) {
            unpack_formatters<Char>(*m_formatters, args...);
        }

        template<typename... Args>
        BasicBoundFormat(BasicFormat<Char>&& format, const Args&... args) :
            m_format(std::move(format)), m_formatters(std::make_shared<typename BasicFormatter<Char>::List>()) {
            unpack_formatters<Char>(*m_formatters, args...);
        }

        inline void write_into(std::basic_ostream<Char>& out) const {
            m_format.apply(out, *m_formatters);
        }

        inline operator std::basic_string<Char> () const {
            std::basic_ostringstream<Char> out;
            m_format.apply(out, *m_formatters);
            return out.str();
        }

        inline std::basic_string<Char> str() const {
            return (std::basic_string<Char>) *this;
        }

    private:
        BasicFormat<Char> m_format;
        std::shared_ptr<typename BasicFormatter<Char>::List> m_formatters;
    };

    template<typename Char>
    template<typename... Args>
    inline BasicBoundFormat<Char> BasicFormat<Char>::bind(const Args&... args) const {
        return BasicBoundFormat<Char>(*this, args...);
    }

    template<typename Char>
    template<typename... Args>
    inline BasicBoundFormat<Char> BasicFormat<Char>::operator () (const Args&... args) const {
        return bind(args...);
    }

    template<typename Char, typename... Args>
    inline BasicBoundFormat<Char> format(const std::basic_string<Char>& fmt, const Args&... args) {
        return BasicBoundFormat<Char>(fmt, args...);
    }

    template<typename Char, typename... Args>
    inline BasicBoundFormat<Char> format(const Char* fmt, const Args&... args) {
        return BasicBoundFormat<Char>(fmt, args...);
    }

    template<typename Char, typename OStream>
    OStream& operator << (OStream& out, const BasicBoundFormat<Char>& fmt) {
        fmt.write_into(out);
        return out;
    }

    template<typename Char>
    inline BasicFormat<Char> compile(const std::basic_string<Char>& fmt) {
        return fmt;
    }

    template<typename Char>
    inline BasicFormat<Char> compile(const Char* fmt) {
        return fmt;
    }
}

#endif // FORMATSTRING_FORMAT_H
