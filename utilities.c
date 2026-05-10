#include "utilities.h"

void __fastcall__ enableBacklight()
{
    #ifdef BACKLIGHT_412B_C
    R412B = 0xFF;
    R412C = 0x00;
    #elif defined(BACKLIGHT_4138_9_F)
    R413F = 0x1F;
    R4138 = 0x0B;
    R4139 = 0x0F;
    #endif
}

void __fastcall__ enableBacklight_G5()
{
    // Seems to be turning on the backlight + screen??
    unsigned char i = 0;
    i = R4139;
    i = i | 0x02;
    R4139 = i;
    R413F = 0x1F;
}

void __fastcall__ initPPU()
{
    PPU.control = 0x80;
    PPU.mask = 0x0A;
    PPU.scroll = 0x00;
    PPU.scroll = 0x00;
}

void __fastcall__ disablePPU()
{
    PPU.control = 0x00;
    PPU.mask = 0x00;
    PPU.scroll = 0x00;
    PPU.scroll = 0x00;
}

void __fastcall__ initPPU_OneBus()
{
    R2016 = 0x00;
    R2017 = 0x02;
    
    R2012 = 0x04;
    R2013 = 0x05;
    R2014 = 0x06;
    R2015 = 0x07;
    
    R2018 = 0x20;
    R201A = 0x00;
}

void __fastcall__ fillBackgroundZeros()
{
    signed char a0 = PPU.status;
    unsigned int i;
    PPU.vram.address = 0x20;
    PPU.vram.address = 0x00;

    for(i = 0; i < 960; ++i)
    {
        PPU.vram.data = 0x00;
    }
}

void __fastcall__ printText(unsigned PPU_addr, char* buffer)
{
    PPU.vram.address = PPU_addr >> 8;
    PPU.vram.address = PPU_addr & 0xff;
    
    while(*buffer)
        PPU.vram.data = *buffer++;
}

void __fastcall__ itoa3(char* digitsChr, unsigned int a)
{
    unsigned char tensPart;
    digitsChr[0] = (a / 100) + 0x30;
    tensPart = a % 100;
    digitsChr[1] = (tensPart / 10) + 0x30;
    digitsChr[2] = (tensPart % 10) + 0x30;
    digitsChr[3] = '\0';
}

void __fastcall__ placeTile(unsigned int PPU_addr, unsigned char tileNo)
{
    PPU.vram.address = PPU_addr >> 8;
    PPU.vram.address = PPU_addr & 0xff;
    
    PPU.vram.data = tileNo;
}

void __fastcall__ disableRender()
{
    //waitvsync();
    //PPU.control = 0x00;
    PPU.mask &= 0b11110111;
}

void __fastcall__ enableRender()
{
    signed char a0 = 0;
    
    waitvsync();
    
    a0 = PPU.status;
    
    PPU.scroll = 0x00;
    PPU.scroll = 0x00;
  
    //PPU.control = 0x80;
    PPU.mask |= 0b00001000;
}