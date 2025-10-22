// Define pins
const int sensorPin = 25;  // DO pin from vibration sensor
const int ledPin = 2;      // Onboard LED pin (you can change to another if you like)

void setup() {
  Serial.begin(115200);

  // Configure pins
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.println("Vibration sensor test started...");
}

void loop() {
  // Read sensor output (HIGH = vibration detected)
  bool vibrationDetected = digitalRead(sensorPin);

  if (vibrationDetected) {
    Serial.println("💥 Vibration detected!");
    digitalWrite(ledPin, HIGH);  // Turn LED ON
  } else {
    Serial.println("No vibration detected.");
    digitalWrite(ledPin, LOW);   // Turn LED OFF
  }

  delay(100); // Small delay to prevent flooding serial output
}
