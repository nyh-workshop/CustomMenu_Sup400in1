; ---------------------------------------------------------------------------
; interrupt.s
; ---------------------------------------------------------------------------
;
; Interrupt handler.

.export   _irq, _nmi

.import _button1PressEvt
.import _button1
.import _manual_NMI

.segment  "CODE"

.PC02 ; Force 65C02 assembly mode

buttons_1 = $00A0

; ---------------------------------------------------------------------------
; Maskable interrupt (IRQ) service routine

_irq:
    rti

_nmi:
    pha
    tya
    pha
    txa
    pha
    
    lda #$00
    sta $2005
    sta $2005
    
    jsr readjoy
    lda buttons_1
    
    ldx $A1
    inx
    cpx #$0A
    bcs @SampleButtonResetCounter
    stx $A1
    jmp @exitNMI
    
@SampleButtonResetCounter: 
    ldx #$00
    stx $A1
    lda buttons_1
    cmp #$00
    bne @buttonPressed
    jmp @exitNMI
    
@buttonPressed:
    lda #$01
    sta _button1PressEvt
    lda buttons_1
    sta _button1
    jmp @exitNMI
    
@exitNMI:
    lda #$01
    sta _manual_NMI
    pla
    tax
    pla
    tay
    pla
    
    rti
    
; https://www.nesdev.org/wiki/Controller_reading_code
; readjoy:
; returns result to buttons_1:
readjoy:
    lda #$01
    sta $4016
    sta buttons_1
    lsr a
    sta $4016
@loop:
    lda $4016
    lsr a
    rol buttons_1
    bcc @loop
    rts