; File: portalGunV2Lite.s
;Class: CDA 3104 85149, Fall 2022
; Devs: Sheryll Jacquet, Katarya Johnson-Williams, Jordan Kooyman, Bradley Walby
; Desc: CDA 3104 final project, a simplified mock-up of a control system to make
;         a prop portal gun respond as it does in the games Portal and Portal 2
;         Sheryll: Recoil and Recoil Recharge ISR
;         Kat: Pin Change ISR
;         Jordan: Serial Coms, Neopixel Control, Button and Switch Logic,
;                   project compilation, code testing, and project idea/design
;         Brad: PWM RGB LED Control
; ------------------------------------------------------------------------------

#include "vectorTable.inc"

#define __SFR_OFFSET 0
#include <avr/io.h>

#include "constants.inc"
#include "setup.inc"
#include "inputLogic.inc"
#include "serialSound.inc"
#include "RGBControl.inc"
#include "recoilLogic.inc"


; endless loop for main application
;-------------------------------------------------------------------------------
loop:
          rjmp      loop                ; end_loop