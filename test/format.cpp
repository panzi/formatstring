#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <cstring>
#include <cstdint>
#include <cctype>

#include <formatstring.h>

using namespace formatstring;

class Character {
public:
    inline Character() : value(0) {}
    inline Character(char value) : value(value) {}
    inline Character(const Character& other) = default;
    inline Character(Character&& other) = default;

    inline Character& operator=(const Character& other) = default;

    inline Character& operator=(char value) {
        this->value = value;
        return *this;
    }

    inline operator char () const {
        return value;
    }

    inline operator int () const {
        return value;
    }

private:
    char value;
};

class UCharacter {
public:
    inline UCharacter() : value(0) {}
    inline UCharacter(unsigned char value) : value(value) {}
    inline UCharacter(const UCharacter& other) = default;
    inline UCharacter(UCharacter&& other) = default;

    inline UCharacter& operator=(const UCharacter& other) = default;

    inline UCharacter& operator=(unsigned char value) {
        this->value = value;
        return *this;
    }

    inline operator unsigned char () const {
        return value;
    }

    inline operator int () const {
        return value;
    }

private:
    unsigned char value;
};

class SCharacter {
public:
    inline SCharacter() : value(0) {}
    inline SCharacter(signed char value) : value(value) {}
    inline SCharacter(const SCharacter& other) = default;
    inline SCharacter(SCharacter&& other) = default;

    inline SCharacter& operator=(const SCharacter& other) = default;

    inline SCharacter& operator=(signed char value) {
        this->value = value;
        return *this;
    }

    inline operator signed char () const {
        return value;
    }

    inline operator int () const {
        return value;
    }

private:
    signed char value;
};

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

template<typename T>
T lexical_cast(const char* str) {
    std::istringstream ss(str);
    T val = T();
    ss >> val;
    if (ss.fail() || !ss.eof()) {
        throw std::invalid_argument(str);
    }
    return val;
}

template<typename Float>
Float float_lexical_cast(std::string str) {
    std::locale loc;
    trim(str);
    std::transform(str.begin(), str.end(), str.begin(), [&loc](char c){ return std::tolower(c, loc); });
    if (str == "inf" || str == "+inf") {
        return INFINITY;
    }
    else if (str == "-inf") {
        return -INFINITY;
    }
    else if (str == "nan" || str == "+nan") {
        return NAN;
    }
    else if (str == "-nan") {
        return -NAN;
    }
    else {
        std::istringstream ss(str);
        Float val = 0;
        ss >> val;
        if (ss.fail() || !ss.eof()) {
            throw std::invalid_argument(str);
        }
        return val;
    }
}

template<>
inline float lexical_cast(const char* str) {
    return float_lexical_cast<float>(str);
}

template<>
inline double lexical_cast(const char* str) {
    return float_lexical_cast<double>(str);
}

template<>
inline long double lexical_cast(const char* str) {
    return float_lexical_cast<long double>(str);
}

template<>
inline const char* lexical_cast(const char* str) {
    return str;
}

template<>
std::int8_t lexical_cast(const char* str) {
    std::istringstream ss(str);
    int val = 0;
    ss >> val;
    if (ss.fail() || !ss.eof() || val < -128 || val > 127) {
        throw std::invalid_argument(str);
    }
    return val;
}

template<>
std::uint8_t lexical_cast(const char* str) {
    std::istringstream ss(str);
    unsigned int val = 0;
    ss >> val;
    if (ss.fail() || !ss.eof() || val > 255) {
        throw std::invalid_argument(str);
    }
    return val;
}

template<>
Character lexical_cast(const char* str) {
    if (std::strlen(str) > 1) {
        throw std::invalid_argument(str);
    }
    return str[0];
}

template<>
UCharacter lexical_cast(const char* str) {
    if (std::strlen(str) > 1) {
        throw std::invalid_argument(str);
    }
    return (unsigned char)str[0];
}

template<>
SCharacter lexical_cast(const char* str) {
    if (std::strlen(str) > 1) {
        throw std::invalid_argument(str);
    }
    return (signed char)str[0];
}

