#include <Servo.h>

// This code will be used to auto drive the audrino
const int triggerPin = 6;
const int echo = 5;

// vcc to 5v ultrasonic gnd to gnd Arduino
long duration;
int distance;

// Motor A
int enA = 9;
int in1 = 8;
int in2 = 7;

// Motor B
int enB = 3;
int in3 = 5;
int in4 = 4;

// Joystick Input
int joyVert = A0; // Vertical  
int joyHorz = A1; // Horizontal

// Motor Speed Values - Start at zero
int MotorSpeed1 = 0;
int MotorSpeed2 = 0;

// Joystick Values - Start at 512 (middle position)
int joyposVert = 512;
int joyposHorz = 512;

// Servo config
Servo servo_test; // initialize a servo object for the connected servo
int angle = 0;

// JPystick butoon In config
const int buttonPin = 2; // the pin for the button on the joystick
int buttonState = 0; // current state of the button
int lastButtonState = 0; // previous state of the button

// Direction constants
int leftservo = 0;
int rightservo = 0;
int frontservo = 0;

void setup() {
  pinMode(triggerPin, OUTPUT); // Sets the triggerPin as an Output
  pinMode(echo, INPUT); // Sets the echo as an Input
  pinMode(buttonPin, INPUT);

  servo_test.attach(11);

  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Start with motors disabled and direction forward

  // Motor A

  digitalWrite(enA, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  // Motor B

  digitalWrite(enB, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  Serial.begin(9600);
}

int getDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echo, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}
void loop() {
  buttonState = digitalRead(buttonPin);
  // put your main code here, to run repeatedly:
  // check if the button has been pressed
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      Serial.println("Button pressed!");
      while ( leftservo == 0 || rightservo == 0 || frontservo == 0 ) {
        servo_test.write(0);
        delayMicroseconds(30);
        int distfront = getDistance();
        if (distfront <= 20) {
          int frontservo = 1;
        }
        servo_test.write(90);
        delayMicroseconds(30);
        int distleft = getDistance();
        if (distleft <= 20) {
          int leftservo = 1;
        }
        servo_test.write(180);
        delayMicroseconds(30);
        int distright = getDistance();
        if ( distright <= 20) {
          int rightservo = 1;
        }
        if (frontservo = 'no') {
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          analogWrite(enA, 200);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          analogWrite(enB, 200);
          delay(2000); // to chekc how much delay travel how much distance
        }
        if ( leftservo == 'no') {
          // turn left
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          analogWrite(enA, 200);
          analogWrite(enB, 200);
          delay(2000); // to chekc how much delay travel how much distance
        }
        if ( rightservo == 'no') {
          // turn right
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);
          analogWrite(enA, 200);
          analogWrite(enB, 200);
          delay(2000); // to chekc how much delay travel how much distance
        }
       }
    }
    if ( leftservo == 'yes' && rightservo == 'yes' && frontservo == 'yes') {
      Serial.println("No way to move shifting to manual mode");
      lastButtonState = 1;
    } else {
      Serial.println("Button released!");
      // Read the Joystick X and Y positions

      joyposVert = analogRead(joyVert);
      joyposHorz = analogRead(joyHorz);

      // Determine if this is a forward or backward motion
      // Do this by reading the Verticle Value
      // Apply results to MotorSpeed and to Direction

      if (joyposVert < 460) {
        // This is Backward

        // Set Motor A backward

        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);

        // Set Motor B backward

        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);

        //Determine Motor Speeds

        // As we are going backwards we need to reverse readings

        joyposVert = joyposVert - 460; // This produces a negative number
        joyposVert = joyposVert * -1; // Make the number positive
        MotorSpeed1 = map(joyposVert, 0, 460, 0, 255);
        MotorSpeed2 = map(joyposVert, 0, 460, 0, 255);

      } else if (joyposVert > 564) {
        // This is Forward

        // Set Motor A forward

        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);

        // Set Motor B forward

        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);

        //Determine Motor Speeds

        MotorSpeed1 = map(joyposVert, 564, 1023, 0, 255);
        MotorSpeed2 = map(joyposVert, 564, 1023, 0, 255);

      } else {
        // This is Stopped

        MotorSpeed1 = 0;
        MotorSpeed2 = 0;

      }

      // Now do the steering
      // The Horizontal position will "weigh" the motor speed
      // Values for each motor

      if (joyposHorz < 460) {
        // Move Left

        // As we are going left we need to reverse readings

        joyposHorz = joyposHorz - 460; // This produces a negative number
        joyposVert = joyposVert * -1; // Make the number positive
        // Map the number to a value of 255 maximum

        joyposHorz = map(joyposHorz, 0, 460, 0, 255);

        MotorSpeed1 = MotorSpeed1 - joyposHorz;
        MotorSpeed2 = MotorSpeed2 + joyposHorz;

        // Don't exceed range of 0-255 for motor speeds

        if (MotorSpeed1 < 0) MotorSpeed1 = 0;
        if (MotorSpeed2 > 255) MotorSpeed2 = 255;

      } else if (joyposHorz > 564) {
        // Move Right

        // Map the number to a value of 255 maximum

        joyposHorz = map(joyposHorz, 564, 1023, 0, 255);

        MotorSpeed1 = MotorSpeed1 + joyposHorz;
        MotorSpeed2 = MotorSpeed2 - joyposHorz;

        // Don't exceed range of 0-255 for motor speeds

        if (MotorSpeed1 > 255) MotorSpeed1 = 255;
        if (MotorSpeed2 < 0) MotorSpeed2 = 0;

      }

      // Adjust to prevent "buzzing" at very low speed

      if (MotorSpeed1 < 8) MotorSpeed1 = 0;
      if (MotorSpeed2 < 8) MotorSpeed2 = 0;

      // Set the motor speeds

      analogWrite(enA, MotorSpeed1);
      analogWrite(enB, MotorSpeed2);
    }
  }
  // update the last button state
  lastButtonState = buttonState;
}