#include "debugging.h"

// Externs go here:
// Zero pages of these OneBus Registers:
extern unsigned char zR2012;
#pragma zpsym ("zR2012");
extern unsigned char zR2013;
#pragma zpsym ("zR2013"); 
extern unsigned char zR2014;
#pragma zpsym ("zR2014"); 
extern unsigned char zR2015;
#pragma zpsym ("zR2015"); 
extern unsigned char zR2016;
#pragma zpsym ("zR2016"); 
extern unsigned char zR2017;
#pragma zpsym ("zR2017"); 
extern unsigned char zR2018;
#pragma zpsym ("zR2018");
extern unsigned char zR201A;
#pragma zpsym ("zR201A");
extern unsigned char zR4100;
#pragma zpsym ("zR4100");
extern unsigned char zR4105;
#pragma zpsym ("zR4105");
extern unsigned char zR4106;
#pragma zpsym ("zR4106");
extern unsigned char zR4107;
#pragma zpsym ("zR4107");
extern unsigned char zR4108;
#pragma zpsym ("zR4108");
extern unsigned char zR4109;
#pragma zpsym ("zR4109");
extern unsigned char zR410A;
#pragma zpsym ("zR410A");
extern unsigned char zR410B;
#pragma zpsym ("zR410B");
extern unsigned char zR4118;
#pragma zpsym ("zR4118");

extern char text1[64];
extern unsigned char button1;
extern unsigned char button1PressEvt;
extern unsigned char manual_NMI;

// Select 20XX = 0, 40XX = 1:
unsigned char select20xxOr40xxRegistr = 0;

signed char jumpToApp_Debug_Arrow_Select = 0;
unsigned int jumpToApp_Debug_Arrow_PPUaddr = ARROW_START_PPUADDR_JUMPTOAPP_DEBUG;
unsigned char digit_blink_ctr = 0;
unsigned char digitBlinkEvt = 0;
unsigned char digitBlinkEvt_nonIntr = 0;
unsigned char digit_show = 0;
unsigned char is_digit_blink = 0;
unsigned char select_byte = 0;
unsigned char temp_select_byte = 0;
signed char select_byte_upper_digit = 0;
signed char select_byte_lower_digit = 0;
unsigned char which_digit_to_blink = 0;

unsigned char jumpToApp_Debug_State = JUMPTOAPP_DEBUG_STATE_SEL_REG;