template<>
bool lexical_cast(const char* str) {
    std::string val = str;
    std::transform(val.begin(), val.end(), val.begin(), [](char ch) { return std::tolower(ch); });
    if (val == "true" || val == "1" || val == "t" || val == "on" || val == "yes") {
        return true;
    }
    else if (val == "false" || val == "0" || val == "f" || val == "off" || val == "no") {
        return false;
    }
    else {
        throw std::invalid_argument(str);
    }
}

template<>
std::string lexical_cast(const char* str) {
    return str;
}

template<typename T>
void format_vector(const char* fmt, const std::size_t n, const char* values[]) {
    std::vector<T> vec;
    for (std::size_t i = 0; i < n; ++ i) {
        vec.push_back(lexical_cast<T>(values[i]));
    }
    std::cout << format(fmt, vec);
}

enum ValueType {
    Bool,
    Char,
    Short,
    Int,
    Long,
    LongLong,
    SChar,
    UChar,
    UShort,
    UInt,
    ULong,
    ULongLong,
    Float,
    Double,
    LongDouble,
    Int8,
    Int16,
    Int32,
    Int64,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    String
};

ValueType parse_value_type(const std::string& type) {
    if (type == "bool") {
        return Bool;
    }
    else if (type == "char") {
        return Char;
    }
    else if (type == "short") {
        return Short;
    }
    else if (type == "int") {
        return Int;
    }
    else if (type == "long") {
        return Long;
    }
    else if (type == "long long") {
        return LongLong;
    }
    else if (type == "signed char") {
        return SChar;
    }
    else if (type == "unsigned char") {
        return UChar;
    }
    else if (type == "unsigned short") {
        return UShort;
    }
    else if (type == "unsigned int") {
        return UInt;
    }
    else if (type == "unsigned long") {
        return ULong;
    }
    else if (type == "unsigned long long") {
        return ULongLong;
    }
    else if (type == "float") {
        return Float;
    }
    else if (type == "double") {
        return Double;
    }
    else if (type == "long double") {
        return LongDouble;
    }
    else if (type == "int8_t") {
        return Int8;
    }
    else if (type == "int16_t") {
        return Int16;
    }
    else if (type == "int32_t") {
        return Int32;
    }
    else if (type == "int64_t") {
        return Int64;
    }
    else if (type == "uint8_t") {
        return UInt8;
    }
    else if (type == "uint16_t") {
        return UInt16;
    }
    else if (type == "uint32_t") {
        return UInt32;
    }
    else if (type == "uint64_t") {
        return UInt64;
    }
    else if (type == "string") {
        return String;
    }
    else {
        throw std::invalid_argument(type);
    }
}

template<typename ParseType, typename UseType = ParseType>
void do_format_value(const char* fmt, const char* value) {
    std::cout << format(fmt, (UseType)lexical_cast<ParseType>(value));
}

template<typename Collection, typename T>
void do_format_collection(const char* fmt, std::size_t argc, const char* argv[]) {
    Collection values;
    for (std::size_t i = 0; i < argc; ++ i) {
        values.push_back(lexical_cast<T>(argv[i]));
    }
    std::cout << format(fmt, values);
}

template<typename Set, typename T>
void do_format_set(const char* fmt, std::size_t argc, const char* argv[]) {
    Set values;
    for (std::size_t i = 0; i < argc; ++ i) {
        values.emplace(lexical_cast<T>(argv[i]));
    }
    std::cout << format(fmt, values);
}

template<typename Map, typename K, typename V>
void do_format_map(const char* fmt, std::size_t argc, const char* argv[]) {
    if (argc % 2 != 0) {
        throw std::range_error("illegal (non-even) number of arguments");
    }
    Map values;
    for (std::size_t i = 0; i < argc; i += 2) {
        values[lexical_cast<K>(argv[i])] = lexical_cast<V>(argv[i + 1]);
    }
    std::cout << format(fmt, values);
}

template<typename ParseFirst, typename ParseSecond, typename First = ParseFirst, typename Second = ParseSecond>
void do_format_pair(const char* fmt, std::size_t argc, const char* argv[]) {
    if (argc != 2) {
        throw std::range_error("illegal number of arguments");
    }
    First  a = lexical_cast<ParseFirst>(argv[0]);
    Second b = lexical_cast<ParseSecond>(argv[1]);
    std::cout << format(fmt, std::make_pair<First,Second>(a, b));
}

