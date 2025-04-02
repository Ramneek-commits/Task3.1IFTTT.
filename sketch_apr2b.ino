// Pin Definitions
const int buttonPin = 2;        // Pin for the push button (interrupt on D2)
const int led1Pin = 13;         // Pin for LED1 (toggled by button)
const int led2Pin = 12;         // Pin for LED2 (toggled by sensor)
const int trigPin = 3;          // Trigger pin for the ultrasonic sensor
const int echoPin = 4;          // Echo pin for the ultrasonic sensor

// Variables for interrupts
volatile bool buttonPressed = false;  // Flag for button interrupt

// Interrupt Service Routine (ISR) for Button Press
void buttonInterrupt() {
  buttonPressed = true;  // Set flag to toggle LED1
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Pin modes
  pinMode(buttonPin, INPUT_PULLUP);  // Internal pull-up resistor for the button
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  
  // Initialize the ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach interrupt to the push button (D2)
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING);  // Trigger on FALLING edge (button press)
  
  // Start with both LEDs off
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
}

void loop() {
  // Handle the button press interrupt (toggle LED1)
  if (buttonPressed) {
    digitalWrite(led1Pin, !digitalRead(led1Pin)); // Toggle LED1
    buttonPressed = false;  // Reset the flag
    Serial.println("Button pressed: LED1 toggled");
  }

  // Handle the sensor (HC-SR04) polling in the main loop
  long duration, distance;

  // Send a pulse to the Trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  // Wait for 2ms to clear the Trigger pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Send a 10ms pulse
  digitalWrite(trigPin, LOW);

  // Read the pulse duration from the Echo pin
  duration = pulseIn(echoPin, HIGH); // Duration of the pulse in microseconds

  // Ensure pulse duration is valid
  if (duration == 0) {
    Serial.println("No pulse received");
  } else {
    // Calculate distance in cm (speed of sound = 343 m/s or 29.1 microseconds per cm)
    distance = (duration / 2) / 29.1;  // Divide by 2 because pulse travels to the object and back

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Toggle LED2 if object detected within 10 cm
    if (distance < 10) {
      digitalWrite(led2Pin, !digitalRead(led2Pin)); // Toggle LED2
      Serial.print("Object detected at: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
  }
}
