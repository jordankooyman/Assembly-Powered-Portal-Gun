; File: recoilLogic.inc
;Class: CDA 3104 85149, Fall 2022
; Devs: Sheryll Jacquet
; Desc: Recoil Logic processing
; ------------------------------------------------------------------------------

recoil_recharge_ISR:
          ; reinitialize timer
          ldi       GPR_TMP,hi8(RECOIL_TIMER_INIT_VAL)
          sts       TCNT1H,GPR_NPXL_TMP
          ldi       GPR_TMP,lo8(RECOIL_TIMER_INIT_VAL)
          sts       TCNT1L,GPR_NPXL_TMP
		  
          lds       GPR_TMP,CHARGE                 ; load current charge from SRAM
          ldi       GPR_CHARGE,RECHARGE_RATE
          ldi       GPR_MAX_CHARGE,MAX_CHARGE 
          add       GPR_TMP,GPR_CHARGE             ; increase charge
          cp        GPR_TMP,GPR_MAX_CHARGE
          brsh      recharge_return                ; if charge over-filled, do not save
          sts       CHARGE, GPR_TMP                ; save new charge to SRAM
recharge_return:
          reti


recoil:
          lds       GPR_TMP,CHARGE                 ; load current charge from SRAM
          subi      GPR_TMP,CHARGE_PER_SHOT 
          brlo      recoil_return
          sts       CHARGE,GPR_TMP                 ; save new charge to SRAM
          sbi       RECOIL_OUT,RECOIL_PIN          ; turn on recoil LED

; wait for 1/4 second at 16MHz
;   1s = 16,000,000 * 0.25 = 4,000,000 cycles
;   5 cycles * 250 * 200 * 16 = 4,000,000 cycles
;-------------------------------------------------------------------------------
recoil_delay:                           ; void wait_25() {
          ldi       GPR_WAIT_1,16       ; GPR_WAIT_1 = 16;
recoil_delay_1:                         ; do {
          ldi       GPR_WAIT_2,200      ;    GPR_WAIT_2 = 200;
recoil_delay_2:                         ;    do {
          ldi       GPR_TMP,250 ;       GPR_TMP = 250;
recoil_delay_3:                         ;       do {
          nop                           ;          nop;            // waste 1 cycle
          nop                           ;          nop;            // waste 1 cycle
          dec       GPR_TMP     ;          GPR_TMP--;          // 1 cycle
          brne      recoil_delay_3      ;       } while (GPR_TMP > 0); // 2 cycles

          dec       GPR_WAIT_2          ;       GPR_WAIT_2--;
          brne      recoil_delay_2      ;    } while (GPR_WAIT_2 > 0);

          dec       GPR_WAIT_1          ;    GPR_WAIT_1--;
          brne      recoil_delay_1      ; } while (GPR_WAIT_1 > 0);
recoil_return:
          cbi       RECOIL_OUT,RECOIL_PIN             ;
          ret                           ; } // end wait_25