#include <iostream>
#include <vector>
#include <array>
#include <tuple>
#include <cmath>

#include "formatstring.h"

using namespace formatstring;

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

std::ostream& operator << (std::ostream& out, const Example1& value) {
    return out << "<Example1 " << value.member << ">";
}

template<typename Char>
class Example2Formatter : public Formatter {
public:
    Example2Formatter(const Example2* value) : value(value) {}

    virtual void format(std::basic_ostream<Char>& out, Conversion conv, const BasicFormatSpec<Char>& spec) const {
        std::basic_ostringstream<Char> buffer;

        switch (conv) {
        case ReprConv:
            buffer << "Example2(" << repr(value->member) << ")";
            break;

        default:
            buffer << "<Example2 " << value->member << ">";
            break;
        }

        format_value(out, buffer.str(), spec);
    }

private:
    const Example2* value;
};

namespace formatstring {

    template<typename Char>
    struct format_traits<Char, Example2> {
        typedef Char char_type;
        typedef Example2 value_type;

        static inline Example2Formatter<Char>* make_formatter(const Example2& value) {
            return new Example2Formatter<Char>(&value);
        }
    };

}

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
    Example2 *ptr = new Example2("ptr");
    std::shared_ptr<Example2> ptr2(new Example2("shared"));
    std::cout << format("{:_<20} ", ex1) << repr(Example1("bla")) << '\n';
    std::cout << format("{}, {!r}, ptr: {!s}, *ptr: {}, shared_ptr: {}, *shared_ptr: {}\n",
                        Example2("blub"), Example2("bla\nbla"), ptr, *ptr, ptr2, *ptr2);
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

    return 0;
}
