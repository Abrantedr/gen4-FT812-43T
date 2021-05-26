// Example built on top of AN_275 FT800 Example with Arduino
// for unit testing purposes.

/* This program shows the FTDI Chip Logo animation on
 * GEN4-FT812-43T resistive screen. Touch and sound are
 * disabled. */

// Author: R. Abrante Delgado
// Date: 26/05/2021

/* Include Files */

#include <Arduino.h>    // Arduino definitions
#include <SPI.h>        // Arduino SPI Library definitions
#include "FT812.h"      // FT812 register, memory and command values

/* Declarations */

// Set LCD display resolution here
#define LCD_WQVGA       // WQVGA = 480 x 272 (4.3")

// Set Arduino platform here
#define TEENSY          // Teensy 3.2 (I/O 10 on SS#)

// Set SPI speed here
#define SPI_SPEED 1000000     // SPI transfer speed
                              // 1-10 MHz is recommended
// FT800 Chip Commands - use with cmdWrite
#define FT812_ACTIVE  0x00      // Initializes FT812
#define FT812_STANDBY 0x41      // Place FT812 in Standby (clk running)
#define FT812_SLEEP   0x42      // Place FT812 in Sleep (clk off)
#define FT812_PWRDOWN 0x50      // Place FT812 in Power Down (core off)
#define FT812_CLKEXT  0x44      // Select external clock source
#define FT812_CLKINT  0x48      // Select internal relaxation oscillator
#define FT812_CLK48M  0x62      // Select 48MHz PLL
#define FT812_CLK36M  0x61      // Select 36MHz PLL
#define FT812_CORERST 0x68      // Reset core - all registers default

// FT812 Memory Commands - use with ft812memWritexx and ft812memReadxx
#define MEM_WRITE 0x80      // FT812 Host Memory Write 
#define MEM_READ  0x00      // FT812 Host Memory Read

// Colors - fully saturated colors defined here
#define RED     0xFF0000UL    // Red
#define GREEN   0x00FF00UL    // Green
#define BLUE    0x0000FFUL    // Blue
#define WHITE   0xFFFFFFUL    // White
#define BLACK   0x000000UL    // Black


/* Global Variables */

// Arduino pins - others defined by Serial and SPI libraries
unsigned int triggerPin;  // Oscilloscope/logic analyzer trigger
unsigned int SPI_INT;     // Interrupt from FT812 to Teensy
unsigned int SPI_PD;      // Power Down from Teensy to FT812
unsigned int SPI_CS;      // SPI chip select

// SPI Settings
SPISettings GEN4(SPI_SPEED, MSBFIRST, SPI_MODE0);

// LCD display parameters
unsigned int lcdWidth;        // Active width of LCD display
unsigned int lcdHeight;       // Active height of LCD display
unsigned int lcdHcycle;       // Total number of clocks per line
unsigned int lcdHoffset;      // Start of active line
unsigned int lcdHsync0;       // Start of horizontal sync pulse
unsigned int lcdHsync1;       // End of horizontal sync pulse
unsigned int lcdVcycle;       // Total number of lines per screen
unsigned int lcdVoffset;      // Start of active screen
unsigned int lcdVsync0;       // Start of vertical sync pulse
unsigned int lcdVsync1;       // End of vertical sync pulse
unsigned char lcdPclk;        // Pixel Clock
unsigned char lcdSwizzle;     // Define RGB output pins
unsigned char lcdPclkpol;     // Define active edge of PCLK

unsigned long ramDisplayList = RAM_DL;    // Beginning of display list memory
unsigned long ramCommandBuffer = RAM_CMD; // Beginning of graphics cmd memory

unsigned int cmdBufferRd = 0x0000;    // Used to navigate command ring buffer
unsigned int cmdBufferWr = 0x0000;    // Used to navigate command ring buffer
unsigned int cmdOffset = 0x0000;      // Used to navigate command rung buffer
unsigned char ft812Gpio;              // Used for FT812 GPIO register


/******************************************************************************
   Function:        void ft812memWritexx(ftAddress, ftDataxx, ftLength)
   PreCondition:    None
   Input:           ftAddress = FT812 memory space address
                    ftDataxx = a byte, int or long to send
   Output:          None
   Side Effects:    None
   Overview:        Writes FT812 internal address space
   Note:            "xx" is one of 8, 16 or 32
 *****************************************************************************/
void ft812memWrite8(unsigned long ftAddress, unsigned char ftData8)
{
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE);  
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer(ftData8);                          // Send data byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
}

void ft812memWrite16(unsigned long ftAddress, unsigned int ftData16)
{
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE);  
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer((char)(ftData16));                 // Send data low byte
  SPI.transfer((char)(ftData16 >> 8));            // Send data high byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
}

