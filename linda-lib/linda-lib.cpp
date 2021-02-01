#include <iostream>
#include <array>
#include <algorithm>
#include <utility>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "linda-lib.hpp"
#include "commands.hpp"
#include "sockets.hpp"

#ifdef DEBUG
#define X_SIZE 20
#define Y_SIZE 8
#define N_COUNTERS 8
#define N_ACQS 8

    unsigned int pixel_register[480];
    unsigned int chip_register[5];
    unsigned int chips_ids[30] = {0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
                                  0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF};

    int enabled_Chip_Founder(unsigned int chips_bitmap){
        bool chip_selected[30];
        for(int i=0; i<30; i++){
            chip_selected[i] = chips_bitmap & (1<<i);
        }
        int enabled_chip = 0;
        for(int i=0; i<30; i++){
            if (chip_selected[i] == true) {
                enabled_chip = i;
                break;
            }
        }
        return enabled_chip;
    }
#endif

template <typename T>
std::pair<int, T> sendCmd(T& cmd) try {
    spdlog::set_level(spdlog::level::debug);
    
    auto resp = send_command(cmd);
    spdlog::debug(resp);
    return {0, resp};
} catch(std::exception& e) {
    spdlog::critical(e.what());
    return {-1, cmd};
} catch(...) {
    spdlog::critical("Unknown error");
    return {-2, cmd};
}

void InitCommunication(const char* str, unsigned sync_port, unsigned async_port) {
#ifdef DEBUG
    for (int i=0; i<5; i++)
        chip_register[i] = 0;
    for (int i=0; i<480; i++)
        pixel_register[i] = 0;
    return strcmp(ip, "172.16.17.189") == 0 && sync_port == 1234 && async_port == 4321 ? 0 : 1;
#else
    set_dest_ip(str);
    set_ports(sync_port, async_port);
#endif
}

int CameraReset(){
#ifdef DEBUG
    for (int i=0; i<5; i++)
        chip_register[i] = 0;
    for (int i=0; i<480; i++)
        pixel_register[i] = 0;
    return 0;
#else
#endif
}

int ReadTemperature(unsigned* temp, unsigned chips_bitmap) {
    if(!temp)
        return -1;

#ifdef DEBUG
    *temp = 765;
    return 0;
#else
    Temperature cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    *temp = resp.second.getAnswer();
    return resp.first;
#endif
}

int SetHV(unsigned counts) {
#ifdef DEBUG
    return 0;
#else
    HVSet cmd(counts);
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int SetTPDAC(unsigned counts) {
#ifdef DEBUG
    return 0;
#else

    TPDACSet cmd(counts);
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int ChipRegisterWrite(const unsigned in[5], int chips_bitmap) {
#ifdef DEBUG
    for (int i=0; i<5; i++)
        chip_register[i] = in[i];
    return 0;
#else
    uint156_t chips_reg = in;
    WriteChipRegister cmd(chips_reg, chips_bitmap);
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int ChipRegisterRead(unsigned out[5], int chips_bitmap) {
#ifdef DEBUG
    for (int i=0; i<5; i++)
        out[i] = chip_register[i];
    return 0;
#else

    ReadChipRegister cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
#endif
}

int FullArrayChipRegisterRead(unsigned int out[150], unsigned int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
#endif
}

int PixelRegisterWrite(const unsigned in[480], int chips_bitmap) {
#ifdef DEBUG
    for (int i=0; i<480; i++)
        pixel_register[i] = in[i];
    return 0;
#else

    uint15360_t pixel_reg = in;
    WritePixelRegister cmd(pixel_reg, chips_bitmap);
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int PixelRegisterRead(unsigned out[480], int chips_bitmap) {
#ifdef DEBUG
    for (int i=0; i<480; i++)
        out[i] = pixel_register[i];
    return 0;
#else
    ReadPixelRegister cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
#endif
}

int ReadEricaID(unsigned *id, int chips_bitmap) {
#ifdef DEBUG
    int enabled_chip = enabled_Chip_Founder(chips_bitmap);
    *id = chips_ids[enabled_chip];
    return 0;
#else
    if(!id)
        return -1;

    ChipIDRead cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    *id = resp.second.getAnswer();
    return resp.first;
#endif
}

int FullArrayReadTemperature(unsigned int temp[30], unsigned int chips_bitmap){
    for(int i = 0; i < 30; i++)
        temp[i]= 750 + i;
    return 0;
}

int ACQuisitionTDI(const unsigned int params[5], unsigned int* data, unsigned int chips_bitmap){
    return 0;
}

int ACQuisitionNonTDI(const unsigned int params[5], unsigned int* data, unsigned int chips_bitmap){
#ifdef DEBUG
    for(int j=0; j<Y_SIZE; j++){
        for(int i=0; i<X_SIZE; i++){
            for(int k=0; k<N_COUNTERS; k++){
                for(int a=0; a < N_ACQS; a++) {
                    int value = (int) (((i + j * X_SIZE * 10.0 * (sin(i * j) + 1)) / 2670.0) * 255);
                    data[i * N_COUNTERS + j * X_SIZE + k * N_ACQS + a] = value;
                }
            }
        }
    }
    return 0;
#else
#endif

}

int FullArrayACQuisitionTDI(const unsigned int params[5], unsigned int* data, unsigned int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
#endif
}

int FullArrayACQuisitionNonTDI(const unsigned int params[5], unsigned int* data, unsigned int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
#endif
}

int LoadFloodNormFactors(const unsigned int in[60], unsigned int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
#endif
}

int DiscCharacF(const unsigned int params[32], const unsigned int reg[20], const unsigned int px_reg[480],
                long int size, unsigned int *counts, unsigned int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
#endif
}

int FullArrayDiscCharacF(const unsigned int params[32], const unsigned int reg[20], const unsigned int px_reg[14400],
                         long int size, unsigned int *counts, unsigned int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
#endif
}
