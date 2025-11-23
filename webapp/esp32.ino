    #include <WiFi.h>

  // WiFi Access Point credentials
  const char* ssid = "BraccioRobot";
  const char* password = "12345678";

  // Declare the WiFi server on port 80
  WiFiServer server(80);

  void setup() {
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, 16, 17);  // Pins for TX/RX to Arduino

    // Create WiFi Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    
    // Print the IP address
    Serial.println("");
    Serial.println("WiFi Access Point created!");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());  // Print the ESP32 AP IP address (usually 192.168.4.1)

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
      if (request.indexOf("GET") >= 0 && request.indexOf("=") == -1) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        
        // HTML Header with meta viewport for responsiveness
        client.println("<!DOCTYPE html>");
        client.println("<html lang='en'>");
        client.println("<head>");
        client.println("<meta charset='UTF-8'>");
        client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
        client.println("<title>Braccio Robot Controller</title>");
        client.println("<style>");
        client.println("* { box-sizing: border-box; margin: 0; padding: 0; }");
        client.println("body {");
        client.println("  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;");
        client.println("  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);");
        client.println("  min-height: 100vh;");
        client.println("  padding: 20px;");
        client.println("  color: #333;");
        client.println("}");
        client.println(".container {");
        client.println("  max-width: 800px;");
        client.println("  margin: 0 auto;");
        client.println("  background: white;");
        client.println("  border-radius: 20px;");
        client.println("  box-shadow: 0 20px 60px rgba(0,0,0,0.3);");
        client.println("  overflow: hidden;");
        client.println("}");
        client.println(".header {");
        client.println("  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);");
        client.println("  color: white;");
        client.println("  padding: 30px 20px;");
        client.println("  text-align: center;");
        client.println("}");
        client.println(".header h1 {");
        client.println("  font-size: 2.5em;");
        client.println("  margin-bottom: 10px;");
        client.println("  text-shadow: 2px 2px 4px rgba(0,0,0,0.2);");
        client.println("}");
        client.println(".header p {");
        client.println("  font-size: 1.1em;");
        client.println("  opacity: 0.9;");
        client.println("}");
        client.println(".content {");
        client.println("  padding: 30px 20px;");
        client.println("}");
        client.println(".slider-card {");
        client.println("  background: #f8f9fa;");
        client.println("  border-radius: 15px;");
        client.println("  padding: 20px;");
        client.println("  margin-bottom: 20px;");
        client.println("  box-shadow: 0 2px 10px rgba(0,0,0,0.1);");
        client.println("  transition: transform 0.2s, box-shadow 0.2s;");
        client.println("}");
        client.println(".slider-card:hover {");
        client.println("  transform: translateY(-2px);");
        client.println("  box-shadow: 0 4px 15px rgba(0,0,0,0.15);");
        client.println("}");
        client.println(".slider-header {");
        client.println("  display: flex;");
        client.println("  justify-content: space-between;");
        client.println("  align-items: center;");
        client.println("  margin-bottom: 15px;");
        client.println("}");
        client.println(".slider-label {");
        client.println("  font-size: 1.2em;");
        client.println("  font-weight: 600;");
        client.println("  color: #495057;");
        client.println("}");
        client.println(".value-output {");
        client.println("  font-size: 1.5em;");
        client.println("  font-weight: bold;");
        client.println("  color: #667eea;");
        client.println("  background: white;");
        client.println("  padding: 8px 16px;");
        client.println("  border-radius: 10px;");
        client.println("  min-width: 60px;");
        client.println("  text-align: center;");
        client.println("  box-shadow: 0 2px 5px rgba(0,0,0,0.1);");
        client.println("}");
        client.println("input[type=range] {");
        client.println("  width: 100%;");
        client.println("  height: 10px;");
        client.println("  border-radius: 5px;");
        client.println("  background: #dee2e6;");
        client.println("  outline: none;");
        client.println("  -webkit-appearance: none;");
        client.println("}");
        client.println("input[type=range]::-webkit-slider-thumb {");
        client.println("  -webkit-appearance: none;");
        client.println("  appearance: none;");
        client.println("  width: 24px;");
        client.println("  height: 24px;");
        client.println("  border-radius: 50%;");
        client.println("  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);");
        client.println("  cursor: pointer;");
        client.println("  box-shadow: 0 2px 5px rgba(0,0,0,0.2);");
        client.println("}");
        client.println("input[type=range]::-moz-range-thumb {");
        client.println("  width: 24px;");
        client.println("  height: 24px;");
        client.println("  border-radius: 50%;");
        client.println("  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);");
        client.println("  cursor: pointer;");
        client.println("  border: none;");
        client.println("  box-shadow: 0 2px 5px rgba(0,0,0,0.2);");
        client.println("}");
        client.println("@media (max-width: 600px) {");
        client.println("  .header h1 { font-size: 2em; }");
        client.println("  .slider-label { font-size: 1em; }");
        client.println("  .value-output { font-size: 1.2em; padding: 6px 12px; }");
        client.println("  .content { padding: 20px 15px; }");
        client.println("}");
        client.println("</style>");
        client.println("<script>");
        client.println("var pageLoaded = false;");
        client.println("var lastSendTime = {};");
        client.println("var sendTimeout = {};");
        client.println("window.addEventListener('load', function() {");
        client.println("  setTimeout(function() { pageLoaded = true; }, 500);");
        client.println("});");
        client.println("function sendData(param, val) {");
        client.println("  if (!pageLoaded) return;");
        client.println("  var now = Date.now();");
        client.println("  var lastTime = lastSendTime[param] || 0;");
        client.println("  if (now - lastTime < 50) {");
        client.println("    clearTimeout(sendTimeout[param]);");
        client.println("    sendTimeout[param] = setTimeout(function() { sendDataImmediate(param, val); }, 50);");
        client.println("    return;");
        client.println("  }");
        client.println("  sendDataImmediate(param, val);");
        client.println("}");
        client.println("function sendDataImmediate(param, val) {");
        client.println("  lastSendTime[param] = Date.now();");
        client.println("  var xhr = new XMLHttpRequest();");
        client.println("  xhr.open('GET', '/' + param + '=' + val, true);");
        client.println("  xhr.send();");
        client.println("}");
        client.println("</script>");
        client.println("</head>");
        client.println("<body>");
        client.println("<div class='container'>");
        client.println("<div class='header'>");
        client.println("<h1>🤖 Braccio Robot Controller</h1>");
        client.println("<p>Real-time Servo Control Interface</p>");
        client.println("</div>");
        client.println("<div class='content'>");
        
        // Sliders for each servo angle with improved styling
        client.println("<div class='slider-card'>");
        client.println("<div class='slider-header'>");
        client.println("<span class='slider-label'>Base</span>");
        client.println("<output class='value-output'>90</output>");
        client.println("</div>");
        client.println("<input type='range' name='base' min='0' max='180' value='90' oninput='sendData(\"base\", this.value); this.parentElement.querySelector(\".value-output\").textContent = this.value'>");
        client.println("</div>");
        
        client.println("<div class='slider-card'>");
        client.println("<div class='slider-header'>");
        client.println("<span class='slider-label'>Shoulder</span>");
        client.println("<output class='value-output'>90</output>");
        client.println("</div>");
        client.println("<input type='range' name='shoulder' min='0' max='180' value='90' oninput='sendData(\"shoulder\", this.value); this.parentElement.querySelector(\".value-output\").textContent = this.value'>");
        client.println("</div>");
        
        client.println("<div class='slider-card'>");
        client.println("<div class='slider-header'>");
        client.println("<span class='slider-label'>Elbow</span>");
        client.println("<output class='value-output'>90</output>");
        client.println("</div>");
        client.println("<input type='range' name='elbow' min='0' max='180' value='90' oninput='sendData(\"elbow\", this.value); this.parentElement.querySelector(\".value-output\").textContent = this.value'>");
        client.println("</div>");
        
        client.println("<div class='slider-card'>");
        client.println("<div class='slider-header'>");
        client.println("<span class='slider-label'>Wrist Rotation</span>");
        client.println("<output class='value-output'>90</output>");
        client.println("</div>");
        client.println("<input type='range' name='wrist_rot' min='0' max='180' value='90' oninput='sendData(\"wrist_rot\", this.value); this.parentElement.querySelector(\".value-output\").textContent = this.value'>");
        client.println("</div>");
        
        client.println("<div class='slider-card'>");
        client.println("<div class='slider-header'>");
        client.println("<span class='slider-label'>Wrist Vertical</span>");
        client.println("<output class='value-output'>90</output>");
        client.println("</div>");
        client.println("<input type='range' name='wrist_ver' min='0' max='180' value='90' oninput='sendData(\"wrist_ver\", this.value); this.parentElement.querySelector(\".value-output\").textContent = this.value'>");
        client.println("</div>");
        
        client.println("<div class='slider-card'>");
        client.println("<div class='slider-header'>");
        client.println("<span class='slider-label'>Gripper</span>");
        client.println("<output class='value-output'>47</output>");
        client.println("</div>");
        client.println("<input type='range' name='gripper' min='20' max='75' value='47' oninput='sendData(\"gripper\", this.value); this.parentElement.querySelector(\".value-output\").textContent = this.value'>");
        client.println("</div>");
        
        client.println("</div>");
        client.println("</div>");
        client.println("</body>");
        client.println("</html>");
      }

      // Parse the GET request to extract angles and send commands
      bool commandReceived = false;
      if (request.indexOf("base=") != -1) {
        int base = extractValue(request, "base");
        if (base >= 0 && base <= 180) {
          sendServoCommand(base, "Base");
          commandReceived = true;
        }
      }
      if (request.indexOf("shoulder=") != -1) {
        int shoulder = extractValue(request, "shoulder");
        if (shoulder >= 0 && shoulder <= 180) {
          sendServoCommand(shoulder, "Shoulder");
          commandReceived = true;
        }
      }
      if (request.indexOf("elbow=") != -1) {
        int elbow = extractValue(request, "elbow");
        if (elbow >= 0 && elbow <= 180) {
          sendServoCommand(elbow, "Elbow");
          commandReceived = true;
        }
      }
      if (request.indexOf("wrist_rot=") != -1) {
        int wrist_rot = extractValue(request, "wrist_rot");
        if (wrist_rot >= 0 && wrist_rot <= 180) {
          sendServoCommand(wrist_rot, "Wrist Rotation");
          commandReceived = true;
        }
      }
      if (request.indexOf("wrist_ver=") != -1) {
        int wrist_ver = extractValue(request, "wrist_ver");
        if (wrist_ver >= 0 && wrist_ver <= 180) {
          sendServoCommand(wrist_ver, "Wrist Vertical");
          commandReceived = true;
        }
      }
      if (request.indexOf("gripper=") != -1) {
        int gripper = extractValue(request, "gripper");
        Serial.print("Gripper value extracted: ");
        Serial.println(gripper);
        if (gripper >= 20 && gripper <= 75) {
          sendServoCommand(gripper, "Gripper");
          commandReceived = true;
        } else {
          Serial.print("Gripper value out of range (20-75): ");
          Serial.println(gripper);
        }
      }

      // Send HTTP response for command requests
      if (commandReceived) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println("Connection: close");
        client.println();
        client.println("OK");
      }

      // Give the client time to receive the response
      delay(10);
      
      client.stop();
      Serial.println("Client disconnected");
    }
  }

  // Helper function to send servo command to Arduino
  void sendServoCommand(int value, String servoName) {
    String command = servoName + ":" + String(value) + "\n";
    Serial1.print(command);
    Serial1.flush();  // Ensure the command is sent completely
    String debugCommand = command;
    debugCommand.trim();
    Serial.println("Command sent to Arduino: " + debugCommand);
    delay(5);  // Small delay to ensure command is processed
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
