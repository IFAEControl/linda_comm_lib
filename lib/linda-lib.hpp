#pragma once

#include "header.hpp"

#ifdef _WIN32
#define DllExport   __declspec( dllexport )
#else
#define DllExport   /* Cause Windows is so weird and unpredictable */
#endif

extern "C" {

DllExport int InitCommunication(const char* str, int sync_port, int async_port);
DllExport void CloseCommunication();
DllExport int CameraReset();
DllExport int ControllerReset();
DllExport int ChipRegisterWrite(const unsigned in[5], int chips_bitmap);
DllExport int ChipRegisterRead(unsigned out[5], int chips_bitmap);
DllExport int FullArrayChipRegisterRead(unsigned out[150], int chips_bitmap);
DllExport int PixelRegisterWrite(const unsigned in[480], int chips_bitmap);
DllExport int PixelRegisterRead(unsigned out[480], int chips_bitmap);
DllExport int FullArrayPixelRegisterRead(unsigned out[14400], int chips_bitmap);
DllExport int ReadEricaID(unsigned *id, int chips_bitmap);
DllExport int FullArrayReadEricaID(unsigned id[30], int chips_bitmap);
DllExport int ReadTemperature(unsigned *temp, int chips_bitmap);
DllExport int FullArrayReadReadTemperature(unsigned temp[30], int chips_bitmap);
DllExport int ACQuisition(AcqInfo info, unsigned frames, int chips_bitmap);
DllExport int ACQuisitionCont(AcqInfo info, int chips_bitmap);

/// @brief copies last frame to provided pointer
/// @return 0 in case of success, -1 if a cancel has been requested
DllExport int PopFrame(unsigned* data);
DllExport int PopFrames(unsigned* data, unsigned frames);
DllExport void CancelPopFrame();
DllExport int ACQuisitionStop();
DllExport int LoadFloodNormFactors(const unsigned in[60], int chips_bitmap);
DllExport int SetHV(unsigned counts);
DllExport int SetTPDAC(unsigned counts);
DllExport int DiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[480],
	long int size, unsigned *counts, int chips_bitmap);
DllExport int FullArrayDiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[14400],
	long int size, unsigned *counts, int chips_bitmap);

// Internal DLL functions for debugging purposes
DllExport void ResetBuffer();
DllExport unsigned GetWriteIdx();
DllExport unsigned GetReadIdx();

} // end extern "C"