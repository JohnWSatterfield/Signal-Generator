

/*------------------------------------------------------------------

   Signal Generator for ESP32 Micro Controllers

      Can be used as external VFO on Atlas 210X
      
        by J. Satterfield / KI5IDZ
       
            Clint Chron  /  W7KEC

               Mar 11, 2025
          
    based on work by KI5IDZ J.Satterfield & Clint Chron
--------------------------------------------------------------------*/
/*The following were contributions by others for software and hardware design:
 * dial.cpp, software Author: JF3HZB / T.UEBO
 * Original dial.cpp modified by T.UEBO 7/1/23
 * si5351mcu - Si5351 library for Arduino MCU tuned for size and click-less
 * 
 * Copyright (C) 2017 Pavel Milanes <pavelmc@gmail.com>
 * si5351mcu - Si5351 library for Arduino MCU tuned for size and click-less
 * Many chunk of codes are derived-from/copied from other libs
 * all GNU GPL licenced:
 *  - Linux Kernel (www.kernel.org)
 *  - Hans Summers libs and demo code (qrp-labs.com)
 *  - Etherkit (NT7S) Si5351 libs on github
 *  - DK7IH example.
 *  - Jerry Gaffke integer routines for the bitx20 group
 * 
 * https://github.com/lovyan03/LovyanGFX/blob/master/license.txt
 *
 * This is version 1.32 
 * Added LARGE_DISPLAY

ESP32 Arduino
"ESP-WROOM-32"
also works on FireBeatle
Version 1.00
*/



/*-------------------------------------------------------
   Included Libraries
--------------------------------------------------------*/

#include "config.h"
#include "dial.hpp"           //https://github.com/tjlab-jf3hzb/Digital_VFO_with_analog_dial
#include "encodersetup.h"     //use when encoder is connected to i2c encoder shield
#include <Wire.h>
#include "si5351mcu.h" 



/*-------------------------------------------------------
   Frequency settings
--------------------------------------------------------*/

//#define CORRECTION   0   //tuned on 20m at 14.250 - 8605000 typically 2.881 x freq = correction change

/*----------------------------------------------------------------------------------
    Control flags
-----------------------------------------------------------------------------------*/

uint8_t f_dchange;  // if need to renew display, set this flag to 1

#define  VFO_DRIVE SI5351_DRIVE_8MA  //define VFO Output Power
#define  VFO_CLOCK SI5351_CLK0       //define clock to use
#define  IF_OFFSET IF           //If offset for use on Atlas band position

/*--------------------------------------------------------
   Device setup
----------------------------------------------------------*/

Si5351mcu si5351;           //create instance of Si5351mcu

/*-------------------------------------------------------
   Display settings
--------------------------------------------------------*/
LGFX lcd;                       //instance of LGFX display screen
LGFX_Sprite sp;                 //instance of LGFX display sprite
LGFX_Sprite sprites[2];         //instance of LGFX display sprite matrix
bool flip;                      //flag for LGFX
int sprite_height;              //height of sprites in display
DIAL dial;                      //instance of DIAL for drawing the analog display


/*-----------------------------------------------------------------------------
 *       Global Variables
-----------------------------------------------------------------------------*/


extern char f_rev;              //define external character for dial rotation direction
extern uint32_t cl_BG;          //define external reg for clearing display screen

long freq=7162000;              // Frequency Variable Startup 1,000,000 Hz

#if RADIO == A180
long freqa[6] = { 1800000, 1850000, 1885000, 1935000, 1985000, 2500000};  //15M band memory presets
long freqb[6] = { 3500000, 3600000, 3750000, 3800000, 3800000, 5000000};  //10M band memory presets
long freqc[6] = { 3800000, 3850000, 3900000, 3916000, 3985000, 5000000};  //80M band memory presets
long freqd[6] = { 7125000, 7162000, 7235000, 7255000, 7275000,10000000};  //40M band memory presets
long freqe[6] = {14150000,14225000,14250000,14300000,14325000,15000000};  //20M band memory presets
long freqf[6] = {   10000,250000,1000000,10000000,30000000,100000000};       //Sig Generator presets

int recall[6] = {1,2,3,3,1,4};                                     //prefered memory recall at startup

// The following are the default band ranges for each band using General License frequencies
const long begofBand[6] = {1800000,3500000, 3800000,7178000,14225000,8000}; //General Class
const long endofBand[6] = {2000000,3800000, 4000000,7300000,14348000,180000000}; //All bands
#endif

#if RADIO == A215
long freqa[6] = { 1800000, 1850000, 1885000, 1935000, 1985000, 2500000};  //15M band memory presets
long freqb[6] = { 3800000, 3846000, 3916000, 3987500, 3950000, 5000000};  //80m band memory presets
long freqc[6] = { 7155000, 7162000, 7235000, 7255000, 7275000, 5000000};  //40M band memory presets
long freqd[6] = {14235000,14225000, 14250000,14300000,14325000,10000000};  //20M band memory presets
long freqe[6] = {21285000,21300000,21320000, 21350000,21400000, 15000000};  //15m band memory presets
long freqf[6] = {   10000,250000,1000000,10000000,30000000,100000000};       //Sig Generator presets

int recall[6] = {1,2,3,3,1,4};      //prefered memory recall at startup

// The following are the default band ranges for each band using General License frequencies
const long begofBand[6] = {1800000,3803000,7178000,14225000,21275000,8000}; //General Class
const long endofBand[6] = {2000000,4000000,7300000,14348000,21448000,180000000}; //All bands
#endif

#if RADIO == A210
#if PREFERENCE == CLINT
long freqa[6] = { 3853000, 3846000, 3900000, 3916000, 3950000, 5000000};  //80M band memory presets
long freqb[6] = { 7155000, 7162000, 7235000, 7255000, 7275000, 5000000};  //40M band memory presets
long freqc[6] = {14235000,14225000, 14250000,14300000,14325000,10000000};  //20M band memory presets
long freqd[6] = {21285000,21300000, 21320000,21350000,21400000,15000000};  //15M band memory presets
long freqe[6] = {28385000,28425000,28450000, 28500000,28500000,28900000};  //10M band memory presets
long freqf[6] = {   10000,250000,1000000,10000000,30000000,100000000};       //Sig Generator presets
int recall[6] = {2,2,3,2,2,4};                                     //prefered memory recall at startup
const long begofBand[6] = {3803000,7178000,14225000,21275000,28300000,8000}; //General Class
const long endofBand[6] = {4000000,7300000,14348000,21448000,28998000,180000000}; //All bands
#else
long freqa[6] = { 3846000, 3900000, 3916000, 3987500, 3950000, 5000000};  //80m band memory presets
long freqb[6] = { 7185000, 7200000, 7235000, 7275000, 7279000, 10000000};  //40m band memory presets
long freqc[6] = {14235000,14250000,14275000,14300000,14332000, 15000000};  //20m band memory presets
long freqd[6] = {21285000,21300000,21320000,21350000,21400000, 15000000};  //15m band memory presets
long freqe[6] = {28385000,28400000,28450000,28785000, 28500000, 28900000};  //10m band memory presets
long freqf[6] = {   10000,250000,1000000,10000000,30000000,100000000};       //Sig Generator presets
int recall[6] = {2,3,1,1,1,4};      //prefered memory recall at startup
// The following are the default band ranges for each band using General License frequencies
const long begofBand[6] = {3803000,7178000,14225000,21275000,28300000,8000}; //General Class
const long endofBand[6] = {4000000,7300000,14348000,21448000,28998000,180000000}; //All bands
#endif
#endif

