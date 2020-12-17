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

int ChipConfigRegisterWrite(const unsigned in[3], unsigned out[3]) {
    uint96_t chip_config = in;
    WriteChipConfig cmd(chip_config);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
}

int PixelConfigRegisterWrite(const unsigned in[560], unsigned out[560]) {
    uint17920_t pixel_config = in;
    WritePixelConfig cmd(pixel_config);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
}

int PixelPulseRegisterWrite(const unsigned in[35], unsigned out[35]) {
    uint1120_t pixel_pulse = in;
    PixelPulseWrite cmd(pixel_pulse);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
}


int GeneratePulses() {
    PulsesGenerate cmd;
    auto resp = sendCmd(cmd);
    return resp.first;
}

int ReadTemperature(unsigned* temp) {
    if(!temp)
        return -1;

    Temperature cmd;
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    *temp = resp.second.getAnswer();
    return resp.first;
}

void initCommunication(const char* str, unsigned sync_port, unsigned async_port) {
    set_dest_ip(str);
}

int LnaHpfReset(unsigned wait_time_us) {
    ResetLnaHpf cmd(wait_time_us);
    auto resp = sendCmd(cmd);
    return resp.first;
}

int LnaNeuronDriving(unsigned wait_time_us, unsigned reset_time_us, bool disable_reset) {
    NeuronDrivingLna cmd(wait_time_us, reset_time_us, disable_reset);
    auto resp = sendCmd(cmd);
    return resp.first;
}

int LnaNoNeuronDriving(unsigned wait_time_us, unsigned reset_time_us, bool disable_reset) {
    NoNeuronDrivingLna cmd(wait_time_us, reset_time_us, disable_reset);
    auto resp = sendCmd(cmd);
    return resp.first;
}

int PllOutResetStatus(unsigned* status) {
    if(!status)
        return -1;
    
    StatusPllOutReset cmd;
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    *status = resp.second.getAnswer();
    return resp.first;
}

int PllBitStreamGenerator(unsigned mode) {
    ModePllBitstream cmd(mode);
    auto resp = sendCmd(cmd);
    return resp.first;
}

int StartLnaThread(bool neuron_driving, unsigned period_ms, unsigned wait_time_us, unsigned reset_time_us,
                    bool disable_reset) {
    StartLna cmd(neuron_driving, period_ms, wait_time_us, reset_time_us, disable_reset);
    auto resp = sendCmd(cmd);
    return resp.first;
}

int StopThread() {
    StopLna cmd;
    auto resp = sendCmd(cmd);
    return resp.first;
}

