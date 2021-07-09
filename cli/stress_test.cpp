#include <iostream>
#include <unistd.h>

#include "linda-lib.hpp"
#include "log.hpp"

int main(int argc, char* argv[]) {
    unsigned data[14400];
    std::string ip = "172.16.17.103";
    if(argc == 2) 
        ip = argv[1];

    InitCommunication(ip.c_str(), 32000, 32001, true);

    std::size_t counter = 0;
    
    AcqInfo info{1000,1,2500,false,true,false};
    while(++counter) {
        logger->debug("Acq {}", counter);
        ACQuisition(info, 1, 1);
        PopData(data);
        logger->debug("Done. Read: {}, Write: {}", GetReadIdx(), GetWriteIdx());
        sleep(1);
    }
}