long fstep = 1000;                 //Variable for controlling the frequency step changes
long interfreq = 0;                //Variable for calibration of different bands
long encodercount = 0;             //Variable for current accumulated encoder count
long encodercountlast = 0;         //Variable for last accumulated encoder count

/*-----------------------------------------------------------------------------
 *      Variables for real time counter (2) if needed
-----------------------------------------------------------------------------*/
unsigned long currentMillis = 0;   //timer 0 current value
unsigned long currentMillis1 = 0;  //timer 1 current value
unsigned long previousMillis = 0;  //timer 0 start value
unsigned long previousMillis1 = 0; //timer 1 start value

int memory=1;                      //Variable for keeping track of current memory displayed
int recall_count=0;                //Variable for first memory in when doing band change
//int recall[6] = {1,1,1,1,1,3};     //matrix for restoring memory on band changes value 1-6
int stpRecall[6] = {1,1,1,1,1,8};  //array for restoring frequency step on band changes value 1-7
int ActiveBand = 1;                //Variable for controlling band selected
int OldBand = 1;                   //Variable for last band selected
int HoldBand = 0;                  //Variable used while changing bands

long stepcount = 1;                //Variable for current step frequency 
long stepcountlast = 1;            //Variable for last step frequency

/*-----------------------------------------------------------------------------
 *      Variables for printing to the screen
-----------------------------------------------------------------------------*/
char mem_str[4];                  //Variable for memory display string
char band_str[10];                 //Variable for band display string
char step_str[10];                 //Variable for step display string
char str[64];                     //General variable for display strings

bool on_off_flag=HIGH;             //Variable Flag if Si5351 is turned off
bool stepLast = HIGH;              //Variable Flag used to change step frequency
bool bandLast = HIGH;              //Variable Flag used to change band
bool memoryLast = HIGH;            //Variable Flag used to change memory
uint8_t devicePresent = 0;         //Variable to detect presence of i2c devices present
bool lockFlag = HIGH;              //Variable Flag to lock the encoder
bool lockFlagLast = HIGH;          //Variable Flag used to change encoder lock 
bool ByPassFlag = HIGH;            //Variable Flag used to change ByPass
bool ByPassFlagLast = HIGH;        //Variable Flag used to change ByPass lock

byte encoder = 1;                  //Variable encoder holding bit
byte stp = 1;                      //Variable step holding bit
byte count;                        //Variable to keep track of band Changes
byte pad = 11;                     //Variable for spacing Frequency display string

void IRAM_ATTR serialprintcount();

//------------------------------------------------------------------------
void setup() {
//------------------------------------------------------------------------
    Serial.begin(115200);
    
    char str[64];           //temporary variable for displaying strings
    bool i2c_found;         //temporary variable flag for determining if i2c is working
    Serial.begin(BIT_RATE); //initialize serial monitor communication rate
    //Startup display

 
    LCD_setup();
    delay(100);

    print_Splash();

    
    // Start serial and initialize the Si5351
    
    delay (100);  
    Wire.begin();           // Set up standard port ID
    delay(500);
    setupencoder();         // Set up the encoder
    delay(500);
    

    //Set up the Si5351

    i2c_found = si5351.init(25000000); //initialize the crystal frequency
    
    si5351.correction(CORRECTION);     //input frequency correction into Si5351
    si5351.setPower(0,SIOUT_4mA);      //set output power of Si5351
  #if  LPF_CTRL == LPFBCD
    set_frequency();                      //send frequency to Si5351
  #endif
  #if LPF_CTRL == LPF4052
    set_frequency_PCF();                  //send frequency to Si5351
  #endif     
    si5351.reset();                    //reset PLL of Si5351
    if (i2c_found) {                   //determine if connected to i2c
      Serial.println("si5351 OK");     //Si5351 connected to i2c
      on_off_flag=HIGH;
    } else {
      Serial.println("si5351 not OK"); //Si5351 not working
      on_off_flag=LOW;
    }

    f_dchange = 1;                     //set display renew flag to true (1)

    //Set up the ESP32 I/O being used              
    #if LPF_CTRL == LPFBCD        //Select BCD LPF control
    pinMode(BCD1,OUTPUT);
    pinMode(BCD2,OUTPUT);
    pinMode(BCD3,OUTPUT);
    pinMode(BCD4,OUTPUT);
    #endif
    #if LPF_CTRL == LPF4052        //Select 74HCT4052 LPF control
    pinMode(MUX0,OUTPUT);
    pinMode(MUX1,OUTPUT);
    #endif
    pinMode(Mem,INPUT_PULLUP);
    pinMode(Lock,INPUT_PULLUP);
    pinMode(Step,INPUT_PULLUP);
    pinMode(Band,INPUT_PULLUP);
    pinMode(ByPass,INPUT_PULLUP);

    bandpresets();                      //initializes the frequency for each band.
    //band_change();
    if (f_dchange==1) display_write();  //Write to the display
    //set_frequency();
}

void RamPSRam () {
      //determine ESP32 memory and PSRAM values
    log_d("Total heap: %d", ESP.getHeapSize());  //Esp heap memory size
    log_d("Free heap: %d", ESP.getFreeHeap());   //ESP heap memory available
    log_d("Total PSRAM: %d", ESP.getPsramSize());//ESP PSRAM memory size
    log_d("Free PSRAM: %d", ESP.getFreePsram()); //ESP PSRAM memory available
}

