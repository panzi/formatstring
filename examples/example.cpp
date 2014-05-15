#include "formatstring.h"

#include <iostream>
#include <vector>
#include <array>
#include <tuple>
#include <cmath>

using namespace formatstring;

struct Custom {
    Custom(const std::string& value) : member(value) {}
    Custom(const char* value) : member(value) {}

    std::string member;
};

std::ostream& operator << (std::ostream& out, const Custom& ref) {
    return out << "<Custom " << ref.member << ">";
}

int main() {
    std::vector<std::string> vec = {"foo", "bar", "baz"};
    std::array<int,3> arr = {1, 2, 3};

    std::string s = hex(123);
    std::cout << format(" {{foo}} {:_^20s} bar {0} baz {:#020B} {} {}\n", "hello", 1234, false, 2345)
              << val(true).upper().width(20).right() << ' ' << s << ' ' << oct(234).alt() << '\n';

    Format fmt = compile("{}-{:c}");

    std::cout << fmt('A', 52) << ' ' << fmt(53, 'B') << '\n';
    std::cout << format("bla {} {:_^20} {} {} {}\n", vec, arr,
                        std::tuple<std::string,int,bool>("foo\n\t\"\\", 12, false),
                        std::tuple<float>(0), std::tuple<>());

    std::string ch = repr('\n');
    Custom var("foo bar");
    std::cout << format("{:_<20} ", var) << repr(var) << '\n';
    std::cout << str(12) << ' ' << repr("foo bar") << ' ' << ch << '\n';

    std::cout << format("{:.3} {:.1%} {:.2f} ({: e}) {:e} pi={:+g} {!r:_^20} {:020} {:.2f}\n",
                        12.12, 1.234, 1.0, 5.2, 1000000, M_PI, -1.2, -0.0, -NAN);

    const char* str = "foo bar baz";
    std::cout << slice(str, str+7) << '\n';

    return 0;
}
