#ifndef FORMATSTRING_FORMAT_TRAITS_H
#define FORMATSTRING_FORMAT_TRAITS_H
#pragma once

#include "formatstring/config.h"
#include "formatstring/formatter.h"
#include "formatstring/format_traits_fwd.h"

#include <type_traits>
#include <vector>
#include <list>
#include <array>
#include <tuple>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace formatstring {

    template<typename Derived, typename Base>
    using if_derived = typename std::enable_if<std::is_base_of<Base, Derived>::value>::type;

    // ---- fallback ----
    template<typename Char, typename T, typename ENABLE>
    struct format_traits {
        typedef Char char_type;
        typedef T value_type;

        static inline BasicFormatter<Char> make_formatter(const T& value) {
            return make_fallback_formatter<Char,T>(&value);
        }
    };

    // ---- special characters ----
    template<typename Char>
    struct format_traits<Char, bool> {
        typedef Char char_type;
        typedef bool value_type;

        static inline BasicFormatter<Char> make_formatter(bool value) {
            return make_value_formatter<Char,bool>(value);
        }
    };

#ifdef FORMATSTRING_CHAR16_SUPPORT
    template<>
    struct format_traits<char16_t, char16_t> {
        typedef char16_t char_type;
        typedef char16_t value_type;

        static inline BasicFormatter<Char> make_formatter(char16_t value) {
            return make_value_formatter<char16_t,char16_t,format_char<char16_t,char16_t>,repr_char<char16_t>>(value);
        }
    };
#endif


#ifdef FORMATSTRING_CHAR32_SUPPORT
    template<>
    struct format_traits<char32_t, char32_t> {
        typedef char32_t char_type;
        typedef char32_t value_type;

        static inline BasicFormatter<Char> make_formatter(char32_t value) {
            return make_value_formatter<char32_t,char32_t,format_char<char32_t,char32_t>,repr_char<char32_t>>(value);
        }
    };
