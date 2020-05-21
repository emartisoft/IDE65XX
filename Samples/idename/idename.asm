/* 
IDEName (c) Apr, 2020
*/

// disk directive
.disk [filename="idename.d64", name="IDENAME", id=" 2A", format="commodore"]
{
	[name="----------------", type="del"],
	[name="IDENAME         ", type="prg", segments="Code"],
	[name="----------------", type="del"]
}

// file directive
.file [name="idename.prg", type="prg", segments="Code"]
.segment Code[]

// macro
#import "mymacro.asm"

// BASIC SYS Line
// 2020 SYS2084:REM POWERED BY IDE 65XX
BasicUpstart3(start, 2020, "powered by Ide 65xx") 
start:
          jsr CLSR                 // clear screen  
          mov #$00 : BORDER        // border colour
          mov #$02 : BACKGROUND    // background colour
          mov #$01 : TEXTCOLOR     // text colour
          ldy #$04                 // cursor position
          ldx #$0c
          clc
          jsr PLOT
          lda #<hello              // print 
          ldy #>hello
          jsr STROUT
          jsr GETIN                // wait for a key to be pressed
          beq *-3
          rts                     // return to BASIC
          
hello:
.text "IDE 65XX KICK ASSEMBLER EDITION"
.byte $00
          
.pseudocommand mov value : address {
    lda value
    sta address
}
