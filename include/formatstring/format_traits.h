#ifndef FORMATSTRING_FORMAT_TRAITS_H
#define FORMATSTRING_FORMAT_TRAITS_H

#include "formatstring/formatter.h"

namespace formatstring {

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

}

#endif // FORMAT_TRAITS_H
