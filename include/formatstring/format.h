#ifndef FORMATSTRING_FORMAT_H
#define FORMATSTRING_FORMAT_H
#pragma once

#include <string>
#include <iosfwd>
#include <sstream>

#include "formatstring/config.h"
#include "formatstring/export.h"

#include "formatstring/formatter.h"
#include "formatstring/formatitem.h"

namespace formatstring {

    template<typename Char>
    class BasicFormat;

    typedef BasicFormat<char> Format;

#ifdef FORMATSTRING_CHAR16_SUPPORT
    typedef BasicFormat<char16_t> U16Format;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    typedef BasicFormat<char32_t> U32Format;
#endif

    typedef BasicFormat<wchar_t> WFormat;

    template<typename Char>
    class BasicBoundFormat;

    typedef BasicBoundFormat<char> BoundFormat;

    template<typename Char>
    void parse_format(const Char* fmt, typename BasicFormatItem<Char>::List *items);

    template<typename Char>
    class FORMATSTRING_EXPORT BasicFormat {
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
    class FORMATSTRING_EXPORT BasicBoundFormat {
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

    template<typename Char, typename OStream>
    inline OStream& operator << (OStream& out, const BasicBoundFormat<Char>& fmt) {
        fmt.write_into(out);
        return out;
    }

    template<typename Char, typename... Args>
    inline BasicBoundFormat<Char> format(const std::basic_string<Char>& fmt, const Args&... args) {
        return BasicBoundFormat<Char>(fmt, args...);
    }

    template<typename Char, typename... Args>
    inline BasicBoundFormat<Char> format(const Char* fmt, const Args&... args) {
        return BasicBoundFormat<Char>(fmt, args...);
    }

    template<typename Char>
    inline BasicFormat<Char> compile(const std::basic_string<Char>& fmt) {
        return fmt;
    }

    template<typename Char>
    inline BasicFormat<Char> compile(const Char* fmt) {
        return fmt;
    }

    // ---- debug ----
    template<typename Char>
    class DummyBoundFormat;

    template<typename Char>
    class DummyFormat {
    public:
        typedef Char char_type;

        inline DummyFormat() {}
        inline DummyFormat(const Char* fmt) { (void)fmt; }
        inline DummyFormat(const std::basic_string<Char>& fmt) { (void)fmt; }

        template<typename... Args>
        inline void format(std::basic_ostream<Char>& out, const Args&...) const {
            (void)out;
        }

        template<typename... Args>
        inline DummyBoundFormat<Char> bind(const Args&...) const {
            return DummyBoundFormat<Char>();
        }

        template<typename... Args>
        inline DummyBoundFormat<Char> operator () (const Args&...) const {
            return DummyBoundFormat<Char>();
        }

        inline void apply(std::basic_ostream<Char>& out, const typename BasicFormatter<Char>::List& formatters) const {
            (void)out;
            (void)formatters;
        }

        inline operator BasicFormat<Char> () const {
            const Char fmt[] = {(Char)0};
            return BasicFormat<Char>(fmt);
        }
    };

    template<typename Char>
    class DummyBoundFormat {
    public:
        typedef Char char_type;

        inline DummyBoundFormat() {}

        template<typename... Args>
        inline DummyBoundFormat(const BasicFormat<Char>& format, const Args&...) {
            (void)format;
        }

        template<typename... Args>
        inline DummyBoundFormat(BasicFormat<Char>&& format, const Args&...) {
            (void)format;
        }

        inline void write_into(std::basic_ostream<Char>& out) const {
            (void)out;
        }

        inline operator std::basic_string<Char> () const {
            return std::basic_string<Char>();
        }

        inline std::basic_string<Char> str() const {
            return std::basic_string<Char>();
        }
    };

    template<typename Char, typename OStream>
    inline OStream& operator << (OStream& out, const DummyBoundFormat<Char>& fmt) {
        (void)fmt;
        return out;
    }

#ifdef NDEBUG
    template<typename Char, typename... Args>
    inline DummyBoundFormat<Char> debug(const std::basic_string<Char>& fmt, const Args&...) {
        (void)fmt;
        return DummyBoundFormat<Char>();
    }

    template<typename Char, typename... Args>
    inline DummyBoundFormat<Char> debug(const Char* fmt, const Args&...) {
        (void)fmt;
        return DummyBoundFormat<Char>();
    }

    template<typename Char>
    inline DummyFormat<Char> debug_compile(const std::basic_string<Char>& fmt) {
        (void)fmt;
        return DummyFormat<Char>();
    }

    template<typename Char>
    inline DummyFormat<Char> debug_compile(const Char* fmt) {
        (void)fmt;
        return DummyFormat<Char>();
    }
#else
    template<typename Char, typename... Args>
    inline BasicBoundFormat<Char> debug(const std::basic_string<Char>& fmt, const Args&... args) {
        return BasicBoundFormat<Char>(fmt, args...);
    }

    template<typename Char, typename... Args>
    inline BasicBoundFormat<Char> debug(const Char* fmt, const Args&... args) {
        return BasicBoundFormat<Char>(fmt, args...);
    }

    template<typename Char>
    inline BasicFormat<Char> debug_compile(const std::basic_string<Char>& fmt) {
        return fmt;
    }

    template<typename Char>
    inline BasicFormat<Char> debug_compile(const Char* fmt) {
        return fmt;
    }
#endif

    extern template FORMATSTRING_EXPORT void parse_format<char>(const char* fmt, FormatItem::List *items);

#ifdef FORMATSTRING_CHAR16_SUPPORT
    extern template FORMATSTRING_EXPORT void parse_format<char16_t>(const char16_t* fmt, U16FormatItem::List *items);
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    extern template FORMATSTRING_EXPORT void parse_format<char32_t>(const char32_t* fmt, U32FormatItem::List *items);
#endif

    extern template FORMATSTRING_EXPORT void parse_format<wchar_t>(const wchar_t* fmt, WFormatItem::List *items);

    extern template class FORMATSTRING_EXPORT BasicFormat<char>;
    extern template class FORMATSTRING_EXPORT BasicBoundFormat<char>;

#ifdef FORMATSTRING_CHAR16_SUPPORT
    extern template class FORMATSTRING_EXPORT BasicFormat<char16_t>;
    extern template class FORMATSTRING_EXPORT BasicBoundFormat<char16_t>;
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
    extern template class FORMATSTRING_EXPORT BasicFormat<char32_t>;
    extern template class FORMATSTRING_EXPORT BasicBoundFormat<char32_t>;
#endif

    extern template class FORMATSTRING_EXPORT BasicFormat<wchar_t>;
    extern template class FORMATSTRING_EXPORT BasicBoundFormat<wchar_t>;
}

#endif // FORMATSTRING_FORMAT_H
