#include "appList.h"

// Menu Item Properties:
// From registers $4100-$410A, $2012-$201A:
// Note: $4106.0 selects the horizontal/vertical scrolling!
// These properties are arranged in this format:
//            $2012, $2013, $2014, $2015, $2016, $2017, $2018, $201A...
// ...cont'd  $4100, $4105, $4106, $4107, $4108, $4109, $410A, $410B
const unsigned char appProperties_0[] = {0x44, 0x45, 0x46, 0x47, 0x40, 0x42, 0x20, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0d, 0x00, 0x40, 0x02};
const unsigned char appProperties_1[] = {0x84, 0x85, 0x86, 0x87, 0x80, 0x82, 0x20, 0x00, 0x00, 0x00, 0x01, 0x04, 0x05, 0x00, 0x50, 0x03};
const unsigned char appProperties_2[] = {0xc4, 0xc5, 0xc6, 0xc7, 0xc0, 0xc2, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x5c, 0x04};
const unsigned char appProperties_3[] = {0x04, 0x05, 0x06, 0x07, 0x00, 0x02, 0x30, 0x00, 0x00, 0x00, 0x00, 0x04, 0x05, 0x00, 0x60, 0x03};
const unsigned char appProperties_4[] = {0x44, 0x45, 0x46, 0x47, 0x40, 0x42, 0x30, 0x00, 0x00, 0x00, 0x01, 0x04, 0x05, 0x00, 0x6d, 0x03};
const unsigned char* menuItemProperties[] = { appProperties_0, appProperties_1, appProperties_2, appProperties_3, appProperties_4 };

// Reset vectors:
const unsigned char appRstVct_0[] = { 0x00, 0x80 };
const unsigned char appRstVct_1[] = { 0x00, 0xC7 };
const unsigned char appRstVct_2[] = { 0xa5, 0x9b };
const unsigned char appRstVct_3[] = { 0xde, 0xaa };
const unsigned char appRstVct_4[] = { 0xed, 0x81 };
const unsigned char* resetVectors[] = { appRstVct_0, appRstVct_1, appRstVct_2, appRstVct_3, appRstVct_4 };

// App titles:
const char emptyTitle[] = "                ";
const char appTitle_0[] = "APP0";
const char appTitle_1[] = "APP1";
const char appTitle_2[] = "APP2";
const char appTitle_3[] = "APP3";
const char appTitle_4[] = "APP4";
const char* appTitleList[] = {appTitle_0, appTitle_1, appTitle_2, appTitle_3,appTitle_4}; 
