//----------------------------------------------------        
// BasicUpstart3
//----------------------------------------------------
.macro BasicUpstart3(address, lineNumber, comment) {
           * = $0801 "Basic"
          .word upstartEnd                   // link address
          .word lineNumber                   // line number
          .byte $9e                          // sys
          .text toIntString(address)         // address
          .byte $3a, $8f, $20                // :REM<space>
          .text comment.toUpperCase()        // <comment>
          .byte 0
upstartEnd:
          .word 0  
          * = $0811 + comment.size() "BasicEnd"   
} 
//----------------------------------------------------  
// constants
//----------------------------------------------------
.const TEXTCOLOR              = $0286
.const BORDER 				  = $d020
.const BACKGROUND             = $d021
.const STROUT                 = $ab1e
.const CLSR                   = $e544
.const PLOT                   = $fff0
.const GETIN                  = $ffe4
//----------------------------------------------------
