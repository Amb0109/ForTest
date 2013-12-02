#define BOOST_TEST_INCLUDED
#include "boost/test/unit_test.hpp"

int add(int a, int b) {return a+b;}
BOOST_AUTO_TEST_SUITE (test_add)
	BOOST_AUTO_TEST_CASE(test_add_1)
{
	BOOST_CHECK_EQUAL(2, add(1, 2));
}
BOOST_AUTO_TEST_SUITE_END ()
