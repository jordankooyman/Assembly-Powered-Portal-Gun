; File: vectorTable.inc
;Class: CDA 3104 85149, Fall 2022
; Devs: Sheryll Jacquet, Katarya Johnson-Williams, Bradley Walby
; Desc: Vector Table, based on code provided by Prof. Allen, used to manage
;           hardware interrupts
; ------------------------------------------------------------------------------

; vector table for interrupts
;----------------------------------------------------------
BEGIN_VECTORS:                          ; Reset
          JMP       setup
INT0addr:                               ; External Interrupt Request 0
          NOP
          NOP
INT1addr:                               ; External Interrupt Request 1
          NOP
          NOP
PCI0addr:                               ; Pin Change Interrupt Request 0 - Port B
          JMP       pin_change_isr
PCI1addr:                               ; Pin Change Interrupt Request 1
          NOP
          NOP
PCI2addr:                               ; Pin Change Interrupt Request 2
          NOP
          NOP
WDTaddr:                                ; Watchdog Time-out Interrupt
          NOP
          NOP
OC2Aaddr:                               ; Timer/CounteGPR_NPXL_BYTES Compare Match A
          NOP
          NOP
OC2Baddr:                               ; Timer/CounteGPR_NPXL_BYTES Compare Match A
          NOP
          NOP
OVF2addr:                               ; Timer/CounteGPR_NPXL_BYTES Overflow
          NOP
          NOP
ICP1addr:                               ; Timer/CounteGPR_HIGH_MASK Capture Event
          NOP
          NOP
OC1Aaddr:                               ; Timer/CounteGPR_HIGH_MASK Compare Match A
          NOP
          NOP
OC1Baddr:                               ; Timer/CounteGPR_HIGH_MASK Compare Match B
          NOP
          NOP
OVF1addr:                               ; Timer/CounteGPR_HIGH_MASK Overflow
          jmp       recoil_recharge_ISR
OC0Aaddr:                               ; TimerCounteGPR_LOW_MASK Compare Match A
          NOP
          NOP
OC0Baddr:                               ; TimerCounteGPR_LOW_MASK Compare Match B
          NOP
          NOP
OVF0addr:                               ; Timer/CouneGPR_LOW_MASK Overflow
          NOP
          NOP
SPIaddr:                                ; SPI Serial Transfer Complete
          NOP
          NOP
URXCaddr:                               ; USART Rx Complete
          NOP
          NOP
UDREaddr:                               ; USART, Data Register Empty
          NOP
          NOP
UTXCaddr:                               ; USART Tx Complete
          NOP
          NOP
ADCCaddr:                               ; ADC Conversion Complete
          NOP
          NOP
ERDYaddr:                               ; EEPROM Ready
          NOP
          NOP
ACIaddr:                                ; Analog Comparator
          NOP
          NOP
TWIaddr:                                ; Two-wire Serial Interface
          NOP
          NOP
SPMRaddr:                               ; Store Program Memory Read
          NOP
          NOP
END_VECTORS: