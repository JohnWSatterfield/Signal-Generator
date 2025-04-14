#ifndef _CONFIG_H_			      // Prevent double inclusion

#define _CONFIG_H_

#define TESTING    1          // For testing the display
#define RUN        2          // For normal operation

#define ETHERKIT   1          // SI5351 Etherkit Driver
#define MCU        2          // SI5351 MCU Driver

#define ATLAS_CO   1          // Radio using the original carrier oscilator (CO)
#define SI5351_CO  2          // Radio using the si5351 CLK1 for carrier oscilator (CO)

#define	SMALL_DISP		1	      // 1.77inch - 128x160 display
#define	LARGE_DISP		2	      // 2.0 inch - 240x320 display
#define	CUSTOM_DISP		3	      // 1.9 inch - 172x320 display

#define OLDER 1               //5.5200 MHz crystal Filter for Atlas
#define NEWER 2               //5.6450 MHz crystal Filter for Atlas

#define A180   1               // Atlas Radio 180
#define A215   2               // Atlas Radio 215 & 215X
#define A210   3               // Atlas Radio 210 & 210X

#define LPF4052   1            // 74HCT4052 LPF Control 
#define LPFBCD    2            // BCD LPF Control

#define JOHN      1
#define CLINT     2
#define MARK      3

//----------From this point forward you must sed definitions to run properly
//          The Preference section on Line 305-387 should allow you to run

//------------Select the Low Pass Filter Control Used--------------------------

#define LPF_CTRL   LPFBCD       // LPFBCD or LPF4052

//-----------Select the radio being used --------------------------------------

#define RADIO  A210             // A180, A215 or A210 

//          The Preference section on Line 305-387 should allow you to run

#define PREFERENCE CLINT      //PREFERENCE can be defined as CLINT MARK JOHN 

#define SI5351_DRV MCU        // SI5351 Driver can be ETHERKIT or MCU

//----------- The following are Feature Choices - Comment out if not using-------


//----------- The following are Micro controller choices
#define WROVER 2  //Esp32 Wrover separate serial SPI display
#define S3ZERO 3  //Esp32-S3 Zero separate serial SPI display
#define S3R8   4  //Esp32-S3R8 with LilyGo integrated T-display parallel SPI
#define S3MINI 5  //Esp32-S3 mini separate serial SPI display
#define R8N16  6  //Esp32-S3 R8N16 with generic integrated display serial SPI
#define T7S3   7  //ESP32-S3 LilyGo T7 Ver 1.1 plug in replacement for CLINT
#define S2MINI 8  //Esp32-S2 mini separate serial SPI display
#define D1MINI 10 //Esp32-D1 mini Wroom replacement for Wrover
#define WROOM 14  //ESP32-D or ESP32-D1 mini Wroom Chip separate SPI display



/*
 * There are 9 choices for the MC Microcontroller type
 * if MC_TYPE is set to WROVER then the Microcontroller is an ESP32-Wrover LilyGo T7 Ver 1.5
 * if MC_TYPE is set to S3ZERO then the Microcontroller is an ESP32-S3-Zero WaveShare
 * if MC_TYPE is set to S3R8 then the Microcontroller is an ESP32-S3R8 LilyGo W/Integrated Display
 * if MC_TYPE is set to S3MINI then the Microcontroller is an ESP32-S3-Mini Lolin
 * if MC_TYPE is set to R8N16 then the Microcontroller is an LVGL ESP32-R8N16 W/integrated Display
 * if MC_TYPE is set to T7S3 then the Microcontroller is an ESP32-S3 LilyGo T7 Ver 1.1
 * if MC_TYPE is set to S2MINI then the Microcontroller is an ESP32-S2-Mini Lolin
 * if MC_TYPE is set to D1MINI then the Microcontroller is an ESP32-D1 Mini
 * if MC_TYPE is set to WROOM then the Microcontroller is an ESP32-D or ESP32-D1 mini Wroom chip
 * 
 */

#define MC_TYPE WROVER 


//-----------------From this point forward you do not need to set anything
//                 I suggest you use the Preference section on Line 305-387 

/*
 *    Define the EncoderStep here
 */
 
#define EncoderStep  48   //  Encoder pulses needed to advance the encoder 1 step


