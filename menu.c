#include "oneBus.h"
#include "jumpToApp.h"
#include "appList.h"
#include "menu.h"
#include "utilities.h"
#include "debugging.h"

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
unsigned char zR2012, zR2013, zR2014, zR2015, zR2016, zR2017, zR2018, zR201A;
unsigned char zR4100, zR4105, zR4106, zR4107, zR4108, zR4109, zR410A, zR410B;
unsigned char zR4118;
unsigned char zRvct_L, zRvct_H;
#pragma data-name (pop)
#pragma bss-name (pop)

unsigned char tft_init_vars[16];

// Texts for menu:
#if defined(G5_NO_SWAP_DATA_BITS_SWAP_OPCODE_BITS_6_7_1_2)
const char menuTitle[] = "G5 500-IN-1";
#else
const char menuTitle[] = "SUP 400-IN-1";
#endif
const char menuTitleHeader[] = "2020-2026 YH WORKSHOP";
const char menuVersion[] = "1.3";

void __fastcall__ loadMenuPalette();

const char numberOfPages = 8;
signed char menuPage = 0;
unsigned char menuSelectItem10 = 0;
unsigned int menuSelect = 0;
unsigned char button1 = 0;
unsigned int menuArrowPositionPPUaddr = ARROW_START_PPUADDR;
const unsigned int totalNumOfApps = sizeof(appTitleList)/2;

unsigned char manual_NMI = 0;
unsigned char button1PressEvt = 0;
unsigned char isAboutScreen = 0;

// Can select fonts: check in the fonts folder!
const unsigned char menuPaletteData[] =
{
    // Background:
    0x0e,0x3c,0x0e,0x0e,
    0x0e,0x3c,0x0e,0x0e,
    0x0e,0x3c,0x0e,0x0e,
    0x0e,0x3c,0x0e,0x0e,
    // Sprite:
    0x0e,0x0e,0x2a,0x0e,
    0x0e,0x0e,0x2a,0x0e,
    0x0e,0x0e,0x2a,0x0e,
    0x0e,0x0e,0x2a,0x0e,
};

char text1[64];
unsigned char i = 0;

// This is the "Type 3" TFT init that is described in the ROM dump analysis:
// https://nyh-workshop.github.io/Custom-ROM-Sup-Game-Box-400in1/ROM_dump_analysis
// Might not work for other types, or ones that doesn't keep these TFT init values in
// 0x110~0x116 and 0xF8 + 0xF9.
void get_tft_init_values()
{
    tft_init_vars[0] = *((unsigned char*)0x0480);
    tft_init_vars[1] = *((unsigned char*)0x0481);
    tft_init_vars[2] = *((unsigned char*)0x0482);
    tft_init_vars[4] = *((unsigned char*)0x0484);
    tft_init_vars[6] = *((unsigned char*)0x0486);
    tft_init_vars[8] = *((unsigned char*)0x0488);
    tft_init_vars[9] = *((unsigned char*)0x0489);
}

int main (void)
{
    get_tft_init_values();

    waitvsync();
    waitvsync();
    waitvsync();
    #if defined(G5_NO_SWAP_DATA_BITS_SWAP_OPCODE_BITS_6_7_1_2)
    enableBacklight_G5();
    #else
    enableBacklight();
    #endif
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
            if(!isAboutScreen)
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
                        if(zR4105 & 0x01) {
                            zR4105 = zR4105 & 0xFE;
                            zR4118 = 0x80;
                        }
                        else {
                            zR4118 = 0x00;
                        }
                        zR4106 = (menuItemProperties[menuSelect])[10];
                        zR4107 = (menuItemProperties[menuSelect])[11];
                        zR4108 = (menuItemProperties[menuSelect])[12];
                        zR4109 = (menuItemProperties[menuSelect])[13];
                        zR410A = (menuItemProperties[menuSelect])[14];
                        zR410B = (menuItemProperties[menuSelect])[15];
                        
                        zRvct_L = (resetVectors[menuSelect])[0];
                        zRvct_H = (resetVectors[menuSelect])[1];
                        
                        #ifdef JUMP_TO_APP_DEBUG
                        jumpToApp_Debug();
                        #endif
                        jumpToApp();
                    }
                }
                // Select button:
                else if(button1 == 0x20)
                {
                    // Wipe screen before jumping to the about screen:
                    disableRender();
                    fillBackgroundZeros();                    
                    
                    sprintf(text1, "ABOUT HANDHELD");
                    printText(0x204A, text1);
                    
                    sprintf(text1, "MODEL:");
                    printText(0x20C2, text1);
                    sprintf(text1, handheldModel);
                    printText(0x20C9, text1);
                    
                    sprintf(text1, "FLASH MEM:");
                    printText(0x2102, text1);
                    sprintf(text1, memoryChipModel);
                    printText(0x210D, text1);
                    
                    sprintf(text1, "DATA BITSWAP:");
                    printText(0x2142, text1);
                    sprintf(text1, dataBitswap);
                    printText(0x2150, text1);
                    
                    sprintf(text1, "OPCODE BITSWAP:");
                    printText(0x2182, text1);
                    sprintf(text1, opcodeBitswap);
                    printText(0x2192, text1);

                    // For Type 3, TFT init possibly uses 0x110,0x111,0x112,0x114,0x116,0x0F8 and 0x0F9!
                    sprintf(text1, "TFT DEBUG:");
                    printText(0x21C2, text1);
                    sprintf(text1, "%02X %02X %02X %02X", tft_init_vars[0], tft_init_vars[1], tft_init_vars[2], tft_init_vars[4]);
                    printText(0x21CD, text1);
                    sprintf(text1, "%02X %02X %02X", tft_init_vars[6], tft_init_vars[8], tft_init_vars[9]);
                    printText(0x220D, text1);
                    
                    sprintf(text1, "PRESS RESET TO RETURN TO MENU");
                    printText(0x2281, text1);
                    
                    enableRender();
                    button1PressEvt = 0;
                    isAboutScreen = 1;
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
        PPU_addrMenuNum += PPU_TILE_NEWLINE;
    }

    sprintf(text1, menuVersion);
    printText(0x23BD-2, text1);
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
