// Code integrated from http://mertarduinotutorial.blogspot.com/2016/12/arduino-project-tutorial-12-line.html

#include <Servo.h> // Library for the use of the servo motors. 
#include <QTRSensors.h> //Pololu QTR Sensor Library. First you must download and install QTRSensors library
  
Servo leftServo
Servo rightServo
  
#define KP 2 //experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define KD 5 //experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define SL_minumum_speed 150  //minimum speed of the leftServo
#define SR_minumum_speed 150  //minimum speed of the rightServo
#define SL_maksimum_speed 250 //max. speed of the leftServo
#define SR_maksimum_speed 250 //max. speed of the rightServo
#define MIDDLE_SENSOR 4       //number of middle sensor used
#define NUM_SENSORS 5         //number of sensors used
#define TIMEOUT 2500          //waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN 0         //emitterPin is the Arduino digital pin that controls whether the IR LEDs are on or off. Emitter is controlled by digital pin 0
#define DEBUG 0

//sensors 1 through 7 are connected to analog inputs 0 through 5, respectively
//we are not using sensor 0 or sensor 8.  We don't have enough analog pins!
QTRSensorsAnalog qtra((unsigned char[]) { 0, 1, 2, 3, 4, 5} ,NUM_SENSORS, NUM_SAMPLES_PERSENSOR, EMITTER_PIN);
  
unsigned int sensorValues[NUM_SENSORS];
  
void setup()
  {
    delay(1500);
    manual_calibration();
    set_servos(0,0);
  }
  
int lastError = 0;
int last_proportional = 0;
int integral = 0;
  
void loop()
  {
    unsigned int sensors[5];
    int position = qtrrc.readLine(sensors); //get calibrated readings along with the line position, refer to the QTR Sensors Arduino Library for more details on line position.
    int error = position - 2000;
  
    int motorSpeed = KP * error + KD * (error - lastError);
    lastError = error;
  
    int leftMotorSpeed = M1_minumum_speed + motorSpeed;
    int rightMotorSpeed = M2_minumum_speed - motorSpeed;
  
    // set motor speeds using the two motor speed variables above
    set_motors(leftMotorSpeed, rightMotorSpeed);
  }
  
void set_motors(int motor1speed, int motor2speed)
  {
    if (motor1speed > M1_maksimum_speed ) motor1speed = M1_maksimum_speed;
    if (motor2speed > M2_maksimum_speed ) motor2speed = M2_maksimum_speed;
    if (motor1speed < 0) motor1speed = 0; 
    if (motor2speed < 0) motor2speed = 0; 
    motor1.setSpeed(motor1speed); 
    motor2.setSpeed(motor2speed);
    motor1.run(FORWARD); 
    motor2.run(FORWARD);
  }

//calibrate for sometime by sliding the sensors across the line, or you may use auto-calibration instead
void manual_calibration() 
  {
  
    int i;
    for (i = 0; i < 250; i++)
      {
        qtrrc.calibrate(QTR_EMITTERS_ON);
        delay(20);
      }
  
    if (DEBUG) {
    Serial.begin(9600);
    for (int i = 0; i < NUM_SENSORS; i++)
      {
        Serial.print(qtrrc.calibratedMinimumOn[i]);
        Serial.print(' ');
      }
    Serial.println();
  
    for (int i = 0; i < NUM_SENSORS; i++)
      {
        Serial.print(qtrrc.calibratedMaximumOn[i]);
        Serial.print(' ');
      }
    Serial.println();
    Serial.println();
  }
}
