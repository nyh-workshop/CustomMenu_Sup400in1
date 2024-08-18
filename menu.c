#include <conio.h>
#include <nes.h>
#include <stdio.h>
#include <stdlib.h>
#include "oneBus.h"
#include "jumpToApp.h"
#include "appList.h"

#define RIGHT_ARROW_TILE 0x5B
#define ARROW_START_PPUADDR 0x20C4
#define ARROW_END_PPUADDR 0x2304

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
unsigned char zR2012, zR2013, zR2014, zR2015, zR2016, zR2017, zR2018, zR201A;
unsigned char zR4100, zR4105, zR4106, zR4107, zR4108, zR4109, zR410A, zR410B;
unsigned char zRvct_L, zRvct_H;
#pragma data-name (pop)
#pragma bss-name (pop)

// Texts for menu:
const char menuTitle[] = "SUP 400-IN-1";
const char menuTitleHeader[] = "2020-2024 YH WORKSHOP";
const char menuVersion[] = "0.6";

void __fastcall__ enableBacklight();
void __fastcall__ disablePPU();
void __fastcall__ initPPU();
void __fastcall__ initPPU_OneBus();
void __fastcall__ fillBackgroundZeros();
void __fastcall__ loadMenuPalette();
void __fastcall__ printText(unsigned PPU_addr, char* buffer);
void __fastcall__ itoa3(char* digitsChr, unsigned int a);
void __fastcall__ placeTile(unsigned int PPU_addr, unsigned char tileNo);
void __fastcall__ disableRender();
void __fastcall__ enableRender();
void __fastcall__ drawMenuPage(unsigned char aMenuPage);

const char numberOfPages = 2;
signed char menuPage = 0;
unsigned char menuSelectItem10 = 0;
unsigned int menuSelect = 0;
unsigned char button1 = 0;
unsigned int menuArrowPositionPPUaddr = ARROW_START_PPUADDR;
const unsigned int totalNumOfApps = sizeof(appTitleList)/2;

unsigned char manual_NMI = 0;
unsigned char button1PressEvt = 0;

const unsigned char menuPaletteData[] =
{
    // Background:
    0x0e,0x0e,0x2a,0x0e,
    0x0e,0x0e,0x2a,0x0e,
    0x0e,0x0e,0x2a,0x0e,
    0x0e,0x0e,0x2a,0x0e,
    // Sprite:
    0x0e,0x0e,0x2a,0x0e,
    0x0e,0x0e,0x2a,0x0e,
    0x0e,0x0e,0x2a,0x0e,
    0x0e,0x0e,0x2a,0x0e,
};

char text1[64];
unsigned char i = 0;