void serialprintcount() {                                //Encoder routine for frequency
if (lockFlag==HIGH) {                                    //Check to see if encoder lock switch is off
  if (quad >= EncoderStep || quad <= -EncoderStep){      //Check for quad overflow on encoder
    if (quad > EncoderStep) quad = EncoderStep;          //clockwise rotation 
    if (quad <-EncoderStep) quad = -EncoderStep;         //counter clockwise rotation
    //put code here for asigning encoder values like freq = freq + long((encodercount * fstep)/EncoderStep); 
    //communicate to the rest of the program the encoder change like: f_dchange=1 and f_fchange=1
    //check for max an min values
    encodercount += quad/EncoderStep;                   //increment encodercount one encoder step unit
    freq = freq + long((quad * fstep)/EncoderStep);     //send encodercount value to frequency + or -
    quad = 0;                                           //reset quad
  } 
  if (encodercount != encodercountlast) {               //check for change in encodercount value
  Serial.print(freq); Serial.print("  ");               //serial monitor diagnostic report
  Serial.println(encodercount);                         //serial monitor diagnostic report
  if(freq<=8000) freq=8000;                             //test for absolute low frequency value
  #if  LPF_CTRL == LPFBCD
    set_frequency();                      //send frequency to Si5351
  #endif
  #if LPF_CTRL == LPF4052
    set_frequency_PCF();                  //send frequency to Si5351
  #endif     
  f_dchange=1;                                          //set display change flat to true (1)
  encodercountlast = 0; encodercount =0;                //reset encodercount and flag
  }
} 
}
  
void print_Splash() {                  //routine to print start-up screen defined in config.h
  lcd.setTextColor(CL_SPLASH);         //set color from config.h 
  lcd.setFont(&fonts::Font4);          //set font size
  #if DISP_SIZE == SMALL_DISP          //if small display, scale text size
    lcd.setFont(&fonts::Font2);        //set font size
    lcd.setTextSize(0.9f);             //set text size to 1/2 normal size
  #endif           
  #if DISP_SIZE == LARGE_DISP || DISP_SIZE == CUSTOM_DISP //if large or custom display
    lcd.setTextSize(1.0f);             //set text size to full size
  #endif
  lcd.setCursor( 0.5f*(lcd.width()-lcd.textWidth(NAME) ), 0.1f*lcd.height() ); //where to write Name intro
  lcd.printf( NAME );                  //send name intro to display
  lcd.setCursor( 0.5f*(lcd.width()-lcd.textWidth(VERSIONID) ), 0.3f*lcd.height()); //where to write Version ID
  lcd.printf(VERSIONID);               //send version ID to display
  lcd.setCursor( 0.5f*(lcd.width()-lcd.textWidth(ID) ), 0.5f*lcd.height());  //where to write ID
  lcd.printf(ID);                      //sendi ID to display
  delay(2000);                         //time to read display
  lcd.clear(BGCol);                    //clear screen
}

void scanstep() {                                        // Encoder routine for step changes
if (digitalRead(Step)==LOW) {                          
  lockFlag = LOW; 
  if (quad >= EncoderStep || quad <= -EncoderStep){     //Check for quad overflow on encoder
    
    if (quad > EncoderStep) quad = EncoderStep;         //clockwise rotation 
    if (quad <-EncoderStep) quad = -EncoderStep;        //counter clockwise rotation

    stepcount += quad/EncoderStep;                      //increment encodercount one encoder step unit
    stp += byte(stepcount);                             //send encodercount value to step + or -
    quad = 0;                                           //reset quad
  } 
  //Serial.println(EncoderStep);                        //diagnostics
  if (stepcount != stepcountlast) {                     //check for change in encodercount value
  Serial.print(stp); Serial.print("  ");                //diagnostics
  Serial.println(stepcount);                            //diagnostics
  if(stp<=0) stp=7;                                     //test for absolute low step value
  if(stp>7) stp = 1;                                    //test for absolute high step value
  setstep();                                            //set the step frequency value
  stepcountlast = 0; stepcount =0;                      //reset stepcount and flag
  }
} 
}

//-----------------------------------------------------------------------------------------------
void loop() {    // begin main loop here
//-----------------------------------------------------------------------------------------------
// initialize pushbutton flags
  
  currentMillis = millis();                             //start currentMillis counter value
  if (currentMillis - previousMillis >= 60000) {        //look for difference since last update
      f_dchange=1;                                      //set display change flat to true (1)
      previousMillis = currentMillis;                   //restart timer value 
  }
 
        //----------------- Code for buttons, switches and relays ------------------       
  //Encoder Lock switch detection
  if (digitalRead(Lock)==LOW) {lockFlag = LOW; lockFlagLast = LOW;}
  if ((digitalRead(Lock)==HIGH) && (!lockFlagLast)){lockFlagLast=HIGH;lockFlag=HIGH;quad=0;}
  //Step change button detection
  if ((digitalRead(Step)==LOW)&& (stepLast)) setstep();
  if ((digitalRead(Step)==HIGH) &&(!stepLast)){delay(20);stepLast=HIGH;} 
  //Band change button detection
  if ((digitalRead(Band)==LOW)&& (bandLast)) band_change();
  if ((digitalRead(Band)==HIGH)&& (!bandLast)){delay(20); bandLast=HIGH;}
  //Memory change button detection
  if ((digitalRead(Mem)==LOW)&& (memoryLast)) mem_change();
  if ((digitalRead(Mem)==HIGH)&& (!memoryLast)){delay(20); memoryLast=HIGH;}
  //Encoder step change detection  
  if ((digitalRead(Step)==LOW)){stepcountlast = 0; scanstep();}
  if ((digitalRead(Step)==HIGH) &&(!stepcountlast)){delay(20);stepcountlast=HIGH;lockFlag=HIGH;}
  //check for changes from the encoder for frequency change
  if ((lockFlag) && (digitalRead(Step)==HIGH))serialprintcount();                     
  //ByPass switch detection
  if (digitalRead(ByPass)==LOW) {ByPassFlag = LOW; ByPassFlagLast = LOW;}
  if ((digitalRead(ByPass)==HIGH) && (!ByPassFlagLast)){
    ByPassFlagLast=HIGH;
    ByPassFlag=HIGH;
  #if  LPF_CTRL == LPFBCD
    set_frequency();                      //send frequency to Si5351
  #endif
  #if LPF_CTRL == LPF4052
    set_frequency_PCF();                  //send frequency to Si5351
  #endif     
    }
  #if LPF_CTRL == LPFBCD        //Select BCD LPF control
  if ((!ByPassFlag)) { digitalWrite(BCD1,HIGH); digitalWrite(BCD2,LOW); digitalWrite(BCD3,LOW);digitalWrite(BCD4,HIGH);ByPassFlag=HIGH;} //BCD 1001
  #endif
  #if LPF_CTRL == LPF4052        //Select 74HCt4052 LPF control
  if ((!ByPassFlag)) { digitalWrite(MUX0,HIGH); digitalWrite(MUX1,HIGH);}
  #endif
  
  if (f_dchange==1) display_write();                       //routine to write the display
 
}



