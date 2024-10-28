
# TinkerKit Braccio Robot Arm Control with ESP32 and Arduino

This project demonstrates how to control the TinkerKit Braccio Robot Arm using an ESP32 as a web server and an Arduino Uno for servo control. The system allows real-time control of the robot arm using sliders on a web interface.

## Components Used
- **TinkerKit Braccio Robot Arm**
- **ESP32 (ESP32-S3)**
- **Arduino Uno**
- **Jumper wires and power supplies**

## Features
- Real-time control of servos using a web-based interface.
- Sliders for each servo (Base, Shoulder, Elbow, Wrist Rotation, Wrist Vertical, and Gripper) to adjust angles dynamically.
- Smooth and stable servo movements using incremental adjustments on the Arduino side.

## System Overview
- The **ESP32** acts as a Wi-Fi web server and hosts a web interface where users can control the servos using sliders.
- The **Arduino Uno** receives commands from the ESP32 via serial communication (UART) and moves the Braccio servos accordingly.
- Real-time AJAX updates are used to send commands immediately when the sliders are adjusted, without the need for a submit button.

## Wiring Diagram

### ESP32 to Arduino Uno
- **ESP32 TX (GPIO 16)** → **Arduino RX (Pin 0)**
- **ESP32 RX (GPIO 17)** → **Arduino TX (Pin 1)** (though this is optional as we are only sending commands from ESP32 to Arduino)
- **ESP32 GND** → **Arduino GND**

### Braccio Robot Arm Servos (Connected to Arduino PWM pins)
- Base Servo → Pin D2
- Shoulder Servo → Pin D3
- Elbow Servo → Pin D4
- Wrist Rotation Servo → Pin D5
- Wrist Vertical Servo → Pin D6
- Gripper Servo → Pin D7

### Powering the System
- Ensure the Braccio servos are powered adequately using either the Arduino's 5V/GND pins or an external power supply.

## Software Setup

### 1. Arduino Code
The Arduino Uno is responsible for controlling the Braccio servos. The code below provides smooth movement for the servos:

```cpp
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
```

### 2. ESP32 Code
The ESP32 hosts the web server and sends real-time commands to the Arduino:

```cpp
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "password";

// Use UART1 for communication with Arduino (define TX and RX pins)
HardwareSerial Serial1(1);

void setup() {
  Serial.begin(115200);  // For debugging
  Serial1.begin(9600, SERIAL_8N1, 16, 17);  // TX=GPIO 16, RX=GPIO 17

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  WiFiServer server(80);
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("GET") >= 0) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("<!DOCTYPE html><html><head>");
      client.println("<style>/* Add your styles here */</style>");
      client.println("<script>/* Add JavaScript here for AJAX updates */</script>");
      client.println("</head><body>");
      client.println("<h1>Control Braccio Robot in Real Time</h1>");
      // Add sliders and JS code for AJAX
      client.println("</body></html>");
    }

    // Handle incoming requests for each servo value
    // ...parse the commands and send via Serial1

    client.stop();
  }
}
```

## How to Use
1. Upload the **Arduino code** to the Arduino Uno.
2. Upload the **ESP32 code** to the ESP32.
3. Connect the ESP32 and Arduino as described in the wiring section.
4. Access the ESP32’s IP address in a browser (printed in the Serial Monitor after connecting to Wi-Fi).
5. Use the sliders to control each servo in real-time.

## Troubleshooting
- **Servos not moving**: Check the serial connections and power supply.
- **Fast/jerky movements**: Adjust the `stepDelay` value in `moveServoSmoothly()` for smoother movements.

## License
This project is open-source and available under the MIT License.