int main (void)
{
    waitvsync();
    waitvsync();
    waitvsync();
    enableBacklight();
    initPPU_OneBus();
  
    disablePPU();
    loadMenuPalette();
    fillBackgroundZeros();
    placeTile(ARROW_START_PPUADDR, RIGHT_ARROW_TILE);
    drawMenuPage(0);
    initPPU();

    while(1)
    {
        if(button1PressEvt)
        {
            // Down button:
            if(button1 == 0x04)
            {
                disableRender();
                placeTile(menuArrowPositionPPUaddr, ' ');
                menuArrowPositionPPUaddr += 0x40;
                ++menuSelectItem10;
                                
                if(menuArrowPositionPPUaddr > ARROW_END_PPUADDR)
                {
                    // Go to next page!
                    placeTile(menuArrowPositionPPUaddr, ' ');
                    menuArrowPositionPPUaddr = ARROW_START_PPUADDR;
                    menuSelectItem10 = 0;
                    ++menuPage;
                    if(menuPage > numberOfPages)
                        menuPage = 0;
                    drawMenuPage(menuPage);
                }
                menuSelect = (menuPage * 10) + menuSelectItem10;
                placeTile(menuArrowPositionPPUaddr, RIGHT_ARROW_TILE);
                enableRender();
                button1PressEvt = 0;
                
            }
            // Up button:
            else if(button1 == 0x08)
            {
                disableRender();
                placeTile(menuArrowPositionPPUaddr, ' ');
                menuArrowPositionPPUaddr -= 0x40;
                --menuSelectItem10;
                
                if(menuArrowPositionPPUaddr < ARROW_START_PPUADDR)
                {
                    // Go to previous page!
                    placeTile(ARROW_START_PPUADDR, ' ');
                    menuArrowPositionPPUaddr = ARROW_END_PPUADDR;
                    menuSelectItem10 = 9;
                    --menuPage;
                    if(menuPage < 0)
                        menuPage = numberOfPages;
                    drawMenuPage(menuPage);
                }
                menuSelect = (menuPage * 10) + menuSelectItem10;
                placeTile(menuArrowPositionPPUaddr, RIGHT_ARROW_TILE);
                enableRender();
                button1PressEvt = 0;
            }
            // Right button:
            else if(button1 == 0x01)
            {
                disableRender();
                ++menuPage;
                if(menuPage > numberOfPages)
                    menuPage = 0;
                drawMenuPage(menuPage);
                menuSelect = (menuPage * 10) + menuSelectItem10;
                enableRender();
                button1PressEvt = 0;
            }
            // Left button:
            else if(button1 == 0x02)
            {
                disableRender();
                --menuPage;
                if(menuPage < 0)
                    menuPage = numberOfPages;
                drawMenuPage(menuPage);
                menuSelect = (menuPage * 10) + menuSelectItem10;
                enableRender();
                button1PressEvt = 0;
            }
            // Start button:
            else if(button1 == 0x10)
            {
                if(menuSelect > (totalNumOfApps - 1))
                {
                    continue;
                }
                else
                {
                    // Wipe screen before jumping to new game:
                    disableRender();
                    fillBackgroundZeros();                    
                    enableRender();
                    
                    // Jump to application:
                    zR2012 = (menuItemProperties[menuSelect])[0];
                    zR2013 = (menuItemProperties[menuSelect])[1];
                    zR2014 = (menuItemProperties[menuSelect])[2];
                    zR2015 = (menuItemProperties[menuSelect])[3];
                    zR2016 = (menuItemProperties[menuSelect])[4];
                    zR2017 = (menuItemProperties[menuSelect])[5];
                    zR2018 = (menuItemProperties[menuSelect])[6];
                    zR201A = (menuItemProperties[menuSelect])[7];
                    
                    zR4100 = (menuItemProperties[menuSelect])[8];
                    zR4105 = (menuItemProperties[menuSelect])[9];
                    zR4106 = (menuItemProperties[menuSelect])[10];
                    zR4107 = (menuItemProperties[menuSelect])[11];
                    zR4108 = (menuItemProperties[menuSelect])[12];
                    zR4109 = (menuItemProperties[menuSelect])[13];
                    zR410A = (menuItemProperties[menuSelect])[14];
                    zR410B = (menuItemProperties[menuSelect])[15];
                    
                    zRvct_L = (resetVectors[menuSelect])[0];
                    zRvct_H = (resetVectors[menuSelect])[1];
                    
                    jumpToApp();
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

void __fastcall__ drawMenuPage(unsigned char aMenuPage)
{
    unsigned char i = 0;
    unsigned int menuSelect = 0;
    unsigned int PPU_addrMenuNum = 0x20C6;
    char* textToPrint;
    
    sprintf(text1, menuTitle);
    printText(0x204A, text1);
        
    sprintf(text1, menuTitleHeader);
    printText(0x2086, text1);
    
    for(i = 0; i < 10; ++i)
    {
        menuSelect = (aMenuPage * 10) + i;
        if(menuSelect < totalNumOfApps)
        {
            textToPrint = (char*)appTitleList[menuSelect];
        }
        else
        {
            textToPrint = (char*)emptyTitle;
        }
        sprintf(text1, "%d. %s", menuSelect + 1, textToPrint);
        printText(PPU_addrMenuNum, "                   ");
        printText(PPU_addrMenuNum, text1);
        PPU_addrMenuNum += 0x40;
    }

    sprintf(text1, menuVersion);
    printText(0x23BD, text1);
}


void __fastcall__ enableBacklight()
{
    R413F = 0x1F;
    R4138 = 0x0B;
    R4139 = 0x0F;
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

void __fastcall__ loadMenuPalette()
{
    signed char a0 = PPU.status;
    unsigned char i;
    PPU.vram.address = 0x3f;
    PPU.vram.address = 0x00;
    
    for(i = 0; i < 32; ++i)
    {
        PPU.vram.data = menuPaletteData[i];
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