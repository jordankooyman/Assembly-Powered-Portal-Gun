; File: setup.inc
;Class: CDA 3104 85149, Fall 2022
; Devs: Sheryll Jacquet, Katarya Johnson-Williams, Jordan Kooyman, Bradley Walby
; Desc: 1-time configuration of arduino pins and internal values
; ------------------------------------------------------------------------------

setup:
          ; configure pins (buttons & switch) for input and pull-up
          cbi       SWI_ONE_DIR, SWI_ONE_PIN
          sbi       SWI_ONE_OUT, SWI_ONE_PIN

          cbi       SWI_TWO_DIR, SWI_TWO_PIN
          sbi       SWI_TWO_OUT, SWI_TWO_PIN

          cbi       BTN_RES_DIR, BTN_RES_PIN
          sbi       BTN_RES_OUT, BTN_RES_PIN

          cbi       BTN_SEC_DIR, BTN_SEC_PIN
          sbi       BTN_SEC_OUT, BTN_SEC_PIN

          cbi       BTN_PRI_DIR, BTN_PRI_PIN
          sbi       BTN_PRI_OUT, BTN_PRI_PIN

          ; button LED pins for output and off
          sbi       RGB_BLU_DIR, RGB_BLU_PIN
          cbi       RGB_BLU_OUT, RGB_BLU_PIN

          sbi       RGB_GRE_DIR, RGB_GRE_PIN
          cbi       RGB_GRE_OUT, RGB_GRE_PIN

          sbi       RGB_RED_DIR, RGB_RED_PIN
          cbi       RGB_RED_OUT, RGB_RED_PIN

          sbi       RECOIL_DIR, RECOIL_PIN
          cbi       RECOIL_OUT, RECOIL_PIN

          sbi       NPXL_DIR, NPXL_PIN
          cbi       NPXL_OUT, NPXL_PIN

          ; configure all pin change interrupt for all pins in PCMSK0
          ldi       GPR_TMP, 0b00000001
          sts       PCICR, GPR_TMP

          ; configure buttons & switch for pin-change request
          ldi       GPR_TMP, 0b00111110               ; enable pin-change on PB1 (PCINT1)
          sts       PCMSK0, GPR_TMP                   ; enable pin-change on PB2 (PCINT2)
                                                  ; enable pin-change on PB3 (PCINT3)
                                                  ; enable pin-change on PB4 (PCINT4)
                                                  ; enable pin-change on PB5 (PCINT5)

          ; configure Timer1 as an interrupt timer for 10ms
          ldi GPR_TMP,hi8(RECOIL_TIMER_INIT_VAL)
          sts TCNT1H,GPR_TMP
          ldi GPR_TMP,lo8(RECOIL_TIMER_INIT_VAL)
          sts TCNT1L,GPR_TMP
          ldi GPR_TMP,0x00
          sts TCCR1A,GPR_TMP
          ldi GPR_TMP,0x02
          sts TCCR1B,GPR_TMP
          ldi GPR_TMP,0x01
          sts TIMSK1,GPR_TMP

          ; configuring USART transmit communications (textbook pg. 375)
          ldi       GPR_TMP,(1<<TXEN0)      ;enable transmitter
          sts       UCSR0B,GPR_TMP

          ldi       GPR_TMP,(1<<UCSZ01)|(1<<UCSZ00) ; 8-bit data,
          sts       UCSR0C,GPR_TMP          ;no parity, 1 stop bit

          ldi       GPR_TMP,104             ;9600 baud rate - textbook says 103
          sts       UBRR0L,GPR_TMP          ;for XTAL = 16MHz
		  
		  clr       GPR_TMP
		  sts       UBRR0H,GPR_TMP
		  
		  ; initialize Stack Pointer
		  ldi       GPR_TMP,hi8(RAMEND)     ;initialize high
          out       SPH,GPR_TMP             ;byte of Stack Pointer

          ldi       GPR_TMP,lo8(RAMEND)     ;initialize low
          out       SPL,GPR_TMP             ;byte of Stack Pointer

          ; Set YX5300 output volume to max - default value
          ;ldi       GPR_YX5300_CMD,Command_Set_Volume
          ;ldi       GPR_YX5300_DATA_HIGH,hi8(Volume_Data)
          ;ldi       GPR_YX5300_DATA_LOW,lo8(Volume_Data)
          ;call      soundComs

          ; Set initial charge to empty
          ldi       GPR_TMP, 0
          sts       CHARGE,GPR_TMP

          ; Set initial fire mode to reset
          ldi       GPR_FIRE_MODE,RESET

          ; Set LED state to reset
          ldi       GPR_RED,0
          ldi       GPR_GREEN,0
          ldi       GPR_BLUE,0
          call      setColorRGB
          call      setColorNeoPixel

          ; Initialize color mode variable
          call      checkSwitch

          ; Enable global interrupts
          sei
	  
	  jmp	    loop