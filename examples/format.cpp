#include <iostream>
#include <sstream>
#include <cstring>
#include <stdexcept>
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

void usage(int argc, const char* argv[]) {
    std::cout << "usage: " << (argc > 0 ? argv[0] : "format") << " <format> [type] <value>...\n";
}

int main(int argc, const char* argv[]) {
    const char* fmt = argv[1];
    if (argc < 3) {
        std::cerr << "illegal number of arguments\n";
        usage(argc, argv);
        return 1;
    }
    else if (argc == 3) {
        std::cout << format(fmt, argv[2]);
    }
    else if (argc == 4) {
        std::string type = argv[2];
        const char* val = argv[3];

        if (type == "bool") {
            std::cout << format(fmt, lexical_cast<bool>(val));
        }
        else if (type == "char") {
            std::cout << format(fmt, lexical_cast<char>(val));
        }
        else if (type == "short") {
            std::cout << format(fmt, lexical_cast<short>(val));
        }
        else if (type == "int") {
            std::cout << format(fmt, lexical_cast<int>(val));
        }
        else if (type == "long") {
            std::cout << format(fmt, lexical_cast<long>(val));
        }
        else if (type == "long long") {
            std::cout << format(fmt, lexical_cast<long long>(val));
        }
        else if (type == "unsigned char") {
            std::cout << format(fmt, lexical_cast<unsigned char>(val));
        }
        else if (type == "unsigned short") {
            std::cout << format(fmt, lexical_cast<unsigned short>(val));
        }
        else if (type == "unsigned int") {
            std::cout << format(fmt, lexical_cast<unsigned int>(val));
        }
        else if (type == "unsigned long") {
            std::cout << format(fmt, lexical_cast<unsigned long>(val));
        }
        else if (type == "unsigned long long") {
            std::cout << format(fmt, lexical_cast<unsigned long long>(val));
        }
        else if (type == "float") {
            std::cout << format(fmt, lexical_cast<float>(val));
        }
        else if (type == "double") {
            std::cout << format(fmt, lexical_cast<double>(val));
        }
        else if (type == "long double") {
            std::cout << format(fmt, lexical_cast<long double>(val));
        }
        else if (type == "int8_t") {
            std::cout << format(fmt, lexical_cast<std::int8_t>(val));
        }
        else if (type == "int16_t") {
            std::cout << format(fmt, lexical_cast<std::int16_t>(val));
        }
        else if (type == "int32_t") {
            std::cout << format(fmt, lexical_cast<std::int32_t>(val));
        }
        else if (type == "int64_t") {
            std::cout << format(fmt, lexical_cast<std::int64_t>(val));
        }
        else if (type == "uint8_t") {
            std::cout << format(fmt, lexical_cast<std::uint8_t>(val));
        }
        else if (type == "uint16_t") {
            std::cout << format(fmt, lexical_cast<std::uint16_t>(val));
        }
        else if (type == "uint32_t") {
            std::cout << format(fmt, lexical_cast<std::uint32_t>(val));
        }
        else if (type == "string") {
            std::cout << format(fmt, lexical_cast<std::string>(val));
        }
        else {
            throw std::invalid_argument(type);
        }
    }
    else {
        std::string type = argv[2];
        std::size_t n = argc - 3;
        const char** values = &argv[3];

        if (type == "bool") {
            format_vector<bool>(fmt, n, values);
        }
        else if (type == "char") {
            format_vector<char>(fmt, n, values);
        }
        else if (type == "short") {
            format_vector<short>(fmt, n, values);
        }
        else if (type == "int") {
            format_vector<int>(fmt, n, values);
        }
        else if (type == "long") {
            format_vector<long>(fmt, n, values);
        }
        else if (type == "long long") {
            format_vector<long long>(fmt, n, values);
        }
        else if (type == "unsigned char") {
            format_vector<unsigned char>(fmt, n, values);
        }
        else if (type == "unsigned short") {
            format_vector<unsigned short>(fmt, n, values);
        }
        else if (type == "unsigned int") {
            format_vector<unsigned int>(fmt, n, values);
        }
        else if (type == "unsigned long") {
            format_vector<unsigned long>(fmt, n, values);
        }
        else if (type == "unsigned long long") {
            format_vector<unsigned long long>(fmt, n, values);
        }
        else if (type == "float") {
            format_vector<float>(fmt, n, values);
        }
        else if (type == "double") {
            format_vector<double>(fmt, n, values);
        }
        else if (type == "long double") {
            format_vector<long double>(fmt, n, values);
        }
        else if (type == "int8_t") {
            format_vector<std::int8_t>(fmt, n, values);
        }
        else if (type == "int16_t") {
            format_vector<std::int16_t>(fmt, n, values);;
        }
        else if (type == "int32_t") {
            format_vector<std::int32_t>(fmt, n, values);
        }
        else if (type == "int64_t") {
            format_vector<std::int64_t>(fmt, n, values);
        }
        else if (type == "uint8_t") {
            format_vector<std::uint8_t>(fmt, n, values);
        }
        else if (type == "uint16_t") {
            format_vector<std::uint16_t>(fmt, n, values);
        }
        else if (type == "uint32_t") {
            format_vector<std::uint32_t>(fmt, n, values);
        }
        else if (type == "string") {
            format_vector<std::string>(fmt, n, values);
        }
        else {
            throw std::invalid_argument(type);
        }
    }

    return 0;
}