/*--------------------------------------------------------
 *   Calibration SI5351  You should calibrate your si5351 chip
 *   The frequency change is inverse to the Correction 
 *   For the EtherKit a change of +116 will lower the frequency by 1 Hz
 *   For the MCU a change of +2.881 will lower the frequency by 1 Hz
 *   I tune on 20m band at 14.250 MHz. Remember the actual frequency is 5.546MHz lower for 20m, 15m, and 10m bands
 *   The actual output frequency of VFO with 5.546MHz CF is 8,605,000 Hz with the 5.546 MHz Carrier Oscillator. 
 *   The actual output frequency of VFO with 5.520MHz CF is 8,730,000 Hz with the 5.546 MHz Carrier Oscillator. 
 *   See the Calibration Target Frequency below in Frequency Settings for each crystal filter
 *   Remember, you are calibrating the Si5351 module, not the radio.  
----------------------------------------------------------*/

#define CORRECTION     3466   //tuned on 20m at 14.250 - 8605000 typically 2.881 x freq = correction change


//T21 3466       MCU SigGen 

#define CF NEWER           //OLDER for 5520 CO, NEWER for 5645 CO and DRAKE for Drake CF

/*-------------------------------------------------------
   Frequency settings
--------------------------------------------------------*/

#if CF == OLDER
#define IF       5520000      //Std. Crystal Filter 5520000;
#define OPPFREQ  5523300      //Std. Crystal Filter 5523300; 
#define CWFREQ   5521200      //Std. Crystal Filter 5521200; 
#define RFFREQ   8730000      //Std. Crystal Filter 8730000; Calibration Target Frequency
#define DFREQ   14250000      //Default startup frequency using 20m band 
#endif

#if CF == NEWER
#define IF       5645000      //Std. Crystal Filter 5645000;
#define OPPFREQ  5648300      //Std. Crystal Filter 5648300;
#define CWFREQ   5646200      //Std. Crystal Filter 5646200;
#define RFFREQ   8605000      //Std. Crystal Filter 8605000;  Calibration Target frequency
#define DFREQ   14250000      //Default startup frequency using 20m band 
#endif


/*
 *  There are three choices for the size of the display being used ("DISP_SIZE"):
 *
 *    If "DISP_SIZE" is set to "SMALL_DISP", then we assume a 128x160 pixel display.
 *    If "DISP_SIZE" is set to "LARGE_DISP", then we assume a 240x320 pixel display.
 *    If "DISP_SIZE" is set to "CUSTOM_DISP", the user can create a 'logical' (large)
 *    display size as I have below for my Atlas 210X
 *
 *  Many of the locations of things on the screen are conditionalized based on the
 *  screen size. The fonts used on the splash screen also vary with the screen size.
 */

#define DISP_SIZE CUSTOM_DISP      // Custom display currently in use


/*
 * There are three choices for the crystal filter for the Atlas Radio
 * The first filter used was 5.520 MHz that was later replaced by the
 * 5.6450 MHz filter.  The third is for the Drake Transceiver at 5.6450 
 * MHz but is is different in that The Atlas Radio filters on the edge
 * of 5.6450 MHz while the center of the filter of the Drake radio is
 * 5.6450 MHz. If "CO" is set to OLDER the IF frequency is 5.520 MHz
 * IF "CO" is set to NEWER the IF frequency is 5.6450 MHz and if "CO"
 * is set to DRAKE the IF frequency is 5.6436 MHz.
 * 
 */
 

/*--------------------------------------------------------
   Splash Screen Startup intro - Version Number
----------------------------------------------------------*/


//----------Start-up Version Number-----------------------------------------------
#define NAME "Si5351 Sig Gen"
#define VERSIONID "Version 1.32"
#define ID " by KI5IDZ & W7KEC"

  
#define	BIT_RATE 115200

/*
 * There are two choices for the STATUS  TESTING and RUN
 * IF "STATUS" is set to TESTING then additional diagnostics is activated. The display
 * will operate normally, External input can be tested. Controller is held in Norm,
 * with the on_off_flag held high without VFO input and encoder will function
 * IF "STATUS" is set to RUN then the radio will operate normally
 */

#define P_STATUS RUN              // TESTING   RUN

/*
 * There are two choices for the carrier oscilator for the Radio
 * IF "CO" is set to ATLAS_CO then the radio is using the original internal CO
 * IF "CO" is set to SI5351_CO then the radio is using the si5351 CLK1 for the CO
 */


#if DISP_SIZE == SMALL_DISP			// Define things for the ST7785 small display (128x160)

  #define TEXT   "  BAND       STEP        MEM"  //Displays above small text
  
	#define DISP_W	    160				// Display width (in landscape mode)
	#define DISP_H	    128				// Display height
  #define DISP_L        0       // Dial Display begins to center dial
  #define DISP_TM       0         // Top Margin moves Dial up and down
	
	#define	D_R			    200				// Dial radius (if 45000, Linear scale)
	#define	DIAL_FONT		0.4				// Font -  0, 1, or 2 (Defaults to '0' in "dial.cpp")
	#define	DIAL_SPACE	 30				// Number of pixels between the main and sub arcs

	#define	DP_WIDTH		  1				// Width of Dial ponter
	#define	DP_LEN	     60				// Normal length of Dial pointer
  #define DP_POS        0       // Position of Dial pointer
  
  #define  F1_POS      10        // Vertical Position of the frequency box 
  #define  T1_POS      30        // Align the secondary text information on this line

#endif

#if DISP_SIZE == LARGE_DISP				// Define things for the large display (240x320)

  #define TEXT "       BAND                        STEP                     MEM"
  	
	#define DISP_W	     320				// Display width (in landscape mode)
	#define DISP_H	     240				// Display height
  #define DISP_L         0        // Dial Display begins to center dial
  #define   DISP_TM     45        // Top Margin moves Dial up and down
	
	#define	D_R			     250				// Dial radius (if 45000, Linear scale)
	#define	DIAL_FONT	  0.65				// Font -  0, 1, or 2 (Defaults to '0' in "dial.cpp")
	#define	DIAL_SPACE    45				// Number of pixels between the main and sub arcs

	#define	DP_WIDTH	     1		  	// Width of Dial pointer 0,1 OR 2
	#define	DP_LEN		   180				// Length of Dial pointer
	#define	DP_POS		     0				// Length Dial pointer extends above dial
  
  #define  F1_POS       26        // Vertical Position of the frequency box 26 72 or 66 26
  #define  T1_POS       72        // Align the secondary text information on this line

#endif

#if DISP_SIZE == CUSTOM_DISP		  // Define things Custom display (1.9" display 170x320)
                                  // Top of the display is 18 not 0 
  #define TEXT "      BAND                            STEP                          MEM"
	
	#define DISP_W	     320				// Display width in landscape mode  260 280 300 320
	#define DISP_H	     170				// Display height full height
  #define DISP_L        0        // Dial Display offset from center Normally set to 0 set to 8 for short 16 pixels
  #define DISP_TM       32        // Top Margin moves Dial up and down
	
	#define	D_R		       250				// Dial radius (if 45000, Linear scale)
	#define	DIAL_FONT   0.65		    // Dial Font width is multiplied by DIAL_FONT height is proportional to width
	#define DIAL_SPACE    44        // Number of pixels between the main and sub arcs

	#define	DP_WIDTH	     1				// Width of Dial pointer
	#define	DP_LEN		   100		    // Length of Dial pointer
	#define DP_POS         0        // Length Dial pointer extends above dial

	#define  F1_POS       18        // Vertical Position of the frequency box 18 63 or 48 18
  #define  T1_POS       63        // Align the secondary text information on this line

#endif

/*
 *	The following are the things that the user can change to modify the appearance
 *	and/or behavior of the dial itself. They were moved from the original "dial_prm.h"
 *	and "display.h"	files and converted from variables to definitions.
 */

#define		F_REV				     1		// Frequency increases CW if '1'; CCW (ACW) if '0'
#define	  F_MAIN_OUTSIDE	 0		// 0 - Main dial is inside;  1 - Main dial is outside

/*
 *	Changing this to anything other than "10000" does weird things with the
 *	displayed numerical values on both dial scales. Still needs to be investigated.
 */

#define		FREQ_TICK_MAIN	 10000	// Tick labels on main dial (if 10000: 10kHz / else： 100kHz)
#define   FREQ_TICK_SUB     1000
#define   TICK_PITCH_MAIN 10.5    // Main Tick Pitch (note small changes make a big difference)
#define   TICK_PITCH_SUB   9.8    // Sub Tick Pitch  (try not to go below 4.0)

#define		TICK_WIDTH1			 3.0		// Width of Tick
#define   TICK_WIDTH2      4.0    // Width of Tick
#define   TICK_WIDTH3      4.5    // Width of Tick

#define   TICK_MAIN1        4    // Length of Main Tick(1)
#define   TICK_MAIN5       14    // Length of Main Tick(5)
#define   TICK_MAIN10      18    // Length of Main Tick(10)

#define   TICK_SUB1         8    // Length of Sub Tick(1)
#define   TICK_SUB5        14    // Length of Sub Tick(5)
#define   TICK_SUB10       18    // Length of Sub Tick(10)

