; File: constants.inc
;Class: CDA 3104 85149, Fall 2022
; Devs: Katarya Johnson-Williams, Jordan Kooyman
; Desc: Constants used for improved readability of assembly code, including new
;           GPR constants using addresses rather than GPR constants
;           Also has variables, pins, and constants/commands
; ------------------------------------------------------------------------------

;D12
.set SWI_ONE_DIR, DDRB                  ; set pin mode in/out
.set SWI_ONE_OUT, PORTB                 ; set pin in-mode hi/pu or out-state high/low
.set SWI_ONE_IN,  PINB                  ; read pin-state high/low
.set SWI_ONE_PIN, 5                     ; (B5/13) DDB5/PORTB5/PINB5
;D13
.set SWI_TWO_DIR, DDRB                  ; set pin mode in/out
.set SWI_TWO_OUT, PORTB                 ; set pin in-mode hi/pu or out-state high/low
.set SWI_TWO_IN,  PINB                  ; read pin-state high/low
.set SWI_TWO_PIN, 4                     ; (B4/12) DDB4/PORTB4/PINB4
;D11
.set BTN_RES_DIR, DDRB                  ; set pin mode in/out
.set BTN_RES_OUT, PORTB                 ; set pin in-mode hi/pu or out-state high/low
.set BTN_RES_IN,  PINB                  ; read pin-state high/low
.set BTN_RES_PIN, 3                     ; (B3/11) DDB3/PORTB3/PINB3
;D10
.set BTN_SEC_DIR, DDRB                  ; set pin mode in/out
.set BTN_SEC_OUT, PORTB                 ; set pin in-mode hi/pu or out-state high/low
.set BTN_SEC_IN,  PINB                  ; read pin-state high/low
.set BTN_SEC_PIN, 2                     ; (B2/10) DDB2/PORTB2/PINB2
;D9
.set BTN_PRI_DIR, DDRB                  ; set pin mode in/out
.set BTN_PRI_OUT, PORTB                 ; set pin in-mode hi/pu or out-state high/low
.set BTN_PRI_IN,  PINB                  ; read pin-state high/low
.set BTN_PRI_PIN, 1                     ; (B1/9) DDB1/PORTB1/PINB1
;D3
.set RGB_BLU_DIR, DDRD                  ; set pin mode in/out
.set RGB_BLU_OUT, PORTD                 ; set pin in-mode hi/pu or out-state high/low
.set RGB_BLU_IN,  PIND                  ; read pin-state high/low
.set RGB_BLU_PIN, 3                     ; (PD3) DDD3/PORTD3/PIND3
;D5
.set RGB_GRE_DIR, DDRD                  ; set pin mode in/out
.set RGB_GRE_OUT, PORTD                 ; set pin in-mode hi/pu or out-state high/low
.set RGB_GRE_IN,  PIND                  ; read pin-state high/low
.set RGB_GRE_PIN, 5                     ; (PD5) DDD5/PORTD5/PIND5
;D6
.set RGB_RED_DIR, DDRD                  ; set pin mode in/out
.set RGB_RED_OUT, PORTD                 ; set pin in-mode hi/pu or out-state high/low
.set RGB_RED_IN,  PIND                  ; read pin-state high/low
.set RGB_RED_PIN, 6                     ; (PD6) DDD6/PORTD6/PIND6
;D7
.set RECOIL_DIR, DDRD                  ; set pin mode in/out
.set RECOIL_OUT, PORTD                 ; set pin in-mode hi/pu or out-state high/low
.set RECOIL_IN,  PIND                  ; read pin-state high/low
.set RECOIL_PIN, 7                     ; (PD7) DDD7/PORTD7/PIND7
;D4
.set NPXL_DIR, DDRD                     ; set pin mode in/out
.set NPXL_OUT, PORTD                    ; set pin in-mode hi/pu or out-state high/low
.set NPXL_IN, PIND                      ; read pin-state high/low
.set NPXL_PIN, 4                        ; (PD4) DDD4/PORTD4/PIND4
.set NPXL_COUNT, 16                     ; How many WS2812 LED chips are connected
.set NPXL_ADDRESS_START, 0x0105         ; Where the data array is stored when loading