void ft812memWrite32(unsigned long ftAddress, unsigned long ftData32)
{
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE);  
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer((char)(ftData32));                 // Send data low byte
  SPI.transfer((char)(ftData32 >> 8));
  SPI.transfer((char)(ftData32 >> 16));
  SPI.transfer((char)(ftData32 >> 24));           // Send data high byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
}


/******************************************************************************
 * Function:        unsigned char ft812memReadxx(ftAddress, ftLength)
 * PreCondition:    None
 * Input:           ftAddress = FT812 memory space address
 * Output:          ftDataxx (byte, int or long)
 * Side Effects:    None
 * Overview:        Reads FT812 internal address space
 * Note:            "xx" is one of 8, 16 or 32
 *****************************************************************************/
unsigned char ft812memRead8(unsigned long ftAddress)
{
  unsigned char ftData8 = ZERO;
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ);   
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer(ZERO);                             // Send dummy byte
    ftData8 = SPI.transfer(ZERO);                 // Read data byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
  return ftData8;                                 // Return byte read
}

unsigned int ft812memRead16(unsigned long ftAddress)
{
  unsigned int ftData16;
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ);
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer(ZERO);                             // Send dummy byte
    ftData16 = (SPI.transfer(ZERO));              // Read low byte
    ftData16 = ((SPI.transfer(ZERO) << 8) | ftData16);  // Read high byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
  return ftData16;                                // Return integer read
}

unsigned long ft812memRead32(unsigned long ftAddress)
{
  unsigned long ftData32;
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ);   
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer(ZERO);                             // Send dummy byte
    ftData32 = (SPI.transfer(ZERO));                  // Read low byte
    ftData32 = (SPI.transfer(ZERO) << 8) | ftData32;
    ftData32 = (SPI.transfer(ZERO) << 16) | ftData32;
    ftData32 = (SPI.transfer(ZERO) << 24) | ftData32; // Read high byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
  return ftData32;                                // Return long read
}


/******************************************************************************
 * Function:        void ft812cmdWrite(ftCommand)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        Sends FT812 command
 * Note:            None
 *****************************************************************************/
void ft812cmdWrite(unsigned char ftCommand)
{
  SPI.beginTransaction(GEN4);     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);      // Set CS# low
  SPI.transfer(ftCommand);        // Send command
  SPI.transfer(0x00);             // Commands consist of two more zero bytes
  SPI.transfer(0x00);             // Send last zero byte
  digitalWrite(SPI_CS, HIGH);     // Set CS# high
  SPI.endTransaction();           // End transaction for GEN4
}

/******************************************************************************
 * Function:        void incCMDOffset(currentOffset, commandSize)
 * PreCondition:    None
 *                    starting a command list
 * Input:           currentOffset = graphics processor command list pointer
 *                  commandSize = number of bytes to increment the offset
 * Output:          newOffset = new ring buffer pointer after adding the cmd
 * Side Effects:    None
 * Overview:        Adds commandSize to the currentOffset.  
 *                  Checks for 4K ring-buffer offset roll-over 
 * Note:            None
 *****************************************************************************/
unsigned int incCMDOffset(unsigned int currentOffset, unsigned char commandSize)
{
    unsigned int newOffset;                   // used to hold new offset
    newOffset = currentOffset + commandSize;  // Calculate new offset
    if(newOffset > 4095)                      // If new offset past boundary...
    {
        newOffset = (newOffset - 4096);       // ... roll over pointer
    }
    return newOffset;                         // Return new offset
}

/******************************************************************************
 * Function:        void setup(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        System initialization routines.  Executed once on reset.
 * Note:            Default Arduino function
 *****************************************************************************/
  /* The Teensy 3.2 I/O pins are connected to the GEN4-FT812-43T as follows:
      Digital
        Pin 0 = n/c  (out)   Pin 8    = n/c
        Pin 1 = n/c  (in)    Pin 9    = n/c
        Pin 2 = n/c          Pin 10   = CS#  (out)
        Pin 3 = INT# (in)    Pin 11   = MOSI (out)
        Pin 4 = PD#  (out)   Pin 12   = MISO (in)
        Pin 5 = n/c          Pin 13   = SCK  (out)
        pin 6 = n/c          Pin GND  = GND
        pin 7 = n/c          Pin AREF = n/c
        
      Analog and power headers = n/c
  */