#define		TNCL_MAIN		     22		 // Space between Number and Tick (Main)
#define		TNCL_SUB		   	 22	   // Space between Number and Tick (Sub)


/*
 *	Let's define some colors that are used to draw things. Feel free to add to the
 *	list. The following link is a handy tool for selecting colors and getting the
 *	numerical color codes in various formats:
 *
 *						https://www.tydac.ch/color/
 *
 *	These definitions are the 24 bit color codes in red, green, blue format.
 */

#define		CL_GREY			0xA0A0A0UL		// 
#define		CL_BLACK		0x000000UL		// 
#define		CL_WHITE		0xFFFFFFUL		// 
#define		CL_RED			0xFF0000UL		// 
#define		CL_GREEN		0x00FF00UL		// 
#define		CL_LT_BLUE	0x00FFFFUL		// 
#define		CL_ORANGE		0xFFD080UL		// 
#define   CL_CYAN     0x00FFFFUL
#define   CL_BLUE     0x0000FFUL
#define   CL_YELLOW   0xFFFF00UL
#define   CL_SKYBLUE  0x87CEEBUL
#define   CL_AQUA     0x1B73A1UL
#define   CL_TOMATO   0xCA261CUL
#define   CL_LBLUE    0X0048A4UL

/*
 *	The following list associates the various components of the display with the
 *	colors we want them to be display as.
 */


#define		CL_BG			    CL_BLACK		  // Display background (Black)
#define		CL_POINTER		CL_RED		  	// Dial pointer (Red)
#define		CL_TICK_MAIN	CL_GREEN      // Main Ticks (Lime green)
#define		CL_NUM_MAIN		CL_WHITE  	  // Main dial numbers (White)
#define		CL_TICK_SUB		CL_SKYBLUE    // Sub Ticks (Light blue)
#define		CL_NUM_SUB		CL_WHITE      // Sub Numbers (White)
#define		CL_DIAL_BG		CL_BLACK		  // Dial background (Black)
#define		CL_SPLASH		  CL_LT_BLUE	  // Splash screen text
#define   CL_FREQ_BOX   CL_LT_BLUE    // Numerical frequency box
#define   CL_F_NUM      CL_ORANGE     // Numerical frequency
#define   CL_NUM        CL_YELLOW     // Numerical small numbers
#define   CL_NUM_O      CL_RED        // 
#define   CL_NUM_TEXT   CL_WHITE 

#if PREFERENCE == CLINT
#define   CL_BG          CL_BLACK    // Display background (Black)
#define   CL_POINTER     CL_RED      // Dial pointer (Red)
#define   CL_TICK_MAIN   CL_GREEN    // Main Ticks (Lime green)
#define   CL_NUM_MAIN    CL_WHITE    // Main dial numbers (White)
#define   CL_TICK_SUB    CL_SKYBLUE  // Sub Ticks (Light blue)
#define   CL_NUM_SUB     CL_WHITE    // Sub Numbers (White)
#define   CL_DIAL_BG     CL_BLACK    // Dial background (Black)
#define   CL_SPLASH      CL_LT_BLUE  // Splash screen text
#define   CL_FREQ_BOX    CL_CYAN     // Numerical frequency box
#define   CL_F_NUM       CL_ORANGE   // Numerical frequency
#define   CL_NUM         CL_YELLOW   // Numerical small numbers
#define   CL_NUM_O       CL_RED      // Step color in CUSTOM_DISP
#define   CL_NUM_NORM    CL_WHITE    // Normal Text inside box
#define   DP_POS           0         // Length Dial pointer extends above dial
#define   DISP_TM         30         // Top Margin moves Dial up and down
#define   F1_POS          15         // Vertical Position of the frequency box 14 60    32,14
#define   T1_POS          60         // Align the secondary text information on this line
#define   DIAL_SPACE      40         // Number of pixels between the main and sub arcs
#define   TICK_SUB1        8         // Length of Sub Tick(1)
#define   TICK_SUB5       14         // Length of Sub Tick(5)
#define   TICK_SUB10      18         // Length of Sub Tick(10)
#define   TICK_MAIN1       4         // Length of Main Tick(1)
#define   TICK_MAIN5      14         // Length of Main Tick(5)
#define   TICK_MAIN10     18         // Length of Main Tick(10)
#define   TNCL_MAIN       18         // Space between Number and Tick (Main)
#define   TNCL_SUB        18         // Space between Number and Tick (Sub)
#define TICK_PITCH_MAIN 10.5         // Main Tick Pitch (note small changes make a big difference)
#define TICK_PITCH_SUB   9.8         // Sub Tick Pitch  (try not to go below 4.0)
#define CORRECTION       0ULL        // Default correction is 0
#define CORRECTION_MCU   0           // Default correction is 0
#define EncoderStep     12           // use for 100 PIR
#endif

