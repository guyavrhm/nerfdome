#include <Servo.h>

const int MOTOR1_B_PIN = 13;
const int MOTOR1_F_PIN = 12;
const int MOTOR1_SPEED_PIN = 11;
const int MOTOR2_SPEED_PIN = 10;
const int SERVO_LOAD_PIN = 9;
const int MOTOR2_B_PIN = 8;
const int MOTOR2_F_PIN = 7;
const int SERVO_PITCH_PIN = 6;
const int SERVO_YAW_PIN = 5;

const int POT_PIN_PITCH = A0;
const int POT_PIN_YAW = A1;

const int LOAD_ANGLE = 20;

Servo load;
Servo pitch;
Servo yaw;

int lastLoadTime = 0;
int loadInterval = 150;
int loadCurrentAngle = 0;

void motorControl(int motor, int direction, int speed = 255) {
  int motorFPin = (motor == 1) ? MOTOR1_F_PIN : MOTOR2_F_PIN;
  int motorBPin = (motor == 1) ? MOTOR1_B_PIN : MOTOR2_B_PIN;
  int motorSPin = (motor == 1) ? MOTOR1_SPEED_PIN : MOTOR2_SPEED_PIN;

  switch (direction) {
    case 0: // stop
      digitalWrite(motorFPin, LOW);
      digitalWrite(motorBPin, LOW);
      break;
    case 1: // forward
      digitalWrite(motorFPin, HIGH);
      digitalWrite(motorBPin, LOW);
      analogWrite(motorSPin, speed);
      break;
    case -1: // backward
      digitalWrite(motorFPin, LOW);
      digitalWrite(motorBPin, HIGH);
      analogWrite(motorSPin, speed);
      break;
    default:
      // Invalid direction
      break;
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(MOTOR1_B_PIN, OUTPUT);
  pinMode(MOTOR1_F_PIN, OUTPUT);
  pinMode(MOTOR2_B_PIN, OUTPUT);
  pinMode(MOTOR2_F_PIN, OUTPUT);
  pinMode(MOTOR1_SPEED_PIN, OUTPUT);
  pinMode(MOTOR2_SPEED_PIN, OUTPUT);

  load.attach(SERVO_LOAD_PIN);
  pitch.attach(SERVO_PITCH_PIN);
  yaw.attach(SERVO_YAW_PIN);

  load.write(0);
  pitch.write(0);
  yaw.write(0);

  motorControl(1, -1);
  motorControl(2, 1);
}


void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastLoadTime >= loadInterval) {
    loadCurrentAngle = (loadCurrentAngle == 0) ? LOAD_ANGLE : 0;

    load.write(loadCurrentAngle);
    lastLoadTime = currentTime;
  }

  int servoAnglePitch = map(analogRead(POT_PIN_PITCH), 0, 1023, 0, 180);
  int servoAngleYaw = map(analogRead(POT_PIN_YAW), 0, 1023, 0, 180);

  pitch.write(servoAnglePitch);
  yaw.write(servoAngleYaw);

  delay(15);
}
