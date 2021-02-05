#pragma once

#ifdef _WIN32
#define DllExport   __declspec( dllexport )
#else
#define DllExport   /* Why windows is so weird and unpredictable */
#endif


extern "C" DllExport int InitCommunication(const char* str, int sync_port, int async_port);
extern "C" DllExport int CameraReset();
extern "C" DllExport int ChipRegisterWrite(const unsigned in[5], int chips_bitmap);
extern "C" DllExport int ChipRegisterRead(unsigned out[5], int chips_bitmap);
extern "C" DllExport int FullArrayChipRegisterRead(unsigned out[150], int chips_bitmap);
extern "C" DllExport int PixelRegisterWrite(const unsigned in[480], int chips_bitmap);
extern "C" DllExport int PixelRegisterRead(unsigned out[480], int chips_bitmap);
extern "C" DllExport int FullArrayPixelRegisterRead(unsigned out[14400], int chips_bitmap);
extern "C" DllExport int ReadEricaID(unsigned *id, int chips_bitmap);
extern "C" DllExport int FullArrayReadEricaID(unsigned id[30], int chips_bitmap);
extern "C" DllExport int ReadTemperature(unsigned *temp, int chips_bitmap);
extern "C" DllExport int FullArrayReadTemperature(unsigned *temp[30], int chips_bitmap);
extern "C" DllExport int ACQuisitionTDI(const unsigned params[5], unsigned* data, int chips_bitmap);
extern "C" DllExport int ACQuisitionNonTDI(const unsigned params[5], unsigned* data, int chips_bitmap);
extern "C" DllExport int FullArrayACQuisitionTDI(const unsigned params[5], unsigned* data, int chips_bitmap);
extern "C" DllExport int FullArrayACQuisitionNonTDI(const unsigned params[5], unsigned* data, int chips_bitmap);
extern "C" DllExport int LoadFloodNormFactors(const unsigned in[60], int chips_bitmap);
extern "C" DllExport int SetHV(unsigned counts);
extern "C" DllExport int SetTPDAC(unsigned counts);
extern "C" DllExport int DISCcharacF(const char params[32], const char reg[20], const char px_reg[480],
										long size, char *counts, int idx);
extern "C" DllExport int FullArrayDiscCharacF(const unsigned int params[32], const unsigned int reg[20], const unsigned int px_reg[14400],
	long int size, unsigned int* counts, unsigned int chips_bitmap);

