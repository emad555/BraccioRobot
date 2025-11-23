#include <Braccio.h>
#include <Servo.h>

// Declare servos (required by Braccio library)
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

// Current angles for servos
int currentBase = 90;
int currentShoulder = 90;
int currentElbow = 90;
int currentWristRot = 90;
int currentWristVer = 90;
int currentGripper = 90;

// Function to move servos using Braccio's coordinated movement
void moveServo(int servoIndex, int targetPos) {
  // Update the current position based on which servo is being moved
  switch(servoIndex) {
    case 0: currentBase = targetPos; break;
    case 1: currentShoulder = targetPos; break;
    case 2: currentElbow = targetPos; break;
    case 3: currentWristRot = targetPos; break;
    case 4: currentWristVer = targetPos; break;
    case 5: currentGripper = targetPos; break;
  }
  
  // Use Braccio's coordinated movement for stable, fast positioning
  // Speed parameter: lower = faster (minimum 10, typical 15-20 for smooth but fast)
  Braccio.ServoMovement(15, currentBase, currentShoulder, currentElbow, 
                        currentWristRot, currentWristVer, currentGripper);
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
  // Wait for data and ensure we have a complete line
  if (Serial.available() > 0) {
    // Wait a bit to ensure the full command has arrived (9600 baud = ~1ms per byte)
    delay(20);
    
    // Read all available data until newline
    String command = "";
    unsigned long startTime = millis();
    bool foundNewline = false;
    
    // Read until we find a newline or timeout
    while (millis() - startTime < 200 && !foundNewline) {
      if (Serial.available() > 0) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
          foundNewline = true;
          break;  // End of command
        }
        if (c >= 32 && c <= 126) {  // Only add printable ASCII characters
          command += c;
        }
      }
    }
    
    // If we didn't find a newline, clear the buffer to prevent corruption
    if (!foundNewline && Serial.available() > 0) {
      // Clear buffer until we find a newline or it's empty
      while (Serial.available() > 0) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
          break;
        }
      }
      // Don't process incomplete command
      return;
    }
    
    command.trim();  // Remove any whitespace/newlines
    
    // Only process if we have a non-empty command with valid format
    if (command.length() > 0 && command.indexOf(':') > 0) {
      Serial.print("Received command: ");
      Serial.println(command);  // Debug: print the received command
      
      // Parse the command and extract the servo and value
      int separatorIndex = command.indexOf(':');
      if (separatorIndex != -1 && separatorIndex > 0) {
        String servoName = command.substring(0, separatorIndex);  // Extract servo name
        servoName.trim();  // Remove whitespace
        String valueStr = command.substring(separatorIndex + 1);
        valueStr.trim();  // Remove whitespace
        
        // Validate that valueStr is not empty and contains only digits
        if (valueStr.length() > 0) {
          // Check if valueStr contains only digits
          bool isValidNumber = true;
          for (int i = 0; i < valueStr.length(); i++) {
            if (!isDigit(valueStr[i])) {
              isValidNumber = false;
              break;
            }
          }
          
          if (isValidNumber) {
            int servoValue = valueStr.toInt();  // Extract value

            // Only process if we got a valid value (not 0 unless explicitly 0, but check range)
            if (servoValue >= 0 && servoValue <= 180) {
              // Move the appropriate servo to the desired value directly and quickly
              if (servoName == "Base") {
                moveServo(0, servoValue);
                Serial.println("Moving Base to " + String(servoValue));
              } else if (servoName == "Shoulder") {
                moveServo(1, servoValue);
                Serial.println("Moving Shoulder to " + String(servoValue));
              } else if (servoName == "Elbow") {
                moveServo(2, servoValue);
                Serial.println("Moving Elbow to " + String(servoValue));
              } else if (servoName == "Wrist Rotation") {
                moveServo(3, servoValue);
                Serial.println("Moving Wrist Rotation to " + String(servoValue));
              } else if (servoName == "Wrist Vertical") {
                moveServo(4, servoValue);
                Serial.println("Moving Wrist Vertical to " + String(servoValue));
              } else if (servoName == "Gripper") {
                // Gripper range is 20-75
                if (servoValue >= 20 && servoValue <= 75) {
                  moveServo(5, servoValue);
                  Serial.println("Moving Gripper to " + String(servoValue));
                } else {
                  Serial.println("Gripper value out of range (20-75): " + String(servoValue));
                }
              } else {
                Serial.println("Unknown servo name: " + servoName);
              }
            } else {
              Serial.println("Invalid servo value (0-180): " + String(servoValue));
            }
          } else {
            Serial.println("Invalid number format in command: " + command);
          }
        } else {
          Serial.println("Empty value in command: " + command);
        }
      } else {
        Serial.println("Invalid command format (expected 'ServoName:value'): " + command);
      }
    } else if (command.length() > 0) {
      // Command exists but doesn't have proper format - ignore it
      Serial.print("Ignoring malformed command: ");
      Serial.println(command);
    }
  }
}
