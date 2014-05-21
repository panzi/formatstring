#include <iostream>
#include <vector>
#include <array>
#include <tuple>
#include <cmath>

#include <fstream>

#include "formatstring.h"

using namespace formatstring;

// helper template
template<typename Char>
struct strings;

// some example classes to illustrate all the different ways to define your own formatting for your own classes
struct Example1 {
    Example1(const std::string& value) : member(value) {}
    Example1(const char* value) : member(value) {}

    std::string member;
};

struct Example2 {
    Example2(const std::string& value) : member(value) {}
    Example2(const char* value) : member(value) {}

    std::string member;
};

template<typename Char>
struct Example3 {
    Example3(const std::basic_string<Char>& value) : member(value) {}
    Example3(const Char* value) : member(value) {}

    std::basic_string<Char> member;
};

template<typename Char>
struct Example3Sub : public Example3<Char> {
    Example3Sub(const std::basic_string<Char>& value) : Example3<Char>(value) {}
    Example3Sub(const char* value) : Example3<Char>(value) {}
};

// most simple case, just an operator<< for ostream:
std::ostream& operator << (std::ostream& out, const Example1& value) {
    return out << "<Example1 " << value.member << ">";
}

// or cover all possible basic_ostream variants at once:
// Note the effort to get the correctly typed stirng constants. Another option would be to overload
// the operator function for the appropriate types instead.
template<typename Char>
std::basic_ostream<Char>& operator << (std::basic_ostream<Char>& out, const Example2& value) {
    return out << format(strings<Char>::str2, value.member);
}

// Or if you would like to do something more complex depending on the format spec and conversion
// when a object of your custom class is formatted you can implement your own formatter.
// You don't need to make it a template class. If you want you just can implement it for char
// based std::ostream and derive the Formatter class (which is a typedef to BasicFormatter<char>)
// instead.
template<typename Char>
class Example3Formatter : public BasicFormatter<Char> {
public:
    Example3Formatter(const Example3<Char>* value) : value(value) {}

    virtual void format(std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const {
        std::basic_ostringstream<Char> buffer;

        switch (conv) {
        case ReprConv:
            buffer << formatstring::format(strings<Char>::repr3, value->member);
            break;

        default:
            buffer << formatstring::format(strings<Char>::str3, value->member);
            break;
        }

        format_value(out, buffer.str(), spec);
    }

private:
    const Example3<Char>* value;
};

// You then need to register your formatter by specializing the format_traits template
// class in the formatstring namespace like this:
namespace formatstring {
#if 0
    // this way only variables of type Example3 are supported by this formatter (no derived classes)
    template<typename Char>
    struct format_traits< Char, Example3<Char> > {
        typedef Char char_type;
        typedef Example3<Char> value_type;

        static inline Example3Formatter<Char>* make_formatter(const Example3<Char>& value) {
            return new Example3Formatter<Char>(&value);
        }
    };
#endif
    // this way also derived classes from Example3 will be supported by this formatter
    template<typename Char, typename T>
    struct format_traits<Char, T, typename std::enable_if<std::is_base_of<Example3<Char>, T>::value>::type> {
        typedef Char char_type;
        typedef T value_type;