template<typename ParseType, typename UseType = ParseType>
void do_format_array(const char* fmt, std::size_t argc, const char* argv[]) {
    UseType* array = new UseType[argc];
    try {
        for (std::size_t i = 0; i < argc; ++ i) {
            array[i] = lexical_cast<ParseType>(argv[i]);
        }
        std::cout << format(fmt, slice(array, array + argc));
        delete[] array;
        array = 0;
    }
    catch (...) {
        delete[] array;
        throw;
    }
}

#define DO_FORMAT_COLLECTION(FUNC,COLLECTION_TYPE,PARAMS,FMT,ARGC,ARGV) \
    if ((PARAMS).size() != 1) { \
        throw std::range_error("illegal number of type parameters"); \
    } \
    switch (parse_value_type((PARAMS)[0])) { \
    case Bool:       FUNC<COLLECTION_TYPE<bool>, bool>((FMT), (ARGC), (ARGV)); break; \
    case Char:       FUNC<COLLECTION_TYPE<char>, Character>((FMT), (ARGC), (ARGV)); break; \
    case Short:      FUNC<COLLECTION_TYPE<short>, short>((FMT), (ARGC), (ARGV)); break; \
    case Int:        FUNC<COLLECTION_TYPE<int>, int>((FMT), (ARGC), (ARGV)); break; \
    case Long:       FUNC<COLLECTION_TYPE<long>, long>((FMT), (ARGC), (ARGV)); break; \
    case LongLong:   FUNC<COLLECTION_TYPE<long long>, long long>((FMT), (ARGC), (ARGV)); break; \
    case SChar:      FUNC<COLLECTION_TYPE<signed char>, SCharacter>((FMT), (ARGC), (ARGV)); break; \
    case UChar:      FUNC<COLLECTION_TYPE<unsigned char>, UCharacter>((FMT), (ARGC), (ARGV)); break; \
    case UShort:     FUNC<COLLECTION_TYPE<unsigned short>, unsigned short>((FMT), (ARGC), (ARGV)); break; \
    case UInt:       FUNC<COLLECTION_TYPE<unsigned int>, unsigned int>((FMT), (ARGC), (ARGV)); break; \
    case ULong:      FUNC<COLLECTION_TYPE<unsigned long>, unsigned long>((FMT), (ARGC), (ARGV)); break; \
    case ULongLong:  FUNC<COLLECTION_TYPE<unsigned long long>, unsigned long long>((FMT), (ARGC), (ARGV)); break; \
    case Float:      FUNC<COLLECTION_TYPE<float>, float>((FMT), (ARGC), (ARGV)); break; \
    case Double:     FUNC<COLLECTION_TYPE<double>, double>((FMT), (ARGC), (ARGV)); break; \
    case LongDouble: FUNC<COLLECTION_TYPE<long double>, long double>((FMT), (ARGC), (ARGV)); break; \
    case Int8:       FUNC<COLLECTION_TYPE<std::int8_t>, std::int8_t>((FMT), (ARGC), (ARGV)); break; \
    case Int16:      FUNC<COLLECTION_TYPE<std::int16_t>, std::int16_t>((FMT), (ARGC), (ARGV)); break; \
    case Int32:      FUNC<COLLECTION_TYPE<std::int32_t>, std::int32_t>((FMT), (ARGC), (ARGV)); break; \
    case Int64:      FUNC<COLLECTION_TYPE<std::int64_t>, std::int64_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt8:      FUNC<COLLECTION_TYPE<std::uint8_t>, std::uint8_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt16:     FUNC<COLLECTION_TYPE<std::uint16_t>, std::uint16_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt32:     FUNC<COLLECTION_TYPE<std::uint32_t>, std::uint32_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt64:     FUNC<COLLECTION_TYPE<std::uint64_t>, std::uint64_t>((FMT), (ARGC), (ARGV)); break; \
    case String:     FUNC<COLLECTION_TYPE<std::string>, std::string>((FMT), (ARGC), (ARGV)); break; \
    default: throw std::runtime_error("unhandeled type"); \
    }