#if PREFERENCE == JOHN
#define   CL_BG         CL_BLACK      // Display background (Black)
#define   CL_POINTER    CL_RED        // Dial pointer (Red)
#define   CL_TICK_MAIN  CL_SKYBLUE    // Main Ticks (Lime green)
#define   CL_NUM_MAIN   CL_BLUE       // Main dial numbers (White)
#define   CL_TICK_SUB   CL_SKYBLUE    // Sub Ticks (Light blue)
#define   CL_NUM_SUB    CL_YELLOW     // Sub Numbers (White)
#define   CL_DIAL_BG    CL_BLACK      // Dial background (Black)
#define   CL_SPLASH     CL_LT_BLUE    // Splash screen text
#define   CL_FREQ_BOX   CL_YELLOW     // Numerical frequency box
#define   CL_F_NUM      CL_CYAN       // Numerical frequency
#define   CL_NUM        CL_YELLOW     // Numerical small numbers
#define   CL_NUM_O      CL_YELLOW     // Step color in CUSTOM_DISP
#define   CL_NUM_NORM   CL_YELLOW     // Normal Text inside box
#endif


//------------Defined MC_TYPE---------------------------

#if MC_TYPE == WROVER         //Compiler directive for Lilygo T7 Ver 1.5 
#if LPF_CTRL == LPFBCD        //Select BCD LPF control
  #define BCD1     27           //Select Filter
  #define BCD2     26           //Select Filter
  #define BCD3      2           //Select Filter
  #define BCD4      0           //Select Filter
#endif
#if LPF_CTRL == LPF4052
  #define MUX0      0           //Select Filter
  #define MUX1      2           //Select Filter
#endif
#define DT       25           // DT Encoder A pin, connected to ESP32 pin 4 (GPIO4)
#define CLK       4           // CLK Encoder B pin, connected to ESP32 pin 25 (GPIO25)
#define Lock     32           //Lock - To lock the frequency
#define Mem      33           //memory
#define Step     34           //step
#define Band     35           //scan (not in use) 
#define TSDA     21           //Default pins defined in si5351.cpp  
#define TSCL     22           //Default pins defined in si5351.cpp
//following pins are for the display
#define ByPass   19           //Norm/opp side band position (pin 13 is inside and pin 12 is outside)
#define TFT_MISO -1           //
#define TFT_CS    5           //     10 or 34 or 5
#define TFT_MOSI 23           //     11 or 36 or 23
#define TFT_SCLK 18           //     12 or 38 0r 18
#define TFT_DC   15           //     14 or 35 or 15
#define TFT_RST  -1           //     14 or 44 or -1 
#endif

#if MC_TYPE == WROOM          //Compiler directive for ESP32 D1 Mini or 
#if LPF_CTRL == LPFBCD        //Select BCD LPF control
  #define BCD1     27           //Select Filter
  #define BCD2     26           //Select Filter
  #define BCD3      2           //Select Filter
  #define BCD4      0           //Select Filter
#endif
#if LPF_CTRL == LPF4052
  #define MUX0      0           //Select Filter
  #define MUX1      2           //Select Filter
#endif
//#define INTERRUPTED_PIN 16    // user defined connection to PCF8574 interrupt pin
#define DT        4           // DT Encoder A pin, connected to ESP32 pin 4 (GPIO4)
#define CLK      16           // CLK Encoder B pin, connected to ESP32 pin 25 (GPIO25)
#define Lock     32           //Lock the frequency
#define Mem      33           //memory
#define Step     34           //step
#define Band     35           //Input to change bands
#define TSDA     21           //Default pins defined in si5351.cpp  
#define TSCL     22           //Default pins defined in si5351.cpp
#define ByPass   19           //Norm/opp side band position (pin 13 is inside and pin 12 is outside)
//following pins are for the display
#define TFT_MISO -1           //
#define TFT_CS    5           //     10 or 34 or 5
#define TFT_MOSI 23           //     11 or 36 or 23
#define TFT_SCLK 18           //     12 or 38 0r 18
#define TFT_DC   15           //     14 or 35 or 15
#define TFT_RST  -1           //     14 or 44 or -1 
#define LOCK     32           //Lock - To lock the frequency
#endif