void setstep() {                                /*procedure to set (advance) the step frequency*/
if (count==6)  {
  switch (stp) {  //used by pushbutton to change frequency step
    case 1: stp = 2; fstep = 1; break;        //step frequency of 1 Hz
    case 2: stp = 3; fstep = 10; break;       //step frequency of 10 Hz
    case 3: stp = 4; fstep = 100;break;       //step frequency of 100 Hz
    case 4: stp = 5; fstep = 500; break;
    case 5: stp = 6; fstep = 1000; break;     //step frequency of 1 KHz
    case 6: stp = 7; fstep = 10000; break;    //step frequency of 10 KHz
    case 7: stp = 8; fstep = 100000; break;   //step frequency of 100 KHz
    case 8: stp = 9; fstep = 1000000; break;  //step frequency of 1 MHz
    case 9: stp = 1; fstep = 10000000; break; //step frequency of 10 MHz
  }
} else
{ 
#if PREFERENCE == CLINT
  switch (stp) {                            //used by pushbutton to change frequency step
    case 1: stp = 2; fstep = 1000; break;   //frequency step change is 1 KHz
    case 2: stp = 3; fstep = 10000; break;  //frequency step change is 10 KHz
    case 3: stp = 4; fstep = 10; break;     //frequency step change is 10 Hz
    case 4: stp = 5; fstep = 100; break;    //frequency step change is 100 Hz
    case 5: stp = 1; fstep = 500; break;    //frequency step change is 500 KHz
  }
#else  
  switch (stp) {                            //used by pushbutton to change frequency step
    case 1: stp = 2; fstep = 1000; break;   //frequency step change is 1KHz
    case 2: stp = 3; fstep = 500; break;    //frequency step change is 500Hz
    case 3: stp = 4; fstep = 100; break;    //frequency step change is 100Hz
    case 4: stp = 5; fstep = 10; break;     //frequency step change is 10Hz
    case 5: stp = 1; fstep = 10000; break;  //frequency step change is 10 KHz
  }
#endif
}
  f_dchange=1;           //set display change flat to true (1)
  stepLast=LOW;          //reset stepLast flag
  stepcountlast=0;       //reset stepcount flag
}

void band_change() {        //procedure called to (advance) the frequency band*/
  freqrecall();             //function for keeping track of the bands, save memory recall new memory
  bandLast=LOW;             //reset bandLast flag to false
  delay(5);                                       
}

void mem_change() {             //procedure for changing memories
  switch (count) {              //First switch makes available the memories tied to the band
    case 1: {switch (memory) {  //second switch determines which memory 1-6 will be used, First is band 1
              case 1: freqa[0] = freq;freq = freqa[1]; memory = 2;break; //save freq to memory A restore memory B
              case 2: freqa[1] = freq;freq = freqa[2]; memory = 3;break; //save freq to memory B restore memory C
              case 3: freqa[2] = freq;freq = freqa[3]; memory = 4;break;
              case 4: freqa[3] = freq;freq = freqa[4]; memory = 5;break;
              case 5: freqa[4] = freq;freq = freqa[5]; memory = 6;break;
              case 6: freqa[5] = freq;freq = freqa[0]; memory = 1;break; //save freq to memory F restore memory A
            }break;
    }//break;
     case 2: {switch (memory) {                                          //second is band 2
              case 1: freqb[0] = freq;freq = freqb[1]; memory = 2;break; //save freq to memory A restore memory B
              case 2: freqb[1] = freq;freq = freqb[2]; memory = 3;break; //save freq to memory B restore memory C
              case 3: freqb[2] = freq;freq = freqb[3]; memory = 4;break;
              case 4: freqb[3] = freq;freq = freqb[4]; memory = 5;break;
              case 5: freqb[4] = freq;freq = freqb[5]; memory = 6;break;
              case 6: freqb[5] = freq;freq = freqb[0]; memory = 1;break; //save freq to memory F restore memory A
            }break;
     }//break;
     case 3: {switch (memory) {                                          //third is band 3
              case 1: freqc[0] = freq;freq = freqc[1]; memory = 2;break;
              case 2: freqc[1] = freq;freq = freqc[2]; memory = 3;break;
              case 3: freqc[2] = freq;freq = freqc[3]; memory = 4;break;
              case 4: freqc[3] = freq;freq = freqc[4]; memory = 5;break;
              case 5: freqc[4] = freq;freq = freqc[5]; memory = 6;break;
              case 6: freqc[5] = freq;freq = freqc[0]; memory = 1;break;
            }break;
     }//break;
     case 4: {switch (memory) {                                          //fourth is band 4
              case 1: freqd[0] = freq;freq = freqd[1]; memory = 2;break;
              case 2: freqd[1] = freq;freq = freqd[2]; memory = 3;break;
              case 3: freqd[2] = freq;freq = freqd[3]; memory = 4;break;
              case 4: freqd[3] = freq;freq = freqd[4]; memory = 5;break;
              case 5: freqd[4] = freq;freq = freqd[5]; memory = 6;break;
              case 6: freqd[5] = freq;freq = freqd[0]; memory = 1;break;
            }break;
     }//break;
     case 5: {switch (memory) {                                          //fifth is band 5
              case 1: freqe[0] = freq;freq = freqe[1]; memory = 2;break;
              case 2: freqe[1] = freq;freq = freqe[2]; memory = 3;break;
              case 3: freqe[2] = freq;freq = freqe[3]; memory = 4;break;
              case 4: freqe[3] = freq;freq = freqe[4]; memory = 5;break;
              case 5: freqe[4] = freq;freq = freqe[5]; memory = 6;break;
              case 6: freqe[5] = freq;freq = freqe[0]; memory = 1;break;
            }break;
     }//break;
     case 6: {switch (memory) {                                          //sixth is band 6
              case 1: freqf[0] = freq;freq = freqf[1]; memory = 2;break;
              case 2: freqf[1] = freq;freq = freqf[2]; memory = 3;break;
              case 3: freqf[2] = freq;freq = freqf[3]; memory = 4;break;
              case 4: freqf[3] = freq;freq = freqf[4]; memory = 5;break;
              case 5: freqf[4] = freq;freq = freqf[5]; memory = 6;break;
              case 6: freqf[5] = freq;freq = freqf[0]; memory = 1;break;
            }break;
     }//break;
  }
  delay(20);
  f_dchange=1;                            //set display renew flag to true (1)
  memoryLast=LOW;                         //reset memoryLast flag to false
#if  LPF_CTRL == LPFBCD
    set_frequency();                      //send frequency to Si5351
  #endif
  #if LPF_CTRL == LPF4052
    set_frequency_PCF();                  //send frequency to Si5351
  #endif     
}

