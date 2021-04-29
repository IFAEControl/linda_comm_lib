#pragma once

#include "header.hpp"

extern "C" int InitCommunication(const char* str, int sync_port, int async_port);
extern "C" void CloseCommunication();
extern "C" int CameraReset();
extern "C" int ControllerReset();
extern "C" int ChipRegisterWrite(const unsigned in[5], int chips_bitmap);
extern "C" int ChipRegisterRead(unsigned out[5], int chips_bitmap);
extern "C" int FullArrayChipRegisterRead(unsigned out[150], int chips_bitmap);
extern "C" int PixelRegisterWrite(const unsigned in[480], int chips_bitmap);
extern "C" int PixelRegisterRead(unsigned out[480], int chips_bitmap);
extern "C" int FullArrayPixelRegisterRead(unsigned out[14400], int chips_bitmap);
extern "C" int ReadEricaID(unsigned *id, int chips_bitmap);
extern "C" int FullArrayReadEricaID(unsigned id[30], int chips_bitmap);
extern "C" int ReadReadTemperature(unsigned *temp, int chips_bitmap);
extern "C" int FullArrayReadReadTemperature(unsigned temp[30], int chips_bitmap);
extern "C" int ACQuisition(AcqInfo info, unsigned frames, int chips_bitmap);
extern "C" int ACQuisitionCont(AcqInfo info, int chips_bitmap);

/// @brief copies last frame to provided pointer
/// @return 0 in case of success, -1 if a cancel has been requested
extern "C" int PopFrame(unsigned* data);
extern "C" int PopFrames(unsigned* data, unsigned frames);
extern "C" void CancelPopFrame();
extern "C" int ACQuisitionStop();
extern "C" int LoadFloodNormFactors(const unsigned in[60], int chips_bitmap);
extern "C" int SetHV(unsigned counts);
extern "C" int SetTPDAC(unsigned counts);
extern "C" int DiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[480],
	long int size, unsigned *counts, int chips_bitmap);
extern "C" int FullArrayDiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[14400],
	long int size, unsigned *counts, int chips_bitmap);

// Internal DLL functions for debugging purposes
extern "C" void ResetBuffer();
extern "C" unsigned GetWriteIdx();
extern "C" unsigned GetReadIdx();

