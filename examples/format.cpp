#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <cstring>
#include <cstdint>

#include <formatstring.h>

using namespace formatstring;

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

template<>
unsigned char lexical_cast(const char* str) {
    if (std::strlen(str) != 1) {
        throw std::invalid_argument(str);
    }
    return (unsigned char)str[0];
}

template<>
char lexical_cast(const char* str) {
    if (std::strlen(str) != 1) {
        throw std::invalid_argument(str);
    }
    return str[0];
}

template<>
bool lexical_cast(const char* str) {
    if (std::strcmp(str, "true") == 0) {
        return true;
    }
    else if (std::strcmp(str, "true") == 0) {
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

template<typename T>
void do_format_value(const char* fmt, const char* value) {
    std::cout << format(fmt, lexical_cast<T>(value));
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

template<typename First, typename Second>
void do_format_pair(const char* fmt, std::size_t argc, const char* argv[]) {
    if (argc != 2) {
        throw std::range_error("illegal number of arguments");
    }
    First  a = lexical_cast<First>(argv[0]);
    Second b = lexical_cast<Second>(argv[1]);
    std::cout << format(fmt, std::make_pair<First,Second>(a, b));
}

template<typename T>
void do_format_array(const char* fmt, std::size_t argc, const char* argv[]) {
    T* array = new T[argc];
    try {
        for (std::size_t i = 0; i < argc; ++ i) {
            array[i] = lexical_cast<T>(argv[i]);
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
    case Char:       FUNC<COLLECTION_TYPE<char>, char>((FMT), (ARGC), (ARGV)); break; \
    case Short:      FUNC<COLLECTION_TYPE<short>, short>((FMT), (ARGC), (ARGV)); break; \
    case Int:        FUNC<COLLECTION_TYPE<int>, int>((FMT), (ARGC), (ARGV)); break; \
    case Long:       FUNC<COLLECTION_TYPE<long>, long>((FMT), (ARGC), (ARGV)); break; \
    case LongLong:   FUNC<COLLECTION_TYPE<long long>, long long>((FMT), (ARGC), (ARGV)); break; \
    case UChar:      FUNC<COLLECTION_TYPE<unsigned char>, unsigned char>((FMT), (ARGC), (ARGV)); break; \
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
    }

#define _DO_FORMAT_MAP(MAP_TYPE,KEY_TYPE,PARAMS,FMT,ARGC,ARGV) \
    switch (parse_value_type((PARAMS)[1])) { \
    case Bool:       do_format_map<MAP_TYPE<KEY_TYPE, bool>, KEY_TYPE, bool>((FMT), (ARGC), (ARGV)); break; \
    case Char:       do_format_map<MAP_TYPE<KEY_TYPE, char>, KEY_TYPE, char>((FMT), (ARGC), (ARGV)); break; \
    case Short:      do_format_map<MAP_TYPE<KEY_TYPE, short>, KEY_TYPE, short>((FMT), (ARGC), (ARGV)); break; \
    case Int:        do_format_map<MAP_TYPE<KEY_TYPE, int>, KEY_TYPE, int>((FMT), (ARGC), (ARGV)); break; \
    case Long:       do_format_map<MAP_TYPE<KEY_TYPE, long>, KEY_TYPE, long>((FMT), (ARGC), (ARGV)); break; \
    case LongLong:   do_format_map<MAP_TYPE<KEY_TYPE, long long>, KEY_TYPE, long long>((FMT), (ARGC), (ARGV)); break; \
    case UChar:      do_format_map<MAP_TYPE<KEY_TYPE, unsigned char>, KEY_TYPE, unsigned char>((FMT), (ARGC), (ARGV)); break; \
    case UShort:     do_format_map<MAP_TYPE<KEY_TYPE, unsigned short>, KEY_TYPE, unsigned short>((FMT), (ARGC), (ARGV)); break; \
    case UInt:       do_format_map<MAP_TYPE<KEY_TYPE, unsigned int>, KEY_TYPE, unsigned int>((FMT), (ARGC), (ARGV)); break; \
    case ULong:      do_format_map<MAP_TYPE<KEY_TYPE, unsigned long>, KEY_TYPE, unsigned long>((FMT), (ARGC), (ARGV)); break; \
    case ULongLong:  do_format_map<MAP_TYPE<KEY_TYPE, unsigned long long>, KEY_TYPE, unsigned long long>((FMT), (ARGC), (ARGV)); break; \
    case Float:      do_format_map<MAP_TYPE<KEY_TYPE, float>, KEY_TYPE, float>((FMT), (ARGC), (ARGV)); break; \
    case Double:     do_format_map<MAP_TYPE<KEY_TYPE, double>, KEY_TYPE, double>((FMT), (ARGC), (ARGV)); break; \
    case LongDouble: do_format_map<MAP_TYPE<KEY_TYPE, long double>, KEY_TYPE, long double>((FMT), (ARGC), (ARGV)); break; \
    case Int8:       do_format_map<MAP_TYPE<KEY_TYPE, std::int8_t>, KEY_TYPE, std::int8_t>((FMT), (ARGC), (ARGV)); break; \
    case Int16:      do_format_map<MAP_TYPE<KEY_TYPE, std::int16_t>, KEY_TYPE, std::int16_t>((FMT), (ARGC), (ARGV)); break; \
    case Int32:      do_format_map<MAP_TYPE<KEY_TYPE, std::int32_t>, KEY_TYPE, std::int32_t>((FMT), (ARGC), (ARGV)); break; \
    case Int64:      do_format_map<MAP_TYPE<KEY_TYPE, std::int64_t>, KEY_TYPE, std::int64_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt8:      do_format_map<MAP_TYPE<KEY_TYPE, std::uint8_t>, KEY_TYPE, std::uint8_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt16:     do_format_map<MAP_TYPE<KEY_TYPE, std::uint16_t>, KEY_TYPE, std::uint16_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt32:     do_format_map<MAP_TYPE<KEY_TYPE, std::uint32_t>, KEY_TYPE, std::uint32_t>((FMT), (ARGC), (ARGV)); break; \
    case UInt64:     do_format_map<MAP_TYPE<KEY_TYPE, std::uint64_t>, KEY_TYPE, std::uint64_t>((FMT), (ARGC), (ARGV)); break; \
    case String:     do_format_map<MAP_TYPE<KEY_TYPE, std::string>, KEY_TYPE, std::string>((FMT), (ARGC), (ARGV)); break; \
    }

#define DO_FORMAT_MAP(MAP_TYPE,PARAMS,FMT,ARGC,ARGV) \
    if ((PARAMS).size() != 2) { \
        throw std::range_error("illegal number of type parameters"); \
    } \
    switch (parse_value_type((PARAMS)[0])) { \
    case Bool:       _DO_FORMAT_MAP(MAP_TYPE,bool,PARAMS,FMT,ARGC,ARGV); break; \
    case Char:       _DO_FORMAT_MAP(MAP_TYPE,char,PARAMS,FMT,ARGC,ARGV); break; \
    case Short:      _DO_FORMAT_MAP(MAP_TYPE,short,PARAMS,FMT,ARGC,ARGV); break; \
    case Int:        _DO_FORMAT_MAP(MAP_TYPE,int,PARAMS,FMT,ARGC,ARGV); break; \
    case Long:       _DO_FORMAT_MAP(MAP_TYPE,long,PARAMS,FMT,ARGC,ARGV); break; \
    case LongLong:   _DO_FORMAT_MAP(MAP_TYPE,long long,PARAMS,FMT,ARGC,ARGV); break; \
    case UChar:      _DO_FORMAT_MAP(MAP_TYPE,unsigned char,PARAMS,FMT,ARGC,ARGV); break; \
    case UShort:     _DO_FORMAT_MAP(MAP_TYPE,unsigned short,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt:       _DO_FORMAT_MAP(MAP_TYPE,unsigned int,PARAMS,FMT,ARGC,ARGV); break; \
    case ULong:      _DO_FORMAT_MAP(MAP_TYPE,unsigned long,PARAMS,FMT,ARGC,ARGV); break; \
    case ULongLong:  _DO_FORMAT_MAP(MAP_TYPE,unsigned long long,PARAMS,FMT,ARGC,ARGV); break; \
    case Float:      _DO_FORMAT_MAP(MAP_TYPE,float,PARAMS,FMT,ARGC,ARGV); break; \
    case Double:     _DO_FORMAT_MAP(MAP_TYPE,double,PARAMS,FMT,ARGC,ARGV); break; \
    case LongDouble: _DO_FORMAT_MAP(MAP_TYPE,long double,PARAMS,FMT,ARGC,ARGV); break; \
    case Int8:       _DO_FORMAT_MAP(MAP_TYPE,std::int8_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int16:      _DO_FORMAT_MAP(MAP_TYPE,std::int16_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int32:      _DO_FORMAT_MAP(MAP_TYPE,std::int32_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int64:      _DO_FORMAT_MAP(MAP_TYPE,std::int64_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt8:      _DO_FORMAT_MAP(MAP_TYPE,std::uint8_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt16:     _DO_FORMAT_MAP(MAP_TYPE,std::uint16_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt32:     _DO_FORMAT_MAP(MAP_TYPE,std::uint32_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt64:     _DO_FORMAT_MAP(MAP_TYPE,std::uint64_t,PARAMS,FMT,ARGC,ARGV); break; \
    case String:     _DO_FORMAT_MAP(MAP_TYPE,std::string,PARAMS,FMT,ARGC,ARGV); break; \
    }

#define _DO_FORMAT_PAIR(FIRST_TYPE,PARAMS,FMT,ARGC,ARGV) \
    switch (parse_value_type((PARAMS)[1])) { \
    case Bool:       do_format_pair<FIRST_TYPE,bool>(FMT,ARGC,ARGV); break; \
    case Char:       do_format_pair<FIRST_TYPE,char>(PARAMS,FMT,ARGC,ARGV); break; \
    case Short:      do_format_pair<FIRST_TYPE,short>(PARAMS,FMT,ARGC,ARGV); break; \
    case Int:        do_format_pair<FIRST_TYPE,int>(PARAMS,FMT,ARGC,ARGV); break; \
    case Long:       do_format_pair<FIRST_TYPE,long>(PARAMS,FMT,ARGC,ARGV); break; \
    case LongLong:   do_format_pair<FIRST_TYPE,long long>(PARAMS,FMT,ARGC,ARGV); break; \
    case UChar:      do_format_pair<FIRST_TYPE,unsigned char>(PARAMS,FMT,ARGC,ARGV); break; \
    case UShort:     do_format_pair<FIRST_TYPE,unsigned short>(PARAMS,FMT,ARGC,ARGV); break; \
    case UInt:       do_format_pair<FIRST_TYPE,unsigned int>(PARAMS,FMT,ARGC,ARGV); break; \
    case ULong:      do_format_pair<FIRST_TYPE,unsigned long>(PARAMS,FMT,ARGC,ARGV); break; \
    case ULongLong:  do_format_pair<FIRST_TYPE,unsigned long long>(PARAMS,FMT,ARGC,ARGV); break; \
    case Float:      do_format_pair<FIRST_TYPE,float>(PARAMS,FMT,ARGC,ARGV); break; \
    case Double:     do_format_pair<FIRST_TYPE,double>(PARAMS,FMT,ARGC,ARGV); break; \
    case LongDouble: do_format_pair<FIRST_TYPE,long double>(PARAMS,FMT,ARGC,ARGV); break; \
    case Int8:       do_format_pair<FIRST_TYPE,std::int8_t>(PARAMS,FMT,ARGC,ARGV); break; \
    case Int16:      do_format_pair<FIRST_TYPE,std::int16_t>(PARAMS,FMT,ARGC,ARGV); break; \
    case Int32:      do_format_pair<FIRST_TYPE,std::int32_t>(PARAMS,FMT,ARGC,ARGV); break; \
    case Int64:      do_format_pair<FIRST_TYPE,std::int64_t>(PARAMS,FMT,ARGC,ARGV); break; \
    case UInt8:      do_format_pair<FIRST_TYPE,std::uint8_t>(PARAMS,FMT,ARGC,ARGV); break; \
    case UInt16:     do_format_pair<FIRST_TYPE,std::uint16_t>(PARAMS,FMT,ARGC,ARGV); break; \
    case UInt32:     do_format_pair<FIRST_TYPE,std::uint32_t>(PARAMS,FMT,ARGC,ARGV); break; \
    case UInt64:     do_format_pair<FIRST_TYPE,std::uint64_t>(PARAMS,FMT,ARGC,ARGV); break; \
    case String:     do_format_pair<FIRST_TYPE,std::string>(PARAMS,FMT,ARGC,ARGV); break; \
    }

#define DO_FORMAT_PAIR(PARAMS,FMT,ARGC,ARGV) \
    if ((PARAMS).size() != 2) { \
        throw std::range_error("illegal number of type parameters"); \
    } \
    switch (parse_value_type((PARAMS)[0])) { \
    case Bool:       _DO_FORMAT_PAIR(bool,PARAMS,FMT,ARGC,ARGV); break; \
    case Char:       _DO_FORMAT_PAIR(char,PARAMS,FMT,ARGC,ARGV); break; \
    case Short:      _DO_FORMAT_PAIR(short,PARAMS,FMT,ARGC,ARGV); break; \
    case Int:        _DO_FORMAT_PAIR(int,PARAMS,FMT,ARGC,ARGV); break; \
    case Long:       _DO_FORMAT_PAIR(long,PARAMS,FMT,ARGC,ARGV); break; \
    case LongLong:   _DO_FORMAT_PAIR(long long,PARAMS,FMT,ARGC,ARGV); break; \
    case UChar:      _DO_FORMAT_PAIR(unsigned char,PARAMS,FMT,ARGC,ARGV); break; \
    case UShort:     _DO_FORMAT_PAIR(unsigned short,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt:       _DO_FORMAT_PAIR(unsigned int,PARAMS,FMT,ARGC,ARGV); break; \
    case ULong:      _DO_FORMAT_PAIR(unsigned long,PARAMS,FMT,ARGC,ARGV); break; \
    case ULongLong:  _DO_FORMAT_PAIR(unsigned long long,PARAMS,FMT,ARGC,ARGV); break; \
    case Float:      _DO_FORMAT_PAIR(float,PARAMS,FMT,ARGC,ARGV); break; \
    case Double:     _DO_FORMAT_PAIR(double,PARAMS,FMT,ARGC,ARGV); break; \
    case LongDouble: _DO_FORMAT_PAIR(long double,PARAMS,FMT,ARGC,ARGV); break; \
    case Int8:       _DO_FORMAT_PAIR(std::int8_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int16:      _DO_FORMAT_PAIR(std::int16_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int32:      _DO_FORMAT_PAIR(std::int32_t,PARAMS,FMT,ARGC,ARGV); break; \
    case Int64:      _DO_FORMAT_PAIR(std::int64_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt8:      _DO_FORMAT_PAIR(std::uint8_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt16:     _DO_FORMAT_PAIR(std::uint16_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt32:     _DO_FORMAT_PAIR(std::uint32_t,PARAMS,FMT,ARGC,ARGV); break; \
    case UInt64:     _DO_FORMAT_PAIR(std::uint64_t,PARAMS,FMT,ARGC,ARGV); break; \
    case String:     _DO_FORMAT_PAIR(std::string,PARAMS,FMT,ARGC,ARGV); break; \
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
    while (std::isalnum(*j) || *j == '_') ++ j;

    std::string name(i, j);
    if (j == e) {
        // value type
        if (argc != 1) {
            throw std::range_error("illegal number of arguments");
        }
        ValueType tp = parse_value_type(name);
        switch (tp) {
        case Bool:       do_format_value<bool>(fmt, argv[0]); break;
        case Char:       do_format_value<char>(fmt, argv[0]); break;
        case Short:      do_format_value<short>(fmt, argv[0]); break;
        case Int:        do_format_value<int>(fmt, argv[0]); break;
        case Long:       do_format_value<long>(fmt, argv[0]); break;
        case LongLong:   do_format_value<long long>(fmt, argv[0]); break;
        case UChar:      do_format_value<unsigned char>(fmt, argv[0]); break;
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
        }
    }
    else if (std::equal(j, e, ARRAY_BRACKETS.cbegin())) {
        // array
        ValueType tp = parse_value_type(name);
        switch (tp) {
        case Bool:       do_format_array<bool>(fmt, argc, argv); break;
        case Char:       do_format_array<char>(fmt, argc, argv); break;
        case Short:      do_format_array<short>(fmt, argc, argv); break;
        case Int:        do_format_array<int>(fmt, argc, argv); break;
        case Long:       do_format_array<long>(fmt, argc, argv); break;
        case LongLong:   do_format_array<long long>(fmt, argc, argv); break;
        case UChar:      do_format_array<unsigned char>(fmt, argc, argv); break;
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
    }
    catch (const std::exception& exc) {
        std::cerr << exc.what() << '\n';
    }
    catch (...) {
        std::cerr << "unhandeled exception\n";
    }

    return 0;
}
