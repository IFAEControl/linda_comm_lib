#include <iostream>
#include <array>
#include <algorithm>
#include <utility>
#include <optional>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "linda-lib.hpp"
#include "commands.hpp"
#include "sockets.hpp"
#include "log.hpp"

Networking n;

#ifdef DUMMY
    #define X_SIZE 20
    #define Y_SIZE 8
    #define N_COUNTERS 6
    #define N_WORDS_PIXEL 3

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
std::pair<int, std::optional<T>> sendCmd(T& cmd) try {
    auto resp = n.sendCommand(std::move(cmd));
    logger->trace(resp);
    return {0, std::move(resp)};
} catch(std::exception& e) {
    logger->critical(e.what());
    return {-1, {}};
} catch(...) {
    logger->critical("Unknown error");
    return {-2, {}};
}

int InitCommunication(const char* str, int sync_port, int async_port) {
#ifdef DUMMY
    for (int i=0; i<5; i++)
        chip_register[i] = 0;
    for (int i=0; i<480; i++)
        pixel_register[i] = 0;
#else
    logger->info("Connecting");
    n.configure(str, sync_port, async_port);
    n.initReceiverThread();
#endif
    return 0;
}

void CloseCommunication() {
    n.joinThread();
}


int CameraReset(){
#ifdef DUMMY
    for (int i=0; i<5; i++)
        chip_register[i] = 0;
    for (int i=0; i<480; i++)
        pixel_register[i] = 0;
    return 0;
#else
    CMD::CameraReset cmd;
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int ControllerReset() {
#ifdef DUMMY
    return 0;
#else
    CMD::ControllerReset cmd;
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}


int ReadReadTemperature(unsigned* temp, int chips_bitmap) {
    if(!temp)
        return -1;

#ifdef DUMMY
    *temp = 765;
    return 0;
#else
    CMD::ReadTemperature cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    *temp = resp.second.value().getAnswer();
    return resp.first;
#endif
}

int SetHV(unsigned counts) {
#ifdef DUMMY
    return 0;
#else
    CMD::SetHV cmd(counts);
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int SetTPDAC(unsigned counts) {
#ifdef DUMMY
    return 0;
#else
    CMD::SetTPDAC cmd(counts);
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int ChipRegisterWrite(const unsigned in[5], int chips_bitmap) {
#ifdef DUMMY
    for (int i=0; i<5; i++)
        chip_register[i] = in[i];
    return 0;
#else
    uint156_t chips_reg = in;
    CMD::ChipRegisterWrite cmd(chips_reg, chips_bitmap);
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}
 
int ChipRegisterRead(unsigned out[5], int chips_bitmap) {
#ifdef DUMMY
    for (int i=0; i<5; i++)
        out[i] = chip_register[i];
    return 0;
#else
    CMD::ChipRegisterRead cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.value().getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
#endif
}

int FullArrayChipRegisterRead(unsigned out[150], int chips_bitmap) {
#ifdef DUMMY
    return 0;
#else
    CMD::FullArrayChipRegisterRead cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.value().getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
#endif
}

int FullArrayPixelRegisterRead(unsigned out[14400], int chips_bitmap){
#ifdef DUMMY
    return 0;
#else
    CMD::FullArrayPixelRegisterRead cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.value().getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
#endif
    }

int PixelRegisterWrite(const unsigned in[480], int chips_bitmap) {
#ifdef DUMMY
    for (int i=0; i<480; i++)
        pixel_register[i] = in[i];
    return 0;
#else
    uint15360_t pixel_reg = in;
    CMD::PixelRegisterWrite cmd(pixel_reg, chips_bitmap);
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int PixelRegisterRead(unsigned out[480], int chips_bitmap) {
#ifdef DUMMY
    for (int i=0; i<480; i++)
        out[i] = pixel_register[i];
    return 0;
#else
    CMD::PixelRegisterRead cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    auto out_arr = resp.second.value().getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), out);
    return resp.first;
#endif
}

int ReadEricaID(unsigned *id, int chips_bitmap) {
#ifdef DUMMY
    int enabled_chip = enabled_Chip_Founder(chips_bitmap);
    *id = chips_ids[enabled_chip];
    return 0;
#else
    if(!id)
        return -1;

    CMD::ReadEricaID cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    *id = resp.second.value().getAnswer();
    return resp.first;
#endif
}

int FullArrayReadEricaID(unsigned id[30], int chips_bitmap) {
#ifdef DUMMY
    int enabled_chip = enabled_Chip_Founder(chips_bitmap); 
    *id = chips_ids[enabled_chip];
    return 0;
#else
    if (!id)
        return -1;

    CMD::FullArrayReadEricaID cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if (resp.first < 0) return resp.first;

    auto out_arr = resp.second.value().getAnswer();
    std::copy(out_arr.begin(), out_arr.end(), id);
    return resp.first;
#endif
}

int FullArrayReadReadTemperature(unsigned temp[30], int chips_bitmap) {
#ifdef DUMMY
    return 0;
#else
    return 0;
#endif
}

int PopFrame(unsigned* data) {
#ifdef DUMMY
        for (uint32_t i = 0; i < X_SIZE * Y_SIZE; i++) {
            for (uint32_t k = 0; k < N_WORDS_PIXEL; k++) {
                data[(i * N_WORDS_PIXEL) + k] = i + (i << 16);
            }
        }
    return 0;
#else
    return fb.moveLastFrame(data);
#endif
}

int PopFrames(unsigned* data, unsigned frames) {
#ifdef DUMMY
    for (uint32_t j = 0; j < frames; j++) {
        for (uint32_t i = 0; i < X_SIZE * Y_SIZE; i++) {
            for (uint32_t k = 0; k < N_WORDS_PIXEL; k++) {
                data[(j * X_SIZE * Y_SIZE * N_WORDS_PIXEL) + (i * N_WORDS_PIXEL) + k] = i + (i << 16);
            }
        }
    }
    return 0;
#else
    unsigned bytes = 0;
    for(unsigned i = 0; i < frames; i++) {
        bytes = fb.moveLastFrame(data + bytes);
        if(bytes < 0)
            return bytes;
    }
    return 0;
#endif
}

void CancelPopFrame() {
    fb.cancel();
}


int ACQuisitionCont(AcqInfo info, int chips_bitmap) {
    CMD::ACQuisitionCont cmd(info, chips_bitmap);
    auto resp = sendCmd(cmd);
    return resp.first;
}

int ACQuisitionStop() {
    CMD::ACQuisitionStop cmd;
    auto resp = sendCmd(cmd);
    return resp.first;
}

int ACQuisition(AcqInfo info, unsigned frames, int chips_bitmap) {
#ifdef DUMMY
    n_frames = frames;
    return 0;
#else
    CMD::ACQuisition cmd(info, frames, chips_bitmap);
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int LoadFloodNormFactors(const unsigned in[60], int chips_bitmap){
#ifdef DUMMY
    return 0;
#else
    LongInt<60> factors = in;
    CMD::LoadFloodNormFactors cmd(factors, chips_bitmap);
    auto resp = sendCmd(cmd);
    return resp.first;
#endif
}

int DiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[480],
    long int size, unsigned* counts, int chips_bitmap) {
#ifdef DUMMY
    return 0;
#else
    return 0;
#endif
}

int FullArrayDiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[14400],
    long int size, unsigned* counts, int chips_bitmap){
#ifdef DUMMY
    return 0;
#else
    return 0;
#endif
}


// Internal DLL functions for debugging purposes

void ResetBuffer() {
    fb.reset();
}

unsigned GetWriteIdx() {
    return fb.getWriteFrame();
}

unsigned GetReadIdx() {
    return fb.getReadFrame();
}