#if MC_TYPE == D1MINI          //Compiler directive for ESP32 D1 Mini or 
#if LPF_CTRL == LPF4052
  #define MUX0     0
  #define MUX1     2
#endif
#if LPF_CTRL == LPFBCD
  #define BCD1     17           //Select Filter
  #define BCD2     26           //Select Filter
  #define BCD3      2           //Select Filter
  #define BCD4      0           //Select Filter
#endif
#define DT        4           // DT Encoder A pin, connected to ESP32 pin 4 (GPIO4)
#define CLK      25           // CLK Encoder B pin, connected to ESP32 pin 25 (GPIO25)
#define Lock     32           //Lock the frequency
#define Mem      33           //memory
#define Step     34           //step
#define Band     35           //scan (not in use) 
#define TSDA     21           //Default pins defined in si5351.cpp  
#define TSCL     22           //Default pins defined in si5351.cpp
#define ByPass   19           //bypass the LPF
//following pins are for the display
#define TFT_MISO -1           //
#define TFT_CS    5           //     10 or 34 or 5
#define TFT_MOSI 23           //     11 or 36 or 23
#define TFT_SCLK 18           //     12 or 38 0r 18
#define TFT_DC   15           //     14 or 35 or 15
#define TFT_RST  -1           //     14 or 44 or -1 
#endif

#if MC_TYPE == T7S3
#if LPF_CTRL == LPF4052
  #define MUX0     0
  #define MUX1     2
#endif
#if LPF_CTRL == LPFBCD
  #define BCD1      3           //Select Filter
  #define BCD2     16           //Select Filter
  #define BCD3      2           //Select Filter
  #define BCD4      0           //Select Filter
#endif
#define DT        4           // DT Encoder A pin, connected to ESP32 pin 4 (GPIO4)
#define CLK      12           // CLK Encoder B pin, connected to ESP32 pin 25 (GPIO25)
#define Lock     38           //Lock the frequency
#define Mem      46           //memory
#define Step     45           //step
#define Band     47           //scan (not in use) 
#define LOCK     38           //Lock - To lock the frequency
#define TSDA     13           //Default pins defined in si5351.cpp  
#define TSCL     14           //Default pins defined in si5351.cpp
#define ByPass   17           //By Pass Filters
//following pins are for the display
#define TFT_MISO -1
#define TFT_CS    5           //     10 or 34 or 5
#define TFT_MOSI  8           //     11 or 36 or 23
#define TFT_SCLK 18           //     12 or 38 0r 18
#define TFT_DC   40           //     14 or 35 or 15 TD0 pin is 40 for S3
#define TFT_RST  -1           //     14 or 44 or -1 
#endif

#if MC_TYPE == S3ZERO
#if LPF_CTRL == LPF4052
  #define MUX0     2
  #define MUX1    13
#endif
#if LPF_CTRL == LPFBCD
  #define BCD1     43           //Select Filter
  #define BCD2     14           //Select Filter
  #define BCD3      2           //Select Filter
  #define BCD4     13           //Select Filter
#endif
//#define INTERRUPTED_PIN 1       //defined in PCF8574inputpins interrupt pin Only when additional pins are needed
#define   CLK      4            //Defined in encodersetup.h A-CLK
#define   DT       6            //Defined in encodersetup.h B-DT
#define   Mem      3            //memory
#define   Step     5            //step
#define   Band    44            //scan 
#define   TSDA     8            //Default pins for esp32-s2 used by wire.h
#define   TSCL     9            //Default pins for esp32-s2 used by wire.h
#define   ByPass   1            //By Pass Filters
#define   Lock    -1
//--------------------------Following pins defined in pcf8574inputpins.h--------------
//pcf8574.pinMode(P0, INPUT_PULLUP);// 
//pcf8574.pinMode(P1, INPUT_PULLUP);// 
//pcf8574.pinMode(P2, INPUT_PULLUP);// 
//pcf8574.pinMode(P3, INPUT_PULLUP);// 
//pcf8574.pinMode(P4, INPUT_PULLUP);// Input for Lock function
//pcf8574.pinMode(P5, OUTPUT, LOW); // Output for MUX0
//pcf8574.pinMode(P6, OUTPUT, LOW); // Output for MUX1
//pcf8574.pinMode(P7, INPUT_PULLUP);// input to bypass LP Filter
#define TFT_MISO   -1   //     13 or 37 Not connected -1
#define TFT_CS     10   //     10 or 34
#define TFT_MOSI   11   //     11 or 36
#define TFT_SCLK    7   //     12 or 38
#define TFT_DC     12   //     14 or 35
#define TFT_RST    -1   //     14 or 44 Not connected -1
#endif   



