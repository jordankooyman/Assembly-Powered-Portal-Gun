/*---------------------------------------------------------------------
  Acrobotic - 01/10/2013
  Author: x1sc0 
  Platforms: Arduino Uno R3
  File: bitbang_whitish.ino

  Description: 
  This code sample accompanies the "How To Control 'Smart' RGB LEDs: 
  WS2811, WS2812, and WS2812B (Bitbanging Tutorial)" Instructable 
  (http://www.instructables.com/id/Bitbanging-step-by-step-Arduino-control-of-WS2811-/) 
  
  The code illustrates how to continuously set one WS2811 (driver)/
  WS2812/WS2812B RGB LEDs to a full intensity whitish.  The color and 
  number of WS281X ICs can be easily modified to explore the full range of
  these wonderful family of LEDs.
  
  The communication is done by bitbanging a self-clocked 800KHz, NZR signal.  
  The implementation was done using assembly so that the timing of the signal 
  was extremely accurate.

  Usage:
  Connect power (5V), ground (GND), and the Arduino Uno pin defined 
  by DIGITAL_PIN to the WS281X ports VCC, GND, and DIN ports, respectively.

  Upload the program to an Arduino Uno, and the Green LED of the first 
  WS281X will light up to full brightness. 

  ------------------------------------------------------------------------
  For a full-blown Arduino library, check out:
  https://github.com/acrobotic/Ai_Library_WS281X/
  ------------------------------------------------------------------------
  Please consider buying products from Acrobotic to help fund future
  Open-Source projects like this! We’ll always put our best effort in every
  project, and release all our design files and code for you to use. 
  http://acrobotic.com/
  ------------------------------------------------------------------------

  License:
  Beerware License; if you find the code useful, and we happen to cross 
  paths, you're encouraged to buy us a beer. The code is distributed hoping
  that you in fact find it useful, but  without warranty of any kind.
------------------------------------------------------------------------*/

#define NUM_RGB       (1)         // Number of WS281X we have connected
#define NUM_BYTES     (NUM_RGB*3) // Number of LEDs (3 per each WS281X)
#define DIGITAL_PIN   (4)         // Digital port number
#define PORT          (PORTD)     // Digital pin's port
#define PORT_PIN      (PORTD4)    // Digital pin's bit position
#define R             (0)        // Intensity of Red LED
#define G             (255)        // Intensity of Green LED
#define B             (0)        // Intensity of Blue LED

#define NUM_BITS      (8)         // Constant value: bits per byte

uint8_t* rgb_arr = NULL;
uint32_t t_f;

void setup() 
{
  pinMode(DIGITAL_PIN,OUTPUT);
  digitalWrite(DIGITAL_PIN,0);
  if((rgb_arr = (uint8_t *)malloc(NUM_BYTES)))             
  {                 
    memset(rgb_arr, 0, NUM_BYTES);                         
  }        
  render();
}

void loop() 
{
  int i;
  for(i=0;i<NUM_RGB;i++)
    setColorRGB(i,R,G,B);
  render();
  delay(1000);
}

void setColorRGB(uint16_t idx, uint8_t r, uint8_t g, uint8_t b) 
{
  if(idx < NUM_RGB) 
  {
    uint8_t *p = &rgb_arr[idx*3]; 
    *p++ = g;  
    *p++ = r;
    *p = b;
  }
}

