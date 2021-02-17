#include <iostream>

#include "linda-lib.hpp"

int main() {
    InitCommunication("172.16.17.94", 32000, 32001);
    unsigned arr[5];
    ChipRegisterWrite(arr, 1);
    /*unsigned t;
	ReadTemperature(&t, 1);*/
	//std::cout << t << std::endl;
    /*unsigned temp = 0;
    ReadTemperature(&temp);
    std::cout << temp << std::endl;*/
    //readReg();
}