void setup() {
  // Teensy <-> FT812 connection
  #ifdef TEENSY
    triggerPin = 2;     // Used for oscilloscope/logic analyzer trigger
    SPI_INT = 3;        // Interrupt from FT812 to Teensy
    SPI_PD = 4;         // Power Down from Teensy to FT812
    SPI_CS = 10;        // SPI chip select
  #endif
  
  #ifdef LCD_WQVGA      // WQVGA display parameters
    lcdWidth   = 480;   // Active width of LCD display
    lcdHeight  = 272;   // Active height of LCD display
    lcdHcycle  = 548;   // Total number of clocks per line
    lcdHoffset = 43;    // Start of active line
    lcdHsync0  = 0;     // Start of horizontal sync pulse
    lcdHsync1  = 41;    // End of horizontal sync pulse
    lcdVcycle  = 292;   // Total number of lines per screen
    lcdVoffset = 12;    // Start of active screen
    lcdVsync0  = 0;     // Start of vertical sync pulse
    lcdVsync1  = 10;    // End of vertical sync pulse
    lcdPclk    = 5;     // Pixel Clock
    lcdSwizzle = 0;     // Define RGB output pins
    lcdPclkpol = 1;     // Define active edge of PCLK
  #endif

  Serial.begin(9600);   // Initialize UART for debugging messages
  
  SPI.begin();                        // Start SPI settings
  
  pinMode(triggerPin, OUTPUT);        // Oscilloscope triggering
  pinMode(SPI_INT, INPUT_PULLUP);     // FT812 interrupt output
  pinMode(SPI_PD, OUTPUT);            // FT812 Power Down (reset) input
  pinMode(SPI_CS, OUTPUT);            // FT812 SPI bus CS# input
   
  digitalWrite(triggerPin, LOW);      // Initialize the oscilloscope trigger
  digitalWrite(SPI_CS, HIGH);         // Set CS# high to start - SPI inactive
  digitalWrite(SPI_PD, HIGH);         // Set PD# high to start
  delay(20);                          // Wait a few MS before waking the FT812

  /* Wake-up FT812 */
  digitalWrite(SPI_PD, LOW);          // 1) lower PD#
  delay(20);                          // 2) hold for 20ms
  digitalWrite(SPI_PD, HIGH);         // 3) raise PD#
  delay(20);                          // 4) wait before sending any commands
  
  ft812cmdWrite(FT812_ACTIVE);        // Start FT812
  delay(300);                         // Give some time to process
  
  // Now FT812 can accept commands at up to 30MHz clock on SPI bus
  // This application leaves the SPI bus at 10MHz

  while (ft812memRead8(REG_ID) != 0x7C) // Read ID register - is it 0x7C?
  {
    // If we don't get 0x7C, the interface isn't working - halt with inf. loop
  }
  
  ft812memWrite8(REG_PCLK, ZERO);       // Set PCLK to zero
                                        // Don't clock the LCD until later
  ft812memWrite8(REG_PWM_DUTY, ZERO);   // Turn off backlight
  /* End of Wake-up FT800 */

  /* Initialize Display */
  ft812memWrite16(REG_HSIZE,   lcdWidth);   // active display width
  ft812memWrite16(REG_HCYCLE,  lcdHcycle);  // total number of clocks per line
  ft812memWrite16(REG_HOFFSET, lcdHoffset); // start of active line
  ft812memWrite16(REG_HSYNC0,  lcdHsync0);  // start of horizontal sync pulse
  ft812memWrite16(REG_HSYNC1,  lcdHsync1);  // end of horizontal sync pulse
  ft812memWrite16(REG_VSIZE,   lcdHeight);  // active display height
  ft812memWrite16(REG_VCYCLE,  lcdVcycle);  // total number of lines per screen
  ft812memWrite16(REG_VOFFSET, lcdVoffset); // start of active screen
  ft812memWrite16(REG_VSYNC0,  lcdVsync0);  // start of vertical sync pulse
  ft812memWrite16(REG_VSYNC1,  lcdVsync1);  // end of vertical sync pulse
  ft812memWrite8(REG_SWIZZLE,  lcdSwizzle); // FT800 output to LCD - pin order
  ft812memWrite8(REG_PCLK_POL, lcdPclkpol); // Data clocked on this PCLK edge
  // Don't set PCLK yet - wait for just after the first display list
  /* End of Initialize Display */

  /* Configure Touch and Audio - not used in this example, so disable both */
  ft812memWrite8(REG_TOUCH_MODE, ZERO);       // Disable touch
  ft812memWrite16(REG_TOUCH_RZTHRESH, ZERO);  // Eliminate any false touches
  
  ft812memWrite8(REG_VOL_PB, ZERO);           // turn recorded volume down
  ft812memWrite8(REG_VOL_SOUND, ZERO);        // turn synthesizer volume down
  ft812memWrite16(REG_SOUND, 0x6000);         // set synthesizer to mute
  /* End of Configure Touch and Audio */

  /* Write Initial Display List & Enable Display */
  ramDisplayList = RAM_DL;                            // start of Display List
  // Clear Color RGB   00000010 RRRRRRRR GGGGGGGG BBBBBBBB
  // (R/G/B = Colour values) default zero / black
  ft812memWrite32(ramDisplayList, DL_CLEAR_RGB);      
  ramDisplayList += 4;                                // point to next location
  // Clear 00100110 -------- -------- -----CST
  // (C/S/T define which parameters to clear)
  ft812memWrite32(ramDisplayList, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));  
  ramDisplayList += 4;                                // point to next location
  // DISPLAY command 00000000 00000000 00000000 00000000 (end of display list)
  ft812memWrite32(ramDisplayList, DL_DISPLAY);                                
  // 00000000 00000000 00000000 000000SS  (SS bits define when render occurs)
  ft812memWrite32(REG_DLSWAP, DLSWAP_FRAME);                                  
  // Nothing is being displayed yet... the pixel clock is still 0x00
  ramDisplayList = RAM_DL;          // Reset Display List pointer for next list

  // Read the FT800 GPIO register for a read/modify/write operation
  ft812Gpio = ft812memRead8(REG_GPIO);
  // set bit 7 of FT800 GPIO register (DISP) - others are inputs
  ft812Gpio = ft812Gpio | 0x80;
  // Enable the DISP signal to the LCD panel        
  ft812memWrite8(REG_GPIO, ft812Gpio);
  // Now start clocking data to the LCD panel
  ft812memWrite8(REG_PCLK, lcdPclk);   
  for(int duty = 0; duty <= 128; duty++)
  {
    // Turn on backlight - ramp up slowly to full brighness
    ft812memWrite8(REG_PWM_DUTY, duty);   
    delay(10);
  }
  /* End of Write Initial Display List & Enable Display */
} // END of setup()


