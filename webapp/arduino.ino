#include <Braccio.h>
#include <Servo.h>

// Declare servos
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

// Current angles for servos
int currentBase = 90;
int currentShoulder = 45;
int currentElbow = 180;
int currentWristRot = 90;
int currentWristVer = 90;
int currentGripper = 73;

// Function to move a servo smoothly from one position to another
void moveServoSmoothly(Servo &servo, int &currentPos, int targetPos, int stepDelay) {
  if (currentPos < targetPos) {
    for (int pos = currentPos; pos <= targetPos; pos++) {
      servo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = currentPos; pos >= targetPos; pos--) {
      servo.write(pos);
      delay(stepDelay);
    }
  }
  currentPos = targetPos;  // Update the current position
}

void setup() {
  // Initialize the servos
  Braccio.begin();
  
  // Initialize serial communication with ESP32
  Serial.begin(9600);

  // Move to initial position
  Braccio.ServoMovement(20, currentBase, currentShoulder, currentElbow, currentWristRot, currentWristVer, currentGripper);
  Serial.println("Ready to receive commands...");
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming string from ESP32
    String command = Serial.readStringUntil('\n');  // Read until newline
    Serial.print("Received command: ");
    Serial.println(command);  // Debug: print the received command
    
    // Parse the command and extract the servo and value
    int separatorIndex = command.indexOf(':');
    if (separatorIndex != -1) {
      String servoName = command.substring(0, separatorIndex);  // Extract servo name
      int servoValue = command.substring(separatorIndex + 1).toInt();  // Extract value

      // Move the appropriate servo to the desired value smoothly
      if (servoName == "Base") {
        moveServoSmoothly(base, currentBase, servoValue, 10);  // Adjust step delay for smoother movement
        Serial.println("Moving Base to " + String(servoValue));
      } else if (servoName == "Shoulder") {
        moveServoSmoothly(shoulder, currentShoulder, servoValue, 10);
        Serial.println("Moving Shoulder to " + String(servoValue));
      } else if (servoName == "Elbow") {
        moveServoSmoothly(elbow, currentElbow, servoValue, 10);
        Serial.println("Moving Elbow to " + String(servoValue));
      } else if (servoName == "Wrist Rotation") {
        moveServoSmoothly(wrist_rot, currentWristRot, servoValue, 10);
        Serial.println("Moving Wrist Rotation to " + String(servoValue));
      } else if (servoName == "Wrist Vertical") {
        moveServoSmoothly(wrist_ver, currentWristVer, servoValue, 10);
        Serial.println("Moving Wrist Vertical to " + String(servoValue));
      } else if (servoName == "Gripper") {
        moveServoSmoothly(gripper, currentGripper, servoValue, 10);
        Serial.println("Moving Gripper to " + String(servoValue));
      }
    }
  }
}
