// adapted for the Huzzah ESP8266 board from Adafruit
// and UDP over serial from Andrew Fisher
// adapted from Control 2 servo motors over Bluetooth using RoboRemo app
// www.roboremo.com

// Hardware setup:
// wifi module   Arduino
// GND ------- GND
// VBat ------- 5V
// TX-O ------ pin0
// RX-I ------ pin1

// Servos       Arduino
// GND ------- GND
// VCC ------- VCC
// signal1 ---- pin7
// signal2 ---- pin4

#define wifi Serial
#include <Servo.h>


Servo myServo1, myServo2;

char cmd[100];
int cmdIndex;



boolean cmdStartsWith(char *st) {
  for(int i=0; ; i++) {
    if(st[i]==0) return true;
    if(cmd[i]==0) return false;
    if(cmd[i]!=st[i]) return false;;
  }
  return false;
}



void exeCmd() {
  
  // "servo1" is the id for servo motor 1
  // "servo2" is the id for servo motor 2
  
  if( cmdStartsWith("servo1 ") ) { // example: if cmd is "servo1 180"
    int val = atoi(cmd+7);  // val will be 180
                            // cmd+7, because value comes after "servo1 " which is 7 characters
    myServo1.write(val);      
  } 
 
  if( cmdStartsWith("servo2 ") ) { 
    int val = atoi(cmd+7); 
    myServo2.write(180-val);      
  } 
  
}



void setup() {
  
  delay(500); // wait for wifi module to start

  wifi.begin(115200); // Huzzah default baud is 115200
  
  myServo1.attach(7, 1000, 2000);
  myServo2.attach(4, 1000, 2000);
  
  cmdIndex = 0;
}


void loop() {
  
  if(wifi.available()) {
    
    char c = (char)wifi.read();
    
    if(c=='\n') {
      cmd[cmdIndex] = 0;
      exeCmd();  // execute the command
      cmdIndex = 0; // reset the cmdIndex
    } else {      
      cmd[cmdIndex] = c;
      if(cmdIndex<99) cmdIndex++;
    }
   
    
  }
  
}
