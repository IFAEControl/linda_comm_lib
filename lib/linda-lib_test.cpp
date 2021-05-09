#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "linda-lib.hpp"

TEST_CASE("Simple test", "[lib]") {
   /* REQUIRE(std::get<0>(pid.update(0)) == 0);
    */
    SECTION("Executing init two times doesn't crash") {
        InitCommunication("www.google.es", 80, 80);
        InitCommunication("www.google.es", 80, 80);
        CloseCommunication();
    }

}