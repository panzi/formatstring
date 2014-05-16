#ifndef FORMATSTRING_FORMAT_H
#define FORMATSTRING_FORMAT_H

#include <string>
#include <iosfwd>
#include <sstream>

#include "formatstring/formatter.h"
#include "formatstring/formatitem.h"

namespace formatstring {

    class Format;
    class BoundFormat;

    class Format {
    public:
        Format(const std::string& fmt);
        Format(const Format& other);
        Format(Format&& rhs) : m_fmt(std::move(rhs.m_fmt)) {}

        template<typename... Args>
        void format(std::ostream& out, const Args&... args) const {
            Formatters formatters;
            Formatter::extend(formatters, args...);
            apply(out, formatters);
        }

        template<typename... Args>
        inline BoundFormat bind(const Args&... args) const;

        template<typename... Args>
        inline BoundFormat operator () (const Args&... args) const;

        void apply(std::ostream& out, const Formatters& formatters) const;

    private:
        std::shared_ptr<FormatItems> m_fmt;
    };

    class BoundFormat {
    public:
        BoundFormat(BoundFormat&& rhs) : m_format(std::move(rhs.m_format)), m_formatters(std::move(rhs.m_formatters)) {}
        BoundFormat(const BoundFormat& other);

        template<typename... Args>
        BoundFormat(const Format& format, const Args&... args) : m_format(format), m_formatters(std::make_shared<Formatters>()) {
            Formatter::extend(*m_formatters, args...);
        }

        template<typename... Args>
        BoundFormat(Format&& format, const Args&... args) : m_format(std::move(format)), m_formatters(std::make_shared<Formatters>()) {
            Formatter::extend(*m_formatters, args...);
        }

        inline void write_into(std::ostream& out) const {
            m_format.apply(out, *m_formatters);
        }

        inline operator std::string () const {
            std::stringstream out;
            m_format.apply(out, *m_formatters);
            return out.str();
        }

        inline std::string str() const {
            return (std::string) *this;
        }

    private:
        Format m_format;
        std::shared_ptr<Formatters> m_formatters;
    };

    template<typename... Args>
    inline BoundFormat Format::bind(const Args&... args) const {
        return BoundFormat(*this, args...);
    }

    template<typename... Args>
    inline BoundFormat Format::operator () (const Args&... args) const {
        return bind(args...);
    }

    template<typename... Args>
    inline BoundFormat format(const std::string& fmt, const Args&... args) {
        return BoundFormat(std::move(fmt), args...);
    }

    template<typename OStream>
    OStream& operator << (OStream& out, const BoundFormat& fmt) {
        fmt.write_into(out);
        return out;
    }

    inline Format compile(const std::string& fmt) {
        return fmt;
    }
}

#endif // FORMATSTRING_FORMAT_H
