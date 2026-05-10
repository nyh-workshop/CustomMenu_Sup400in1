#include "handheld.h"

// Handheld specific settings here:

#ifdef NO_SWAP_DATA_BITS_01
const char handheldModel[] = "SUP VS";
const char memoryChipModel[] = "S29GL256P90";
const char dataBitswap[] = "NONE";
const char opcodeBitswap[] = "NONE";
#define BACKLIGHT_412B_C

#elif defined(SWAP_DATA_BITS_4_TO_5)
const char handheldModel[] = "SUP 400-IN-1";
const char memoryChipModel[] = "K5L2731CAM";
const char dataBitswap[] = "4,5";
const char opcodeBitswap[] = "NONE";
#define BACKLIGHT_4138_9_F

#elif defined(SWAP_DATA_BITS_9_A_TO_0_1__SWAP_OPCODE_BITS_0_TO_1__STARTUP_CUSTOM_ADDR)
const char handheldModel[] = "SUP 400-IN-1";
const char memoryChipModel[] = "S29GL128P10TFI01";
const char dataBitswap[] = "9,10-1,2";
const char opcodeBitswap[] = "0,1";
#define BACKLIGHT_4138_9_F

#elif defined(G5_NO_SWAP_DATA_BITS_SWAP_OPCODE_BITS_6_7_1_2)
const char handheldModel[] = "G5 500-IN-1";
const char memoryChipModel[] = "M36L0R7050T0";
const char dataBitswap[] = "NONE";
const char opcodeBitswap[] = "6,7-1,2";
#define BACKLIGHT_4138_9_F_G5

#else
    #error "Not defined the handheld model in the Makefile!"
#endif