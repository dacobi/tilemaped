.include "x16.inc"

.export _install_irq
.export _remove_irq
.export _getkeystat
.export _set_keyboard_irq
.export _rm_keyboard_irq

.import _mblank
.import _msline
.import _mscol
.import _maflow
.import _mauien
.import _curkey

.import _isdone
.import _linedone
.import _linestop
.code
.proc irqhandler: near
  lda #$04 ; SCOL
  bit VERA_isr 
  beq SLINE
SCOL:
  sta VERA_isr
  jsr _mscol
  ply           ; return 
  plx
  pla
  rti
SLINE:
  lda #$02 ; SLINE
  bit VERA_isr 
  beq AFLOW
  sta VERA_isr
  jsr _msline
  ply           ; return 
  plx
  pla
  rti
AFLOW:
  lda _mauien
  cmp #$01
  bne VBLANK
  lda #$08 ; Audio FIFO Low
  bit VERA_isr
  beq VBLANK
  ;sta VERA_isr
  jsr _maflow
  ply           ; return
  plx
  pla
  rti
VBLANK:
  jsr _mblank
  jmp $0000
.endproc

system_irq := (*-2) ; self-mods the jmp $ffff at DONE: in irqhandler

.proc _install_irq: near
  lda IRQVec
  ldx IRQVec+1
  cmp #<irqhandler
  bne INSTALL
  cpx #>irqhandler
  beq EXIT
INSTALL:
  sta system_irq
  stx system_irq+1
  lda #<irqhandler
  ldx #>irqhandler
  sei
  sta IRQVec
  stx IRQVec+1
  cli
EXIT:
  rts
.endproc

.proc _remove_irq: near
  lda IRQVec
  ldx IRQVec+1
  cmp #<irqhandler
  bne EXIT
  cpx #>irqhandler
  bne EXIT
  lda system_irq
  ldx system_irq+1
  sei
  sta IRQVec
  stx IRQVec+1
  cli
EXIT:
  rts
.endproc

old_keyboard : 
.byte 0, 0


.proc _set_keyboard_irq: near
sei
lda $032e
ldx $032f
sta old_keyboard
stx old_keyboard + 1
lda #<keyhandler
sta $032e
lda #>keyhandler
sta $032f
cli
rts
.endproc

.proc _rm_keyboard_irq: near
sei
lda old_keyboard
ldx old_keyboard + 1
sta $032e
stx $032f
cli
rts
.endproc


.proc _getkeystat: near
  lda keyboard_scancodes_status , x
  sta _curkey
  rts
.endproc


.proc keyhandler: near
php
pha
phx ; 65c02 Specific Instruction
phy ; 65c02 Specific Instruction
sta key_scancode ; // Save Scancode
lda #$80
bit key_scancode
beq mkey_loop_0
lda key_scancode
and #$7f
tax
lda #0
sta keyboard_scancodes_status , x
jmp mkey_loop_1
mkey_loop_0:
ldx key_scancode
lda #1
sta keyboard_scancodes_status , x
mkey_loop_1:
ply ; 65c02 Specific Instruction
plx ; 65c02 Specific Instruction
pla
plp
rts
.endproc


key_scancode:
.byte 0

keyboard_scancodes_status:

.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
.byte 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
