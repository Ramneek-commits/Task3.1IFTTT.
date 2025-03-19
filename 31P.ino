#include <WiFiNINA.h>  // Include the WiFiNINA library for the Arduino Nano 33 IoT
#include <WiFiUdp.h>    // For UDP communication (optional)

// Pin setup
const int lightSensorPin = A4;  // LDR connected to A4 pin (Light Sensor)

// Wi-Fi credentials
const char* ssid = "Ramneek";  
const char* password = "f9f2prbh";  

// IFTTT Event and Key (Replace these with your values)
const String IFTTT_EVENT_NAME = "SUNLIGHT%20START";  // Your IFTTT event name (no spaces)
const String IFTTT_KEY = "IBQVnOimtzVxgwR5WbNgi";  // 

void setup() {
  Serial.begin(115200);
  pinMode(lightSensorPin, INPUT);  // Set light sensor pin as input

  // Connect to Wi-Fi with a timeout (5 minutes in this case)
  unsigned long startMillis = millis();
  unsigned long timeout = 300000;  // 5 minutes timeout
  WiFi.begin(ssid, password);

  // Print Wi-Fi status for debugging
  Serial.print("Wi-Fi Status: ");
  Serial.println(WiFi.status());  // Initial status check before connecting

  // Wait until connected or timeout occurs
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startMillis >= timeout) {
      Serial.println("Failed to connect to WiFi within the timeout period.");
      return;  // Exit if Wi-Fi connection is not successful after timeout
    }
    delay(500);
    Serial.print(".");
    Serial.print("Status Code: ");
    Serial.println(WiFi.status());  // Print Wi-Fi status code for debugging
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  int lightLevel = analogRead(lightSensorPin);  // Read the light level from the sensor
  Serial.println(lightLevel);  // Print the light level for debugging

  // Set the threshold value for sunlight detection (tune this value)
  int threshold = 700;  // Example threshold for sunlight intensity (you may need to adjust this)

  // Check if the light level is above or below the threshold
  if (lightLevel > threshold) {
    sendIFTTTNotification("SUNLIGHT%20START");  // Trigger IFTTT event for sunlight start
    delay(60000); // Delay for 1 minute to prevent excessive requests
  } else {
    sendIFTTTNotification("sunlight_stop");  // Trigger IFTTT event for sunlight stop
    delay(60000); // Delay for 1 minute to prevent excessive requests
  }
}

// Function to send data to IFTTT Webhook
void sendIFTTTNotification(String eventName) {
  WiFiClient client;
  const char* host = "maker.ifttt.com";
  String url = "/trigger/" + eventName + "/with/key/" + IFTTT_KEY;  // Ensure event name matches

  // Connect to the IFTTT server
  if (client.connect(host, 80)) {
    // Send the HTTP request
    client.print("GET " + url + " HTTP/1.1\r\n");
    client.print("Host: maker.ifttt.com\r\n");
    client.print("Connection: close\r\n\r\n");
    
    // Wait for the response from the server
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);  // Print the server response (for debugging)
    }
  } else {
    Serial.println("Connection failed");
  }

  client.stop();  // Close the connection
}
