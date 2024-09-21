# CustomMenu_Sup400in1

## Note: Under construction!

Custom menu template for Sup 400-in-1 handhelds.

Written in C and some 6502 assembly.

Requires ROM dump from the actual handheld.

Please read the [article](https://nyh-workshop.github.io/Custom-ROM-Sup-Game-Box-400in1/) to understand how to dump the ROM, and to modify it before attempting this one.

## Requirements:

- CC65 - latest version will do.
- Visual Studio 2022 with C#.
- [OneBusCalc](https://github.com/nyh-workshop/OneBusCalc) and [Menu Builder](https://github.com/nyh-workshop/MenuBuilder_SupGameBox) app to help you build the compilation.

## Preliminary setup:

In the Makefile you need to specify the ROMs there:

```
cat romdump_sup400in1_E7E0_init.bin output.bin apps.bin > output_final.bin
cat romdump_sup400in1_E7E0_init_swapBits45.bin output.bin apps.bin > output_final_swapBits45.bin
cat nesHeader.bin output_final.bin > output_final_nes.nes
```

From the ROM dump, cut out the first 512KB (0x00000-0x7FFFF) and give it a name such as `romdump_sup400in1_init.bin`.

If you have a ROM dump that have swapped bits in the opcodes and the data lines, you are required to have both copies of the bin file:

- One is without the bitswaps and can run in EmuVT. Example name: `romdump_sup400in1_init.bin`.

- Another one is with the bitswaps and it can run on the actual unit. Example name according to the bitswaps: `romdump_sup400in1_E7E0_init_swapBits45.bin`.

## How to compile and add games:

1. Get [OneBusCalc](https://github.com/nyh-workshop/OneBusCalc) and the [Menu Builder](https://github.com/nyh-workshop/MenuBuilder_SupGameBox). Compile these and run the executables.

2. On the Menu Builder, put this project location into the "Build Folder".

3. Afterwards, get an NES file. The game must be **Mapper 0 or 4**! Click on the "+" symbol in the Menu Builder to add the game.

4. You can rename the game inside by double clicking the title on that row. It must be only 16 characters long!

5. On that row, click on the "Set Start CHR and PRG". On these games you must provide the addresses for the CHR and PRG *individually*. The games **must be placed in ROM locations 0x90000 and onwards!**

6. To determine the OneBus values for the system to jump into the game and locate the CHRs, you can use the [OneBusCalc](https://github.com/nyh-workshop/OneBusCalc).

7. At the OneBusCalc, set the values in the registers according to the CHR and PRG locations. Then press "Generate Code" and Ctrl-C the values (except the square bracket).

8. Back to the Menu Builder app and at the same row, click on the "Config OneBus". Paste the values inside and press OK.

9. When all is done, press "Compile!" button. You have an app compilation binary file generated.

10. Wait! You need to go back to this folder "CustomMenu_Sup400in1" and run "make" to generate the complete binary! When it is done, you can run the game in EmuVT using `output_final.bin` or the NintendulatorNRS using `output_final_nes.nes`.

This is an example menu:

![Screenshot](Screenshot.png)

## How to - Manual Compilation:

1. At the appList.c and appList.h, if you need to add your custom app inside, make sure you have the menu item properties, reset vector and app title ready inside.

For the appProperties, use the [OneBusCalc](https://github.com/nyh-workshop/OneBusCalc) to calculate the OneBus' registers for the PRG and CHR locations. Then copy the generated output into the appProperties_X array.

Example, if you have a "MyGame" here at slot no. X, you need to add in the appList.c:

```
// Menu Item Properties:
const unsigned char appProperties_X[] = {...};

// Reset vectors:
const unsigned char appRstVct_X[] = { 0x00, 0x80 };

// App titles:
const char appTitle_X[] = "MyGame";

```

2. In the Makefile, you need to add the .bin file of the "MyGame" before the '>'. If you have ROM dump that have bit swaps, you need to include this there too. Example:

```
	cat romdump_sup400in1_E7E0_init.bin output.bin > output_final.bin
	cat romdump_sup400in1_E7E0_init_swapBits45.bin output.bin > output_final_swapBits45.bin
```
Note: You may need to **rearrange** the CHR and PRG in the MyGame.bin file according to the OneBusCalc app provided. More improvements to automatically include nes files coming soon!

3. Enter "make" in the cmd. 

4. After building, use EmuVT 1.36 to run. If you are satisfied with the contents, you can copy this compilation "output_final.bin" into the 29GL flash using the T56 programmer.