        static inline Example3Formatter<Char>* make_formatter(const T& value) {
            return new Example3Formatter<Char>(&value);
        }
    };
}

// string table to support all kinds of string types:
template<>
struct strings<char> {
    static constexpr const char*const str2  = "<Example2 {}>";
    static constexpr const char*const str3  = "<Example3 {}>";
    static constexpr const char*const repr3 = "Example3({!r})";
};

template<>
struct strings<wchar_t> {
    static constexpr const wchar_t*const str2  = L"<Example2 {}>";
    static constexpr const wchar_t*const str3  = L"<Example3 {}>";
    static constexpr const wchar_t*const repr3 = L"Example3({!r})";
};

// The header formatstring/config.h defines these feature test macros.
// You can use them to condittionally support char16_t and char32_t
// based string operations. Support is automatically detected when the
// formatstring library is compiled.
#ifdef FORMATSTRING_CHAR16_SUPPORT
template<>
struct strings<char16_t> {
    static constexpr const char16_t*const str2  = u"<Example2 {}>";
    static constexpr const char16_t*const str3  = u"<Example3 {}>";
    static constexpr const char16_t*const repr3 = u"Example3({!r})";
};
#endif

#ifdef FORMATSTRING_CHAR32_SUPPORT
template<>
struct strings<char32_t> {
    static constexpr const char32_t*const str2  = U"<Example2 {}>";
    static constexpr const char32_t*const str3  = U"<Example3 {}>";
    static constexpr const char32_t*const repr3 = U"Example3({!r})";
};
#endif

int main() {
    std::vector<std::string> vec = {"foo", "bar", "baz"};
    std::array<int,3> arr = {1, 2, 3};

    std::string bla = "bla";
    std::string s = hex(123);
    std::cout << format("{} {{foo}} {:_^20s} bar {0} baz {:#020B} {} {} {!s}\n", bla, "hello", 1234, false, 2345, "bla bla")
              << val(true).upper().width(20).right() << '\n'
              << val(true," >20S") << '\n'
              << s << ' ' << oct(234).alt() << '\n';

    Format fmt = compile("{}-{:c}");

    std::cout << fmt('A', 52) << ' ';
    std::cout << fmt(53, 'B') << '\n';
    std::cout << format("bla {} {:_^20} {} {} {} {}\n", vec, arr,
                        std::tuple<std::string,int,bool>("foo\n\t\"\\", 12, false),
                        std::tuple<float>(0), std::tuple<>(), std::pair<int,std::string>(32,"bla"));

    std::string ch = repr('\n');

    Example1 ex1("foo bar");
    Example2 ex2_char("char");
    Example3<wchar_t> ex3_wchar(L"wchar_t");
    Example2 *ptr = new Example2("ptr");
    std::shared_ptr<Example2> ptr2(new Example2("shared"));
    std::cout << format("{:_<20} {:_>20}", ex1, ex2_char) << repr(Example1("bla")) << '\n';
    std::cout << format("{}, {!r}, ptr: {!s}, *ptr: {}, shared_ptr: {}, *shared_ptr: {}\n",
                        Example2("blub"), Example2("bla\nbla"), ptr, *ptr, ptr2, *ptr2);
    std::cout << format("subtype support: {}\n", Example3Sub<char>("Example3Sub"));
    std::cout.flush();
    std::wcout << format(L"{}\n", ex3_wchar) << std::flush;
    delete ptr;

    std::cout << str(12) << ' ' << repr("foo bar") << ' ' << ch << '\n';

    std::cout << bin(1234567890).alt() << ' ' << oct(1234567890).alt() << ' ' << dec(1234567890).alt() << ' ' << hex(1234567890).alt() << '\n';

    std::cout << format("{:020} {:,} {:,} {:,} {:,}\n", -0.0, 100000000000000, 1, 100, 1000.0);

    std::cout << format("{:.3} {:.1%} {:.2f} ({: e}) {:e} pi={:+g} {!r:_^20} '{:020}' {:.2f}\n",
                        12.12, 1.234, 1.0, 5.2, 1000000, M_PI, -1.2, -0.0, -NAN);

    const char* str = "foo bar baz";
    std::cout << slice(str, str+7) << ' ' << slice(str+8, str+11).repr() << '\n';

    std::unordered_set<std::string> xs = {"foo", "bar", "baz"};
    std::unordered_map<std::string,int> ys = {{"A",12},{"B",23},{"C",34}};
    std::cout << format("{} {}\n", xs, ys);

    std::cout << format(  " string {} {:_^10} {} {} {1!r} {3!r}\n", 12,  'A', arr,  "bla") << std::flush;
    std::wcout << format(L"wstring {} {:_^10} {} {} {1!r} {3!r}\n", 12, L'A', arr, L"bla") << std::flush;

    std::cout << format(".{:_^20}.\n", 'A');

    int intarr[] = {1, 2, -3};
    std::cout << format(".{:_^20}.\n", intarr);

    void *vptr = intarr;
    std::cout << format(".{:_^20}.\n", vptr);

    std::cout << format("{} {}\n", FormatSpec("_^020") == FormatSpec("_^20"), FormatSpec(" =") != FormatSpec(""));

    auto dbgfmt = debug_compile("debug: {} {!r}\n");
    Format emptyfmt = dbgfmt;
    std::cout << debug("debug: {:_^20}\n", "foo bar") << dbgfmt(12, 'A') << emptyfmt(12, 'A');

    std::cout << "formatstring library version: " << formatstring::version::major << '.' << formatstring::version::minor << '.' << formatstring::version::patch << '\n';

    try {
        std::cout << format("hex float: {:a} {0:020A}\n", 1234.5678);
    }
    catch (const std::runtime_error& exc) {
        std::cerr << exc.what() << std::endl;
    }

    return 0;
}
