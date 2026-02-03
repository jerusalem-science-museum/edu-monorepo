#ifndef Basic_Routines
#define Basic_Routines

#include "CONSTANTS.h"

//--------------------------Subroutines ----------------------------//


/////////////////////////////////////// 7SEG ////////////////////////////////
// send digit to 4511 
void Display_Digit(int digit_to_show) { 
    digitalWrite(BCD_A, LOW);// asume bit zero 
    if ((digit_to_show & 1) == 1) {digitalWrite(BCD_A, HIGH);}
    digitalWrite(BCD_B, LOW);// asume bit zero 
    if ((digit_to_show & 2) == 2) {digitalWrite(BCD_B, HIGH);}
    digitalWrite(BCD_C, LOW);// asume bit zero 
    if ((digit_to_show & 4) == 4) {digitalWrite(BCD_C, HIGH);}
    digitalWrite(BCD_D, LOW);// asume bit zero 
    if ((digit_to_show & 8) == 8) {digitalWrite(BCD_D, HIGH);}  
}

void enable(int digit_enable){
 switch(digit_enable) {
      case RIGHT_DIGIT: //range 0-9  '00000110'
       digitalWrite(LE_UNITS, LOW);
       digitalWrite(LE_UNITS, HIGH); 
       break;
      case MIDDLE_DIGIT: //range 9-99 '000000101'
       digitalWrite(LE_DOZENS, LOW);
       digitalWrite(LE_DOZENS, HIGH); 
       break;
      case LEFT_DIGIT: //range 100-999 '00000011'
       digitalWrite(LE_CENTURIES, LOW);
       digitalWrite(LE_CENTURIES, HIGH); 
      
       break;
    }
}


/***********/ 
// digtal from number extrude the 3 digits to display (dlobal variables) 
void Digits_from_Number(int in_number){
    Digit_3_To_Display = in_number / 100;// left digit to display 
    Digit_2_To_Display = (in_number - 100 * Digit_3_To_Display)/10;// mid digit to display 
    Digit_1_To_Display = in_number % 10 ;// right digit to display 

}
//************************
//Blank Digit (send FF)
void blank_Digit(int digit_enable){ //blank digit right to the left
  digitalWrite(BCD_A, HIGH);
  digitalWrite(BCD_B, HIGH);
  digitalWrite(BCD_C, HIGH);
  digitalWrite(BCD_D, HIGH);

  enable(digit_enable);

}
//************************
void blank_All_Digit(){
  for(int j=1;j <=  Number_OF_7SEG; j++){
         blank_Digit(j);
    }
}

// display full number 
void NUMBER_TO_DISPLAY(float variable){
  Digits_from_Number(variable);
  Display_Digit(Digit_1_To_Display);
  enable(RIGHT_DIGIT);
  
  if(variable < 10 ){
    blank_Digit(MIDDLE_DIGIT);
  }
  else{
    Display_Digit(Digit_2_To_Display);
    enable(MIDDLE_DIGIT);
  }
  
  if(variable < 100 ){
    blank_Digit(LEFT_DIGIT);
  }
  else{
    Display_Digit(Digit_3_To_Display);
    enable(LEFT_DIGIT);
  }
}
//////////////////////////////////////////////////////////////////////////////


// initial IO to be output and set to zero - may not nead
void Init_Output(int IO_Pin){
  digitalWrite(IO_Pin, LOW);/* try avoid high output */
  pinMode(IO_Pin, OUTPUT);
  digitalWrite(IO_Pin, LOW);/* make sure low  output */
}

bool PRESS_BUTTON() {
  // Check if the button is pressed
  if (digitalRead(SW_IN) == LOW && check_state == LOW) {
     //Serial.println("press :");
     check_state = HIGH;         // Mark that the button is being pressed
    delay(BOUNCE_TIME); // Apply debounce delay
  }

  // Check if the button is released
  if (digitalRead(SW_IN) == HIGH && check_state == HIGH) {
    //Serial.println("unpress");
    check_state = LOW;  // Reset the state for the next button press
    return HIGH;  // Indicate that the button was successfully pressed and released
  }
  return LOW; // Return false if the button is not in the desired state
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
  NUMBER_TO_DISPLAY(88*UNIT_CALIBRATION) ;
  while  (switch_state == LOW) {
     analogWrite(ELECTROD_PWM, pwm);
     trimmer_read = analogRead(SET_CURRENT_TRIMER_IN);
     pwm = map(trimmer_read,0,1024, MIN_PWM, MAX_PWM);
     NUMBER_TO_DISPLAY(pwm*UNIT_CALIBRATION);
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
      electrolysis_time = temp_time_to_show;
      temp_time_to_show = (Time_To_Count - temp_pass_time/1000) ;
      NUMBER_TO_DISPLAY (temp_time_to_show*UNIT_CALIBRATION);
      if (temp_time_to_show <=BLINK_TIME){
        delay (BLINK_ON);    
        blank_Digit(LEFT_DIGIT);
        blank_Digit(MIDDLE_DIGIT);
        delay (BLINK_OFF);
        }
    temp_pass_time = millis() - time_from_start;
    if (PRESS_BUTTON()) {break;} ; // exit count down if sw pressed 
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
     Error_counter = Error_counter+1;//for test only (or not...)
     break;
    }
  return temp_return;  
}



#endif