void freqrecall(){                        //code for recalling the saved frequencies 
  //Serial.println(memory);               //diagnostics
  //Serial.println(count);                //diagnostics
  switch (count) {                        //code for saving existing frequency
    case 1: freqa[memory-1] = freq;break; //count=1 store freq into memory band 1
    case 2: freqb[memory-1] = freq;break; //count=2 store freq into memory band 2
    case 3: freqc[memory-1] = freq;break;
    case 4: freqd[memory-1] = freq;break; 
    case 5: freqe[memory-1] = freq;break; 
    case 6: freqf[memory-1] = freq;break; 
  }
  //Serial.print(count);                  //diagnostics
  //Serial.println(stp);                  //diagnostics
  if (count==6) if (stp ==1) stp = 9;else stp = stp-1;  //set stp to correct position to save
  if (count!=6) if (stp ==1) stp = 4;else stp = stp-1;  //set stp to correct position to save
  stpRecall[count-1] = stp;               //save step frequency for band change
  recall[count-1] = memory;               //save memory number for band change
  count = count+1;                        //increment count by one
  if (count == 7) count = 1;              //test if count overflows restart at 1
  memory = recall[count-1];               //memory number recalled from last band save
  stp = stpRecall[count-1];               //stp number recalled from last band save 
  //Serial.print(count);                  //diagnostics
  //Serial.println(stp);                  //diagnostics
  switch (count) {                        //code for recalling saved frequencies
    case 1: freq = freqa[memory-1];break; //make freq = saved memory band 1
    case 2: freq = freqb[memory-1];break; //make freq = saved memory band 2
    case 3: freq = freqc[memory-1];break;
    case 4: freq = freqd[memory-1];break;
    case 5: freq = freqe[memory-1];break;
    case 6: freq = freqf[memory-1];break;
  }
  setstep();                              //set step for default in current band
  #if  LPF_CTRL == LPFBCD
    set_frequency();                      //send frequency to Si5351
  #endif
  #if LPF_CTRL == LPF4052
    set_frequency_PCF();                  //send frequency to Si5351
  #endif                       
  f_dchange=1;                            //write the band change to the display
  delay(5);           
  //Serial.println(count);                //diagnostics
  //Serial.println(freq);                 //diagnostics
}

void display_step() {                      //code to display step frequency on screen
  switch (fstep) {
    case 1: sprintf(step_str,       " 1 Hz"); break;    
    case 10: sprintf(step_str,      "10 Hz");break;
    case 100: sprintf(step_str,     "100 Hz");break;
    case 500: sprintf(step_str,     "500 Hz");break;
    case 1000: sprintf(step_str,    " 1 KHz");break;
    case 10000: sprintf(step_str,   "10 KHz"); break;
    case 100000: sprintf(step_str,  "100 KHz");break;
    case 1000000: sprintf(step_str, " 1 MHz");break;
    case 10000000: sprintf(step_str,"10 MHz");break;
  } 
  #if DISP_SIZE == LARGE_DISP
  sprites[flip].setFont(&fonts::Font4); // Set font
  sprites[flip].setTextSize(0.75f);     // Scale font size
  sprites[flip].setTextColor(CL_NUM);   // Set font color
  sprites[flip].setCursor(140, T1_POS+18 );    // Place cursor here
  sprites[flip].print(step_str);        // send to sprite memory
  #endif
  #if DISP_SIZE == SMALL_DISP
    sprites[flip].setFont(&fonts::Font2); // Set font
    sprites[flip].setTextSize(0.9,0.8);  // Set font scale
    sprites[flip].setTextColor(CL_NUM);   // Set font color
    sprites[flip].setCursor(64, T1_POS+12  );// place cursor here
    sprites[flip].print(step_str);     // Send sprite to memory
  #endif
  #if DISP_SIZE == CUSTOM_DISP
  sprites[flip].setFont(&fonts::Font4); // Set font
  #if PREFERENCE == CLINT
    sprites[flip].setTextSize(0.9f);      // Scale font size
    sprites[flip].setTextColor(CL_NUM_O); // Set font color
    sprites[flip].setCursor(128, T1_POS+3 );    // Place cursor here - Step value - up/down
  #else
    sprites[flip].setTextSize(0.6f);      // Scale font size
    sprites[flip].setTextColor(CL_NUM_O); // Set font color
    sprites[flip].setCursor(145, T1_POS+15 );    // Place cursor here - Step value - up/down
  #endif
  sprites[flip].print(step_str);        // send to sprite memory
  #endif
}

void display_mem() {                    //code to display the current memory A, B, or C
  
  switch (memory) {
    case 1: sprintf(mem_str, " A");break;      //display memory M:A
    case 2: sprintf(mem_str, " B");break;      //display memory M:B
    case 3: sprintf(mem_str, " C");break;      //display memory M:C
    case 4: sprintf(mem_str, " D");break;      //display memory M:D
    case 5: sprintf(mem_str, " E");break;      //display memory M:E
    case 6: sprintf(mem_str, " F");break;      //display memory M:F
  } 
  #if DISP_SIZE == LARGE_DISP
  sprites[flip].setFont(&fonts::Font4);// Set font size
  sprites[flip].setTextSize(0.75f);    // Set font scale
  sprites[flip].setTextColor(CL_NUM);  // Set font color
  sprites[flip].setCursor(265, T1_POS+10 );   // Place cursor
  sprites[flip].print(mem_str);        // Send sprites to memory
  #endif
  #if DISP_SIZE == SMALL_DISP
    sprites[flip].setFont(&fonts::Font2); // Set font
    sprites[flip].setTextSize(0.9,0.8);  // Set font scale
    sprites[flip].setTextColor(CL_NUM);   // Set font color
    sprites[flip].setCursor(134, T1_POS+12 );// place cursor here
    sprites[flip].print(mem_str);     // Send sprite to memory
  #endif
  #if DISP_SIZE == CUSTOM_DISP
  sprites[flip].setFont(&fonts::Font4);// Set font size
  #if PREFERENCE == CLINT
    sprites[flip].setTextSize(0.8f);     // Set font scale
    sprites[flip].setTextColor(CL_NUM);  // Set font color
    sprites[flip].setCursor(285, T1_POS+15 );   // Place cursor  - Move Mem Channel up/down
  #else
    sprites[flip].setTextSize(0.6f);     // Set font scale
    sprites[flip].setTextColor(CL_NUM);  // Set font color
    sprites[flip].setCursor(275, T1_POS+15 );   // Place cursor  - Move Mem Channel up/down
  #endif
  sprites[flip].print(mem_str);        // Send sprites to memory
  #endif
}