#endif

    template<>
    struct format_traits<wchar_t, wchar_t> {
        typedef wchar_t char_type;
        typedef wchar_t value_type;

        static inline BasicFormatter<wchar_t> make_formatter(wchar_t value) {
            return make_value_formatter<wchar_t,wchar_t,format_char<wchar_t,wchar_t>,repr_char<wchar_t>>(value);
        }
    };

    // ---- integers ----
    template<typename Char>
    struct format_traits<Char, char> {
        typedef Char char_type;
        typedef char value_type;

        static inline BasicFormatter<Char> make_formatter(char value) {
            return make_value_formatter<Char,char,format_char<char,char>,repr_char<char>>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, signed char> {
        typedef Char char_type;
        typedef signed char value_type;

        static inline BasicFormatter<Char> make_formatter(signed char value) {
            return make_value_formatter<Char,signed char,format_char<char,signed char>,repr_int_char<char,signed char>>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, short> {
        typedef Char char_type;
        typedef short value_type;

        static inline BasicFormatter<Char> make_formatter(short value) {
            return make_value_formatter<Char,short,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, int> {
        typedef Char char_type;
        typedef int value_type;

        static inline BasicFormatter<Char> make_formatter(int value) {
            return make_value_formatter<Char,int,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, long> {
        typedef Char char_type;
        typedef long value_type;

        static inline BasicFormatter<Char> make_formatter(long value) {
            return make_value_formatter<Char,long,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, long long> {
        typedef Char char_type;
        typedef long long value_type;

        static inline BasicFormatter<Char> make_formatter(long long value) {
            return make_value_formatter<Char,long long,format_integer>(value);
        }
    };

    // ---- unsigned integers ----
    template<typename Char>
    struct format_traits<Char, unsigned char> {
        typedef Char char_type;
        typedef unsigned char value_type;

        static inline BasicFormatter<Char> make_formatter(unsigned char value) {
            return make_value_formatter<Char,unsigned char,format_char<char,unsigned char>,repr_int_char<char,unsigned char>>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, unsigned short> {
        typedef Char char_type;
        typedef unsigned short value_type;

        static inline BasicFormatter<Char> make_formatter(unsigned short value) {
            return make_value_formatter<Char,unsigned short,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, unsigned int> {
        typedef Char char_type;
        typedef unsigned int value_type;

        static inline BasicFormatter<Char> make_formatter(unsigned int value) {
            return make_value_formatter<Char,unsigned int,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, unsigned long> {
        typedef Char char_type;
        typedef unsigned long value_type;

        static inline BasicFormatter<Char> make_formatter(unsigned long value) {
            return make_value_formatter<Char,unsigned long,format_integer>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, unsigned long long> {
        typedef Char char_type;
        typedef unsigned long long value_type;

        static inline BasicFormatter<Char> make_formatter(unsigned long long value) {
            return make_value_formatter<Char,unsigned long long,format_integer>(value);
        }
    };

    // ---- floating point ----
    template<typename Char>
    struct format_traits<Char, float> {
        typedef Char char_type;
        typedef float value_type;

        static inline BasicFormatter<Char> make_formatter(float value) {
            return make_value_formatter<Char,float,format_float>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, double> {
        typedef Char char_type;
        typedef double value_type;

        static inline BasicFormatter<Char> make_formatter(double value) {
            return make_value_formatter<Char,double,format_float>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, long double> {
        typedef Char char_type;
        typedef long double value_type;

        static inline BasicFormatter<Char> make_formatter(long double value) {
            return make_value_formatter<Char,long double,format_float>(value);
        }
    };

    // ---- string ----
    template<typename Char>
    struct format_traits<Char, const Char[]> {
        typedef Char char_type;
        typedef const Char value_type[];

        static inline BasicFormatter<Char> make_formatter(const Char value[]) {
            return make_value_formatter<Char,const Char*>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, Char[]> {
        typedef Char char_type;
        typedef Char value_type[];

        static inline BasicFormatter<Char> make_formatter(const Char value[]) {
            return make_value_formatter<Char,const Char*>(value);
        }
    };

    template<typename Char, std::size_t N>
    struct format_traits<Char, const Char[N]> {
        typedef Char char_type;
        typedef const Char value_type[N];

        static inline BasicFormatter<Char> make_formatter(const Char value[]) {
            return make_value_formatter<Char,const Char*>(value);
        }
    };

    template<typename Char, std::size_t N>
    struct format_traits<Char, Char[N]> {
        typedef Char char_type;
        typedef Char value_type[N];

        static inline BasicFormatter<Char> make_formatter(const Char value[]) {
            return make_value_formatter<Char,const Char*>(value);
        }
    };

    template<typename Char>
    struct format_traits<Char, const Char*> {
        typedef Char char_type;
        typedef const Char* value_type;

        static inline BasicFormatter<Char> make_formatter(const Char* value) {
            return make_value_formatter<Char,const Char*>(value);
        }
    };

    template<typename Char>
    struct format_traits< Char, std::basic_string<Char> > {
        typedef Char char_type;
        typedef std::basic_string<Char> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_ptr_formatter<Char,value_type>(&value);
        }
    };

    // ---- array ----
    template<typename Char, typename T, std::size_t N>
    struct format_traits<Char, const T[N]> {
        typedef Char char_type;
        typedef const T value_type[N];

        static inline BasicFormatter<Char> make_formatter(const T value[]) {
            return make_slice_formatter<Char,const T*>(value, value + N);
        }
    };

    template<typename Char, typename T, std::size_t N>
    struct format_traits<Char, T[N]> {
        typedef Char char_type;
        typedef T value_type[N];

        static inline BasicFormatter<Char> make_formatter(const T value[]) {
            return make_slice_formatter<Char,const T*>(value, value + N);
        }
    };

    // ---- standard containers ----
    template<typename Char, typename T>
    struct format_traits< Char, std::vector<T> > {
        typedef Char char_type;
        typedef std::vector<T> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_slice_formatter<Char,typename value_type::const_iterator>(value.begin(), value.end());
        }
    };

    template<typename Char, typename T>
    struct format_traits< Char, std::list<T> > {
        typedef Char char_type;
        typedef std::list<T> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_slice_formatter<Char,typename value_type::const_iterator>(value.begin(), value.end());
        }
    };

    template<typename Char, typename T, std::size_t N>
    struct format_traits< Char, std::array<T,N> > {
        typedef Char char_type;
        typedef std::array<T,N> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_slice_formatter<Char,typename value_type::const_iterator>(value.begin(), value.end());
        }
    };

    template<typename Char, typename T>
    struct format_traits< Char, std::set<T> > {
        typedef Char char_type;
        typedef std::set<T> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_slice_formatter<Char,typename value_type::const_iterator,'{','}'>(value.begin(), value.end());
        }
    };

    template<typename Char, typename T>
    struct format_traits< Char, std::unordered_set<T> > {
        typedef Char char_type;
        typedef std::unordered_set<T> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_slice_formatter<Char,typename value_type::const_iterator,'{','}'>(value.begin(), value.end());
        }
    };

    template<typename Char, typename... Args>
    struct format_traits< Char, std::tuple<Args...> > {
        typedef Char char_type;
        typedef std::tuple<Args...> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_ptr_formatter<Char,value_type>(&value);
        }
    };

    template<typename Char, typename First, typename Second>
    struct format_traits< Char, std::pair<First,Second> > {
        typedef Char char_type;
        typedef std::pair<First,Second> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_ptr_formatter<Char,value_type>(&value);
        }
    };

    template<typename Char, typename K, typename V>
    struct format_traits< Char, std::map<K,V> > {
        typedef Char char_type;
        typedef std::map<K,V> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_slice_formatter<Char,typename value_type::const_iterator,'{','}',format_map,repr_map>(value.begin(), value.end());
        }
    };

    template<typename Char, typename K, typename V>
    struct format_traits< Char, std::unordered_map<K,V> > {
        typedef Char char_type;
        typedef std::unordered_map<K,V> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_slice_formatter<Char,typename value_type::const_iterator,'{','}',format_map,repr_map>(value.begin(), value.end());
        }
    };

    template<typename Char, typename T>
    struct format_traits< Char, std::initializer_list<T> > {
        typedef Char char_type;
        typedef std::initializer_list<T> value_type;

        static inline BasicFormatter<Char> make_formatter(const value_type& value) {
            return make_slice_formatter<Char,typename value_type::const_iterator>(value.begin(), value.end());
        }
    };
}

#endif // FORMAT_TRAITS_H
