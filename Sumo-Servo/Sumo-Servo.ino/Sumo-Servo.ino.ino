#include <Servo.h>
#include <QTRSensors.h>

// Behavior indicator LED
// This tells us when to calibrate, when the robot is ready to run and that it's running it's program
const int LEDpin = 13;

// Distance sensor connections
const int frontEchoPin = 11;
const int frontTriggerPin = 10;

// Distance sensor settings
volatile float maxFrontDistance = 5.00;
volatile float frontDuration, frontDistanceCm;

// Servo instantiation, connections and settings
Servo leftServo;
Servo rightServo;

const int leftServoPin = 9;        // continuous rotation servo
const int rightServoPin = 8;      // continuous rotation servo

// how fast do you want your motors to go? 0-89 forward, 90 stop, 91-180 reverse.
int forward = 95;
int attackSpeedRight = 0;
int retreatSpeedRight = 180;
int attackSpeedLeft = 180;
int retreatSpeedLeft = 0;
int back = 85;
int stop = 90;

// Reflectivity sensor connection and settings
// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
#define NUM_SENSORS             6  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             2  // emitter is controlled by digital pin 2
#define R_THRESHOLD           950  // The threshold beyond which the sensor reads a line

QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3, 4, 5}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

unsigned int sensorMinAvg;
unsigned int sensorMaxAvg;
unsigned int sensorMinSum;
unsigned int sensorMaxSum;

void setup() {
  Serial.begin(9600); // make sure we can see the logs in the serial monitor

  // reflectivity sensor calibration
  delay(500);
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
    {
      qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
    }
  digitalWrite(LEDpin, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMinimumOn[i]);
    Serial.print(' ');
    sensorMinSum += qtra.calibratedMinimumOn[i];

  }

  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMaximumOn[i]);
    Serial.print(' ');
    sensorMaxSum += qtra.calibratedMaximumOn[i];

  }

  // Ultrasonic sensor configuration
  pinMode(frontTriggerPin, OUTPUT);
  pinMode(frontEchoPin, INPUT);

  // Servo configuration
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);

  // ensure the servos are stopped
  leftServo.write(stop);
  rightServo.write(stop);
  
  delay(5000);
}

void loop() {
  // Let everyone know that we're running the loop
  digitalWrite(LEDpin, HIGH);

  //  ------------------ Line Following -----------
  // uncomment if using line following
  // check line values
  //  unsigned int position = qtra.readLine(sensorValues);

  // prints the weighted position of the line.
  // when 0, the line is to the right of the robot
  // when 5000, the line is to the left of the robot
  // any number in between, the robot is on the line over one of the sensors
  // 
  //  Serial.println(position);

  //  while ( 0 < position < 5000)  {
  //      moveForward();
  //  }
  //  ------------------- End Line Following --------

  //  ------------------- Sumo ----------------------
  // uncomment if using Sumo
  
  // check line values
  qtra.readCalibrated(sensorValues);
  unsigned int s0 = sensorValues[0];
  unsigned int s1 = sensorValues[1];
  unsigned int s2 = sensorValues[2];
  unsigned int s3 = sensorValues[3];
  unsigned int s4 = sensorValues[4];
  unsigned int s5 = sensorValues[5];
       
  // prints the values from the sensor to determine the position of the line.
  // 
  Serial.print(s0);
  Serial.print(" ");
  Serial.print(s1);
  Serial.print(" ");
  Serial.print(s2);
  Serial.print(" ");
  Serial.print(s3);
  Serial.print(" ");
  Serial.print(s4);
  Serial.print(" ");
  Serial.print(s5);
  Serial.println();

  if ( s0 > R_THRESHOLD || s1 > R_THRESHOLD || s2 > R_THRESHOLD || s3 > R_THRESHOLD || s4 > R_THRESHOLD || s5 > R_THRESHOLD) {
    // front distance check
    checkFrontDistance();
    if (frontDistanceCm < maxFrontDistance) {
      attack();
 
    } // goes with the if above
    else {
        Serial.println("Searching...");
        moveLeft();
        delay(500);

    } // goes with the else above
  } // end outer if
  else { 
    Serial.println("Line detected! Back Up!");
    moveBackward();
    //delay(500);
    //moveRight();
  }
  //  ----------------- End Sumo  --------------------
} // end void loop

void checkFrontDistance() {
  digitalWrite(frontTriggerPin, LOW);  
  delayMicroseconds(4);
  digitalWrite(frontTriggerPin, HIGH);  
  delayMicroseconds(10);
  digitalWrite(frontTriggerPin, LOW);
  frontDuration = pulseIn(frontEchoPin, HIGH);  // We measure the time between pulses, in microseconds
  frontDistanceCm = frontDuration * 10 / 292 / 2;  // Convert the distance to cm.
  Serial.print("                     Distance: ");
  Serial.print(frontDistanceCm);
  Serial.println(" cm");
}

// basic movement
void moveBackward() {
  Serial.println("Backward retreat!");
  leftServo.write(retreatSpeedLeft);
  rightServo.write(retreatSpeedRight);
  delay(500);
}

void moveForward() {
  Serial.println("Forward.");
  leftServo.write(forward);
  rightServo.write(back);
}

void moveLeft() {
  Serial.println("Left.");
  leftServo.write(back);
  rightServo.write(forward);
}

void moveRight() {
  Serial.println("Right.");
  leftServo.write(forward);
  rightServo.write(stop);
}

void moveStop() {
  Serial.println("Stopping.");
  leftServo.write(stop);
  rightServo.write(stop);
}

// Attack!!
void attack() {
  Serial.println("Attack!!!");
  leftServo.write(attackSpeedLeft);
  rightServo.write(attackSpeedRight);
}

