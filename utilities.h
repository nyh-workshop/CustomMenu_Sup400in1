#ifndef UTILITIES_H
#define UTILITIES_H

#include <conio.h>
#include <nes.h>
#include <stdio.h>
#include <stdlib.h>
#include "oneBus.h"
#include "handheld.h"

// Inspired by https://github.com/alavifazel/utilities.h.
void __fastcall__ enableBacklight();
void __fastcall__ enableBacklight_G5();
void __fastcall__ disablePPU();
void __fastcall__ initPPU();
void __fastcall__ initPPU_OneBus();
void __fastcall__ fillBackgroundZeros();
void __fastcall__ printText(unsigned PPU_addr, char* buffer);
void __fastcall__ itoa3(char* digitsChr, unsigned int a);
void __fastcall__ placeTile(unsigned int PPU_addr, unsigned char tileNo);
void __fastcall__ disableRender();
void __fastcall__ enableRender();
void __fastcall__ drawMenuPage(unsigned char aMenuPage);

#endif