void display_band() {                              //code to display the current selected band
 
#if RADIO == A180 
  switch(count) {                             //determine which position the band switch is located
    case 1: sprintf(band_str, " 160 M"); break; //set to 80M position USB
    case 2: sprintf(band_str, "  80 M"); break; //set to 40M position USB
    case 3: sprintf(band_str, "  80 M"); break; //set to 20M position USB
    case 4: sprintf(band_str, "  40 M"); break; //set to 15M position USB
    case 5: sprintf(band_str, "  15 M"); break; //set to 10M position USB
    case 6: sprintf(band_str, "Sig Gen"); break; //Signal Generator
  } 
#endif

#if RADIO == A215 
  switch(count) {                             //determine which position the band switch is located
    case 1: sprintf(band_str, "160 M"); break; //set to 80M position USB
    case 2: sprintf(band_str, " 80 M"); break; //set to 40M position USB
    case 3: sprintf(band_str, " 40 M"); break; //set to 20M position USB
    case 4: sprintf(band_str, " 20 M"); break; //set to 15M position USB
    case 5: sprintf(band_str, " 15 M"); break; //set to 10M position USB
    case 6: sprintf(band_str, "Sig. Gen"); break; //Signal Generator
  } 
#endif

#if RADIO == A210 
  switch(count) {                             //determine which position the band switch is located
    case 1: sprintf(band_str, " 80 M"); break; //set to 80M position USB
    case 2: sprintf(band_str, " 40 M"); break; //set to 40M position USB
    case 3: sprintf(band_str, " 20 M"); break; //set to 20M position USB
    case 4: sprintf(band_str, " 15 M"); break; //set to 15M position USB
    case 5: sprintf(band_str, " 10 M"); break; //set to 10M position USB
    case 6: sprintf(band_str, "Sig. Gen"); break; //Signal Generator
  } 
#endif 



  #if DISP_SIZE == LARGE_DISP
  sprites[flip].setFont(&fonts::Font4); // Set font
  sprites[flip].setTextSize(0.75f);      // Scale font size
  sprites[flip].setTextColor(CL_NUM);   // Set font color
  sprites[flip].setCursor(0, T1_POS+18 );     // place cursor here - Band up/down
  sprites[flip].print(band_str);       // send to sprite memory
  #endif
  #if DISP_SIZE == SMALL_DISP
    sprites[flip].setFont(&fonts::Font2); // Set font
    sprites[flip].setTextSize(0.85,0.8);  // Set font scale
    sprites[flip].setTextColor(CL_NUM);   // Set font color
    sprites[flip].setCursor(11, T1_POS+12 );// place cursor here
    sprites[flip].print(band_str);     // Send sprite to memory
  #endif
  #if DISP_SIZE == CUSTOM_DISP
  sprites[flip].setFont(&fonts::Font4); // Set font
  #if PREFERENCE == CLINT
    sprites[flip].setTextSize(0.8f);      // Scale font size
    sprites[flip].setTextColor(CL_NUM);   // Set font color
    sprites[flip].setCursor(20, T1_POS+15 );     // place cursor here - Band value up/down
  #else
    sprites[flip].setTextSize(0.6f);      // Scale font size
    sprites[flip].setTextColor(CL_NUM);   // Set font color
    sprites[flip].setCursor(20, T1_POS+15 );     // place cursor here - Band value up/down
  #endif
  sprites[flip].print(band_str);        // send to sprite memory
  #endif
}

void display_freq() {    //code to display the current selected frequency in the correct position and aligned
      char strl[24];    // strings to manipulate appearance of the digital frequency 
      if (freq > 999999) {pad = 10; sprintf(str,"%3d.%03d,%03d",  freq/1000000, (freq/1000)%1000, (freq)%1000 );}
      else if (freq > 999) {pad = 40; sprintf(str,"%3d.%03d",(freq/1000)%1000,(freq)%1000 );}
      else {pad = 88;sprintf(str,"%3d",(freq)%1000 );}
      int cc=0;                   // integer for manipulating the string
      if(strl[0]=='0') {          // gets rid of leading 0
        cc++;                     // increments CC value by 1
        if(strl[1]=='0') cc++;    // if 2 leading 0's increment cc vale by 1 again
      }
      for(int i=0; i<12; i++) {   // creates value of i and loops while i is less than 12
        strl[2*i]=str[i+cc];      // moves string from str to strl
        strl[2*i+1]=' ';          // add a space between each letter to stretch out string
      }
    #if DISP_SIZE == LARGE_DISP
    sprites[flip].setFont(&fonts::Font4);    // Set font
    sprites[flip].setTextSize(0.75f);        // Scale font size
    sprites[flip].setTextColor(CL_NUM_TEXT); // Set font color
    sprites[flip].setCursor(2, T1_POS );         // place cursor here
    sprites[flip].print(TEXT);               // send to sprite memory
    sprites[flip].drawRoundRect(0,F1_POS,320,40,5,CL_FREQ_BOX); // draw box (x1,y1,x2,y2,thick,color) (0,15,320,40,5,)
    sprites[flip].drawRoundRect(1,F1_POS+1,318,38,5,CL_FREQ_BOX);
    sprites[flip].setFont(&fonts::Font6);  // set font type and size to Font6
    sprites[flip].setTextSize(0.55, 0.81); // text size 0.55x, 0.81y 
    sprites[flip].setTextColor(CL_F_NUM);  // text color set in config
    sprites[flip].setCursor(34,F1_POS+6);        // put cursor to position x,y
    sprites[flip].print(strl);             // print string strl
    sprites[flip].setFont(&fonts::Font4);  // set font type and size to Font4
    sprites[flip].setTextSize(1.0f);       // text size 1.0
    sprites[flip].setTextColor(CL_F_NUM);  // text color set in config
    sprites[flip].setCursor(248,F1_POS+15);       // put cursor to position x,y
              // print "MHz" to 
    if (freq >999999) sprites[flip].print("MHz");
    else if (freq > 999) sprites[flip].print("KHz");
    else sprites[flip].print(" Hz");
    #endif
    #if DISP_SIZE == SMALL_DISP
    sprites[flip].setFont(&fonts::Font2);    // Set font
    sprites[flip].setTextSize(0.8f);        // Scale font size
    sprites[flip].setTextColor(CL_NUM_TEXT); // Set font color
    sprites[flip].setCursor(14, T1_POS );         // place cursor here
    sprites[flip].print(TEXT);               // send to sprite memory
    sprites[flip].drawRoundRect(0,F1_POS,160,30,5,CL_FREQ_BOX); // draw box (x1,y1,x2,y2,thick,color) (0,15,320,40,5,)
    sprites[flip].drawRoundRect(1,F1_POS+1,158,28,5,CL_FREQ_BOX);
    sprites[flip].setFont(&fonts::Font4);  // set font type and size to Font6
    sprites[flip].setTextSize(0.6,1.0); // text size 0.55x, 0.81y 
    sprites[flip].setTextColor(CL_F_NUM);  // text color set in config
    sprites[flip].setCursor(20,F1_POS+6);        // put cursor to position x,y
    sprites[flip].print(strl);             // print string strl
    sprites[flip].setFont(&fonts::Font4);  // set font type and size to Font4
    sprites[flip].setTextSize(0.5f);       // text size 1.0
    sprites[flip].setTextColor(CL_F_NUM);  // text color set in config
    sprites[flip].setCursor(120,F1_POS+14);       // put cursor to position x,y
              // print "MHz" to 
    if (freq >999999) sprites[flip].print("MHz");
    else if (freq > 999) sprites[flip].print("KHz");
    else sprites[flip].print(" Hz");
    #endif
    #if DISP_SIZE == CUSTOM_DISP
    sprites[flip].setFont(&fonts::Font4);  // Set font
    sprites[flip].setTextSize(0.65f);      // Scale font size
    sprites[flip].setTextColor(CL_NUM_TEXT);   // Set font color
    sprites[flip].setCursor(2, T1_POS );       // place cursor here - Band/Step/MEM up/down
    sprites[flip].print(TEXT);             // send to sprite memory
    sprites[flip].drawRoundRect(10,F1_POS,309,40,15,CL_FREQ_BOX); // draw box (x1,y1,x2,y2,thick,color) (0,15,320,40,5,)
    //sprites[flip].drawRoundRect(6,F1_POS+1,312,38,15,CL_FREQ_BOX);
    sprites[flip].setFont(&fonts::Font6);  // set font type and size to Font6
    sprites[flip].setTextSize(0.55, 0.81); // text size 0.55x, 0.81y - Frequency text size
    sprites[flip].setTextColor(CL_F_NUM);  // text color set in config
    sprites[flip].setCursor(34,F1_POS+5);        // put cursor to position x,y - Frequency position
    sprites[flip].print(strl);             // print string strl
    sprites[flip].setFont(&fonts::Font4);  // set font type and size to Font4
    sprites[flip].setTextSize(1.0f);       // text size 1.0
    sprites[flip].setTextColor(CL_F_NUM);  // text color set in config
    sprites[flip].setCursor(248,F1_POS+15);       // put cursor to position x,y - MHz position up/down
              // print "MHz" to 
    if (freq >999999) sprites[flip].print("MHz");
    else if (freq > 999) sprites[flip].print("KHz");
    else sprites[flip].print(" Hz");
    #endif
}

