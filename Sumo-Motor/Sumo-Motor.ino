#include <QTRSensors.h>

// Behavior indicator LED
// This tells us when to calibrate, when the robot is ready to run and that it's running it's program
const int LEDpin = 7;

// Distance sensor connections
const int frontEchoPin = 9;
const int frontTriggerPin = 8;

// Distance sensor settings
volatile float maxFrontDistance = 12;
volatile float frontDuration, frontDistanceCm;

// digital write will receive the full 5v signal.  This may be too fast.
// use analog write to send a PWM signal with a value of 0 - 255.
const int left_wheel = 3;        // Green wire, IN1, pin 3
const int left_wheel_back = 6;   // Yellow wire, IN2, pin 6
const int right_wheel = 11;      // Blue wire, IN3, pin 11
const int right_wheel_back = 5;  // Purple wire, IN4, pin 5

// how fast do you want your motors to go? 0-255.
int forwardSpeed = 95;
int backSpeed = 85;
int attackSpeed = 195;
int stop = 0;

// Reflectivity sensor connection and settings
// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
// in our example, we're only using 4 sensors on A0-A3
// we are using sensors 1, 3, 6, and 8 on the QTR8A
#define NUM_SENSORS             4  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             2  // emitter is controlled by digital pin 2
#define R_THRESHOLD           900  // The threshold beyond which the sensor reads a line

QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3},
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];
unsigned int s0;
unsigned int s1;
unsigned int s2;
unsigned int s3;
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
  Serial.println("Calibrated Minimum Values:");
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMinimumOn[i]);
    Serial.print(' ');
    sensorMinSum += qtra.calibratedMinimumOn[i];

  }

  Serial.println();
  Serial.println();
  Serial.println("Calibrated Maximum Values:");

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

  // motors
  pinMode(left_wheel, OUTPUT);
  pinMode(left_wheel_back, OUTPUT);
  pinMode(right_wheel, OUTPUT);
  pinMode(right_wheel_back, OUTPUT);

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
  // not complete, more testing to be done.
  //  ------------------- End Line Following --------

  //  ------------------- Sumo ----------------------
  // front distance check
  checkFrontDistance();
  do {
    // sensor read
    qtra.readCalibrated(sensorValues);
    s0 = sensorValues[0];
    s1 = sensorValues[1];
    s2 = sensorValues[2];
    s3 = sensorValues[3];

    // prints the values from the sensor to determine the position of the line.
    //
    Serial.print(s0);
    Serial.print(" ");
    Serial.print(s1);
    Serial.print(" ");
    Serial.print(s2);
    Serial.print(" ");
    Serial.print(s3);
    Serial.println();

    Serial.println("Searching...");

    if (frontDistanceCm < maxFrontDistance) {
      attack();
    } else {
      moveForward();
    }

   } while (s0 > R_THRESHOLD || s3 > R_THRESHOLD);

   moveBackward();
   delay(500);
   moveRight();
   delay(400);



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
  Serial.println("Backward.");
  analogWrite(left_wheel_back, backSpeed);  // alogWrite values from 0 to 255
  analogWrite(right_wheel_back, backSpeed);
  analogWrite(left_wheel, stop);
  analogWrite(right_wheel, stop);
}

void moveForward() {
  Serial.println("Forward.");
  analogWrite(left_wheel, forwardSpeed);
  analogWrite(right_wheel, forwardSpeed);
  analogWrite(left_wheel_back, stop);
  analogWrite(right_wheel_back, stop);
}

void moveLeft() {
  Serial.println("Left.");
  analogWrite(left_wheel, stop);
  analogWrite(right_wheel, forwardSpeed);
  analogWrite(left_wheel_back, stop);
  analogWrite(right_wheel_back, stop);
}

void moveRight() {
  Serial.println("Right.");
  analogWrite(left_wheel, forwardSpeed);
  analogWrite(right_wheel, stop);
  analogWrite(left_wheel_back, stop);
  analogWrite(right_wheel_back, stop);
}

void moveStop() {
  Serial.println("Stopping.");
  analogWrite(left_wheel_back, stop);
  analogWrite(right_wheel_back, stop);
  analogWrite(left_wheel, stop);
  analogWrite(right_wheel, stop);
}

// Attack!!
void attack() {
  Serial.println("Attack!!!");
  analogWrite(left_wheel, attackSpeed);
  analogWrite(right_wheel, attackSpeed);
  analogWrite(left_wheel_back, stop);
  analogWrite(right_wheel_back, stop);
}
