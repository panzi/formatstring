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

        PtrFormatter(ptr_type ptr) : ptr(ptr) {}

        virtual void format(std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const {
            switch (conv) {
            case ReprConv:
            {
                std::basic_ostringstream<Char> buffer;
                _repr(buffer, *ptr);
                format_value(out, buffer.str(), spec);
                break;
            }
            case StrConv:
            {
                std::basic_ostringstream<Char> buffer;
                _format(buffer, *ptr, BasicFormatSpec<Char>::DEFAULT);
                format_value(out, buffer.str(), spec);
                break;
            }
            default:
                _format(out, *ptr, spec);
                break;
            }
        }

    private:
        ptr_type ptr;
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

    template<typename Char, typename Iter, Char left = '[', Char right = ']',
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

    // ==== format_traits ====

    // ---- fallback ----
    template<typename Char, typename T>
    struct format_traits;

    template<typename Char, typename T>
    struct format_traits {
        typedef Char char_type;
        typedef T value_type;

        static inline FallbackFormatter<Char,T>* make_formatter(const T& value) {
            return new FallbackFormatter<Char,T>(&value);
        }
    };

    // ---- special characters ----
    template<typename Char>
    struct format_traits<Char, bool> {
        typedef Char char_type;
        typedef bool value_type;

        static inline ValueFormatter<Char,bool>* make_formatter(bool value) {
            return new ValueFormatter<Char,bool>(value);
        }
    };

    template<>
    struct format_traits<char16_t, char16_t> {
        typedef char16_t char_type;
        typedef char16_t value_type;

        static inline ValueFormatter<char16_t,char16_t,format_char,repr_char>* make_formatter(char16_t value) {
            return new ValueFormatter<char16_t,char16_t,format_char,repr_char>(value);
        }
    };

    template<>
    struct format_traits<char32_t, char32_t> {
        typedef char32_t char_type;
        typedef char32_t value_type;

        static inline ValueFormatter<char32_t,char32_t,format_char,repr_char>* make_formatter(char32_t value) {
            return new ValueFormatter<char32_t,char32_t,format_char,repr_char>(value);
        }
    };

    template<>
    struct format_traits<wchar_t, wchar_t> {
        typedef wchar_t char_type;
        typedef wchar_t value_type;

        static inline ValueFormatter<wchar_t,wchar_t,format_char,repr_char>* make_formatter(wchar_t value) {
            return new ValueFormatter<wchar_t,wchar_t,format_char,repr_char>(value);
        }
    };

    // ---- integers ----
    template<typename Char>
    struct format_traits<Char, char> {
        typedef Char char_type;
        typedef char value_type;

        static inline ValueFormatter<Char,char,format_char,repr_char>* make_formatter(char value) {
            return new ValueFormatter<Char,char,format_char,repr_char>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, short> {
        typedef Char char_type;
        typedef short value_type;

        static inline ValueFormatter<Char,short,format_integer>* make_formatter(short value) {
            return new ValueFormatter<Char,short,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, int> {
        typedef Char char_type;
        typedef int value_type;

        static inline ValueFormatter<Char,int,format_integer>* make_formatter(int value) {
            return new ValueFormatter<Char,int,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, long> {
        typedef Char char_type;
        typedef long value_type;

        static inline ValueFormatter<Char,long,format_integer>* make_formatter(long value) {
            return new ValueFormatter<Char,long,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, long long> {
        typedef Char char_type;
        typedef long long value_type;

        static inline ValueFormatter<Char,long long,format_integer>* make_formatter(long long value) {
            return new ValueFormatter<Char,long long,format_integer>(value);
        }
    };

    // ---- unsigned integers ----
    template<typename Char>
    struct format_traits<Char, unsigned char> {
        typedef Char char_type;
        typedef unsigned char value_type;

        static inline ValueFormatter<Char,unsigned char,format_integer>* make_formatter(unsigned char value) {
            return new ValueFormatter<Char,unsigned char,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, unsigned short> {
        typedef Char char_type;
        typedef unsigned short value_type;

        static inline ValueFormatter<Char,unsigned short,format_integer>* make_formatter(unsigned short value) {
            return new ValueFormatter<Char,unsigned short,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, unsigned int> {
        typedef Char char_type;
        typedef unsigned int value_type;

        static inline ValueFormatter<Char,unsigned int,format_integer>* make_formatter(unsigned int value) {
            return new ValueFormatter<Char,unsigned int,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, unsigned long> {
        typedef Char char_type;
        typedef unsigned long value_type;

        static inline ValueFormatter<Char,unsigned long,format_integer>* make_formatter(unsigned long value) {
            return new ValueFormatter<Char,unsigned long,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, unsigned long long> {
        typedef Char char_type;
        typedef unsigned long long value_type;

        static inline ValueFormatter<Char,unsigned long long,format_integer>* make_formatter(unsigned long long value) {
            return new ValueFormatter<Char,unsigned long long,format_integer>(value);
        }
    };

    // ---- floating point ----
    template<typename Char>
    struct format_traits<Char, float> {
        typedef Char char_type;
        typedef float value_type;

        static inline ValueFormatter<Char,float,format_float>* make_formatter(float value) {
            return new ValueFormatter<Char,float,format_float>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, double> {
        typedef Char char_type;
        typedef double value_type;

        static inline ValueFormatter<Char,double,format_float>* make_formatter(double value) {
            return new ValueFormatter<Char,double,format_float>(value);
        }
    };

    // ---- string ----
    template<typename Char>
    struct format_traits<Char, const Char[]> {
        typedef Char char_type;
        typedef const Char value_type[];

        static inline ValueFormatter<Char,const Char*>* make_formatter(const Char value[]) {
            return new ValueFormatter<Char,const Char*>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, Char[]> {
        typedef Char char_type;
        typedef Char value_type[];

        static inline ValueFormatter<Char,const Char*>* make_formatter(const Char value[]) {
            return new ValueFormatter<Char,const Char*>(value);
        }
    };

    template<typename Char, std::size_t N>
    struct format_traits<Char, const Char[N]> {
        typedef Char char_type;
        typedef const Char value_type[N];

        static inline ValueFormatter<Char,const Char*>* make_formatter(const Char value[]) {
            return new ValueFormatter<Char,const Char*>(value);
        }
    };

    template<typename Char, std::size_t N>
    struct format_traits<Char, Char[N]> {
        typedef Char char_type;
        typedef Char value_type[N];

        static inline ValueFormatter<Char,const Char*>* make_formatter(const Char value[]) {
            return new ValueFormatter<Char,const Char*>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, const Char*> {
        typedef Char char_type;
        typedef const Char* value_type;

        static inline ValueFormatter<Char,const Char*>* make_formatter(const Char* value) {
            return new ValueFormatter<Char,const Char*>(value);
        }
    };

    template<typename Char>
    struct format_traits< Char, std::basic_string<Char> > {
        typedef Char char_type;
        typedef std::basic_string<Char> value_type;

        static inline PtrFormatter<Char,value_type>* make_formatter(const value_type& value) {
            return new PtrFormatter<Char,value_type>(&value);
        }
    };

    // ---- array ----
    template<typename Char, typename T, std::size_t N>
    struct format_traits<Char, const T[N]> {
        typedef Char char_type;
        typedef const T value_type[N];

        static inline SliceFormatter<Char,const T*>* make_formatter(const T value[]) {
            return new SliceFormatter<Char,const T*>(value, value + N);
        }
    };

    template<typename Char, typename T, std::size_t N>
    struct format_traits<Char, T[N]> {
        typedef Char char_type;
        typedef T value_type[N];

        static inline SliceFormatter<Char,const T*>* make_formatter(const T value[]) {
            return new SliceFormatter<Char,const T*>(value, value + N);
        }
    };

    // ---- standard containers ----
    template<typename Char, typename T>
    struct format_traits< Char, std::vector<T> > {
        typedef Char char_type;
        typedef std::vector<T> value_type;

        static inline SliceFormatter<Char,typename value_type::const_iterator>* make_formatter(const value_type& value) {
            return new SliceFormatter<Char,typename value_type::const_iterator>(value.begin(), value.end());
        }
    };

    template<typename Char, typename T>
    struct format_traits< Char, std::list<T> > {
        typedef Char char_type;
        typedef std::list<T> value_type;

        static inline SliceFormatter<Char,typename value_type::const_iterator>* make_formatter(const value_type& value) {
            return new SliceFormatter<Char,typename value_type::const_iterator>(value.begin(), value.end());
        }
    };

    template<typename Char, typename T, std::size_t N>
    struct format_traits< Char, std::array<T,N> > {
        typedef Char char_type;
        typedef std::array<T,N> value_type;

        static inline SliceFormatter<Char,typename value_type::const_iterator>* make_formatter(const value_type& value) {
            return new SliceFormatter<Char,typename value_type::const_iterator>(value.begin(), value.end());
        }
    };

    template<typename Char, typename T>
    struct format_traits< Char, std::set<T> > {
        typedef Char char_type;
        typedef std::set<T> value_type;

        static inline SliceFormatter<Char,typename value_type::const_iterator,'{','}'>* make_formatter(const value_type& value) {
            return new SliceFormatter<Char,typename value_type::const_iterator,'{','}'>(value.begin(), value.end());
        }
    };

    template<typename Char, typename T>
    struct format_traits< Char, std::unordered_set<T> > {
        typedef Char char_type;
        typedef std::unordered_set<T> value_type;

        static inline SliceFormatter<Char,typename value_type::const_iterator,'{','}'>* make_formatter(const value_type& value) {
            return new SliceFormatter<Char,typename value_type::const_iterator,'{','}'>(value.begin(), value.end());
        }
    };

    template<typename Char, typename... Args>
    struct format_traits< Char, std::tuple<Args...> > {
        typedef Char char_type;
        typedef std::tuple<Args...> value_type;

        static inline PtrFormatter<Char,value_type>* make_formatter(const value_type& value) {
            return new PtrFormatter<Char,value_type>(&value);
        }
    };

    template<typename Char, typename First, typename Second>
    struct format_traits< Char, std::pair<First,Second> > {
        typedef Char char_type;
        typedef std::pair<First,Second> value_type;

        static inline PtrFormatter<Char,value_type>* make_formatter(const value_type& value) {
            return new PtrFormatter<Char,value_type>(&value);
        }
    };

    template<typename Char, typename K, typename V>
    struct format_traits< Char, std::map<K,V> > {
        typedef Char char_type;
        typedef std::map<K,V> value_type;

        static inline SliceFormatter<Char,typename value_type::const_iterator,'{','}',format_map,repr_map>* make_formatter(const value_type& value) {
            return new SliceFormatter<Char,typename value_type::const_iterator,'{','}',format_map,repr_map>(value.begin(), value.end());
        }
    };

    template<typename Char, typename K, typename V>
    struct format_traits< Char, std::unordered_map<K,V> > {
        typedef Char char_type;
        typedef std::unordered_map<K,V> value_type;

        static inline SliceFormatter<Char,typename value_type::const_iterator,'{','}',format_map,repr_map>* make_formatter(const value_type& value) {
            return new SliceFormatter<Char,typename value_type::const_iterator,'{','}',format_map,repr_map>(value.begin(), value.end());
        }
    };

    // ==== unpack_formatters ====
    template<typename Char, typename First, typename... Rest>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const First& first, const Rest&... rest) {
        formatters.emplace_back(format_traits<Char,First>::make_formatter(first));
        unpack_formatters<Char, Rest...>(formatters, rest...);
    }

    template<typename Char, typename Last>
    inline void unpack_formatters(typename BasicFormatter<Char>::List& formatters, const Last& last) {
        formatters.emplace_back(format_traits<Char,Last>::make_formatter(last));
    }
}

#endif // FORMATSTRING_FORMATTER_H