#define _DO_FORMAT_MAP(MAP_TYPE,KEY_PARSE,KEY_TYPE,PARAMS,FMT,ARGC,ARGV) \
    switch (parse_value_type((PARAMS)[1])) { \
    case Bool:       do_format_map<MAP_TYPE<KEY_TYPE, bool>, KEY_PARSE, bool>((FMT), (ARGC), (ARGV)); break; \
    case Char:       do_format_map<MAP_TYPE<KEY_TYPE, char>, KEY_PARSE, Character>((FMT), (ARGC), (ARGV)); break; \
    case Short:      do_format_map<MAP_TYPE<KEY_TYPE, short>, KEY_PARSE, short>((FMT), (ARGC), (ARGV)); break; \
    case Int:        do_format_map<MAP_TYPE<KEY_TYPE, int>, KEY_PARSE, int>((FMT), (ARGC), (ARGV)); break; \
    case Long:       do_format_map<MAP_TYPE<KEY_TYPE, long>, KEY_PARSE, long>((FMT), (ARGC), (ARGV)); break; \
    case LongLong:   do_format_map<MAP_TYPE<KEY_TYPE, long long>, KEY_PARSE, long long>((FMT), (ARGC), (ARGV)); break; \
    case SChar:      do_format_map<MAP_TYPE<KEY_TYPE, signed char>, KEY_PARSE, SCharacter>((FMT), (ARGC), (ARGV)); break; \
    case UChar:      do_format_map<MAP_TYPE<KEY_TYPE, unsigned char>, KEY_PARSE, UCharacter>((FMT), (ARGC), (ARGV)); break; \
    case UShort:     do_format_map<MAP_TYPE<KEY_TYPE, unsigned short>, KEY_PARSE, unsigned short>((FMT), (ARGC), (ARGV)); break; \
    case UInt:       do_format_map<MAP_TYPE<KEY_TYPE, unsigned int>, KEY_PARSE, unsigned int>((FMT), (ARGC), (ARGV)); break; \
    case ULong:      do_format_map<MAP_TYPE<KEY_TYPE, unsigned long>, KEY_PARSE, unsigned long>((FMT), (ARGC), (ARGV)); break; \
    case ULongLong:  do_format_map<MAP_TYPE<KEY_TYPE, unsigned long long>, KEY_PARSE, unsigned long long>((FMT), (ARGC), (ARGV)); break; \
    case Float:      do_format_map<MAP_TYPE<KEY_TYPE, float>, KEY_PARSE, float>((FMT), (ARGC), (ARGV)); break; \
    case Double:     do_format_map<MAP_TYPE<KEY_TYPE, double>, KEY_PARSE, double>((FMT), (ARGC), (ARGV)); break; \
    case LongDouble: do_format_map<MAP_TYPE<KEY_TYPE, long double>, KEY_PARSE, long double>((FMT), (ARGC), (ARGV)); break; \
    case Int8:       do_format_map<MAP_TYPE<KEY_TYPE, std::int8_t>, KEY_PARSE, std::int8_t>((FMT), (ARGC), (ARGV)); break; \
    case Int16:      do_format_map<MAP_TYPE<KEY_TYPE, std::int16_t>, KEY_PARSE, std::int16_t>((FMT), (ARGC), (ARGV)); break; \
    case Int32:      do_format_map<MAP_TYPE<KEY_TYPE, std::int32_t>, KEY_PARSE, std::int32_t>((FMT), (ARGC), (ARGV)); break; \
    case Int64:      do_format_map<MAP_TYPE<KEY_TYPE, std::int64_t>, KEY_PARSE, std::int64_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt8:      do_format_map<MAP_TYPE<KEY_TYPE, std::uint8_t>, KEY_PARSE, std::uint8_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt16:     do_format_map<MAP_TYPE<KEY_TYPE, std::uint16_t>, KEY_PARSE, std::uint16_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt32:     do_format_map<MAP_TYPE<KEY_TYPE, std::uint32_t>, KEY_PARSE, std::uint32_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt64:     do_format_map<MAP_TYPE<KEY_TYPE, std::uint64_t>, KEY_PARSE, std::uint64_t>((FMT), (ARGC), (ARGV)); break; \
    case String:     do_format_map<MAP_TYPE<KEY_TYPE, std::string>, KEY_PARSE, std::string>((FMT), (ARGC), (ARGV)); break; \
    default: throw std::runtime_error("unhandeled type"); \
    }