#if MC_TYPE == S3R8
#if LPF_CTRL == LPF4052         //cw & VFO in Radio Plug  9,14 free pins
  #define MUX0     2            //Select Filter
  #define MUX1     1            //Select Filter
#endif
#if LPF_CTRL == LPFBCD          //4 pins in Radio Plug
  #define BCD1     12           //Select Filter
  #define BCD2     13           //Select Filter
  #define BCD3      2           //Select Filter
  #define BCD4      1           //Select Filter
#endif
#define PIN_POWER_ON 15  // LCD and battery Power Enable
//#define INTERRUPTED_PIN 18      //PCF8574inputpins interrupt pin 
#define   CLK     17            //Defined in encodersetup.h A-CLK
#define   DT      21            //Defined in encodersetup.h B-DT
#define   Mem      3            //memory
#define   Step    10            //step
#define   Band    11            //scan 
#define   TSDA    43            //Default pins for esp32-s2 used by wire.h
#define   TSCL    44            //Default pins for esp32-s2 used by wire.h
#define   Lock    16
#define   ByPass  18            //input to bypass LP Filter Bypass becomes P7 when using PCF8574
//--------------------------Following pins defined in pcf8574inputpins.h--------------
//pcf8574.pinMode(P0, INPUT_PULLUP);// 
//pcf8574.pinMode(P1, INPUT_PULLUP);// 
//pcf8574.pinMode(P2, INPUT_PULLUP);// 
//pcf8574.pinMode(P3, INPUT_PULLUP);// 
//pcf8574.pinMode(P4, INPUT_PULLUP);// 
//pcf8574.pinMode(P5, OUTPUT, LOW); // Output for MUX0
//pcf8574.pinMode(P6, OUTPUT, LOW); // Output for MUX1
//pcf8574.pinMode(P7, INPUT_PULLUP);// input to bypass LP Filter
#define TFT_D0 39
#define TFT_D1 40
#define TFT_D2 41
#define TFT_D3 42
#define TFT_D4 45
#define TFT_D5 46
#define TFT_D6 47
#define TFT_D7 48
#define TFT_CS  6
#define TFT_DC  7
#define TFT_WR  8
#define TFT_RD  9
#define TFT_RST 5
#define TFT_BL 38
#endif

#if MC_TYPE == S3MINI           //Free Pins 1,15,17,18,21,33,37,43
#if LPF_CTRL == LPF4052         //cw & VFO in Radio Plug  9,14 free pins
  #define MUX0     3            //Select Filter
  #define MUX1    10            //Select Filter
#endif
#if LPF_CTRL == LPFBCD          //4 pins in Radio Plug
  #define BCD1      9           //Select Filter
  #define BCD2     14           //Select Filter
  #define BCD3      3           //Select Filter
  #define BCD4     10           //Select Filter
#endif
//#define INTERRUPTED_PIN 16      //PCF8574 interrupt pin Only when additional pins are needed
#define   DT       6            // CLK pin, connected to ESP32 pin A
#define   CLK      5            // DT pin, connected to ESP32 pin B
#define   Mem      2            //memory
#define   Step     4            //step
#define   Band    12            //scan 
#define   Lock    11            //lock function
#define   TSDA     7            //SDA
#define   TSCL    13            //SCL
#define   ByPass   8            //input to bypass LP Filter 
//--------------------------Following pins defined in pcf8574inputpins.h--------------
//pcf8574.pinMode(P0, INPUT_PULLUP);// 
//pcf8574.pinMode(P1, INPUT_PULLUP);// 
//pcf8574.pinMode(P2, INPUT_PULLUP);// 
//pcf8574.pinMode(P3, INPUT_PULLUP);// 
//pcf8574.pinMode(P4, INPUT_PULLUP);// 
//pcf8574.pinMode(P5, OUTPUT, LOW); // Output for MUX0
//pcf8574.pinMode(P6, OUTPUT, LOW); // Output for MUX1
//pcf8574.pinMode(P7, INPUT_PULLUP);// 
#define TFT_CS   34 //     10 or 34 34
#define TFT_MOSI 36 //     11 or 36 35
#define TFT_SCLK 38 //     12 or 38 36
#define TFT_DC   35 //     14 or 35 33
#define TFT_RST  44 //     14 or 44 37
#endif


