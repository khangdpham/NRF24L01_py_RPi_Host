
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;
int CAPSENSE = 4;
String code = "****";
String secret = "1324";
const int colorR = 255;
const int colorG = 0;

const int colorB = 0;

const int pin1 = 4;
const int pin2 = 5;
const int pin3 = 6;
const int pin4 = 7;
uint8_t s1 = 0 ;
uint8_t s2 = 0 ;
uint8_t s3 = 0 ;
uint8_t s4 = 0 ;
bool l1 = false;
bool l2  = false;
bool l3  = false;
bool l4  = false;
bool unlocked = false;
bool reset    = true;


uint8_t readCapacitivePin(int pinToMeasure) {

  // Variables used to translate from Arduino to AVR pin naming

  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  uint8_t SREG_old = SREG; //back up the AVR Status Register
  // Prevent the timer IRQ from disturbing our measurement
  noInterrupts();
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.

  uint8_t cycles = 17;
  if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}
  else if (*pin & bitmask) { cycles =  4;}
  else if (*pin & bitmask) { cycles =  5;}
  else if (*pin & bitmask) { cycles =  6;}
  else if (*pin & bitmask) { cycles =  7;}
  else if (*pin & bitmask) { cycles =  8;}
  else if (*pin & bitmask) { cycles =  9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}
  // End of timing-critical section; turn interrupts back on if they were on before, or leave them off if they were off before
  SREG = SREG_old;

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.

  *port &= ~(bitmask);
  *ddr  |= bitmask;
  return cycles;
}
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
  /*
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);
  pinMode(pin4, INPUT);
  */
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(rand()%254,rand()%254,rand()%254);
    
    // Print a message to the LCD.
    resetLock();
    delay(1000);
}

void loop() 
{
   s1 = readCapacitivePin(pin1);
   s2 = readCapacitivePin(pin2);
   s3 = readCapacitivePin(pin3);
   s4 = readCapacitivePin(pin4);
   if(s1 > CAPSENSE )
    setUnlock(1);
   else if(s2 > CAPSENSE)
    setUnlock(2);  
   else if(s3 > CAPSENSE)
    setUnlock(3);  
   else if(s4 > CAPSENSE)
    setUnlock(4);
   Serial.print("Sense ");
   Serial.print(s1);Serial.print("\t");
   Serial.print(s2);Serial.print("\t");
   Serial.print(s3);Serial.print("\t");
   Serial.print(s4);Serial.println("\t");
   
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
