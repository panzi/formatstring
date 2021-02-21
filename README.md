formatstring
============

A C++14 type safe format string library heavily inspired by Python's str.format()
function.

This is more like a proof of concept, not used by anyone in any production
environment, there are tests, it is not maintained, and it's probably inefficient.
So I don't think you want to use it, but you might want to look at it if you want
to write something similar yourself.

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

MIT License
-----------

Copyright 2021 Mathias Panzenböck

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
