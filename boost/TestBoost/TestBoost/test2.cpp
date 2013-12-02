#define BOOST_TEST_INCLUDED
#include "boost/test/unit_test.hpp"

int sub(int a, int b) {return a-b;}
BOOST_AUTO_TEST_SUITE (test_sub)
	BOOST_AUTO_TEST_CASE(test_sub_1)
{
	BOOST_CHECK_EQUAL(-1, sub(1, 2));
}
BOOST_AUTO_TEST_SUITE_END ()
