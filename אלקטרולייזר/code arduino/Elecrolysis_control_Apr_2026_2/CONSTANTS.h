#ifndef CONSTANTS
#define CONSTANTS
#define DEMO
/*  IO map
 *  Encoder bits: BIT_0 - D2,   BIT_1 - D3
 *  BCD bits: A-D3, B-D4, C-D5,  D-D6
 *  LE (
 *  Left digit (LE_100) LE (Negative Pulse) - D8; !! used as 10th digit
 *  Right digit (10th)LE (Negative Pulse) - D9 ; !! used as units
 *  *==========Arduino Nano pinout======
 *                      _______
 *                 TXD-|       |-Vin
 *                 RXD-|       |-Gnd
 *                 RST-|       |-RST
 *                 GND-|       |-+5V
 *            SW_IN D2-|       |-A7  
 *            BCD_A D3-|       |-A6  SET_CURRENT_TRIMER_IN
 *            BCD_B D4-|       |-A5,
 *            BCD_C D5-|       |-A4,
 *            BCD_D D6-|       |-A3, 
 *           LE_100 D7-|       |-A2, 
 *            LE_10 D8-|       |-A1, ENCODER_BIT_1
 *                  D9-|       |-A0, ENCODER_BIT_0
 *      SPARK_OUT  D10-|       |-Ref
 *    ELECTROD_PWM D11-|       |-3.3V
 *                 D12-|       |-D13 
 *                      --USB--
 */

#define SERIAL_BAUD_RATE (115200)
#define SW_IN (2) // input from pushbutton switch (nead pull up SW/HW)

#define BCD_A (3) // Output to BCD_A (4511)
#define BCD_B (4) // Output to BCD_B (4511)
#define BCD_C (5) // Output to BCD_C (4511)
#define BCD_D (6) // Output to BCD_D (4511)

#define LE_CENTURIES (7) // Output Left (100th) digit LE (negative pulse) used as 100th
#define LE_DOZENS (8) //Output middle (10th) digit LE (negative pulse) used as 10th
#define LE_UNITS (9) // Output right digit LE (negative pulse) used as units 
#define SPARK_OUT (10) // Output to FET drive HV spark module
#define ELECTROD_PWM (11) // Output to FET drive electrod current

#define SET_CURRENT_TRIMER_IN (A6) // analog input to set PWM

#define ENCODER_BIT_0 (A1) // input IO for gray code bit 0, Green wire in 400 and 100 ppr encoders 
#define ENCODER_BIT_1 (A0) // input IO for gray code bit 1, white wire in 400 and 100 ppr encoders 


int const  MIN_ELECTROLYSIS_TIME = 5 ; //Seconds
int const  MAX_ELECTROLYSIS_TIME = 60 ; //Seconds
int const  MAX_ELECTROLYSIS_TIME_FIRST_READ = 80;
uint32_t const DEFAULT_ELECTROLYSIS_TIME = 30; //sec 

int const PRESCALAR = 4 ; //encoder count per "click"

int const  MIN_PWM = 0 ; //PWM set electrolysis current
int const  MAX_PWM = 75; //PWM set electrolysis current
int const  DEFAULT_PWM = 40 ; //asume 3Volt to set 2A current. based on Dec experiments with 50# stainless steel mesh

int const  SPARK_TIME = 500 ; //mSeconds

int const  WAIT_FOR_CALIBRATION = 4000 ; //mSeconds waiting during switch preased for calibration

int const  BLINK_ON = 800 ; //mSeconds
int const  BLINK_OFF = 200 ; //mSeconds
int const  BLINK_TIME = 5;// Seconds !!
int const  BOUNCE_TIME = 50; //ms
bool check_state = false;
//-- Parameters and global variable ---
// -- encoder reading parameters and vairables 

uint32_t electrolysis_time = DEFAULT_ELECTROLYSIS_TIME ; //Seconds
bool first_read = true;//allow to increase the eletrolise time for only the first session

int prescalar_counter = 0 ; //encoder count per "click"
int Move_Sum  = 0; //Counting total encoder movment 
int Error_counter = 0;// to check if encoder reading is OK
byte Old_Read = 0; // old 2 bits read from encoder 
byte New_Read = 0; // new 2 bits read from encoder 
byte Check_Direction = 0; // 4 bits (old1, old0, new1, new0) 

int switch_state = HIGH ; 
int trimmer_read = 0 ;//
int pwm  = DEFAULT_PWM ; // drive electrolysis cyrrent 

//uint32_t Idle_Time_Counter = 0; 
//uint32_t Current_Time = 0; 
//uint32_t Time_From_Start = 0; 

// 7 segments parameters
//const int MAX_NUM_TO_DISPLAY  = 99 ; //the maximum number to display
//const int MIN_NUM_TO_DISPLAY  = 0 ; //the minimum number to display
int Number_To_Display = 0;
int Digit_3_To_Display = 0;// Left (100th) digit to disply - used as 10th
int Digit_2_To_Display = 0;// Mid right (10th) digit to disply  - used as units 
int Digit_1_To_Display = 0;// right (external unit) digit to disply - not used for winf tunnel

const int8_t  Number_OF_7SEG = 3;
const int8_t RIGHT_DIGIT = 1;
const int8_t MIDDLE_DIGIT = 2;
const int8_t LEFT_DIGIT = 3;
const int UNIT_CALIBRATION = 10; //if we use only the left and the midlle digit multiply the result by 10

///////////////////////////////////////////////

/********************************/
#endif