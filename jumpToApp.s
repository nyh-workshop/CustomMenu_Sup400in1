    .export _jumpToApp

    .importzp _zR2012, _zR2013, _zR2014, _zR2015, _zR2016, _zR2017, _zR2018, _zR201A
    .importzp _zR4100, _zR4105, _zR4106, _zR4107, _zR4108, _zR4109, _zR410A, _zR410B
    .importzp _zRvct_L,_zRvct_H;

.segment "CODE"

.PC02 ; Force 65C02 assembly mode

_jumpToApp:
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
    lda _zR410A
    sta $410A
    lda _zR410B
    sta $410B
    
    ; _zRvct_L and _zRvct_H for reset vector:
    jmp (_zRvct_L)
    