const unsigned char oneDigitIntToCharLookup[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void __fastcall__ jumpToApp_Debug_Refresh_Registers()
{
    // Fill the debug menu with these OneBus Registers addr:
    // R20XX:
    sprintf(text1, "R2012: ");
    printText(0x2084, text1);
    sprintf(text1, "R2013: ");
    printText(0x20C4, text1);
    sprintf(text1, "R2014: ");
    printText(0x2104, text1);
    sprintf(text1, "R2015: ");
    printText(0x2144, text1);
    sprintf(text1, "R2016: ");
    printText(0x2184, text1);
    sprintf(text1, "R2017: ");
    printText(0x21C4, text1);
    sprintf(text1, "R2018: ");
    printText(0x2204, text1);
    sprintf(text1, "R201A: ");
    printText(0x2244, text1);

    // R20XX's values:
    sprintf(text1, "%02X", zR2012);
    printText(START_PPUADDR_R20XX_TEXT, text1);
    sprintf(text1, "%02X", zR2013);
    printText(0x20C4 + 7, text1);
    sprintf(text1, "%02X", zR2014);
    printText(0x2104 + 7, text1);
    sprintf(text1, "%02X", zR2015);
    printText(0x2144 + 7, text1);
    sprintf(text1, "%02X", zR2016);
    printText(0x2184 + 7, text1);
    sprintf(text1, "%02X", zR2017);
    printText(0x21C4 + 7, text1);
    sprintf(text1, "%02X", zR2018);
    printText(0x2204 + 7, text1);
    sprintf(text1, "%02X", zR201A);
    printText(0x2244 + 7, text1);

    // R40XX:
    sprintf(text1, "R4100: ");
    printText(0x2092, text1);
    sprintf(text1, "R4105: ");
    printText(0x20D2, text1);
    sprintf(text1, "R4106: ");
    printText(0x2112, text1);
    sprintf(text1, "R4107: ");
    printText(0x2152, text1);
    sprintf(text1, "R4108: ");
    printText(0x2192, text1);
    sprintf(text1, "R4109: ");
    printText(0x21D2, text1);
    sprintf(text1, "R410A: ");
    printText(0x2212, text1);
    sprintf(text1, "R410B: ");
    printText(0x2252, text1);
    sprintf(text1, "*R4118: "); // Asterisk indicates a CHR-RAM game!
    printText(0x2292-1, text1);

    // R40XX's values:
    sprintf(text1, "%02X", zR4100);
    printText(0x2092 + 7, text1);
    sprintf(text1, "%02X", zR4105);
    printText(0x20D2 + 7, text1);
    sprintf(text1, "%02X", zR4106);
    printText(0x2112 + 7, text1);
    sprintf(text1, "%02X", zR4107);
    printText(0x2152 + 7, text1);
    sprintf(text1, "%02X", zR4108);
    printText(0x2192 + 7, text1);
    sprintf(text1, "%02X", zR4109);
    printText(0x21D2 + 7, text1);
    sprintf(text1, "%02X", zR410A);
    printText(0x2212 + 7, text1);
    sprintf(text1, "%02X", zR410B);
    printText(0x2252 + 7, text1);
    sprintf(text1, "%02X", zR4118);
    printText(0x2292 + 7, text1);
}

void __fastcall__ jumpToApp_Debug_Save_Register(unsigned char select_byte)
{
    if (!select20xxOr40xxRegistr)
    {
        switch (jumpToApp_Debug_Arrow_Select)
        {
        case 0:
            zR2012 = select_byte;
            break;
        case 1:
            zR2013 = select_byte;
            break;
        case 2:
            zR2014 = select_byte;
            break;
        case 3:
            zR2015 = select_byte;
            break;
        case 4:
            zR2016 = select_byte;
            break;
        case 5:
            zR2017 = select_byte;
            break;
        case 6:
            zR2018 = select_byte;
            break;
        case 7:
            zR201A = select_byte;
            break;
        default:
            zR2012 = select_byte;
            break;
        }
    }
    else
    {
        switch (jumpToApp_Debug_Arrow_Select)
        {
        case 0:
            zR4100 = select_byte;
            break;
        case 1:
            zR4105 = select_byte;
            break;
        case 2:
            zR4106 = select_byte;
            break;
        case 3:
            zR4107 = select_byte;
            break;
        case 4:
            zR4108 = select_byte;
            break;
        case 5:
            zR4109 = select_byte;
            break;
        case 6:
            zR410A = select_byte;
            break;
        case 7:
            zR410B = select_byte;
            break;
        default:
            zR4100 = select_byte;
            break;
        }
    }
}

void __fastcall__ jumpToApp_Debug()
{
    button1PressEvt = 0;

    disableRender();
    fillBackgroundZeros();

    sprintf(text1, "ONEBUS DEBUG");
    printText(0x204A, text1);

    placeTile(ARROW_START_PPUADDR_JUMPTOAPP_DEBUG, RIGHT_ARROW_TILE);

    jumpToApp_Debug_Refresh_Registers();

    enableRender();

    while(1)
    {
        if (manual_NMI)
        {
            if (is_digit_blink)
            {
                if (digit_blink_ctr >= 32)
                {
                    digit_blink_ctr = 0;
                    digitBlinkEvt_nonIntr = 1;
                }
                else
                {
                    ++digit_blink_ctr;
                }
            }

            manual_NMI = 0;
        }

        if (digitBlinkEvt_nonIntr)
        {
            digitBlinkEvt_nonIntr = 0;

            digit_show ^= 0x01;

            disableRender();

            // 1. Choose which digit to blink.
            // 2. Digit selected to blink, show or not show.
            select_byte_upper_digit = (select_byte & 0xf0) >> 4;
            select_byte_lower_digit = select_byte & 0x0f;

            // Blink current digit!
            if (digit_show)
            {
                if (jumpToApp_Debug_State == JUMPTOAPP_DEBUG_STATE_REG_SEL_DIGIT0)
                {
                    if(!select20xxOr40xxRegistr)
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select), oneDigitIntToCharLookup[select_byte_upper_digit]);
                    else
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select) + 0x0e, oneDigitIntToCharLookup[select_byte_upper_digit]);
                }
                else
                {
                    if(!select20xxOr40xxRegistr)
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select) + 1, oneDigitIntToCharLookup[select_byte_lower_digit]);
                    else
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select) + 0x0e + 1, oneDigitIntToCharLookup[select_byte_lower_digit]);
                }                
            }
            else
            {
                if (jumpToApp_Debug_State == JUMPTOAPP_DEBUG_STATE_REG_SEL_DIGIT0)
                {
                    if(!select20xxOr40xxRegistr)
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select), ' ');
                    else
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select) + 0x0e, ' ');
                }
                else
                {
                    if(!select20xxOr40xxRegistr)
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select) + 1, ' ');
                    else
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select) + 0x0e + 1, ' ');
                }
            }

            enableRender();
        }

        if(button1PressEvt)
        {
            if(jumpToApp_Debug_State == JUMPTOAPP_DEBUG_STATE_SEL_REG)
            {
                // Up button:
                if (button1 == BUTTON_UP)
                {
                    disableRender();

                    placeTile(jumpToApp_Debug_Arrow_PPUaddr, ' ');
                    --jumpToApp_Debug_Arrow_Select;

                    if (jumpToApp_Debug_Arrow_Select < 0)
                    {
                        jumpToApp_Debug_Arrow_Select = 8 - 1;
                        if (!select20xxOr40xxRegistr)
                        {
                            jumpToApp_Debug_Arrow_PPUaddr = ARROW_START_PPUADDR_JUMPTOAPP_DEBUG + (0x40 * 7);
                            placeTile(jumpToApp_Debug_Arrow_PPUaddr, RIGHT_ARROW_TILE);
                        }
                        else
                        {
                            jumpToApp_Debug_Arrow_PPUaddr = ARROW_START_PPUADDR_JUMPTOAPP_DEBUG + (0x40 * 7) + 0x0E;
                            placeTile(jumpToApp_Debug_Arrow_PPUaddr, RIGHT_ARROW_TILE);
                        }
                    }
                    else
                    {
                        jumpToApp_Debug_Arrow_PPUaddr -= 0x40;
                        placeTile(jumpToApp_Debug_Arrow_PPUaddr, RIGHT_ARROW_TILE);
                    }

                    button1PressEvt = 0;

                    enableRender();
                }
                // Down button:
                else if (button1 == BUTTON_DOWN)
                {
                    disableRender();

                    placeTile(jumpToApp_Debug_Arrow_PPUaddr, ' ');
                    jumpToApp_Debug_Arrow_Select++;

                    if (jumpToApp_Debug_Arrow_Select > (8 - 1))
                    {
                        jumpToApp_Debug_Arrow_Select = 0;
                        if (!select20xxOr40xxRegistr)
                        {
                            jumpToApp_Debug_Arrow_PPUaddr = ARROW_START_PPUADDR_JUMPTOAPP_DEBUG;
                            placeTile(jumpToApp_Debug_Arrow_PPUaddr, RIGHT_ARROW_TILE);
                        }
                        else
                        {
                            jumpToApp_Debug_Arrow_PPUaddr = ARROW_START_PPUADDR_JUMPTOAPP_DEBUG + 0x0E;
                            placeTile(jumpToApp_Debug_Arrow_PPUaddr, RIGHT_ARROW_TILE);
                        }
                    }
                    else
                    {
                        jumpToApp_Debug_Arrow_PPUaddr += 0x40;
                        placeTile(jumpToApp_Debug_Arrow_PPUaddr, RIGHT_ARROW_TILE);
                    }

                    button1PressEvt = 0;

                    enableRender();
                }
                // Left button:
                else if (button1 == BUTTON_LEFT)
                {
                    disableRender();

                    if (select20xxOr40xxRegistr)
                    {
                        select20xxOr40xxRegistr = 0;
                        placeTile(jumpToApp_Debug_Arrow_PPUaddr, ' ');
                        jumpToApp_Debug_Arrow_PPUaddr -= 0x0E;
                        placeTile(jumpToApp_Debug_Arrow_PPUaddr, RIGHT_ARROW_TILE);
                    }

                    button1PressEvt = 0;

                    enableRender();
                }
                // Right button:
                else if (button1 == BUTTON_RIGHT)
                {
                    disableRender();

                    if (!select20xxOr40xxRegistr)
                    {
                        select20xxOr40xxRegistr = 1;
                        placeTile(jumpToApp_Debug_Arrow_PPUaddr, ' ');
                        jumpToApp_Debug_Arrow_PPUaddr += 0x0E;
                        placeTile(jumpToApp_Debug_Arrow_PPUaddr, RIGHT_ARROW_TILE);
                    }

                    button1PressEvt = 0;

                    enableRender();
                }
                // Select button:
                else if (button1 == BUTTON_START)
                {
                    // Exit OneBus Debugging menu:
                    button1PressEvt = 0;
                    return;
                }
                // A button:
                else if (button1 == BUTTON_A)
                {
                    // select digit on the selected register!
                    disableRender();

                    if (!select20xxOr40xxRegistr)
                    {
                        switch (jumpToApp_Debug_Arrow_Select)
                        {
                        case 0:
                            select_byte = zR2012;
                            break;
                        case 1:
                            select_byte = zR2013;
                            break;
                        case 2:
                            select_byte = zR2014;
                            break;
                        case 3:
                            select_byte = zR2015;
                            break;
                        case 4:
                            select_byte = zR2016;
                            break;
                        case 5:
                            select_byte = zR2017;
                            break;
                        case 6:
                            select_byte = zR2018;
                            break;
                        case 7:
                            select_byte = zR201A;
                            break;
                        default:
                            select_byte = zR2012;
                            break;
                        }
                    }
                    else
                    {
                        switch (jumpToApp_Debug_Arrow_Select)
                        {
                        case 0:
                            select_byte = zR4100;
                            break;
                        case 1:
                            select_byte = zR4105;
                            break;
                        case 2:
                            select_byte = zR4106;
                            break;
                        case 3:
                            select_byte = zR4107;
                            break;
                        case 4:
                            select_byte = zR4108;
                            break;
                        case 5:
                            select_byte = zR4109;
                            break;
                        case 6:
                            select_byte = zR410A;
                            break;
                        case 7:
                            select_byte = zR410B;
                            break;
                        default:
                            select_byte = zR4100;
                            break;
                        }
                    }

                    is_digit_blink = 1;

                    jumpToApp_Debug_State = JUMPTOAPP_DEBUG_STATE_REG_SEL_DIGIT0;

                    button1PressEvt = 0;

                    enableRender();
                }
                else if(button1 == BUTTON_B)
                {
                    // Refresh OneBus debug screen register values:
                    disableRender();
                    jumpToApp_Debug_Refresh_Registers();
                    enableRender();

                    button1PressEvt = 0;
                }
            }
            else if (jumpToApp_Debug_State == JUMPTOAPP_DEBUG_STATE_REG_SEL_DIGIT0)
            {
                // 1. take that selected digit.
                // 2. up-down changes the digit from 0-F.
                // 3. button A advances to next digit.

                select_byte_upper_digit = (select_byte & 0xf0) >> 4;
                
                if(button1 == BUTTON_UP)
                {
                    if(select_byte_upper_digit > 0x0f)
                        select_byte_upper_digit = 0;
                    else
                        ++select_byte_upper_digit;
                    
                    select_byte = (select_byte & 0x0f) | (select_byte_upper_digit << 0x04);
                    button1PressEvt = 0;
                }
                else if(button1 == BUTTON_DOWN)
                {
                    if(select_byte_upper_digit < 0x00)
                        select_byte_upper_digit = 0x0f;
                    else
                        --select_byte_upper_digit;
                    
                    select_byte = (select_byte & 0x0f) | (select_byte_upper_digit << 0x04);
                    button1PressEvt = 0;
                }
                else if(button1 == BUTTON_A)
                {
                    disableRender();
                    // Digit should stop blinking and restore the tile!
                    if(!select20xxOr40xxRegistr)
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select), oneDigitIntToCharLookup[select_byte_upper_digit]);
                    else
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select) + 0x0e, oneDigitIntToCharLookup[select_byte_upper_digit]);
                    jumpToApp_Debug_State = JUMPTOAPP_DEBUG_STATE_REG_SEL_DIGIT1;
                    enableRender();

                    button1PressEvt = 0;
                }
            }
            else if (jumpToApp_Debug_State == JUMPTOAPP_DEBUG_STATE_REG_SEL_DIGIT1)
            {
                if (button1 == BUTTON_UP)
                {
                    select_byte_lower_digit = (select_byte & 0x0f);

                    if (select_byte_lower_digit > 0x0f)
                        select_byte_lower_digit = 0;
                    else
                        ++select_byte_lower_digit;

                    select_byte = (select_byte & 0xf0) | (select_byte_lower_digit);
                    button1PressEvt = 0;
                }
                else if (button1 == BUTTON_DOWN)
                {
                    select_byte_lower_digit = (select_byte & 0x0f);

                    if (select_byte_lower_digit < 0x00)
                        select_byte_lower_digit = 0x0f;
                    else
                        --select_byte_lower_digit;

                    select_byte = (select_byte & 0xf0) | (select_byte_lower_digit);
                    button1PressEvt = 0;
                }
                else if (button1 == BUTTON_A)
                {
                    disableRender();
                    // Digit should stop blinking and restore the tile!
                    if(!select20xxOr40xxRegistr)
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select) + 1, oneDigitIntToCharLookup[select_byte_lower_digit]);
                    else
                        placeTile(START_PPUADDR_R20XX_TEXT + (0x40 * jumpToApp_Debug_Arrow_Select) + 0x0e + 1, oneDigitIntToCharLookup[select_byte_lower_digit]);
                    enableRender();
                    
                    // Save this back into the register!
                    jumpToApp_Debug_Save_Register(select_byte);

                    is_digit_blink = 0;
                    jumpToApp_Debug_State = JUMPTOAPP_DEBUG_STATE_SEL_REG;

                    button1PressEvt = 0;
                }
            }
        }
    }
}