#if MC_TYPE == R8N16      //ESP32-R8N16 W/integrated Display  free pins 38,45,48,47,21
#if LPF_CTRL == LPF4052         //cw & VFO in Radio Plug  18,19 free pins
  #define MUX0     5            //Select Filter
  #define MUX1     7            //Select Filter
#endif
#if LPF_CTRL == LPFBCD          //4 pins in Radio Plug
  #define BCD1     19           //Select Filter
  #define BCD2     18           //Select Filter
  #define BCD3      5           //Select Filter
  #define BCD4      7           //Select Filter
#endif
#define INTERRUPTED_PIN 42      // user defined connection to PCF8574 interrupt pin
#define   DT       4            // CLK pin, connected to ESP32 pin 6 (GPIO4) A
#define   CLK      6            // DT pin, connected to ESP32 pin 4 (GPIO25) B           
#define   Mem     17            //memory
#define   Step    16            //step
#define   Band    12            //scan 
#define   Lock    20            //lock function
#define   TSDA    40            //Default pins for esp32-s2 used by wire.h
#define   TSCL    41            //Default pins for esp32-s2 used by wire.h
#define   ByPass  39            //input to bypass LP Filter 
//----------------------Following pins defined in pcf8574inputpins.h--------------
//pcf8574.pinMode(P0, INPUT_PULLUP);// 
//pcf8574.pinMode(P1, INPUT_PULLUP);// 
//pcf8574.pinMode(P2, INPUT_PULLUP);// 
//pcf8574.pinMode(P3, INPUT_PULLUP);// 
//pcf8574.pinMode(P4, INPUT_PULLUP);// 
//pcf8574.pinMode(P5, OUTPUT, LOW); // Output for MUX0
//pcf8574.pinMode(P6, OUTPUT, LOW); // Output for MUX1
//pcf8574.pinMode(P7, INPUT_PULLUP);// 
//following pins are for the display
#define TFT_BL   14 //     Back light
#define TFT_MISO -1 //     Normally set to -1 if not using
#define TFT_CS   10 //     10 or 34
#define TFT_MOSI 13 //     11 or 36
#define TFT_SCLK 12 //     12 or 38
#define TFT_DC   11 //     14 or 35
#define TFT_RST   1 //     14 or 44 
#endif

#if MC_TYPE == S2MINI           //Free Pins 1,15,17,18,21,39,40,44
#if LPF_CTRL == LPF4052         //cw & VFO in Radio Plug  9,14 free pins
  #define MUX0     2            //Select Filter
  #define MUX1    12            //Select Filter
#endif
#if LPF_CTRL == LPFBCD          //4 pins in Radio Plug
  #define BCD1     13           //Select Filter
  #define BCD2     14           //Select Filter
  #define BCD3      2           //Select Filter
  #define BCD4     12           //Select Filter
#endif
#define INTERRUPTED_PIN 16      // user defined connection to PCF8574 interrupt pin
#define   DT       6            // CLK pin, connected to ESP32 pin A
#define   CLK      4            // DT pin, connected to ESP32 pin B
#define   Mem      3            //memory
#define   Step     5            //step
#define   Band     7            //scan 
#define   Lock    11            //lock function
#define   TSDA     8            //SDA
#define   TSCL     9            //SCL
#define  ByPass   10            //input to bypass LP Filter   
//--------------------------Following pins defined in pcf8574inputpins.h--------------
//pcf8574.pinMode(P0, INPUT_PULLUP);// 
//pcf8574.pinMode(P1, INPUT_PULLUP);// 
//pcf8574.pinMode(P2, INPUT_PULLUP);// 
//pcf8574.pinMode(P3, INPUT_PULLUP);// 
//pcf8574.pinMode(P4, INPUT_PULLUP);// 
//pcf8574.pinMode(P5, OUTPUT, LOW); // Output for MUX0
//pcf8574.pinMode(P6, OUTPUT, LOW); // Output for MUX1
//pcf8574.pinMode(P7, INPUT_PULLUP);// 
#define TFT_MISO -1 //
#define TFT_CS   34 //     10 or 34 or 34
#define TFT_MOSI 35 //     11 or 36 or 35
#define TFT_SCLK 36 //     12 or 38 or 36
#define TFT_DC   33 //     14 or 35 or 33
#define TFT_RST  37 //     14 or 44 or 37
#endif



#endif    //_CONFIG_H_