/******************************************************************************
 * Function:        void loop(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        Repetitive activities - Draw a dot on the LCD
 *                  Continually change colors from white to red and back
 * Note:            Default Arduino function
 *****************************************************************************/
void loop() 
{
  // Wait for graphics processor to complete executing the current command list
  // This happens when REG_CMD_READ matches REG_CMD_WRITE, indicating that all 
  // commands have been executed.  The next commands get executed when 
  // REG_CMD_WRITE is updated again...
  // then REG_CMD_READ again catches up to REG_CMD_WRITE
  // This is a 4Kbyte ring buffer, so keep pointers within the 4K roll-over
  do
  {
    // Read the graphics processor read pointer
    cmdBufferRd = ft812memRead16(REG_CMD_READ);
    // Read the graphics processor write pointer
    cmdBufferWr = ft812memRead16(REG_CMD_WRITE);  
  } while (cmdBufferWr != cmdBufferRd);   // Wait until the two registers match
  
  // The new starting point the first location after the last command
  cmdOffset = cmdBufferWr;  
    
  // Start the display list
  ft812memWrite32(RAM_CMD + cmdOffset, (CMD_DLSTART));
  cmdOffset = incCMDOffset(cmdOffset, 4);         // Update the command pointer

  // Set the default clear color to black
  ft812memWrite32(RAM_CMD + cmdOffset, (DL_CLEAR_RGB | BLACK));
  cmdOffset = incCMDOffset(cmdOffset, 4);         // Update the command pointer

  // Clear the screen - this and the previous prevent artifacts between lists
  // Attributes are the color, stencil and tag buffers
  ft812memWrite32(RAM_CMD + cmdOffset, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
  cmdOffset = incCMDOffset(cmdOffset, 4);         // Update the command pointer

  // Show FTDI Chip Logo animation
  ft812memWrite32(RAM_CMD + cmdOffset, (CMD_LOGO)); 
  cmdOffset = incCMDOffset(cmdOffset, 4);

  // Instruct the graphics processor to show the list
  ft812memWrite32(RAM_CMD + cmdOffset, (DL_DISPLAY));
  cmdOffset = incCMDOffset(cmdOffset, 4);         // Update the command pointer
  
  // Make this list active
  ft812memWrite32(RAM_CMD + cmdOffset, (CMD_SWAP));
  cmdOffset = incCMDOffset(cmdOffset, 4); // Update the command pointer

  // Update the ring buffer pointer so the graphics processor starts executing
  ft812memWrite16(REG_CMD_WRITE, (cmdOffset));  

  delay(3000);         // Wait for animation to end (around 2.5 seconds)
} // End of loop()

/** EOF GEN4.ino ****************************************/