void render(void) 
{
  if(!rgb_arr) return;

  while((micros() - t_f) < 50L);  // wait for 50us (data latch)

  cli(); // Disable interrupts so that timing is as precise as possible
  volatile uint8_t  
   *p    = rgb_arr,   // Copy the start address of our data array
    val  = *p++,      // Get the current byte value & point to next byte
    high = PORT |  _BV(PORT_PIN), // Bitmask for sending HIGH to pin
    low  = PORT & ~_BV(PORT_PIN), // Bitmask for sending LOW to pin
	// _BV is a macro which returns the “value” of a bit. So, _BV(2) expands to (1<<(2)), which is 0b00000001 shifted left twice: 0b00000100. In this case it is then being used as a mask to select a specific bit within the IRpin_PIN macro, which equates to PIND - so it checks the status of pin D2
	// ~_BV is the inverse of _BV
    tmp  = low,       // Swap variable to adjust duty cycle 
    nbits= NUM_BITS;  // Bit counter for inner loop
  volatile uint16_t
    nbytes = NUM_BYTES; // Byte counter for outer loop
  asm volatile(
  // The volatile attribute is used to tell the compiler not to optimize 
  // this section.  We want every instruction to be left as is.
  //
  // Generating an 800KHz signal (1.25us period) implies that we have
  // exactly 20 instructions clocked at 16MHz (0.0625us duration) to 
  // generate either a 1 or a 0---we need to do it within a single 
  // period. 
  // 
  // By choosing 1 clock cycle as our time unit we can keep track of 
  // the signal's phase (T) after each instruction is executed.
  //
  // To generate a value of 1, we need to hold the signal HIGH (maximum)
  // for 0.8us, and then LOW (minimum) for 0.45us.  Since our timing has a
  // resolution of 0.0625us we can only approximate these values. Luckily, 
  // the WS281X chips were designed to accept a +/- 300ns variance in the 
  // duration of the signal.  Thus, if we hold the signal HIGH for 13 
  // cycles (0.8125us), and LOW for 7 cycles (0.4375us), then the variance 
  // is well within the tolerated range.
  //
  // To generate a value of 0, we need to hold the signal HIGH (maximum)
  // for 0.4us, and then LOW (minimum) for 0.85us.  Thus, holding the
  // signal HIGH for 6 cycles (0.375us), and LOW for 14 cycles (0.875us)
  // will maintain the variance within the tolerated range.
  //
  // For a full description of each assembly instruction consult the AVR
  // manual here: http://www.atmel.com/images/doc0856.pdf
    // Instruction        CLK     Description                 Phase
   "nextbit:\n\t"         // -    label                       (T =  0) 
    "sbi  %0, %1\n\t"     // 2    signal HIGH                 (T =  2) 
    "sbrc %4, 7\n\t"      // 1-2  if MSB set                  (T =  ?)          
    "mov  %6, %3\n\t"     // 0-1  tmp'll set signal high      (T =  4) 
    "dec  %5\n\t"         // 1    decrease bitcount           (T =  5) 
    "nop\n\t"             // 1    nop (idle 1 clock cycle)    (T =  6)
    "st   %a2, %6\n\t"    // 2    set PORT to tmp             (T =  8)
    "mov  %6, %7\n\t"     // 1    reset tmp to low (default)  (T =  9)
    "breq nextbyte\n\t"   // 1-2  if bitcount ==0 -> nextbyte (T =  ?)                
    "rol  %4\n\t"         // 1    shift MSB leftwards         (T = 11)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 13)
    "cbi   %0, %1\n\t"    // 2    signal LOW                  (T = 15)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 17)
    "nop\n\t"             // 1    nop                         (T = 18)
    "rjmp nextbit\n\t"    // 2    bitcount !=0 -> nextbit     (T = 20)
    "nextbyte:\n\t"       // -    label                       -
    "ldi  %5, 8\n\t"      // 1    reset bitcount              (T = 11)
    "ld   %4, %a8+\n\t"   // 2    val = *p++                  (T = 13)
    "cbi   %0, %1\n\t"    // 2    signal LOW                  (T = 15)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 17)
    "nop\n\t"             // 1    nop                         (T = 18)
    "dec %9\n\t"          // 1    decrease bytecount          (T = 19)
    "brne nextbit\n\t"    // 2    if bytecount !=0 -> nextbit (T = 20)
    ::
    // Input operands         Operand Id (w/ constraint)
    "I" (_SFR_IO_ADDR(PORT)), // %0
    "I" (PORT_PIN),           // %1
    "e" (&PORT),              // %a2
    "r" (high),               // %3
    "r" (val),                // %4 - use pointer z -> 30 (low) and 31 (high) -- lo8 and hi8 on a variable
    "r" (nbits),              // %5
    "r" (tmp),                // %6
    "r" (low),                // %7
    "e" (p),                  // %a8
    "w" (nbytes)              // %9
	      //;nbits = gpr -> 8
          //;nbytes = gpr -> 3 times the number of pixels
          //;tmp = gpr that controls whether a 0 or 1 is sent
          //;val = current byte to send
          //;high = bitmask for high - read in PORTD, then set NPXL_PIN bit to 1 and use for reference
          //;low = bitmask for low - read in PORTD, then set NPXL_PIN bit to 0 and use for reference
          //;p = pointer to next byte - need to make the data array of GRB values in SRAM
  );
  sei();                          // Enable interrupts
  t_f = micros();                 // t_f will be used to measure the 50us 
                                  // latching period in the next call of the 
                                  // function.
}
