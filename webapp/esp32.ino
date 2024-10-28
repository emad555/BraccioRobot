#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "emad";
const char* password = "04112023";

// Declare the WiFi server on port 80
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 16, 17);  // Pins for TX/RX to Arduino

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Print the IP address once connected
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print the ESP32 IP address

  // Start the server
  server.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");

    // Read the client's request
    String request = client.readStringUntil('\r');
    client.flush();

    // Send the webpage to the client
    if (request.indexOf("GET") >= 0) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("<!DOCTYPE html><html><head>");
      client.println("<style>");
      client.println("body { font-family: Arial, sans-serif; margin: 50px; }");
      client.println("h1 { color: #333; }");
      client.println("input[type=range] { width: 80%; }");
      client.println(".slider-container { margin-bottom: 20px; }");
      client.println(".slider-label { font-size: 18px; color: #555; margin-right: 10px; }");
      client.println(".value-output { font-weight: bold; color: #000; }");
      client.println("</style>");
      client.println("<script>");
      // JavaScript for real-time sending of slider values via AJAX
      client.println("function sendData(param, val) {");
      client.println("  var xhr = new XMLHttpRequest();");
      client.println("  xhr.open('GET', '/' + param + '=' + val, true);");
      client.println("  xhr.send();");
      client.println("}");
      client.println("</script>");
      client.println("</head><body>");
      client.println("<h1>Control Braccio Robot in Real Time</h1>");
      
      // Sliders for each servo angle with oninput for real-time updates
      client.println("<div class='slider-container'><label class='slider-label'>Base: </label>");
      client.println("<input type='range' name='base' min='0' max='180' value='90' oninput='sendData(\"base\", this.value); this.nextElementSibling.value = this.value'><output class='value-output'>90</output></div>");
      
      client.println("<div class='slider-container'><label class='slider-label'>Shoulder: </label>");
      client.println("<input type='range' name='shoulder' min='0' max='180' value='45' oninput='sendData(\"shoulder\", this.value); this.nextElementSibling.value = this.value'><output class='value-output'>45</output></div>");
      
      client.println("<div class='slider-container'><label class='slider-label'>Elbow: </label>");
      client.println("<input type='range' name='elbow' min='0' max='180' value='180' oninput='sendData(\"elbow\", this.value); this.nextElementSibling.value = this.value'><output class='value-output'>180</output></div>");
      
      client.println("<div class='slider-container'><label class='slider-label'>Wrist Rotation: </label>");
      client.println("<input type='range' name='wrist_rot' min='0' max='180' value='90' oninput='sendData(\"wrist_rot\", this.value); this.nextElementSibling.value = this.value'><output class='value-output'>90</output></div>");
      
      client.println("<div class='slider-container'><label class='slider-label'>Wrist Vertical: </label>");
      client.println("<input type='range' name='wrist_ver' min='0' max='180' value='90' oninput='sendData(\"wrist_ver\", this.value); this.nextElementSibling.value = this.value'><output class='value-output'>90</output></div>");
      
      client.println("<div class='slider-container'><label class='slider-label'>Gripper: </label>");
      client.println("<input type='range' name='gripper' min='0' max='180' value='73' oninput='sendData(\"gripper\", this.value); this.nextElementSibling.value = this.value'><output class='value-output'>73</output></div>");
      
      client.println("</body></html>");
    }

    // Parse the GET request to extract angles
    if (request.indexOf("base=") != -1) {
      int base = extractValue(request, "base");
      sendServoCommand(base, "Base");
    }
    if (request.indexOf("shoulder=") != -1) {
      int shoulder = extractValue(request, "shoulder");
      sendServoCommand(shoulder, "Shoulder");
    }
    if (request.indexOf("elbow=") != -1) {
      int elbow = extractValue(request, "elbow");
      sendServoCommand(elbow, "Elbow");
    }
    if (request.indexOf("wrist_rot=") != -1) {
      int wrist_rot = extractValue(request, "wrist_rot");
      sendServoCommand(wrist_rot, "Wrist Rotation");
    }
    if (request.indexOf("wrist_ver=") != -1) {
      int wrist_ver = extractValue(request, "wrist_ver");
      sendServoCommand(wrist_ver, "Wrist Vertical");
    }
    if (request.indexOf("gripper=") != -1) {
      int gripper = extractValue(request, "gripper");
      sendServoCommand(gripper, "Gripper");
    }

    client.stop();
    Serial.println("Client disconnected");
  }
}

// Helper function to send servo command to Arduino
void sendServoCommand(int value, String servoName) {
  String command = servoName + ":" + String(value) + "\n";
  Serial1.print(command);
  Serial.println("Command sent to Arduino: " + command);
}

// Function to extract values from GET request
int extractValue(String request, String param) {
  int paramIndex = request.indexOf(param + "=");
  if (paramIndex != -1) {
    int valueStart = paramIndex + param.length() + 1;
    int valueEnd = request.indexOf('&', valueStart);
    if (valueEnd == -1) valueEnd = request.indexOf(' ', valueStart);
    return request.substring(valueStart, valueEnd).toInt();
  }
  return 0;  // Default value if not found
}
