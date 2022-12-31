; File: RGBControl.inc
;Class: CDA 3104 85149, Fall 2022
; Devs: Jordan Kooyman, Bradley Walby
; Desc: Both RGB LED control functions, hardware PWM and Neopixel (ws2812b)
;           communication protocol
; ------------------------------------------------------------------------------

; Transmit data by Bit-Banging to Neopixels (WS2812 LEDs) to set each pixel to
; the same color
; Communication code provided by:
; https://www.instructables.com/Bitbanging-step-by-step-Arduino-control-of-WS2811-/
setColorNeoPixel:
; loop pixel count times, each time outputting GRB bits (high bit first)
; first, prep color data into SRAM, then set first color, then wait 50us
;   before doing next pixel, until all pixels have been done
          ; Initialize Neopixel pixel count
          ldi       GPR_NPXL_TMP,NPXL_COUNT
          mov       GPR_NPXL_PXL_CNT,GPR_NPXL_TMP

npxl_trnsmt:
          ; Initialze Color Data into SRAM
          sts       NPXL_ADDRESS_START,GPR_GREEN            ;green
          sts       NPXL_ADDRESS_START+1,GPR_RED            ;red
          sts       NPXL_ADDRESS_START+2,GPR_BLUE           ;blue

          ; Initialize bytes
          ldi       GPR_NPXL_TMP,3;(NPXL_COUNT * 3)
          mov       GPR_NPXL_BYTES,GPR_NPXL_TMP

          ; Initalize bits
          ldi       GPR_NPXL_BITS, 8

          ; Initalize masks
          lds       GPR_NPXL_TMP,NPXL_IN
          sbr       GPR_NPXL_TMP,(1<<NPXL_PIN); set NPXL_PIN bit high
          mov       GPR_HIGH_MASK,GPR_NPXL_TMP
          cbr       GPR_NPXL_TMP,(1<<NPXL_PIN); set NPXL_PIN bit low
          mov       GPR_LOW_MASK,GPR_NPXL_TMP

          ; Initialize Color Data Pointer (Z)
          ldi       ZH, hi8(NPXL_ADDRESS_START)
          ldi       ZL, lo8(NPXL_ADDRESS_START)

          ; Disable global interrupts during data transmission (should be off,
          ;  since called from ISR)
          cli

          ; Initialize first byte of data
          ld        GPR_NPXL_VAL,Z+
  ; To generate a value of 1, we need to hold the signal HIGH (maximum)
  ; for 0.8us, and then LOW (minimum) for 0.45us.  Since our timing has a
  ; resolution of 0.0625us we can only approximate these values. Luckily,
  ; the WS281X chips were designed to accept a +/- 300ns variance in the
  ; duration of the signal.  Thus, if we hold the signal HIGH for 13
  ; cycles (0.8125us), and LOW for 7 cycles (0.4375us), then the variance
  ; is well within the tolerated range.
  ; To generate a value of 0, we need to hold the signal HIGH (maximum)
  ; for 0.4us, and then LOW (minimum) for 0.85us.  Thus, holding the
  ; signal HIGH for 6 cycles (0.375us), and LOW for 14 cycles (0.875us)
  ; will maintain the variance within the tolerated range.

  ; Each pixel must be sent 24 consecutive bits containing 8 bits for green,
  ; then red, then blue. Additional connected pixels recieve their data through
  ; the first one, so all data must be sent consecutively. Each pixel will latch
  ; the first 24 bits it recieves, then transmit the rest of the data down the
  ; line to the next pixel (all pixels are daisy-chained. The following code
  ; loops through an array in SRAM containing the GRB data for each pixel. The
  ; byte-count variable has been set to only do a single pixel worth of data,
  ; after which the array pointer is reset to the start of the array and recalls
  ; the transmit code until all the pixels have been transmitted to.
nextbit:
          sbi       NPXL_OUT,NPXL_PIN             ; Signal high
          sbrc      GPR_NPXL_VAL,7                ; if MSB set
          mov       GPR_NPXL_TMP,GPR_HIGH_MASK    ; tmp will set signal high
          dec       GPR_NPXL_BITS                 ; decrease bitcount
          nop
          out       NPXL_OUT,GPR_NPXL_TMP         ; set NPXL Port to tmp
          mov       GPR_NPXL_TMP,GPR_LOW_MASK     ; reset tmp to low (default)
          tst       GPR_NPXL_BITS
          breq      nextbyte                      ; if bitcount == 0, go to nextbyte
          rol       GPR_NPXL_VAL                  ; shift MSB leftwards
          nop
          nop
          cbi       NPXL_OUT,NPXL_PIN             ; signal low
          nop
          nop
          nop
          rjmp      nextbit                       ; bitcount != 0, go to nextbit
nextbyte:
          ldi       GPR_NPXL_BITS,8               ; reset bitcount
          ld        GPR_NPXL_VAL,Z+               ; load next byte from SRAM to send (pointer) with post increment
          cbi       NPXL_OUT,NPXL_PIN             ; signal low
          nop
          nop
          dec       GPR_NPXL_BYTES                ; decrease bytecount
          tst       GPR_NPXL_BYTES
          brne      nextbit                       ; if bytecount != 0 go to next bit

          dec       GPR_NPXL_PXL_CNT              ; Decrease remaining count of Neopixels to talk to
          tst       GPR_NPXL_PXL_CNT
          brne      npxl_trnsmt                   ; if NPXL_PXL_CNT != 0, then send next pixel data

          cbi       NPXL_OUT,NPXL_PIN             ; Stop writing data
          ; Don't enable global interrupts, already disabled since called from ISR
          ;sei                                     ;enable global interrupts
          ret


;Following code provided by Professor Allen, modified for this project based on
; assembly textbook
setColorRGB: ;avoid 255 entirely
          out       OCR0A,GPR_RED
          out       OCR0B,GPR_GREEN
          ;fast pwm non-inverted for wave generator A and B (timeGPR_LOW_MASK) overflowing
          ; at end of timer (background)
          ldi       GPR_TMP,(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00)
          out       TCCR0A,GPR_TMP
          ldi       GPR_TMP,1
          out       TCCR0B,GPR_TMP

          sts       OCR2B,GPR_BLUE
          ;fast pwm non-inverted for wave generator B (timeGPR_NPXL_BYTES) overflowing
          ; at end of timer (background)
          ldi       GPR_TMP,(1<<COM2B1)|(1<<WGM21)|(1<<WGM20)
          sts       TCCR2A,GPR_TMP
          ldi       GPR_TMP,1
          sts       TCCR2B,GPR_TMP

          ret


