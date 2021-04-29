#pragma once

#include "header.hpp"

#ifdef _WIN32
#define DllExport   __declspec( dllexport )
#else
#define DllExport   /* Cause Windows is so weird and unpredictable */
#endif

extern "C" DllExport int InitCommunication(const char* str, int sync_port, int async_port);
extern "C" DllExport void CloseCommunication();
extern "C" DllExport int CameraReset();
extern "C" DllExport int ControllerReset();
extern "C" DllExport int ChipRegisterWrite(const unsigned in[5], int chips_bitmap);
extern "C" DllExport int ChipRegisterRead(unsigned out[5], int chips_bitmap);
extern "C" DllExport int FullArrayChipRegisterRead(unsigned out[150], int chips_bitmap);
extern "C" DllExport int PixelRegisterWrite(const unsigned in[480], int chips_bitmap);
extern "C" DllExport int PixelRegisterRead(unsigned out[480], int chips_bitmap);
extern "C" DllExport int FullArrayPixelRegisterRead(unsigned out[14400], int chips_bitmap);
extern "C" DllExport int ReadEricaID(unsigned *id, int chips_bitmap);
extern "C" DllExport int FullArrayReadEricaID(unsigned id[30], int chips_bitmap);
extern "C" DllExport int ReadReadTemperature(unsigned *temp, int chips_bitmap);
extern "C" DllExport int FullArrayReadReadTemperature(unsigned temp[30], int chips_bitmap);
extern "C" DllExport int ACQuisition(AcqInfo info, unsigned frames, int chips_bitmap);
extern "C" DllExport int ACQuisitionCont(AcqInfo info, int chips_bitmap);

/// @brief copies last frame to provided pointer
/// @return 0 in case of success, -1 if a cancel has been requested
extern "C" DllExport int PopFrame(unsigned* data);
extern "C" DllExport int PopFrames(unsigned* data, unsigned frames);
extern "C" DllExport void CancelPopFrame();
extern "C" DllExport int ACQuisitionStop();
extern "C" DllExport int LoadFloodNormFactors(const unsigned in[60], int chips_bitmap);
extern "C" DllExport int SetHV(unsigned counts);
extern "C" DllExport int SetTPDAC(unsigned counts);
extern "C" DllExport int DiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[480],
	long int size, unsigned *counts, int chips_bitmap);
extern "C" DllExport int FullArrayDiscCharacF(const unsigned params[32], const unsigned reg[20], const unsigned px_reg[14400],
	long int size, unsigned *counts, int chips_bitmap);

// Internal DLL functions for debugging purposes
extern "C" DllExport void ResetBuffer();
extern "C" DllExport unsigned GetWriteIdx();
extern "C" DllExport unsigned GetReadIdx();