#if  LPF_CTRL == LPFBCD
void set_frequency() { //procedure for setting output frequency
  if (count != 6) {    //Atlas only bands
    si5351.setPower(0,SIOUT_4mA);
    if (freq > 9990000) si5351.setFreq(0,(freq-IF_OFFSET)); 
      else si5351.setFreq(0,(freq+IF_OFFSET)); //above 9.99MHz freq - offset else freq + offset
    if ((digitalRead(ByPass)==HIGH)) {  //Bypass is off (High)
    if (freq > 23500000) {   //10m band
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,HIGH); digitalWrite(BCD3,HIGH);digitalWrite(BCD4,LOW);}       //1110
    if ((freq > 15000000) && (freq < 23500001)) {   //15m band
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,LOW); digitalWrite(BCD3,HIGH);digitalWrite(BCD4,LOW);}        //1010
    if (freq < 15000001) {  //160m, 80m, 40m, 20m band
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,HIGH); digitalWrite(BCD3,LOW); digitalWrite(BCD4,LOW);}       //1100
    } else {digitalWrite(BCD1,HIGH); digitalWrite(BCD2,LOW); digitalWrite(BCD3,LOW);digitalWrite(BCD4,HIGH);}  //1001 in byPass mode
    } else {
    si5351.setFreq(0,freq);                 //send freq to Si5351
    if ((digitalRead(ByPass)==HIGH)) {      //Bypass is off (High)
    Serial.print("ByPassSwitch is High ");  //diagnostics
    if (freq > 150000000) { 
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,LOW); digitalWrite(BCD3,LOW);digitalWrite(BCD4,HIGH); //1001
      si5351.setPower(0,SIOUT_4mA);
      } 
    if ((freq > 100000000) && (freq < 150000001)) { 
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,LOW); digitalWrite(BCD3,LOW);digitalWrite(BCD4,HIGH);  //1001
      si5351.setPower(0,SIOUT_6mA);
    }
    if ((freq > 34000000) && (freq < 100000001)) { 
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,LOW); digitalWrite(BCD3,LOW);digitalWrite(BCD4,HIGH);  //1001
      si5351.setPower(0,SIOUT_8mA);
    }
    if ((freq > 23500000) && (freq < 34000001)) { 
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,HIGH); digitalWrite(BCD3,HIGH);digitalWrite(BCD4,LOW); //1110
      si5351.setPower(0,SIOUT_8mA);
    }
    if ((freq > 15000000) && (freq < 23500001)) { 
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,LOW); digitalWrite(BCD3,HIGH);digitalWrite(BCD4,LOW);  //1010
    }
    if ((freq >  8000000) && (freq < 15000001)) { 
      digitalWrite(BCD1,LOW); digitalWrite(BCD2,HIGH); digitalWrite(BCD3,LOW);digitalWrite(BCD4,LOW);  //0100
      si5351.setPower(0,SIOUT_8mA);
    }
    if ((freq >  4500000) && (freq <  8000001)) { 
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,LOW); digitalWrite(BCD3,LOW);digitalWrite(BCD4,LOW);  //1000
      si5351.setPower(0,SIOUT_8mA);
    }
    if ((freq >  2200000) && (freq <  4500001)) { 
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,HIGH); digitalWrite(BCD3,LOW);digitalWrite(BCD4,LOW);  //1100
      si5351.setPower(0,SIOUT_8mA);
    }
    if ( freq <  2200000) { 
      digitalWrite(BCD1,LOW); digitalWrite(BCD2,LOW); digitalWrite(BCD3,LOW);digitalWrite(BCD4,LOW);    //0000
      si5351.setPower(0,SIOUT_8mA);
    }
    } else { 
      digitalWrite(BCD1,HIGH); digitalWrite(BCD2,LOW); digitalWrite(BCD3,LOW);digitalWrite(BCD4,HIGH);   //1001 in byPass mode
      Serial.print("ByPassSwitch is Low ");  //diagnostics
    }
  }
  Serial.print(digitalRead(BCD1));Serial.print(digitalRead(BCD2));    //diagnostics
  Serial.print(digitalRead(BCD3));Serial.println(digitalRead(BCD4));  //diagnostics
  }  
