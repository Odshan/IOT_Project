#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <AFMotor.h>

AF_DCMotor motor1(1);  // for
AF_DCMotor motor2(2);  // back
AF_DCMotor motor3(3);  // for
AF_DCMotor motor4(4);  // back

bool driving = true;

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 9);  // RX, TX

void printResult(HUSKYLENSResult result);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  motor1.setSpeed(200);
  motor2.setSpeed(200);
  motor3.setSpeed(200);
  motor4.setSpeed(200);

  while (!huskylens.begin(mySerial)) {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
}

void loop() {
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if (!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if (!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
  else {
    Serial.println(F("###########"));
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      printResult(result);
    }
  }

  while (driving != true) {
    stopBitch();

    delay(3000);

    driving = true;
  }

  moveBitch();
}

void moveBitch() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

void stopBitch() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void printResult(HUSKYLENSResult result) {
  if (result.command == COMMAND_RETURN_BLOCK) {
    Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
    driving = false;
  } else if (result.command == COMMAND_RETURN_ARROW) {
    Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
  } else {
    Serial.println("Object unknown!");
  }
}