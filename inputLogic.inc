; File: inputLogic.inc
;Class: CDA 3104 85149, Fall 2022
; Devs: Katarya Johnson-Williams, Jordan Kooyman
; Desc: Port B Pin Change Interrupt, and the main logic for changing LED color
;           based on button pressed and current switch position
; ------------------------------------------------------------------------------

; pin change interrupt service routine (for all of Port B)
;----------------------------------------------------------
pin_change_isr:
; code for button checking
          ; see if primary button is pressed
          sbis      BTN_PRI_IN, BTN_PRI_PIN  ; if low jump to function
          jmp       button_primary           ; else fall through
          ; see if secondary button is pressed
          sbis      BTN_SEC_IN, BTN_SEC_PIN  ; if low jump to function
          jmp       button_secondary         ; else fall through
          ; see if reset button is pressed
          sbis      BTN_RES_IN, BTN_RES_PIN  ; if low jump to function
          jmp       button_reset             ; else fall through
; code for switch checking
          call      checkSwitch
          jmp       pin_change_return

button_primary:;falling to red/yellow after reset clicked, fixed after first run
          ; actually, this logic appears to reset colorMode to blue/orange, reset restores prior state
          ;ldi       GPR_YX5300_CMD,Command_Stop_Sound
          ;ldi       GPR_YX5300_DATA_HIGH,hi8(Stop_Data)
          ;ldi       GPR_YX5300_DATA_LOW,lo8(Stop_Data)
          ;call      soundComs

          ldi       GPR_YX5300_CMD,Command_Play_Sound
          ldi       GPR_YX5300_DATA_HIGH,Sound_Data_High
          ldi       GPR_YX5300_DATA_LOW,Sound_Primary_Fire

          ;clr       GPR_TMP

          ldi       GPR_FIRE_MODE,PRIMARY

          lds       GPR_TMP,colorMode
          cpi       GPR_TMP,BLUE_ORANGE
          brne      color_1_p                       ;if colorMode blueOrange
          ldi       GPR_RED,R_BLUE
          ldi       GPR_GREEN,G_BLUE
          ldi       GPR_BLUE,B_BLUE

          jmp       fire_call
color_1_p:
          cpi       GPR_TMP,LIGHT_BLUE_PURPLE
          brne      color_2_p                       ;else if colorMode LIGHT_BLUE_PURPLE
          ldi       GPR_RED,R_LBLUE
          ldi       GPR_GREEN,G_LBLUE
          ldi       GPR_BLUE,B_LBLUE

          jmp       fire_call
color_2_p:                                          ;else colorMode RED_YELLOW
          ldi       GPR_RED,R_RED
          ldi       GPR_GREEN,G_RED
          ldi       GPR_BLUE,B_RED

          jmp       fire_call

button_secondary:;falling to red/yellow after reset clicked, fixed after first run
          ;ldi       GPR_YX5300_CMD,Command_Stop_Sound
          ;ldi       GPR_YX5300_DATA_HIGH,hi8(Stop_Data)
          ;ldi       GPR_YX5300_DATA_LOW,lo8(Stop_Data)
          ;call      soundComs

          ldi       GPR_YX5300_CMD,Command_Play_Sound
          ldi       GPR_YX5300_DATA_HIGH,Sound_Data_High
          ldi       GPR_YX5300_DATA_LOW,Sound_Secondary_Fire

          clr       GPR_TMP

          ldi       GPR_FIRE_MODE,SECONDARY

          lds       GPR_TMP,colorMode
          cpi       GPR_TMP,BLUE_ORANGE
          brne      color_1_s                       ;if colorMode blueOrange
          ldi       GPR_RED,R_ORANGE
          ldi       GPR_GREEN,G_ORANGE
          ldi       GPR_BLUE,B_ORANGE

          jmp       fire_call
color_1_s:
          cpi       GPR_TMP,LIGHT_BLUE_PURPLE
          brne      color_2_s                       ;else if colorMode LIGHT_BLUE_PURPLE
          ldi       GPR_RED,R_PURPLE
          ldi       GPR_GREEN,G_PURPLE
          ldi       GPR_BLUE,B_PURPLE

          jmp       fire_call
color_2_s:                                          ;else colorMode RED_YELLOW
          ldi       GPR_RED,R_YELLOW
          ldi       GPR_GREEN,G_YELLOW
          ldi       GPR_BLUE,B_YELLOW

          jmp       fire_call

button_reset:
          ;ldi       GPR_YX5300_CMD,Command_Stop_Sound
          ;ldi       GPR_YX5300_DATA_HIGH,hi8(Stop_Data)
          ;ldi       GPR_YX5300_DATA_LOW,lo8(Stop_Data)
          ;call      soundComs

          ldi       GPR_YX5300_CMD,Command_Play_Sound
          ldi       GPR_YX5300_DATA_HIGH,Sound_Data_High
          ldi       GPR_YX5300_DATA_LOW,Sound_Close

          ldi       GPR_FIRE_MODE,RESET

          ldi       GPR_RED,0
          ldi       GPR_GREEN,0
          ldi       GPR_BLUE,0

          call      setColorRGB
          call      soundComs
          call      setColorNeoPixel
          jmp       pin_change_return

