#include <iostream>

#include "linda-lib.hpp"

int main() {
    InitCommunication("172.16.17.94", 32000, 32001);
    unsigned id[30];
    FullArrayReadEricaID(id, 5);
    /*unsigned t;
	ReadTemperature(&t, 1);*/
	//std::cout << t << std::endl;
    /*unsigned temp = 0;
    ReadTemperature(&temp);
    std::cout << temp << std::endl;*/
    //readReg();
}
