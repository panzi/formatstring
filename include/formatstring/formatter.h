#ifndef FORMATSTRING_FORMATTER_H
#define FORMATSTRING_FORMATTER_H

#include <iosfwd>
#include <memory>
#include <vector>
#include <list>
#include <array>
#include <tuple>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "formatstring/format.h"
#include "formatstring/formatvalue.h"
#include "formatstring/conversion.h"
#include "formatstring/formatspec.h"

namespace formatstring {

    template<typename Char>
    class BasicFormatter {
    public:
        typedef Char char_type;
        typedef std::vector< std::unique_ptr< BasicFormatter<char_type> > > List;

        virtual ~BasicFormatter() {}
        virtual void format(std::basic_ostream<char_type>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const = 0;
    };

    typedef BasicFormatter<char> Formatter;
    typedef Formatter::List Formatters;

    template<typename Char, typename First, typename... Rest>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const First& first, const Rest&... rest);

    template<typename Char, typename... Rest>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const char first[], const Rest&... rest);

    template<typename Char, typename Last>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const Last& last);

    template<typename Char>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const char last[]);

    template<typename Char, typename T,
             void _format(std::basic_ostream<Char>& out, T value, const BasicFormatSpec<Char>& spec) = format_value,
             void _repr(std::basic_ostream<Char>& out, T value) = repr_value>
    class ValueFormatter : public BasicFormatter<Char> {
    public:
        typedef Char char_type;
        typedef T value_type;

        ValueFormatter(const value_type& value) : value(value) {}

        virtual void format(std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const {
            switch (conv) {
            case ReprConv:
            {
                std::basic_ostringstream<Char> buffer;
                _repr(buffer, value);
                format_value(out, buffer.str(), spec);
                break;
            }
            case StrConv:
            {
                std::basic_ostringstream<Char> buffer;
                _format(buffer, value, BasicFormatSpec<Char>::DEFAULT);
                format_value(out, buffer.str(), spec);
                break;
            }
            default:
                _format(out, value, spec);
                break;
            }
        }

    private:
        const value_type value;
    };

    template<typename Char, typename T, typename Ptr = const T*,
             void _format(std::basic_ostream<Char>& out, const T& value, const BasicFormatSpec<Char>& spec) = format_value,
             void _repr(std::basic_ostream<Char>& out, const T& value) = repr_value>
    class PtrFormatter : public BasicFormatter<Char> {
    public:
        typedef Char char_type;
        typedef T value_type;
        typedef Ptr ptr_type;

        PtrFormatter(ptr_type value) : value(value) {}

        virtual void format(std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const {
            switch (conv) {
            case ReprConv:
            {
                std::basic_ostringstream<Char> buffer;
                _repr(buffer, *value);
                format_value(out, buffer.str(), spec);
                break;
            }
            case StrConv:
            {
                std::basic_ostringstream<Char> buffer;
                _format(buffer, *value, BasicFormatSpec<Char>::DEFAULT);
                format_value(out, buffer.str(), spec);
                break;
            }
            default:
                _format(out, *value, spec);
                break;
            }
        }

    private:
        ptr_type value;
    };

    template<typename Char,typename T>
    class FallbackFormatter : public PtrFormatter<Char,T,const T*,format_value_fallback,repr_value_fallback> {
    public:
        typedef Char char_type;
        typedef PtrFormatter<Char,T,const T*,format_value_fallback,repr_value_fallback> super_type;
        typedef typename super_type::value_type value_type;
        typedef typename super_type::ptr_type ptr_type;

        FallbackFormatter(ptr_type ptr) : super_type(ptr) {}
    };

    template<typename Char,typename Iter, Char left = '[', Char right = ']',
             void _format(std::basic_ostream<Char>& out, Iter begin, Iter end, const BasicFormatSpec<Char>& spec, Char left, Char right) = format_slice,
             void _repr(std::basic_ostream<Char>& out, Iter begin, Iter end, Char left, Char right) = repr_slice>
    class SliceFormatter : public BasicFormatter<Char> {
    public:
        typedef Char char_type;
        typedef Iter iterator_type;
        typedef typename std::iterator_traits<iterator_type>::value_type value_type;

        SliceFormatter(iterator_type begin, iterator_type end) :
            begin(begin), end(end) {}

        virtual void format(std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const {
            switch (conv) {
            case ReprConv:
            {
                std::basic_ostringstream<Char> buffer;
                _repr(buffer, begin, end, left, right);
                format_value(out, buffer.str(), spec);
                break;
            }
            case StrConv:
            {
                std::basic_ostringstream<Char> buffer;
                _format(buffer, begin, end, BasicFormatSpec<Char>::DEFAULT, left, right);
                format_value(out, buffer.str(), spec);
                break;
            }
            default:
                _format(out, begin, end, spec, left, right);
                break;
            }
        }

        const iterator_type begin;
        const iterator_type end;
    };

    // ---- make_formatter ----

    template<typename Char> inline ValueFormatter<Char,bool>* make_formatter(bool value) { return new ValueFormatter<Char,bool>(value); }

    inline ValueFormatter<char16_t,char16_t>* make_formatter(char16_t value) { return new ValueFormatter<char16_t,char16_t>(value); }
    inline ValueFormatter<char32_t,char32_t>* make_formatter(char32_t value) { return new ValueFormatter<char32_t,char32_t>(value); }
    inline ValueFormatter<wchar_t,wchar_t>*   make_formatter(wchar_t  value) { return new ValueFormatter<wchar_t,wchar_t>(value); }

    template<typename Char> inline ValueFormatter<Char,char>*      make_formatter(char      value) { return new ValueFormatter<Char,char>(value); }
    template<typename Char> inline ValueFormatter<Char,short>*     make_formatter(short     value) { return new ValueFormatter<Char,short>(value); }
    template<typename Char> inline ValueFormatter<Char,int>*       make_formatter(int       value) { return new ValueFormatter<Char,int>(value); }
    template<typename Char> inline ValueFormatter<Char,long>*      make_formatter(long      value) { return new ValueFormatter<Char,long>(value); }
    template<typename Char> inline ValueFormatter<Char,long long>* make_formatter(long long value) { return new ValueFormatter<Char,long long>(value); }

    template<typename Char> inline ValueFormatter<Char,unsigned char>*      make_formatter(unsigned char      value) { return new ValueFormatter<Char,unsigned char>(value); }
    template<typename Char> inline ValueFormatter<Char,unsigned short>*     make_formatter(unsigned short     value) { return new ValueFormatter<Char,unsigned short>(value); }
    template<typename Char> inline ValueFormatter<Char,unsigned int>*       make_formatter(unsigned int       value) { return new ValueFormatter<Char,unsigned int>(value); }
    template<typename Char> inline ValueFormatter<Char,unsigned long>*      make_formatter(unsigned long      value) { return new ValueFormatter<Char,unsigned long>(value); }
    template<typename Char> inline ValueFormatter<Char,unsigned long long>* make_formatter(unsigned long long value) { return new ValueFormatter<Char,unsigned long long>(value); }

    template<typename Char> inline ValueFormatter<Char,float>*  make_formatter(float  value) { return new ValueFormatter<Char,float>(value); }
    template<typename Char> inline ValueFormatter<Char,double>* make_formatter(double value) { return new ValueFormatter<Char,double>(value); }

    template<typename Char>
    inline ValueFormatter<Char,const Char*>* make_formatter(const Char value[]) {
        return new ValueFormatter<Char,const Char*>(value);
    }

    template<typename Char>
    inline PtrFormatter< Char,std::basic_string<Char> >* make_formatter(const std::basic_string<Char>& value) {
        return new PtrFormatter< Char,std::basic_string<Char> >(&value);
    }

    template<typename Char,typename T>
    inline FallbackFormatter<Char,T>* make_formatter(const T& value) {
        return new FallbackFormatter<Char,T>(&value);
    }

    template<typename Char,typename T>
    inline SliceFormatter<Char,typename std::vector<T>::const_iterator>* make_formatter(const std::vector<T>& value) {
        return new SliceFormatter<Char,typename std::vector<T>::const_iterator>(value.begin(), value.end());
    }

    template<typename Char,typename T>
    inline SliceFormatter<Char,typename std::list<T>::const_iterator>* make_formatter(const std::list<T>& value) {
        return new SliceFormatter<Char,typename std::list<T>::const_iterator>(value.begin(), value.end());
    }

    template<typename Char,typename T,std::size_t N>
    inline SliceFormatter<Char,typename std::array<T,N>::const_iterator>* make_formatter(const std::array<T,N>& value) {
        return new SliceFormatter<Char,typename std::array<T,N>::const_iterator>(value.begin(), value.end());
    }

    template<typename Char,typename T>
    inline SliceFormatter<Char,typename std::set<T>::const_iterator,'{','}'>* make_formatter(const std::set<T>& value) {
        return new SliceFormatter<Char,typename std::set<T>::const_iterator,'{','}'>(value.begin(), value.end());
    }

    template<typename Char,typename T>
    inline SliceFormatter<Char,typename std::unordered_set<T>::const_iterator,'{','}'>* make_formatter(const std::unordered_set<T>& value) {
        return new SliceFormatter<Char,typename std::unordered_set<T>::const_iterator,'{','}'>(value.begin(), value.end());
    }

    template<typename Char, typename... Args>
    inline PtrFormatter< Char,std::tuple<Args...> >* make_formatter(const std::tuple<Args...>& value) {
        return new PtrFormatter< Char,std::tuple<Args...> >(&value);
    }

    template<typename Char,typename First, typename Second>
    inline PtrFormatter< Char,std::pair<First,Second> >* make_formatter(const std::pair<First,Second>& value) {
        return new PtrFormatter< Char,std::pair<First,Second> >(&value);
    }

    template<typename Char, typename K, typename V>
    inline SliceFormatter<Char,typename std::map<K,V>::const_iterator,'{','}',format_map,repr_map>* make_formatter(const std::map<K,V>& value) {
        return new SliceFormatter<Char,typename std::map<K,V>::const_iterator,'{','}',format_map,repr_map>(value.begin(), value.end());
    }

    template<typename Char, typename K, typename V>
    inline SliceFormatter<Char,typename std::unordered_map<K,V>::const_iterator,'{','}',format_map,repr_map>* make_formatter(const std::unordered_map<K,V>& value) {
        return new SliceFormatter<Char,typename std::unordered_map<K,V>::const_iterator,'{','}',format_map,repr_map>(value.begin(), value.end());
    }

    // ---- unpack_formatters ----

    template<typename Char, typename First, typename... Rest>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const First& first, const Rest&... rest) {
        formatters.emplace_back(make_formatter<Char>(first));
        unpack_formatters<Char, Rest...>(formatters, rest...);
    }

    template<typename Char, typename... Rest>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const char first[], const Rest&... rest) {
        formatters.emplace_back(make_formatter(first));
        unpack_formatters(formatters, rest...);
    }

    template<typename Char, typename Last>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const Last& last) {
        formatters.emplace_back(make_formatter<Char>(last));
    }

    template<typename Char>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const char last[]) {
        formatters.emplace_back(make_formatter(last));
    }
}

#endif // FORMATSTRING_FORMATTER_H
