#include <iostream>
#include <unistd.h>
#include "linda-lib.hpp"
#include "log.hpp"

std::size_t counter = 1000;

unsigned data[14400];
std::size_t dma_timeouts = 0;
std::size_t irq_err = 0;
std::size_t packets_lost = 0;
unsigned irqs;
unsigned prev_irqs = 0;
AcqInfo info{1000,14,3000,false,true,true /* tdi */};
    std::string ip = "172.16.17.94";

int RunACQ(unsigned short frames, unsigned bitmap) {

    if(ACQuisitionCont(info, bitmap) < 0) return -1;
    //for(unsigned j = 0; j < frames; j++) {
    counter = 0;
    while(true) {
        for(unsigned i = 0;  GetElemCounter() == 0 && i < 9000; i++) usleep(60);
        //std::cout << "Counter: " << GetElemCounter() << std::endl;

        unsigned elem_count = GetElemCounter();
        if((elem_count != 0 && PopData(data) < 0) || GetTimeoutsCounter() > 0) {
            ResetTimeoutsCounter();
            dma_timeouts++;
            std::cout << "Error: DMA TIMEOUT" << std::endl;
            std::cout << "DMA_TIMEOUTS: " << dma_timeouts << " Error on IRQs: " << irq_err << " Packets lost: " << packets_lost << std::endl;
            //if(ACQuisition(info, frames-j-1, 1) < 0) return -1;
        } else if(elem_count == 0) {
            GetDataIRQs(&irqs);
            if(irqs <= prev_irqs) {
                irq_err++;
                std::cout << "Error: IRQs: " << irq_err << std::endl;
                std::cout << "DMA_TIMEOUTS: " << dma_timeouts << " Error on IRQs: " << irq_err << " Packets lost: " << packets_lost << std::endl;
                if(ACQuisitionCont(info, bitmap) < 0) return -1;
                //if(ACQuisition(info, frames-j-1, 1) < 0) return -1;
            } /*else if(irqs > prev_irqs) {
                packets_lost++;
                std::cout << "Error: UDP. " << irqs << " " << prev_irqs << std::endl;
                std::cout << "DMA_TIMEOUTS: " << dma_timeouts << " Error on IRQs: " << irq_err << " Packets lost: " << packets_lost << std::endl;
                // packets lost, reconenct just in case
                //InitCommunication(ip.c_str(), 32000, 32001);
            }*/
            prev_irqs = irqs;
        }
        std::cout << "Counter=" << ++counter << "\r";
        //std::cout << j << "/" << frames << " frames. Counter=" << ++counter << "\r";
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if(argc == 2) 
        ip = argv[1];

    InitCommunication(ip.c_str(), 32000, 32001);

    RunACQ(1, 1);
    return 0;
    while(--counter) {
        RunACQ(1000, 1);
    }
    std::cout << "\n" << "First phase completed. DMA_TIMEOUTS: " << dma_timeouts << " Error on IRQs: " << irq_err << "Packets lost: " << packets_lost << std::endl;

    counter = 1000 * 500;
    while(--counter) {
        RunACQ(1, 1);
    }
    
    std::cout << "\n" << "Second phase completed. DMA_TIMEOUTS: " << dma_timeouts << " Error on IRQs: " 
              << irq_err << "Packets lost: " << packets_lost << std::endl;

    counter = 1000 * 500;
    while(--counter) {
        RunACQ(1, 5);
    }
    
    std::cout << "\n" << "Third phase completed. DMA_TIMEOUTS: " << dma_timeouts << " Error on IRQs: " 
              << irq_err << "Packets lost: " << packets_lost << std::endl;
}
