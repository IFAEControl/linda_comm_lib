#define CATCH_CONFIG_MAIN

#include <thread>

#include <catch2/catch.hpp>

#include "linda-lib.hpp"

unsigned arr[1920];
std::thread _reader = std::thread(PopData, arr);

TEST_CASE("Simple test", "[lib]") {
   /* REQUIRE(std::get<0>(pid.update(0)) == 0);
    */
    SECTION("Executing init two times doesn't crash") {
        InitCommunication("www.google.es", 80, 80);
        InitCommunication("www.google.es", 80, 80);
        CloseCommunication();
    }

    SECTION("Executing cancel pop data") {
    	CancelPopFrame();
    	_reader.join();
    }

}