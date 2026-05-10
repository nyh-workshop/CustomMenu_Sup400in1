#ifndef DEBUGGING_H
#define DEBUGGING_H

#include "menu.h"
#include "utilities.h"

#define ARROW_START_PPUADDR_JUMPTOAPP_DEBUG 0x2084 - 2
#define START_PPUADDR_R20XX_TEXT 0x2084 + 7

void __fastcall__ jumpToApp_Debug_Save_Register(unsigned char select_byte);
void __fastcall__ jumpToApp_Debug_Refresh_Registers();
void __fastcall__ jumpToApp_Debug();

#endif