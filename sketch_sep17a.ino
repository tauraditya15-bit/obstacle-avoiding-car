#include <AFMotor.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // RX=10, TX=11 for HC-05

#define TRIG A1
#define ECHO A0

char command;
bool autoMode = false;

long duration;
int distance;

// Shield के हिसाब से motors define
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
  BT.begin(9600);

  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor3.setSpeed(150);
  motor4.setSpeed(150);
}

void loop() {
  if (BT.available()) {
    command = BT.read();
    Serial.println(command);

    if (command == 'A') {
      autoMode = true;  // Auto mode ON
    } else {
      autoMode = false; // Auto mode OFF if manual command received
      controlCar(command);
    }
  }

  if (autoMode) {
    distance = getDistance();
    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance > 20) {
      forward();
    } else {
      stopCar();
      delay(200);
      backward();
      delay(300);
      left();   // या right(); आप test करके बेहतर चुन सकते हो
      delay(500);
    }
  }
}

int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2; // Distance in cm
}

void controlCar(char cmd) {
  switch (cmd) {
    case 'F': forward(); break;
    case 'B': backward(); break;
    case 'L': left(); break;
    case 'R': right(); break;
    case 'S': stopCar(); break;
  }
}

void forward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void backward() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void left() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void right() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

void stopCar() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
