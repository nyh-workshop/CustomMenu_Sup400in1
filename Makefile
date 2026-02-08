CC65 = cc65
CA65 = ca65
LD65 = ld65
CL65 = cl65
NAME = menu
CFG = nes.cfg
ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

# These ROM types shows:
# 1. Bit swaps on opcodes at startup and/or the entire ROM data.
# 2. If it jumps to another custom address ex. "JSR #$3000".
#
# Known ROM types:
# 1. SWAP_DATA_BITS_4_TO_5
# 2. SWAP_DATA_BITS_9_A_TO_0_1__SWAP_OPCODE_BITS_0_TO_1__STARTUP_CUSTOM_ADDR
#

ROM_TYPE = SWAP_DATA_BITS_9_A_TO_0_1__SWAP_OPCODE_BITS_0_TO_1__STARTUP_CUSTOM_ADDR

ROMDUMP_INIT_TITLE = S29GL128P10TFI01_romdump_sup400in1_09082024
ROMDUMP_INIT_EMULATOR_BIN = $(ROMDUMP_INIT_TITLE)_init_emulator.bin
ROMDUMP_INIT_ORIGINAL_BIN = $(ROMDUMP_INIT_TITLE)_init_original.bin
ROM_HANDHELD_OUTPUT_FOLDER = output_rom_handheld

NESHEADERFOLDER = nes_header
ROMDUMPFOLDER = romdump_init

.PHONY: default clean

all: $menu compile_to_handheld_rom

$menu: $(NAME).c
	$(CC65) -Oir -Cl -t nes $(NAME).c
	$(CC65) -Oir -Cl -t nes appList.c
	$(CA65) $(NAME).s
	$(CA65) crt0.s
	$(CA65) interrupt.s
	$(CA65) jumpToApp.s
	$(CA65) appList.s
	$(LD65) -C target.cfg crt0.o interrupt.o jumpToApp.o appList.o $(NAME).o nes.lib -o output.bin

	python buildCHR.py "fonts/powerpak_font.chr"

	mkdir -p $(ROMDUMPFOLDER)
	cat $(ROMDUMPFOLDER)/$(ROMDUMP_INIT_EMULATOR_BIN) output.bin apps.bin > output_final_emulator.bin
	cat $(NESHEADERFOLDER)/nesHeader.bin output_final_emulator.bin > output_final_emulator.nes

compile_to_handheld_rom:
	@echo "Compiling to Handheld ROM!"
ifeq ($(ROM_TYPE), SWAP_DATA_BITS_4_TO_5)
	cat $(ROMDUMPFOLDER)/$(ROMDUMP_INIT_ORIGINAL_BIN) output.bin apps.bin > output_final_swapBits45.bin

else ifeq ($(ROM_TYPE), SWAP_DATA_BITS_9_A_TO_0_1__SWAP_OPCODE_BITS_0_TO_1__STARTUP_CUSTOM_ADDR)
	$(ROOT_DIR)flips.exe --apply patch/$(ROMDUMP_INIT_TITLE)_init_convert_to_original.bps romdump_init/$(ROMDUMP_INIT_EMULATOR_BIN) $(ROM_HANDHELD_OUTPUT_FOLDER)/$(ROMDUMP_INIT_TITLE)_init_original.bin
	cat $(ROM_HANDHELD_OUTPUT_FOLDER)/$(ROMDUMP_INIT_ORIGINAL_BIN) output.bin apps.bin > $(ROM_HANDHELD_OUTPUT_FOLDER)/$(ROMDUMP_INIT_TITLE)_recompile.bin
	
	python sup_game_box_swap_bits.py $(ROM_HANDHELD_OUTPUT_FOLDER)/$(ROMDUMP_INIT_TITLE)_recompile.bin O SWAP_DATA_BITS_9A_TO_12
endif
@echo "Done compiling to Handheld ROM! :D"

clean:
	rm $(NAME).o
	rm $(NAME).s
	rm apps.bin
	rm appList.s
	rm crt0.o
	rm interrupt.o
	rm jumpToApp.o
	rm appList.o
	rm output.bin
	rm output_final_emulator.bin
	rm output_final_emulator.nes
	rm $(ROM_HANDHELD_OUTPUT_FOLDER)/*.bin
	rm appList.c
	rm appList.h