.set colorMode, 0x0100                  ; color mode variable (updated to show current color set) - SRAM address
.set BLUE_ORANGE, 0
.set LIGHT_BLUE_PURPLE, 1
.set RED_YELLOW, 2

;.set fireMode, 0x0101
.set PRIMARY, 0
.set SECONDARY, 1
.set RESET, 2

;Colors for portal color combinations
.set R_BLUE, 0;0
.set G_BLUE, 0;101
.set B_BLUE, 254;254

.set R_ORANGE, 254;254
.set G_ORANGE, 100;93
.set B_ORANGE, 0;0

.set R_LBLUE, 0;107
.set G_LBLUE, 150;203
.set B_LBLUE, 254;254

.set R_PURPLE, 254;66
.set G_PURPLE, 0;0
.set B_PURPLE, 254;216

.set R_YELLOW, 254;254
.set G_YELLOW, 254;236
.set B_YELLOW, 0;93

.set R_RED, 254;142
.set G_RED, 0;29
.set B_RED, 0;24

; YX5300 Commands and Data
.set Command_Play_Sound, 0x03           ; Play with Index Command
.set Command_Stop_Sound, 0x16
.set Command_Set_Volume, 0x06
;.set Sound_Data_High, 0x1D              ; Play with Volume, 29 volume (1 below max)
.set Sound_Data_High, 0x00
.set Sound_Primary_Fire, 0x02
.set Sound_Secondary_Fire, 0x01
.set Sound_Close, 0x03
.set Sound_Extra_1, 0x04
.set Sound_Extra_2, 0x05
.set Sound_Extra_3, 0x06
.set Sound_Extra_4, 0x07
.set Sound_Extra_5, 0x08
.set Sound_Extra_6, 0x09
.set Sound_Extra_7, 0x0A
.set Stop_Data, 0x0000
.set Volume_Data, 0x001E

.set RECHARGE_RATE,1
.set CHARGE, 0x0102
.set CHARGE_PER_SHOT,10
.set MAX_CHARGE,100
.set RECOIL_TIMER_INIT_VAL, 0xB1E0

; GPR shortcuts - not working with register constants, using address (in decimal)
.set GPR_TMP, 17      ; GPR reserved for initializing values
.set GPR_USART_BYTE, 26       ; variable passed to trnsmt function
.set GPR_YX5300_CMD, 27       ; variable passed to soundComs function
.set GPR_YX5300_DATA_LOW, 28  ; variable passed to soundComs function
.set GPR_YX5300_DATA_HIGH, 29 ; variable passed to soundComs function

.set GPR_NPXL_TMP, 17         ; GPR for current neopixel output pin state
.set GPR_NPXL_BITS, 19        ; GPR for current neopixel transmission bit count
.set GPR_NPXL_BYTES, 2        ; GPR for current neopixel transmission byte count
.set GPR_NPXL_VAL, 3          ; GPR for current neopixel data byte to transmit
.set GPR_NPXL_PXL_CNT, 4      ; GPR for current neopixel pixel counter
.set GPR_HIGH_MASK, 1         ; GPR for neopixel bit mask for high pin state
.set GPR_LOW_MASK, 0          ; GPR for neopixel bit mask for low pin state

.set GPR_CHARGE, 20           ; GPR representing virtual capacitor for recoil check - move to SRAM?
.set GPR_FIRE_MODE, 21        ; GPR storing most recent fire state (primary, secondary, off)
.set GPR_COLOR_MODE, 15       ; GPR temporarily storing current color mode
.set GPR_MAX_CHARGE, 22       ; GPR storing current color mode selection (blue/orange, light blue/purple, red/yellow)

.set GPR_RED, 23              ; variable passed to setColor functions
.set GPR_GREEN, 24            ; variable passed to setColor functions
.set GPR_BLUE, 25             ; variable passed to setColor functions

.set GPR_WAIT_1, 16           ; counter reserved for blocking wait loops
.set GPR_WAIT_2, 18           ; counter reserved for blocking wait loops