#include "formatstring.h"

#include <iostream>

using namespace formatstring;

int main() {
    std::string s = hex(123);
    std::cout << format(" foo {:_^20s} bar {0} baz {:#020B} {} {}\n", "hello", 1234, false, 2345)
              << val(true).upper() << ' ' << s << ' ' << oct(234).alt() << '\n';

    Format fmt = compile("{}-{:c}\n");

    std::cout << fmt('A', 52);

    return 0;
}
