#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <formatstring.h>

using namespace formatstring;

BOOST_AUTO_TEST_CASE(format_int) {
	std::string s = format("{}", 12);
    BOOST_CHECK_EQUAL(s, "12");
}

// TODO: write tests
