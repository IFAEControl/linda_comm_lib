#include <iostream>
#include <unistd.h>
#include "linda-lib.hpp"
#include "log.hpp"

std::size_t counter = 500;

unsigned data[14400];
std::size_t dma_timeouts = 0;
std::size_t irq_err = 0;
std::size_t packets_lost = 0;
unsigned irqs;
unsigned prev_irqs = 0;
AcqInfo info{1000,14,976,false,true,true /* tdi */};
    std::string ip = "172.16.17.94";

int RunACQ(unsigned short frames, unsigned bitmap) {
    if(ACQuisition(info, frames, bitmap) < 0) return -1;
    for(unsigned j = 0; j < frames; j++) {
        for(unsigned i = 0;  GetElemCounter() == 0 && i < 9000; i++) usleep(60);
        //std::cout << "Counter: " << GetElemCounter() << std::endl;

        if(GetTimeoutsCounter() != 0 || (GetElemCounter() != 0 && PopData(data) < 0)) {
            ResetTimeoutsCounter();
            dma_timeouts++;
            std::cout << "Error: DMA TIMEOUT" << std::endl;
            std::cout << "DMA_TIMEOUTS: " << dma_timeouts << " Error on IRQs: " << irq_err << " Packets lost: " << packets_lost << std::endl;
            if(ACQuisition(info, frames-j-1, 1) < 0) return -1;
        } else if(GetElemCounter() == 0) {
            GetDataIRQs(&irqs);
            if(irqs < j) {
                irq_err++;
                std::cout << "Error: IRQs: " << irq_err << std::endl;
                std::cout << "DMA_TIMEOUTS: " << dma_timeouts << " Error on IRQs: " << irq_err << " Packets lost: " << packets_lost << std::endl;
                if(ACQuisition(info, frames-j-1, 1) < 0) return -1;
            } else if(irqs > j) {
                packets_lost++;
                std::cout << "Error: UDP" << std::endl;
                std::cout << "DMA_TIMEOUTS: " << dma_timeouts << " Error on IRQs: " << irq_err << " Packets lost: " << packets_lost << std::endl;
                // packets lost, reconenct just in case
                InitCommunication(ip.c_str(), 32000, 32001);
            }
        }
        std::cout << j << "/" << frames << " frames. Counter=" << counter << "\r";
    }
    std::cout << std::endl;

    return 0;
}

int main(int argc, char* argv[]) {
    if(argc == 2) 
        ip = argv[1];

    InitCommunication(ip.c_str(), 32000, 32001);


    //if(ACQuisitionCont(info, 1) < 0) return -1;
    while(--counter) {
        RunACQ(1000, 1);
    }
    
    std::cout << "\n" << "DMA_TIMEOUTS: " << dma_timeouts << " Error on IRQs: " << irq_err << "Packets lost: " << packets_lost << std::endl;
}
