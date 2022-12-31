; File: serialSound.inc
;Class: CDA 3104 85149, Fall 2022
; Desc: Control and transmission functions for 9600 baud USART control of YX5300
;           Serial MP3 Module (v1.2 using GD3200B chip, should work with v1.0 cmds)
;           Final submission using a v1.0 module
; ------------------------------------------------------------------------------

; Send full command to YX5300 Serial MP3 Sound Module, based on data sheet
; pass in command in GPR_YX5300_CMD, data in GPR_YX5300_DATA_LOW and GPR_YX5300_DATA_HIGH
;-------------------------------------------------------------------------------
soundComs:
          ; start data packet
          ldi       GPR_USART_BYTE,0x7E
          call      trnsmt
          ; version
          ldi       GPR_USART_BYTE,0xFF;0x10?
          call      trnsmt
          ; command length (byte count, exlcuding start and checksum/end)
          ldi       GPR_USART_BYTE,0x06
          call      trnsmt
          ; command
          mov       GPR_USART_BYTE,GPR_YX5300_CMD
	  ;ldi       GPR_USART_BYTE,0x03
          call      trnsmt
          ; feedback - none
          ldi       GPR_USART_BYTE,0x00
          call      trnsmt
          ; data high
          mov       GPR_USART_BYTE,GPR_YX5300_DATA_HIGH
	  ;ldi       GPR_USART_BYTE,0x00
          call      trnsmt
          ; data low
          mov       GPR_USART_BYTE,GPR_YX5300_DATA_LOW
	  ;ldi       GPR_USART_BYTE,0x01
          call      trnsmt
		  
	  ; Checksum = sum of all data counted in the length bit, reversed, then add 1
	  ; Calculate Checksum: 
	  ; ((FF + 06 + CMD + 00 + Data Low + Data High) EOR FFFF) + 1 = FEF7 for CMD 03, Data 0001 - secondary fire sound
	  ; GPR_USART_BYTE:GPR_TMP
	  ;ldi       GPR_TMP, (0x10 + 0x06) ;0x05; constant data
	  ;ldi	    GPR_USART_BYTE, 0x01
	  ;add       GPR_TMP, GPR_YX5300_CMD
	  ;clr       GPR_YX5300_CMD; make high byte zero before adding with carry
	  ;adc       GPR_USART_BYTE, GPR_YX5300_CMD
	  ;add       GPR_TMP, GPR_YX5300_DATA_LOW
	  ;clr       GPR_YX5300_DATA_LOW; make high byte zero before adding with carry
	  ;adc       GPR_USART_BYTE, GPR_YX5300_DATA_LOW
	  ;add       GPR_TMP, GPR_YX5300_DATA_HIGH
	  ;clr       GPR_YX5300_DATA_HIGH; make high byte zero before adding with carry
	  ;adc       GPR_USART_BYTE, GPR_YX5300_DATA_HIGH

	  ;ldi       GPR_YX5300_DATA_HIGH, 0xFF
	  ;eor       GPR_TMP, GPR_YX5300_DATA_HIGH
	  ;eor       GPR_USART_BYTE, GPR_YX5300_DATA_HIGH

	  ;ldi       GPR_YX5300_DATA_LOW, 1
	  ;ldi       GPR_YX5300_DATA_HIGH, 0
	  ;add       GPR_TMP, GPR_YX5300_DATA_LOW
	  ;adc       GPR_USART_BYTE, GPR_YX5300_DATA_HIGH

	  ; checksum high - only for GD3200B chip
	  ;ldi       GPR_USART_BYTE,0xFF
          ;call      trnsmt
		  ; checksum low - only for GD3200B chip
          ;mov       GPR_USART_BYTE,GPR_TMP
	  ;ldi       GPR_USART_BYTE,0xE6
          ;call      trnsmt
		  
          ; end data packet
          ldi       GPR_USART_BYTE,0xEF
          call      trnsmt
          ret
	  
	
; Transmit the byte in GPR_USART_BYTE over 9600 baud serial connection to YX5300
; MP3 Sound Module
;-------------------------------------------------------------------------------
trnsmt:
          lds       GPR_TMP,UCSR0A
          sbrs      GPR_TMP,UDRE0         ;is transmit buffer empty?
          rjmp      trnsmt                        
          sts       UDR0,GPR_USART_BYTE           ;transmit pending byte
          ret