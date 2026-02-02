#ifndef Basic_Routines
#define Basic_Routines

#include "CONSTANTS.h"

//--------------------------Subroutines ----------------------------//


/////////////////////////////////////// 7SEG ////////////////////////////////
// send digit to 4511 
void Display_Digit(int digit_number, int digit_to_show) {
 if ((digit_number >=1)&&(digit_number <=3)&&(digit_to_show >=0)&&(digit_to_show <=9)) {
    digitalWrite(BCD_A, LOW);// asume bit zero 
    if ((digit_to_show & 1) == 1) {digitalWrite(BCD_A, HIGH);}
    digitalWrite(BCD_B, LOW);// asume bit zero 
    if ((digit_to_show & 2) == 2) {digitalWrite(BCD_B, HIGH);}
    digitalWrite(BCD_C, LOW);// asume bit zero 
    if ((digit_to_show & 4) == 4) {digitalWrite(BCD_C, HIGH);}
    digitalWrite(BCD_D, LOW);// asume bit zero 
    if ((digit_to_show & 8) == 8) {digitalWrite(BCD_D, HIGH);}
    switch(digit_number) {
      case 2:
       digitalWrite(LE_RIGHT, LOW);
       digitalWrite(LE_RIGHT, HIGH); 
       break;
      case 3:
       digitalWrite(LE_LEFT, LOW);
       digitalWrite(LE_LEFT, HIGH); 
       break;
    }
 }
}

/***********/ 
// digtal from number extrude the 3 digits to display (dlobal variables) 
void Digits_from_Number(int in_number){
    Digit_3_To_Display = 100*(in_number/100)/100;// left digit to disply 
    int Temp_10 = in_number - 100*Digit_3_To_Display;
    Digit_2_To_Display = 10*(Temp_10/10)/10;// mid digit to disply 
    Digit_1_To_Display = Temp_10-10*Digit_2_To_Display;// right digit to disply 
}

//************************
//Blank Digit (send FF)
void Blank_Digit(int digit_number){
    digitalWrite(BCD_A, HIGH);// set bit one
    digitalWrite(BCD_B, HIGH);// set bit one
    digitalWrite(BCD_C, HIGH);// set bit one
    digitalWrite(BCD_D, HIGH);// set bit one
    switch(digit_number) {
      case 2:
       digitalWrite(LE_RIGHT, LOW);
       digitalWrite(LE_RIGHT, HIGH); 
       break;
      case 3:
       digitalWrite(LE_LEFT, LOW);
       digitalWrite(LE_LEFT, HIGH); 
       break;
    }
}
//************************

// display full number 
void Display_full_Number(int Number_To_Display) {
  Digits_from_Number(Number_To_Display);
  Serial.print ("Now display");  
  Serial.print ("\t"); //tab
  Serial.println (Number_To_Display);  
   Display_Digit(2, Digit_1_To_Display);  // for 3 digits use Display_Digit(1, Digit_1_To_Display)
   Display_Digit(3, Digit_2_To_Display); // for 3 digits use Display_Digit(2, Digit_2_To_Display)
   //Display_Digit(3, Digit_3_To_Display); // not used    
}
//********************

//Test 7 segments display 0-9 same digit on all 3 digits 
void Test_7_segments(){
for (int i = 0; i <= 9; i++){
  Digits_from_Number(Number_To_Display);
  Serial.print ("testing 7 segments");  
  Serial.print ("\t"); //tab
  Serial.println (i);  

   Display_Digit(1, i);  
   Display_Digit(2, i);
   Display_Digit(3, i);    
   delay(1000);
  }
}
//////////////////////////////////////////////////////////////////////////////
/***********/ 

// initial IO to be output and set to zero - may not nead
void Init_Output(int IO_Pin){
  digitalWrite(IO_Pin, LOW);/* try avoid high output */
  pinMode(IO_Pin, OUTPUT);
  digitalWrite(IO_Pin, LOW);/* make sure low  output */
}
// Ignition - ignition time set in mS !!!
void Ignition(int ignition_time) {
  digitalWrite (SPARK_OUT, HIGH);
  delay (ignition_time);
  digitalWrite (SPARK_OUT, LOW);
  digitalWrite (SPARK_OUT, LOW);// make sure :) 
}
/***********/ 
// Calibration - read trimer and set PWM only during SW pressed !!!
void Calibration(){
  Ignition(SPARK_TIME);
  Display_full_Number(88) ;
  while  (switch_state == LOW) {
     analogWrite(ELECTROD_PWM, pwm);
     trimmer_read = analogRead(SET_CURRENT_TRIMER_IN);
     pwm = map(trimmer_read,0,1024, MIN_PWM, MAX_PWM);
     Display_full_Number(pwm);
     switch_state = digitalRead(SW_IN); // wait switch release 
    }
  digitalWrite (ELECTROD_PWM, LOW);
  digitalWrite (ELECTROD_PWM, LOW);// make sure
}



/***********/ 
// Count_Down - time to count in SEC (not mS)!!!
void Count_Down(uint32_t Time_To_Count) {
//int switch_state = HIGH ; 
uint32_t time_from_start = millis()  ;
uint32_t temp_pass_time = millis() - time_from_start; 
uint32_t temp_time_to_show = (Time_To_Count - temp_pass_time/1000) ;
//Display_full_Number (temp_time_to_show);
while (temp_pass_time <= Time_To_Count*1000) {
    temp_time_to_show = (Time_To_Count - temp_pass_time/1000) ;
    Display_full_Number (temp_time_to_show);
    if (temp_time_to_show <=BLINK_TIME){
      delay (BLINK_ON);    
      switch_state = digitalRead(SW_IN);
      if (switch_state == LOW) {break;} ; // exit count down if sw pressed 
      Blank_Digit(3);
      Blank_Digit(2);
      delay (BLINK_OFF);
      }
  temp_pass_time = millis() - time_from_start;
  switch_state = digitalRead(SW_IN);
  if (switch_state == LOW) {break;} ; // exit count down if sw preased 
  }
}

/****************************************************/

// read encoder and return total counting (serial print also errors..) use global variable 
int Read_Encoder(){
     Old_Read = New_Read ;
     int temp_return = 0 ;
 //  New_Read = PINB & 3;// only if ATmega 168/328 and encoder bits are 8,9 (port b 0, 1)
     New_Read = (2 * (digitalRead(ENCODER_BIT_1)) + (digitalRead(ENCODER_BIT_0))) ;
     Check_Direction  = 4*Old_Read + New_Read ; // x4 = 2 rotate left 
     
    //Serial.println (Check_Direction, DEC) ;
     switch (Check_Direction)
     {
     case 0: 
     break;
     case 5:
     break;
     case 10:
     break;
     case 15:
     break;
     case 1:
     temp_return = 1;
     break;
     case 7:
     temp_return = 1;
     break;
     case 14:
     temp_return = 1;
     break;
     case 8:
     temp_return = 1; 
     break;
     case 4:
     temp_return = -1;
     break;
     case 13:
     temp_return = -1;
     break;
     case 11:
     temp_return = -1;
     break;
     case 2:
     temp_return = -1;
     break;
     default:
     Eror_counter = Eror_counter+1;//for test only (or not...)
     break;
    }
  return temp_return;  
}

#endif