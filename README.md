formatstring
============

A C++14 type safe format string library heavily inspired by Python's str.format()
function.

Example:

```c++
#include <iostream>
#include <formatstring.h>

using namespace formatstring;

int main() {
	// create a temp. formatting object that gets written to cout
	std::cout << format("hex: {:#x}, centerd: {:_^20}, padded: {:+010.3f}\n",
	                    1234, "test", 3.14159);

	// create a temp. formatting object that gets converted into a string
	std::string s1 = format("{:d} {:c}", 'A', 66);

	// convert a single value to a string
	std::string s2 = hex(1234).fill('0', 20).alt();

	std::cout << s1 << ' ' << s2 << '\n';

	// compile a format object for later use
	Format fmt = compile("{:d} {:c}");

	std::cout << fmt(65, 'B') << '\n';

	// doesn't generate any output if NDEBUG is defined
	// Because of appropriate inline template functions it won't even
	// generate any code in the binary.
	std::cout << debug("test {}\n", 123);

	// I even added a format literal, just for fun
	std::cout << "{}\n"_fmt("test");

	return 0;
}
```

Output:

	hex: 0x4d2, centerd: ________test________, padded: +00003.142
	A B 0x000000000000000004d2
	65 B
	test

TODO
----

 * Unit Tests
 * Documentation
 * Add license information (will probably be MIT or BSD)
