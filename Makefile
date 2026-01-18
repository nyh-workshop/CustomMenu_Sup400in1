CC65 = cc65
CA65 = ca65
LD65 = ld65
CL65 = cl65
NAME = menu
CFG = nes.cfg
NESHEADERFOLDER = nes_header
ROMDUMPFOLDER = romdump_init

.PHONY: default clean

$menu: $(NAME).c
	$(CC65) -Oir -Cl -t nes $(NAME).c
	$(CC65) -Oir -Cl -t nes appList.c
	$(CA65) $(NAME).s
	$(CA65) crt0.s
	$(CA65) interrupt.s
	$(CA65) jumpToApp.s
	$(CA65) appList.s
	$(LD65) -C target.cfg crt0.o interrupt.o jumpToApp.o appList.o $(NAME).o nes.lib -o output.bin
	python buildCHR.py
	mkdir -p $(ROMDUMPFOLDER)
	cat $(ROMDUMPFOLDER)/romdump_sup400in1_E7E0_init.bin output.bin apps.bin > output_final.bin
	cat $(ROMDUMPFOLDER)/romdump_sup400in1_E7E0_init_swapBits45.bin output.bin apps.bin > output_final_swapBits45.bin
	cat $(NESHEADERFOLDER)/nesHeader.bin output_final.bin > output_final_nes.nes
    
clean:
	rm $(NAME).o
	rm $(NAME).s
	rm appList.s
	rm crt0.o
	rm interrupt.o
	rm jumpToApp.o
	rm appList.o
	rm output.bin
	rm output_final.bin
	rm output_final_swapBits45.bin
	rm output_final_nes.nes