.export   _init, _exit

.import initlib, donelib, callmain
.import zerobss, copydata
.import _nmi, _irq
.import _main

.import __SRAM_START__, __SRAM_SIZE__

.export __STARTUP__ : absolute = 1        ; Mark as startup

.include "zeropage.inc"
.include "nes.inc"

; ---------------------------------------------------------------------------
; Place the startup code in a special segment

.segment "STARTUP"

_init: 
        ; disable interrupts on APU:
        lda #$40
        sta $4017
        lda #$00
        sta $4010  
        
        clear_ZP_RAM:    
            tax
        @l0:
            sta $000,x
            sta $100,x
            sta $200,x
            sta $300,x
            sta $400,x
            sta $500,x
            sta $600,x
            sta $700,x
            inx
            bne @l0
            
        jsr zerobss
        
        jsr copydata
        
        setup_Stack:
            lda #<(__SRAM_START__ + __SRAM_SIZE__)
            ldx #>(__SRAM_START__ + __SRAM_SIZE__)
            sta sp
            stx sp+1 ; Set argument stack ptr
            
        jsr initlib
        
        jsr callmain

        _exit: jsr donelib ; Run module destructors

        jmp _init
        
; ------------------------------------------------------------------------
; Hardware vectors
; ------------------------------------------------------------------------

.segment "VECTORS"
        .word _nmi
        .word _init       ; $fffc reset
        .word _irq