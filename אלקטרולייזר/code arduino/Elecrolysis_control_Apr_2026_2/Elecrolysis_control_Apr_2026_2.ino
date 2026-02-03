/* 
 *  Control Electrolysis demo  Dec 2019 
 *  see G:\Bloomfield\פעילויות חינוך\מתקן אלקטרוליזה פיצוץ מימן חנוכה 201
 *  
 *  7 segments for Wid tunel and Piston Robot Bike, Bicycle Exhebition 2017
 *  See Seven_Segments_Arduino_Main_Board_June_2017.sch
 *  library H:\Amir Design\PCB_Design\Bloomfield\Bicycle_2017\Seven_Segments_Arduino
 *  control 2 digit BCD 4511 
 *  read encoder -.
 *  Blink for ZZZ sec
 *
 *  IO map
 *  Encoder bits: BIT_0 - D2,   BIT_1 - D3 
 *  BCD bits: A-D4, B-D5, C-D6,  D-D7
 *  Left digit (100th) LE (Negative Pulse) - D8; !! used as 10th digit
 *  Right digit (10th)LE (Negative Pulse) - D9 ; !! used as units 
  *  
 */
#include "CONSTANTS.h"
#include "Basic_Routines.h"

void setup() {
  TCCR1B = TCCR1B & B11111000 | B00000010;// to set fast PWM on D9, D10 to 3.9 Khz see https://etechnophiles.com/change-frequency-pwm-pins-arduino-uno/
  //TCCR1B = TCCR1B & B11111000 | B00000001;// to set fast PWM on D9, D10 to 30 Khz see https://etechnophiles.com/change-frequency-pwm-pins-arduino-uno/
	Serial.begin(SERIAL_BAUD_RATE);

  Init_Output(BCD_A);
  Init_Output(BCD_B);
  Init_Output(BCD_C);
  Init_Output(BCD_D);
  Init_Output(LE_CENTURIES);
  digitalWrite(LE_CENTURIES, HIGH);// lock digit
  Init_Output(LE_DOZENS);
  digitalWrite(LE_DOZENS, HIGH);// lock digit
  Init_Output(ELECTROD_PWM);
  Init_Output(SPARK_OUT);
 
  pinMode(ENCODER_BIT_0, INPUT_PULLUP);
  pinMode(ENCODER_BIT_1, INPUT_PULLUP);
  pinMode(SW_IN, INPUT_PULLUP);

  
  
  for (int i = 1; i<= 3; i++ ) {blank_Digit(i);}// blank all digits 
  //if you push a long press in the init do calibration
  switch_state = digitalRead(SW_IN);
  if (switch_state == LOW) { // check switch  
      delay (WAIT_FOR_CALIBRATION) ;}//
  switch_state = digitalRead(SW_IN);
  if (switch_state == LOW) { // start count down 
      Calibration() ;}//  if switch presed long enought - call calibration 

 trimmer_read = analogRead(SET_CURRENT_TRIMER_IN);
 pwm = map(trimmer_read,0,1024, MIN_PWM, MAX_PWM);   
 Serial.println (pwm);
 NUMBER_TO_DISPLAY (electrolysis_time*UNIT_CALIBRATION);
 Read_Encoder();
 Serial.println ("init");
}

void loop() {

//  int switch_state = HIGH ; 
  int temp_read = Read_Encoder();
  
  if (temp_read != 0) {
    prescalar_counter = prescalar_counter + temp_read;
    if (prescalar_counter >= PRESCALAR ) {
      electrolysis_time = electrolysis_time +  (prescalar_counter / PRESCALAR);
      prescalar_counter = 0 ;
    }
    if (prescalar_counter <= (- PRESCALAR) ) {
      electrolysis_time = electrolysis_time +  (prescalar_counter / PRESCALAR);
      prescalar_counter = 0 ;
    }
    if (electrolysis_time <= MIN_ELECTROLYSIS_TIME) { electrolysis_time = MIN_ELECTROLYSIS_TIME;}

    if (first_read) {
      if (electrolysis_time >= MAX_ELECTROLYSIS_TIME_FIRST_READ) {electrolysis_time = MAX_ELECTROLYSIS_TIME_FIRST_READ;}
    }
    else {
      if (electrolysis_time >= MAX_ELECTROLYSIS_TIME) {electrolysis_time = MAX_ELECTROLYSIS_TIME;}
    }

    Serial.println (electrolysis_time);
    NUMBER_TO_DISPLAY (electrolysis_time*UNIT_CALIBRATION);
  }
  if (PRESS_BUTTON()) { // start count down 
    first_read = false;
    trimmer_read = analogRead(SET_CURRENT_TRIMER_IN);// to enable trimmer change between demo
    pwm = map(trimmer_read,0,1024, MIN_PWM, MAX_PWM);
    analogWrite(ELECTROD_PWM, pwm);

    Count_Down (electrolysis_time); // exit count down is sw preased 
    
    if(electrolysis_time <= BLINK_TIME){
      Ignition(SPARK_TIME);
      electrolysis_time = DEFAULT_ELECTROLYSIS_TIME;
      NUMBER_TO_DISPLAY (electrolysis_time*UNIT_CALIBRATION);
    }
    analogWrite(ELECTROD_PWM, 0);
    digitalWrite (ELECTROD_PWM,LOW);
    delay (1000);
    
    
  }
}
