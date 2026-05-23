#ifndef MENU_H
#define MENU_H

// Jump to app debug:
// #define JUMP_TO_APP_DEBUG

// Menu defines:
#define RIGHT_ARROW_TILE 0x5B
#define ARROW_START_PPUADDR 0x20C4
#define ARROW_END_PPUADDR 0x2304
#define PPU_TILE_NEWLINE 0x40

// Buttons defines:
#define BUTTON_RIGHT (1<<0)
#define BUTTON_LEFT (1<<1)
#define BUTTON_DOWN (1<<2)
#define BUTTON_UP (1<<3)
#define BUTTON_START (1<<4)
#define BUTTON_SELECT (1<<5)
#define BUTTON_B (1<<6)
#define BUTTON_A (1<<7)

// JumpToApp Debug State:
// 0 -> Select registers in menu.
// 1,2 -> Select digits in the register.
#define JUMPTOAPP_DEBUG_STATE_SEL_REG 0
#define JUMPTOAPP_DEBUG_STATE_REG_SEL_DIGIT0 1
#define JUMPTOAPP_DEBUG_STATE_REG_SEL_DIGIT1 2

#endif