#endif

#if  LPF_CTRL == LPF4052
void set_frequency_PCF() { //procedure for setting output frequency
  if (count != 6) {    //Atlas only band
    if (freq > 10500000) si5351.setFreq(0,(freq-IF_OFFSET)); 
      else si5351.setFreq(0,(freq+IF_OFFSET)); //above 9.99MHz freq - offset else freq + offset
    if ((digitalRead(ByPass)==HIGH)) {  //Bypass is off (High)
    if (freq > 23500000) {   //10m band
      digitalWrite(MUX0,LOW); digitalWrite(MUX1,HIGH);  // 01
      si5351.setPower(0,SIOUT_4mA);
      }
    if ((freq > 15000000) && (freq < 23500001)) {   //15m band
      digitalWrite(MUX0,HIGH); digitalWrite(MUX1,LOW); // 10
      si5351.setPower(0,SIOUT_4mA);
      } 
    if ((freq >  8000000) && (freq < 15000001)) {   //20m band
      digitalWrite(MUX0,LOW); digitalWrite(MUX1,LOW);  // 00
      si5351.setPower(0,SIOUT_8mA);
      }
    if ((freq >  4500000) && (freq <  8000001)) {   //40m band
      digitalWrite(MUX0,HIGH); digitalWrite(MUX1,LOW); // 10
      si5351.setPower(0,SIOUT_4mA);
      }
    if ((freq >  2200000) && (freq <  4500001)) {   //80m band
      digitalWrite(MUX0,LOW); digitalWrite(MUX1,LOW); // 00
      si5351.setPower(0,SIOUT_8mA);
      }  
    if (freq  <  2200001) {                         //160m band
      digitalWrite(MUX0,LOW); digitalWrite(MUX1,LOW); // 00
      si5351.setPower(0,SIOUT_8mA);
      }  
    } else {
      digitalWrite(MUX0,HIGH); digitalWrite(MUX1,HIGH);  // 11 in byPass mode
      si5351.setPower(0,SIOUT_4mA);
      Serial.print("ByPassSwitch is Low ");  //diagnostics
    }
    } else {
    si5351.setFreq(0,freq);                 //send freq to Si5351
    if ((digitalRead(ByPass)==HIGH)) {      //Bypass is off (High)
    Serial.print("ByPassSwitch is High ");  //diagnostics
    if (freq > 150000000) { 
      digitalWrite(MUX0,HIGH); digitalWrite(MUX1,HIGH);   // 11
      si5351.setPower(0,SIOUT_8mA);
      } 
    if ((freq > 100000000) && (freq < 150000001)) { 
      digitalWrite(MUX0,HIGH); digitalWrite(MUX1,HIGH);   // 11
      si5351.setPower(0,SIOUT_6mA);
    }
    if ((freq > 34000000) && (freq < 100000001)) { 
      digitalWrite(MUX0,HIGH); digitalWrite(MUX1,HIGH);   // 11
      si5351.setPower(0,SIOUT_4mA);
    }
    if ((freq > 22000000) && (freq < 34000001)) { 
      digitalWrite(MUX0,LOW); digitalWrite(MUX1,HIGH); 
      si5351.setPower(0,SIOUT_4mA);
      }  // 01
    if ((freq > 11800000) && (freq < 22000001)) { 
      digitalWrite(MUX0,HIGH); digitalWrite(MUX1,LOW); 
      si5351.setPower(0,SIOUT_4mA);
      }  // 10
    if ((freq >  8000000) && (freq < 11800001)) { 
      digitalWrite(MUX0,LOW); digitalWrite(MUX1,LOW); 
      si5351.setPower(0,SIOUT_8mA);
      }   // 00
    if ((freq >  4500000) && (freq <  8000001)) { 
      digitalWrite(MUX0,LOW); digitalWrite(MUX1,LOW); 
      si5351.setPower(0,SIOUT_8mA);
      }   // 00
    if ((freq >  2200000) && (freq <  4500001)) { 
      digitalWrite(MUX0,LOW); digitalWrite(MUX1,LOW); 
      si5351.setPower(0,SIOUT_8mA);
      }   // 00
    if ( freq <  2200000) { digitalWrite(MUX0,LOW); digitalWrite(MUX1,LOW);}  //00
    } else {
      digitalWrite(MUX0,HIGH); digitalWrite(MUX1,HIGH);   // 11 in byPass mode
      si5351.setPower(0,SIOUT_4mA);
      Serial.print("ByPassSwitch is Low ");  //diagnostics
    }
  }
  Serial.print(digitalRead(MUX0));Serial.print(digitalRead(MUX1));  //diagnostics
  }  
#endif   

void bandpresets() {                      //procedure for settings first time through in setup
  count = 2;                              //set to band number - 1 (band 2 set count=1)
  memory = recall[count-1];               //memory number recalled from last band save
  stp = stpRecall[count-1];               //stp number recalled from last band save 
  //Serial.print(count);                  //diagnostics
  //Serial.println(stp);                  //diagnostics
  switch (count) {                        //code for recalling saved frequencies
    case 1: freq = freqa[memory-1];break; //make freq = saved memory band 1
    case 2: freq = freqb[memory-1];break; //make freq = saved memory band 2
    case 3: freq = freqc[memory-1];break;
    case 4: freq = freqd[memory-1];break;
    case 5: freq = freqe[memory-1];break;
    case 6: freq = freqf[memory-1];break;
  }
  stp = 1;          //set default step position of band to 1 KHz
  setstep();                              //set step for default in current band
  Serial.println(freq);
  #if  LPF_CTRL == LPFBCD
    set_frequency();                        //send frequency to Si5351
  #endif
  #if LPF_CTRL == LPF4052
    set_frequency_PCF();
  #endif
  f_dchange=1;                            //write the band change to the display
  delay(5); 
}

void display_write() {                               // procedure to write to the screen
 //if (f_dchange==1) {  
   f_dchange==0;
   sprites[flip].clear(BGCol);                       // Clear screen
   //Serial.println(sprite_height);                  // Diagnostics
   dial.draw(freq,0);                                // Send dial to screen
   display_freq();                                   // Send frequency to screen
   display_band();                                   // Send band to screen
   display_step();                                   // Send step to screen
   display_mem();                                    // Send memory to screen
   sprites[flip].pushSprite(&lcd, 0, 0);             // Send frequency and 1/2 of dial to screen
   flip = !flip;                                     // clear sprite flag
   dial.draw(freq,sprite_height);                    // draw 2nd 1/2 of dial to screen
   sprites[flip].pushSprite(&lcd, 0, sprite_height); // Send 2nd half of dial to screen
   flip = !flip;                                     // clear sprite flag
 //}
} // End of Display process
