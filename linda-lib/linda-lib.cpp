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
    #define N_COUNTERS 6


    unsigned int n_frames = 0;
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

int InitCommunication(const char* str, int sync_port, int async_port) {
#ifdef DEBUG
    for (int i=0; i<5; i++)
        chip_register[i] = 0;
    for (int i=0; i<480; i++)
        pixel_register[i] = 0;
#else
    init_thread();
    set_dest_ip(str);
    set_ports(sync_port, async_port);
#endif
    return 0;
}

int CameraReset(){
#ifdef DEBUG
    for (int i=0; i<5; i++)
        chip_register[i] = 0;
    for (int i=0; i<480; i++)
        pixel_register[i] = 0;
    return 0;
#else
    ResetCamera cmd;
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int ReadTemperature(unsigned* temp, int chips_bitmap) {
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

int FullArrayChipRegisterRead(unsigned out[150], int chips_bitmap) {
#ifdef DEBUG
    return 0;
#else
    ReadFullArrayChipRegister cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
#endif
}

int FullArrayPixelRegisterRead(unsigned out[14400], int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
    ReadFullArrayPixelRegister cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
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

int FullArrayReadEricaID(unsigned id[30], int chips_bitmap) {
#ifdef DEBUG
    int enabled_chip = enabled_Chip_Founder(chips_bitmap); 
    *id = chips_ids[enabled_chip];
    return 0;
#else
    if (!id)
        return -1;

    FullChipIDRead cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if (resp.first < 0) return resp.first;

    auto out_arr = resp.second.getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), id);
    return resp.first;
#endif
}

int FullArrayReadTemperature(unsigned temp[30], int chips_bitmap) {
#ifdef DEBUG
    return 0;
#else
    return 0;
#endif
}

void PopFrame(unsigned* data) {
#ifdef DEBUG
    // data = (unsigned*) malloc((n_frames * X_SIZE * Y_SIZE * N_COUNTERS) * sizeof(unsigned)); 
    for (uint32_t j = 0; j < n_frames*N_COUNTERS; j++) {
        for (uint32_t i = 0; i < X_SIZE * Y_SIZE; i++) {
            data[(j * X_SIZE * Y_SIZE) + i] = i + (j*(X_SIZE * Y_SIZE));
        }
    }
#else
    cv_m.lock();
    std::memcpy(data, buffer, bytes);
    cv_m.unlock();
#endif
}

int ACQuisitionCont(AcqInfo info, unsigned* data, int chips_bitmap) {
    ContAcq cmd(info, chips_bitmap);
    auto resp = sendCmd(cmd);
    return resp.first;
}

int ACQuisitionStop() {
    StopAcq cmd;
    auto resp = sendCmd(cmd);
    return resp.first;
}

int ACQuisition(AcqInfo info, unsigned frames, unsigned* data, int chips_bitmap) {
#ifdef DEBUG
    n_frames = frames;
    return 0;
#else
    if (!data)
        return -1;

    NonContAcq cmd(info, frames, chips_bitmap);
    auto resp = sendCmd(cmd);
    if (resp.first < 0) return resp.first;

    //auto out_arr = resp.second.getAnswer();
    //std::copy(out_arr.begin(), out_arr.end(), data);
    auto size = 480*frames;
    auto d = read_bytes(size);
    std::memcpy(data, d.get(), size);

    return resp.first;
#endif
}

int FullArrayACQuisitionTDI(const unsigned params[5], unsigned* data, int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
    return 0;
#endif
}

int FullArrayACQuisitionNonTDI(const unsigned params[5], unsigned* data, int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
    return 0;
#endif
}

int LoadFloodNormFactors(const unsigned in[60], int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
    return 0;
#endif
}

int DiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[480],
    long int size, unsigned* counts, int chips_bitmap) {
#ifdef DEBUG
    return 0;
#else
    return 0;
#endif
}

int FullArrayDiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[14400],
    long int size, unsigned* counts, int chips_bitmap){
#ifdef DEBUG
    return 0;
#else
    return 0;
#endif
}
