#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>
#include <NewPing.h>
#include <Servo.h>
#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 200

Servo myservo;
Servo mop;
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int wforward = 8;     // left forward
int wbackward = 7;    // left backward
int w1forward = 4;    // right forward
int w1backward = 12;  // right backward
int led = 11;
bool tri = false;
int pinout123 = 6;
bool goesForward = false;
bool sev = false;
bool downb = false;
int distance = 100;
int val = 0;
bool last = false;

void setup() {
  // put your setup code here, to run once
  pinMode(A3, INPUT);
  pinMode(pinout123, OUTPUT);
  pinMode(wforward, OUTPUT);
  pinMode(wbackward, OUTPUT);
  pinMode(w1forward, OUTPUT);
  pinMode(w1backward, OUTPUT);
  pinMode(led, OUTPUT);

  // Serial.begin(115200);
  Dabble.begin(9600, 3, 2);

  mop.attach(10);
  mop.write(90);
  myservo.attach(9);
  myservo.write(80);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {
  if (analogRead(A3) >= 800) {
    tri = true;
  }
  if ((analogRead(A3) <= 800)&&(last == true)) {
    tri = true;
  }
  else{
    if ((analogRead(A3) <= 800)&&(last == false)){
      	tri = false;
    }
  }
  // if (analogRead(A3) >= 800){
  //   tri = true;
  // }else {
  //   tri=false;
  // }
  Dabble.processInput();
  // Serial.println("hello");
  digitalWrite(pinout123, LOW);
  digitalWrite(wforward, LOW);
  digitalWrite(wbackward, LOW);
  digitalWrite(w1backward, LOW);
  digitalWrite(w1forward, LOW);

  if (tri == false) {
    /////////////////
    if (GamePad.isUpPressed()) {
      // if (sev == true) {
      //   mop.write(30);
      //   sev = false;
      //   delay(500);
      // }
      digitalWrite(wforward, HIGH);
      digitalWrite(w1forward, HIGH);
      Serial.println("UP");
    }
    /////////////////
    if (GamePad.isDownPressed()) {
      // if (sev == true) {
      //   mop.write(30);
      //   sev = false;
      //   delay(500);
      // }
      digitalWrite(wbackward, HIGH);
      digitalWrite(w1backward, HIGH);
      Serial.println("DOWN");
    }
    /////////////////
    if (GamePad.isLeftPressed()) {
      // if (sev == false) {
      //   mop.write(30);
      //   sev = true;
      //   delay(500);
      // }
      digitalWrite(w1forward, HIGH);
      digitalWrite(wbackward, HIGH);
      Serial.println("Left");
    }
    /////////////////
    if (GamePad.isRightPressed()) {
      // if (sev == false) {
      //   mop.write(30);
      //   sev = true;
      //   delay(500);
      // }
      digitalWrite(w1backward, HIGH);
      digitalWrite(wforward, HIGH);
      Serial.println("Right");
    }
    /////////////////
    /////////////////
    /////////////////
    if (GamePad.isCirclePressed()) {
      digitalWrite(pinout123, HIGH);
      Serial.println("WATER PUMPING");
    }
    if (GamePad.isTrianglePressed()) {
      tri = true;
      last = true;
      // digitalWrite(led, HIGH);
    }
    if (GamePad.isStartPressed()) {
      mop.write(68);
      downb = true;
      // Serial.println("90");
    }
    if (GamePad.isSelectPressed()) {
      mop.write(90);
    }

  } else {

    if (downb == true) {
      mopdown();
      downb = false;
    }
    int distanceR = 0;
    int distanceL = 0;
    if (distance <= 15) {
      waterflowoff();
      moveStop();  // stop
      delay(100);
      moveBackward();  // go back a bit
      delay(300);
      moveStop();  // stop
      delay(200);
      distanceR = lookRight();  // look right
      delay(200);
      distanceL = lookLeft();  // look left
      delay(200);

      // compare right and left
      if (distanceR >= distanceL) {
        turnRight();
        moveStop();
      } else {
        turnLeft();
        moveStop();
      }
    } else {
      waterflow();
      moveForward();
      // digitalWrite(pinout123, uint8_t val)
    }
    distance = readPing();
    if (GamePad.isSquarePressed()) {
      tri = false;
      last = false;
      // digitalWrite(led, LOW);
    }
  }
}
///////////////////////////////////////
int lookRight() {
  myservo.write(0);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(80);
  return distance;
}

int lookLeft() {
  myservo.write(180);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(80);
  return distance;
  delay(100);
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}
///////////////////////////////////////

void moveStop() {
  digitalWrite(wforward, LOW);
  digitalWrite(wbackward, LOW);
  digitalWrite(w1backward, LOW);
  digitalWrite(w1forward, LOW);
}

void moveBackward() {
  digitalWrite(wbackward, HIGH);
  digitalWrite(w1backward, HIGH);
}

void turnRight() {
  mopup();
  delay(500);
  digitalWrite(w1backward, HIGH);
  digitalWrite(wforward, HIGH);
  delay(300);
  mopdown();
  delay(150);
  digitalWrite(wforward, HIGH);
  digitalWrite(w1forward, HIGH);
}

void turnLeft() {
  mopup();
  delay(500);
  digitalWrite(w1forward, HIGH);
  digitalWrite(wbackward, HIGH);
  delay(300);
  mopdown();
  delay(150);
  digitalWrite(wforward, HIGH);
  digitalWrite(w1forward, HIGH);
}

void moveForward() {
  digitalWrite(wforward, HIGH);
  digitalWrite(w1forward, HIGH);
}
/////////////////////////////////////////
void mopup() {
  mop.write(68);
}

void mopdown() {
  mop.write(90);
}
/////////////////////////////////////////
void waterflow() {
  digitalWrite(pinout123, HIGH);
}
void waterflowoff() {
  digitalWrite(pinout123, LOW);
}
