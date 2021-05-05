#include <iostream>

#include "linda-lib.hpp"
#include "log.hpp"

int main(int argc, char* argv[]) {
    unsigned data[14400];
    std::string ip = "172.16.17.94";
    if(argc == 2) 
        ip = argv[1];

    InitCommunication(ip.c_str(), 32000, 32001);

    std::size_t counter = 0;

    while(++counter) {
        logger->debug("Acq {}", counter);
        AcqInfo info{1000,4,2500,false,true,false};
        ACQuisition(info, 1, 1);
        PopFrame(data);
        logger->debug("Done. Read: {}, Write: {}", GetReadIdx(), GetWriteIdx());
    }
}
