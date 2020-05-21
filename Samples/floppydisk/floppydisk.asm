/*
FloppyDisk (c) May 2020
*/

.file [name="floppydisk.prg", type="prg", segments="floppydisk"]

.disk [filename="floppydisk.d64", name="FLOPPYDISK", id=" 2A", format="commodore"]
{
	[name="----------------", type="rel"],
	[name="FLOPPYDISK      ", type="prg", segments="floppydisk"],
	[name="----------------", type="rel"],
	[name="- IDE 65XX     -", type="rel"],
	[name="- SAMPLE       -", type="rel"],
	[name="----------------", type="rel"]
}

.segment floppydisk[]

BasicUpstart2(start)
start:
			.var picture = LoadBinary("ide65xx_floppy.kla", BF_KOALA)
			.var music = LoadSid("lionsinacage.sid")
			
			sei
			lda #$38
			sta $d018
			lda #$d8
			sta $d016
			lda #$3b
			sta $d011
			lda #BLUE
			sta $d020
			lda #picture.getBackgroundColor()
			sta $d021
			lda $dd00
			and #$02
			ora #$fc
			sta $dd00
			lda #<irq
			sta $0314
			lda #>irq
			sta $0315
			lda #$33
			sta $d012
			lda #$7f
			sta $dc0d
			sta $dd0d
			lda #$81
			sta $d01a
			lda $dc0d
			lda $dd0d
			asl $d019
			cli
			
			ldx #0
loop:		.for (var i=0; i<4; i++) 
			{
				lda colorRam+i*$100,x
				sta $d800+i*$100,x
			}
			inx
			bne loop
			lda #music.startSong-1
			jsr music.init	
			jmp *
			
irq:  	    	asl $d019
			inc $d020
			jsr music.play
			dec $d020
			jmp $ea81

			*=music.location "Music"
			.fill music.size, music.getData(i)		

			*=$4c00	"ScreenRam" 			
			.fill picture.getScreenRamSize(), picture.getScreenRam(i)

			*=$5c00	"ColorRam:"
colorRam: 	
			.fill picture.getColorRamSize(), picture.getColorRam(i)
			
			*=$6000	"Bitmap"			
			.fill picture.getBitmapSize(), picture.getBitmap(i)
