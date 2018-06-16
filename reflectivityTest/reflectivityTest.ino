#include <QTRSensors.h>

const int LEDpin = 13;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, HIGH);

  int i;
  for ( i = 0; i < 250 ; i++ ) {
    qtr.calibrate();
    delay(20);
  }

  Serial.print(qtr.calibratedMinimumOn[i]);
  Serial.print(' ');
  Serial.print(qtr.calibratedMaximumOn[i]);
  Serial.print(' ');

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int sensors[0];
  
  int position = qtr.readLine(sensors);

  Serial.println(position);
}

