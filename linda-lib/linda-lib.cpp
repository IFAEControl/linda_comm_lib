#include <iostream>
#include <array>
#include <algorithm>
#include <utility>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "linda-lib.hpp"
#include "commands.hpp"
#include "sockets.hpp"

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
    set_dest_ip(str);
    set_ports(sync_port, async_port);
}

int ReadTemperature(unsigned* temp, unsigned chips_bitmap) {
    if(!temp)
        return -1;

    Temperature cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    *temp = resp.second.getAnswer();
    return resp.first;
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
    TPDACSet cmd(counts);
    auto resp = sendCmd(cmd);
    return resp.first;
}

int ChipRegisterWrite(const unsigned in[5], int chips_bitmap) {
    uint156_t chips_reg = in;
    WriteChipRegister cmd(chips_reg, chips_bitmap);
    auto resp = sendCmd(cmd);
    return resp.first;   
}

int ChipRegisterRead(unsigned out[5], int chips_bitmap) {
    ReadChipRegister cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
}

int PixelRegisterWrite(const unsigned in[480], int chips_bitmap) {
    uint15360_t pixel_reg = in;
    WritePixelRegister cmd(pixel_reg, chips_bitmap);
    auto resp = sendCmd(cmd);
    return resp.first;   
}

int PixelRegisterRead(unsigned out[480], int chips_bitmap) {
    ReadPixelRegister cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
}

int ReadChipID(unsigned *id, int chips_bitmap) {
    if(!id)
        return -1;

    ChipIDRead cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    *id = resp.second.getAnswer();
    return resp.first;
}
