#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "logformat.hpp"

using namespace logformat;

TEST_CASE( "string conversion tests", "[param_to_string]" ) {
	REQUIRE( param_to_string(1.0f) == std::string("1.000000") );
	REQUIRE( param_to_string("string") == std::string("string") );

	class test_t {
		std::string to_string() {
			return std::string("test_t");
		}
	} test_v;
	REQUIRE( param_to_string(test_v) == std::string("test_t") );
}

// vim: syntax=cpp11:sts=4:ts=4:sw=4:noet
