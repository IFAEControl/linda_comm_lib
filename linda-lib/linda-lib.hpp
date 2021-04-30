#pragma once

#include "header.hpp"

extern "C" {

int InitCommunication(const char* str, int sync_port, int async_port);
void CloseCommunication();
int CameraReset();
int ControllerReset();
int ChipRegisterWrite(const unsigned in[5], int chips_bitmap);
int ChipRegisterRead(unsigned out[5], int chips_bitmap);
int FullArrayChipRegisterRead(unsigned out[150], int chips_bitmap);
int PixelRegisterWrite(const unsigned in[480], int chips_bitmap);
int PixelRegisterRead(unsigned out[480], int chips_bitmap);
int FullArrayPixelRegisterRead(unsigned out[14400], int chips_bitmap);
int ReadEricaID(unsigned *id, int chips_bitmap);
int FullArrayReadEricaID(unsigned id[30], int chips_bitmap);
int ReadReadTemperature(unsigned *temp, int chips_bitmap);
int FullArrayReadReadTemperature(unsigned temp[30], int chips_bitmap);
int ACQuisition(AcqInfo info, unsigned frames, int chips_bitmap);
int ACQuisitionCont(AcqInfo info, int chips_bitmap);

/// @brief copies last frame to provided pointer
/// @return 0 in case of success, -1 if a cancel has been requested
int PopFrame(unsigned* data);
int PopFrames(unsigned* data, unsigned frames);
void CancelPopFrame();
int ACQuisitionStop();
int LoadFloodNormFactors(const unsigned in[60], int chips_bitmap);
int SetHV(unsigned counts);
int SetTPDAC(unsigned counts);
int DiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[480],
	long int size, unsigned *counts, int chips_bitmap);
int FullArrayDiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[14400],
	long int size, unsigned *counts, int chips_bitmap);

// Internal DLL functions for debugging purposes
void ResetBuffer();
unsigned GetWriteIdx();
unsigned GetReadIdx();

} // end extern "C"