#define DO_FORMAT_MAP(MAP_TYPE,PARAMS,FMT,ARGC,ARGV) \
    if ((PARAMS).size() != 2) { \
        throw std::range_error("illegal number of type parameters"); \
    } \
    switch (parse_value_type((PARAMS)[0])) { \
    case Bool:       _DO_FORMAT_MAP(MAP_TYPE,bool,bool,PARAMS,FMT,ARGC,ARGV); break; \
    case Char:       _DO_FORMAT_MAP(MAP_TYPE,Character,char,PARAMS,FMT,ARGC,ARGV); break; \
    case Short:      _DO_FORMAT_MAP(MAP_TYPE,short,short,PARAMS,FMT,ARGC,ARGV); break; \
    case Int:        _DO_FORMAT_MAP(MAP_TYPE,int,int,PARAMS,FMT,ARGC,ARGV); break; \
    case Long:       _DO_FORMAT_MAP(MAP_TYPE,long,long,PARAMS,FMT,ARGC,ARGV); break; \
    case LongLong:   _DO_FORMAT_MAP(MAP_TYPE,long long,long long,PARAMS,FMT,ARGC,ARGV); break; \
    case SChar:      _DO_FORMAT_MAP(MAP_TYPE,SCharacter,signed char,PARAMS,FMT,ARGC,ARGV); break; \
    case UChar:      _DO_FORMAT_MAP(MAP_TYPE,UCharacter,unsigned char,PARAMS,FMT,ARGC,ARGV); break; \
    case UShort:     _DO_FORMAT_MAP(MAP_TYPE,unsigned short,unsigned short,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt:       _DO_FORMAT_MAP(MAP_TYPE,unsigned int,unsigned int,PARAMS,FMT,ARGC,ARGV); break; \
    case ULong:      _DO_FORMAT_MAP(MAP_TYPE,unsigned long,unsigned long,PARAMS,FMT,ARGC,ARGV); break; \
    case ULongLong:  _DO_FORMAT_MAP(MAP_TYPE,unsigned long long,unsigned long long,PARAMS,FMT,ARGC,ARGV); break; \
    case Float:      _DO_FORMAT_MAP(MAP_TYPE,float,float,PARAMS,FMT,ARGC,ARGV); break; \
    case Double:     _DO_FORMAT_MAP(MAP_TYPE,double,double,PARAMS,FMT,ARGC,ARGV); break; \
    case LongDouble: _DO_FORMAT_MAP(MAP_TYPE,long double,long double,PARAMS,FMT,ARGC,ARGV); break; \
    case Int8:       _DO_FORMAT_MAP(MAP_TYPE,std::int8_t,std::int8_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int16:      _DO_FORMAT_MAP(MAP_TYPE,std::int16_t,std::int16_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int32:      _DO_FORMAT_MAP(MAP_TYPE,std::int32_t,std::int32_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int64:      _DO_FORMAT_MAP(MAP_TYPE,std::int64_t,std::int64_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt8:      _DO_FORMAT_MAP(MAP_TYPE,std::uint8_t,std::uint8_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt16:     _DO_FORMAT_MAP(MAP_TYPE,std::uint16_t,std::uint16_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt32:     _DO_FORMAT_MAP(MAP_TYPE,std::uint32_t,std::uint32_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt64:     _DO_FORMAT_MAP(MAP_TYPE,std::uint64_t,std::uint64_t,PARAMS,FMT,ARGC,ARGV); break; \
    case String:     _DO_FORMAT_MAP(MAP_TYPE,std::string,std::string,PARAMS,FMT,ARGC,ARGV); break; \
    default: throw std::runtime_error("unhandeled type"); \
    }

#define _DO_FORMAT_PAIR(FIRST_PARSE,FIRST_TYPE,PARAMS,FMT,ARGC,ARGV) \
    switch (parse_value_type((PARAMS)[1])) { \
    case Bool:       do_format_pair<FIRST_PARSE,bool,FIRST_TYPE>(FMT,ARGC,ARGV); break; \
    case Char:       do_format_pair<FIRST_PARSE,Character,FIRST_TYPE,char>(PARAMS,FMT,ARGC,ARGV); break; \
    case Short:      do_format_pair<FIRST_PARSE,short,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case Int:        do_format_pair<FIRST_PARSE,int,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case Long:       do_format_pair<FIRST_PARSE,long,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case LongLong:   do_format_pair<FIRST_PARSE,long long,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case SChar:      do_format_pair<FIRST_PARSE,SCharacter,FIRST_TYPE,signed char>(PARAMS,FMT,ARGC,ARGV); break; \
    case UChar:      do_format_pair<FIRST_PARSE,UCharacter,FIRST_TYPE,unsigned char>(PARAMS,FMT,ARGC,ARGV); break; \
    case UShort:     do_format_pair<FIRST_PARSE,unsigned short,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case UInt:       do_format_pair<FIRST_PARSE,unsigned int,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case ULong:      do_format_pair<FIRST_PARSE,unsigned long,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case ULongLong:  do_format_pair<FIRST_PARSE,unsigned long long,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case Float:      do_format_pair<FIRST_PARSE,float,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case Double:     do_format_pair<FIRST_PARSE,double,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case LongDouble: do_format_pair<FIRST_PARSE,long double,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case Int8:       do_format_pair<FIRST_PARSE,std::int8_t,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case Int16:      do_format_pair<FIRST_PARSE,std::int16_t,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case Int32:      do_format_pair<FIRST_PARSE,std::int32_t,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case Int64:      do_format_pair<FIRST_PARSE,std::int64_t,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case UInt8:      do_format_pair<FIRST_PARSE,std::uint8_t,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case UInt16:     do_format_pair<FIRST_PARSE,std::uint16_t,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case UInt32:     do_format_pair<FIRST_PARSE,std::uint32_t,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case UInt64:     do_format_pair<FIRST_PARSE,std::uint64_t,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    case String:     do_format_pair<FIRST_PARSE,std::string,FIRST_TYPE>(PARAMS,FMT,ARGC,ARGV); break; \
    default: throw std::runtime_error("unhandeled type"); \
    }

#define DO_FORMAT_PAIR(PARAMS,FMT,ARGC,ARGV) \
    if ((PARAMS).size() != 2) { \
        throw std::range_error("illegal number of type parameters"); \
    } \
    switch (parse_value_type((PARAMS)[0])) { \
    case Bool:       _DO_FORMAT_PAIR(bool,bool,PARAMS,FMT,ARGC,ARGV); break; \
    case Char:       _DO_FORMAT_PAIR(Character,char,PARAMS,FMT,ARGC,ARGV); break; \
    case Short:      _DO_FORMAT_PAIR(short,short,PARAMS,FMT,ARGC,ARGV); break; \
    case Int:        _DO_FORMAT_PAIR(int,int,PARAMS,FMT,ARGC,ARGV); break; \
    case Long:       _DO_FORMAT_PAIR(long,long,PARAMS,FMT,ARGC,ARGV); break; \
    case LongLong:   _DO_FORMAT_PAIR(long long,long long,PARAMS,FMT,ARGC,ARGV); break; \
    case SChar:      _DO_FORMAT_PAIR(SCharacter,signed char,PARAMS,FMT,ARGC,ARGV); break; \
    case UChar:      _DO_FORMAT_PAIR(UCharacter,unsigned char,PARAMS,FMT,ARGC,ARGV); break; \
    case UShort:     _DO_FORMAT_PAIR(unsigned short,unsigned short,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt:       _DO_FORMAT_PAIR(unsigned int,unsigned int,PARAMS,FMT,ARGC,ARGV); break; \
    case ULong:      _DO_FORMAT_PAIR(unsigned long,unsigned long,PARAMS,FMT,ARGC,ARGV); break; \
    case ULongLong:  _DO_FORMAT_PAIR(unsigned long long,unsigned long long,PARAMS,FMT,ARGC,ARGV); break; \
    case Float:      _DO_FORMAT_PAIR(float,float,PARAMS,FMT,ARGC,ARGV); break; \
    case Double:     _DO_FORMAT_PAIR(double,double,PARAMS,FMT,ARGC,ARGV); break; \
    case LongDouble: _DO_FORMAT_PAIR(long double,long double,PARAMS,FMT,ARGC,ARGV); break; \
    case Int8:       _DO_FORMAT_PAIR(std::int8_t,std::int8_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int16:      _DO_FORMAT_PAIR(std::int16_t,std::int16_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int32:      _DO_FORMAT_PAIR(std::int32_t,std::int32_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int64:      _DO_FORMAT_PAIR(std::int64_t,std::int64_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt8:      _DO_FORMAT_PAIR(std::uint8_t,std::uint8_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt16:     _DO_FORMAT_PAIR(std::uint16_t,std::uint16_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt32:     _DO_FORMAT_PAIR(std::uint32_t,std::uint32_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt64:     _DO_FORMAT_PAIR(std::uint64_t,std::uint64_t,PARAMS,FMT,ARGC,ARGV); break; \
    case String:     _DO_FORMAT_PAIR(std::string,std::string,PARAMS,FMT,ARGC,ARGV); break; \
    default: throw std::runtime_error("unhandeled type"); \
    }

const std::string STD_NS = "std::";
const std::string ARRAY_BRACKETS = "[]";

void do_format(const char* fmt, const std::string& type, std::size_t argc, const char* argv[]) {
    auto i = type.cbegin();
    auto e = type.cend();

    // strip std:: namespace
    if (std::equal(i, i + STD_NS.size(), STD_NS.cbegin())) {
        i += STD_NS.size();
    }
    auto j = i;
    while (std::isalnum(*j) || *j == '_' || std::isspace(*j)) ++ j;

    std::string name(i, j);
    if (j == e) {
        // value type
        if (argc != 1) {
            throw std::range_error("illegal number of arguments");
        }
        ValueType tp = parse_value_type(name);
        switch (tp) {
        case Bool:       do_format_value<bool>(fmt, argv[0]); break;
        case Char:       do_format_value<Character,char>(fmt, argv[0]); break;
        case Short:      do_format_value<short>(fmt, argv[0]); break;
        case Int:        do_format_value<int>(fmt, argv[0]); break;
        case Long:       do_format_value<long>(fmt, argv[0]); break;
        case LongLong:   do_format_value<long long>(fmt, argv[0]); break;
        case SChar:      do_format_value<SCharacter,signed char>(fmt, argv[0]); break;
        case UChar:      do_format_value<UCharacter,unsigned char>(fmt, argv[0]); break;
        case UShort:     do_format_value<unsigned short>(fmt, argv[0]); break;
        case UInt:       do_format_value<unsigned int>(fmt, argv[0]); break;
        case ULong:      do_format_value<unsigned long>(fmt, argv[0]); break;
        case ULongLong:  do_format_value<unsigned long long>(fmt, argv[0]); break;
        case Float:      do_format_value<float>(fmt, argv[0]); break;
        case Double:     do_format_value<double>(fmt, argv[0]); break;
        case LongDouble: do_format_value<long double>(fmt, argv[0]); break;
        case Int8:       do_format_value<std::int8_t>(fmt, argv[0]); break;
        case Int16:      do_format_value<std::int16_t>(fmt, argv[0]); break;
        case Int32:      do_format_value<std::int32_t>(fmt, argv[0]); break;
        case Int64:      do_format_value<std::int64_t>(fmt, argv[0]); break;
        case UInt8:      do_format_value<std::uint8_t>(fmt, argv[0]); break;
        case UInt16:     do_format_value<std::uint16_t>(fmt, argv[0]); break;
        case UInt32:     do_format_value<std::uint32_t>(fmt, argv[0]); break;
        case UInt64:     do_format_value<std::uint64_t>(fmt, argv[0]); break;
        case String:     do_format_value<std::string>(fmt, argv[0]); break;
        default: throw std::runtime_error("unhandeled type");
        }
    }
    else if (std::equal(j, e, ARRAY_BRACKETS.cbegin())) {
        // array
        ValueType tp = parse_value_type(name);
        switch (tp) {
        case Bool:       do_format_array<bool>(fmt, argc, argv); break;
        case Char:
            if (argc != 1) {
                throw std::range_error("illegal number of arguments");
            }
            do_format_value<const char*>(fmt, argv[0]);
            break;
        case Short:      do_format_array<short>(fmt, argc, argv); break;
        case Int:        do_format_array<int>(fmt, argc, argv); break;
        case Long:       do_format_array<long>(fmt, argc, argv); break;
        case LongLong:   do_format_array<long long>(fmt, argc, argv); break;
        case SChar:      do_format_array<SCharacter,signed char>(fmt, argc, argv); break;
        case UChar:      do_format_array<UCharacter,unsigned char>(fmt, argc, argv); break;
        case UShort:     do_format_array<unsigned short>(fmt, argc, argv); break;
        case UInt:       do_format_array<unsigned int>(fmt, argc, argv); break;
        case ULong:      do_format_array<unsigned long>(fmt, argc, argv); break;
        case ULongLong:  do_format_array<unsigned long long>(fmt, argc, argv); break;
        case Float:      do_format_array<float>(fmt, argc, argv); break;
        case Double:     do_format_array<double>(fmt, argc, argv); break;
        case LongDouble: do_format_array<long double>(fmt, argc, argv); break;
        case Int8:       do_format_array<std::int8_t>(fmt, argc, argv); break;
        case Int16:      do_format_array<std::int16_t>(fmt, argc, argv); break;
        case Int32:      do_format_array<std::int32_t>(fmt, argc, argv); break;
        case Int64:      do_format_array<std::int64_t>(fmt, argc, argv); break;
        case UInt8:      do_format_array<std::uint8_t>(fmt, argc, argv); break;
        case UInt16:     do_format_array<std::uint16_t>(fmt, argc, argv); break;
        case UInt32:     do_format_array<std::uint32_t>(fmt, argc, argv); break;
        case UInt64:     do_format_array<std::uint64_t>(fmt, argc, argv); break;
        case String:     do_format_array<std::string>(fmt, argc, argv); break;
        default: throw std::runtime_error("unhandeled type");
        }
    }
    else if (*j == '<') {
        // template
        ++ j;
        std::vector<std::string> params;
        i = j;
        while (j != e && *j != '>') {
            if (*j == ',') {
                if (i == j) {
                    throw std::invalid_argument(type);
                }
                params.emplace_back(std::string(i,j));
                i = j + 1;
            }
            else if (!std::isalnum(*j) && *j != '_') {
                throw std::invalid_argument(type);
            }
            ++ j;
        }
        if (i == j) {
            throw std::invalid_argument(type);
        }
        params.emplace_back(std::string(i,j));
        if (*j != '>' || ++ j != e) {
            throw std::invalid_argument(type);
        }
        if (name == "vector") {
            DO_FORMAT_COLLECTION(do_format_collection,std::vector,params,fmt,argc,argv);
        }
        else if (name == "list") {
            DO_FORMAT_COLLECTION(do_format_collection,std::list,params,fmt,argc,argv);
        }
        else if (name == "set") {
            DO_FORMAT_COLLECTION(do_format_set,std::set,params,fmt,argc,argv);
        }
        else if (name == "unordered_set") {
            DO_FORMAT_COLLECTION(do_format_set,std::unordered_set,params,fmt,argc,argv);
        }
        else if (name == "map") {
            DO_FORMAT_MAP(std::map,params,fmt,argc,argv);
        }
        else if (name == "unordered_map") {
            DO_FORMAT_MAP(std::unordered_map,params,fmt,argc,argv);
        }
        else if (name == "pair") {

        }
        else {
            throw std::invalid_argument(type);
        }
    }
    else {
        throw std::invalid_argument(type);
    }
}

void usage(int argc, const char* argv[]) {
    std::cout << "usage: " << (argc > 0 ? argv[0] : "format") << " <format> <type> <value>...\n";
}

int main(int argc, const char* argv[]) {
    const char* fmt = argv[1];

    if (argc < 3) {
        std::cerr << "illegal number of arguments\n";
        usage(argc, argv);
        return 1;
    }

    try {
        do_format(fmt, argv[2], argc - 3, argv + 3);
    }
    catch (const std::invalid_argument& exc) {
        std::cerr << "invalid argument: " << exc.what() << '\n';
        return 1;
    }
    catch (const std::exception& exc) {
        std::cerr << exc.what() << '\n';
        return 1;
    }
    catch (...) {
        std::cerr << "unhandeled exception\n";
        return 1;
    }

    return 0;
}
