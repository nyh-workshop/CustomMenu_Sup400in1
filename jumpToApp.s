    .export _jumpToApp

    .importzp _zR2012, _zR2013, _zR2014, _zR2015, _zR2016, _zR2017, _zR2018, _zR201A
    .importzp _zR4100, _zR4105, _zR4106, _zR4107, _zR4108, _zR4109, _zR410A, _zR410B
    .importzp _zRvct_L,_zRvct_H;

.segment "CODE"

.PC02 ; Force 65C02 assembly mode

_jumpToApp:

    lda #$00
    sta $a000
    lda #$00
    sta $e000
    
_ppu_reset:
    lda #$00
    sta $2000
    sta $2001
@l0:lda $2002
    bmi @l0
@l1:lda $2002
    bpl @l1
    
_mmc3_init:
    lda #$00
    nop
    asl a
    asl a
    asl a
    ldx #$00
@l0:stx $8000
    sta $8001
    clc
    adc #$02
    inx
    cpx #$02
    bne @l0
    clc
    ldx #$02
@l1:stx $8000
    sta $8001
    adc #$01
    inx
    cpx #$06
    bne @l1
    
    lda _zR2012
    sta $2012
    lda _zR2013
    sta $2013
    lda _zR2014
    sta $2014
    lda _zR2015
    sta $2015
    lda _zR2016
    sta $2016
    lda _zR2017
    sta $2017
    lda _zR2018
    sta $2018
    lda _zR201A
    sta $201A

    ; Bank switches at $410x can only be done in RAM.
    ; Copy these into $400 and run it from there:
    
                ldx #$30
@cpyToRAM_Loop: lda jumpToProgramInRAM,X
                sta $0400,X
                dex
                bpl @cpyToRAM_Loop
                jmp $0400
                
                
jumpToProgramInRAM:
    lda _zR410A
    sta $410A
    lda _zR410B
    sta $410B
    lda _zR4100
    sta $4100
    lda _zR4105
    sta $4105
    lda _zR4106
    sta $4106
    lda _zR4107
    sta $4107
    lda _zR4108
    sta $4108
    lda _zR4109
    sta $4109

    ;lda #$80
    ;sta $4118
    
    ; _zRvct_L and _zRvct_H for reset vector:
    jmp (_zRvct_L)
    