fire_call:
          call      setColorRGB
          call      soundComs
          call      setColorNeoPixel
          call      recoil ; has a blocking wait loop, run last
          rjmp      pin_change_return

; wait for 100  micro-seconds at 16MHz
;   100us = 1,600 cycles
;   5 cycles * 160 * 2 = 1,600 cycles
pin_change_return:                      ; void pin_change_return() {
          ldi       GPR_WAIT_1,2        ; GPR_WAIT_1 = 2;
pin_change_wait_1:                      ;    do {
          ldi       GPR_WAIT_2,160      ;       GPR_WAIT_2 = 160;
pin_change_wait_2:                      ;       do {
          nop                           ;          nop;            // waste 1 cycle
          nop                           ;          nop;            // waste 1 cycle
          dec       GPR_WAIT_2          ;          GPR_WAIT_2--;          // 1 cycle
          brne      pin_change_wait_2   ;       } while (GPR_WAIT_2 > 0); // 2 cycles
          dec       GPR_WAIT_1          ;    GPR_WAIT_1--;
          brne      pin_change_wait_1   ; } while (GPR_WAIT_1 > 0);
          reti                          ; end pin_change_isr

; check switches to update color mode variable
;----------------------------------------------------------
checkSwitch:
; code for switch checking
          ; see if left switch is low
          sbis      SWI_ONE_IN, SWI_ONE_PIN  ; if low jump to function
          jmp       switch_left              ; else fall through
          ; see if right switch is low
          sbis      SWI_TWO_IN, SWI_TWO_PIN  ; if low jump to function
          jmp       switch_right             ; else fall through
          ; else both are high
          jmp       switch_middle            ; else fall through

end_switch_extension:
          jmp       end_switch

; Handle color mode select switch, including updating current LED color
switch_left: ; one goes low - different LEDs (0) - left
          ; If color mode does match, don't do anything
          lds       GPR_TMP, colorMode
          cpi       GPR_TMP, BLUE_ORANGE
          breq      end_switch_extension

          ldi       GPR_TMP, BLUE_ORANGE
          mov       GPR_COLOR_MODE,GPR_TMP

          cpi       GPR_FIRE_MODE,PRIMARY
          brne      left_2                        ; If not primary, secondary?

          ldi       GPR_RED,R_BLUE
          ldi       GPR_GREEN,G_BLUE
          ldi       GPR_BLUE,B_BLUE
          call      setColorRGB
          call      setColorNeoPixel
          jmp       end_switch                    ; jump to end
left_2:
          cpi       GPR_FIRE_MODE,SECONDARY
          brne      end_switch_extension          ; else, LEDs off, do not need to update state, so jump to end

          ldi       GPR_RED,R_ORANGE
          ldi       GPR_GREEN,G_ORANGE
          ldi       GPR_BLUE,B_ORANGE
          call      setColorRGB
          call      setColorNeoPixel
          jmp       end_switch                    ; jump to end

switch_middle: ; both are high - different LEDs (1) - middle
          ; If color mode does match, don't do anything
          lds       GPR_TMP, colorMode
          cpi       GPR_TMP, LIGHT_BLUE_PURPLE
          breq      end_switch

          ldi       GPR_TMP, LIGHT_BLUE_PURPLE
          mov       GPR_COLOR_MODE,GPR_TMP

          cpi       GPR_FIRE_MODE,PRIMARY
          brne      middle_2                      ; If not primary, secondary?

          ldi       GPR_RED,R_LBLUE
          ldi       GPR_GREEN,G_LBLUE
          ldi       GPR_BLUE,B_LBLUE
          call      setColorRGB
          call      setColorNeoPixel
          rjmp      end_switch                    ; jump to end
middle_2:
          cpi       GPR_FIRE_MODE,SECONDARY
          brne      end_switch                    ; else, LEDs off, do not need to update state, so jump to end

          ldi       GPR_RED,R_PURPLE
          ldi       GPR_GREEN,G_PURPLE
          ldi       GPR_BLUE,B_PURPLE
          call      setColorRGB
          call      setColorNeoPixel
          rjmp      end_switch               ; jump to end

switch_right: ; one goes low - different LEDs (2) - right
          ; If color mode does match, don't do anything
          lds       GPR_TMP, colorMode
          cpi       GPR_TMP, RED_YELLOW
          breq      end_switch

          ldi       GPR_TMP, RED_YELLOW
          mov       GPR_COLOR_MODE,GPR_TMP

          cpi       GPR_FIRE_MODE,PRIMARY
          brne      right_2                       ; If not primary, secondary?

          ldi       GPR_RED,R_RED
          ldi       GPR_GREEN,G_RED
          ldi       GPR_BLUE,B_RED
          call      setColorRGB
          call      setColorNeoPixel
          rjmp      end_switch                    ; jump to end
right_2:
          cpi       GPR_FIRE_MODE,SECONDARY
          brne      end_switch                    ; else, LEDs off, do not need to update state, so jump to end

          ldi       GPR_RED,R_YELLOW
          ldi       GPR_GREEN,G_YELLOW
          ldi       GPR_BLUE,B_YELLOW
          call      setColorRGB
          call      setColorNeoPixel
          rjmp      end_switch               ; jump to end

end_switch:
          sts       colorMode, GPR_COLOR_MODE
          call      setColorRGB
          call      setColorNeoPixel
          ret