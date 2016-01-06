
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

//char code[5] = { '*','*','*','*','\0'};
String code = "****";
String secret = "1324";
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;
const int pin1 = 4;
const int pin2 = 5;
const int pin3 = 6;
const int pin4 = 7;
int s1 = 0 ;
int s2 = 0 ;
int s3 = 0 ;
int s4 = 0 ;
bool l1 = false;
bool l2  = false;
bool l3  = false;
bool l4  = false;
bool unlocked = false;
bool reset    = true;

void setUnlock(int p){
  if(code[0] == '*')
    code[0] = String(p)[0];
  else if (code[1] == '*')
    code[1] = String(p)[0];
  else if (code[2] == '*')
    code[2] = String(p)[0];
  else if (code[3] == '*')
    code[3] = String(p)[0];  

}

void checkLock(){
  for (int i = 0 ; i < 4; i++){
   if(code[i] == '*')
      return; 
  }
  
  if(code[0] == secret[0] &&
     code[1] == secret[1] &&
     code[2] == secret[2] &&
     code[3] == secret[3]
 
  ){
   delay(500);
   unlocked = true; 
  }
  
 
  lcd.print(code);
  if(unlocked)
    lcd.print(" - Unlocked");
  else
    lcd.print(" - Wrong PIN");
    
  for (int i = 0 ; i < 20 ; i++){
    
  for(int j = 0 ; j < 10 ; j++){
    lcd.setRGB(rand()%254,rand()%254,rand()%254);
    delay(10);
   }
   delay(100);
   resetLock(); 
  }
 
 
}
void resetLock(){
 //code[0] = code[1] = code[2] = code[3] = '*';
 code = "****";
 reset = true;
 l1 = l2 = l3 = l4 = false;
 unlocked = false; 
}

void setup() 
{
  Serial.begin(9600);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);
  pinMode(pin4, INPUT);
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(rand()%254,rand()%254,rand()%254);
    
    // Print a message to the LCD.
    resetLock();
    delay(1000);
}

void loop() 
{
   s1 = digitalRead(pin1);
   s2 = digitalRead(pin2);
   s3 = digitalRead(pin3);
   s4 = digitalRead(pin4);
   if(s1 == 1){
    //lcd.print("1");
    setUnlock(1);
   }
      if(s2 == 1){
    //lcd.print("2");
 setUnlock(2);  
 }
      if(s3 == 1){
   ///lcd.print("3");
 setUnlock(3);  
 
 }
   if(s4 == 1){
   //lcd.print("4");
   setUnlock(4);
   }
   
   delay(100);
   char buff[5];
   code.toCharArray(buff,5);
   Serial.write(buff);
   Serial.write("\n");
   
   checkLock();
   lcd.print(code);
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    lcd.display();
    delay(500);
    lcd.clear();
    //lcd.setRGB(rand()%254,rand()%254,rand()%254);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
