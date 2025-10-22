#include <TinyGPSPlus.h>

TinyGPSPlus gps;
#define gpsSerial Serial2  // Use Serial2 for GPS communication

unsigned long lastUpdate = 0;  // for 1-second timing

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17
  Serial.println("Waiting for GPS fix and satellites...");
}

void loop() {
  // Feed incoming GPS data into the parser
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // Update display every 1 second (1000 ms)
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    if (gps.location.isUpdated()) {
      displayLocationInfo();
    }
  }

  // Safety check: if no GPS data is received in 5 seconds
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true); // Stop execution
  }
}

void displayLocationInfo() {
  Serial.println(F("-------------------------------------"));
  Serial.println("\n Location Info:");

  Serial.print("Latitude:  ");
  Serial.print(gps.location.lat(), 6);
  Serial.print(" ");
  Serial.println(gps.location.rawLat().negative ? "S" : "N");

  Serial.print("Longitude: ");
  Serial.print(gps.location.lng(), 6);
  Serial.print(" ");
  Serial.println(gps.location.rawLng().negative ? "W" : "E");

  // 🗺️ Print Google Maps Link
  if (gps.location.isValid()) {
    Serial.print("Google Maps Link: ");
    Serial.print("https://www.google.com/maps/place/");
    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println("Google Maps Link: Invalid (No Fix Yet)");
  }

  Serial.print("Fix Quality: ");
  Serial.println(gps.location.isValid() ? "Valid" : "Invalid");

  Serial.print("Satellites: ");
  Serial.println(gps.satellites.value());

  Serial.print("Altitude:   ");
  Serial.print(gps.altitude.meters());
  Serial.println(" m");

  Serial.print("Speed:      ");
  Serial.print(gps.speed.kmph());
  Serial.println(" km/h");

  Serial.print("Course:     ");
  Serial.print(gps.course.deg());
  Serial.println("°");

  Serial.print("Date:       ");
  if (gps.date.isValid()) {
    Serial.printf("%02d/%02d/%04d\n", gps.date.day(), gps.date.month(), gps.date.year());
  } else {
    Serial.println("Invalid");
  }

  Serial.print("Time (UTC): ");
  if (gps.time.isValid()) {
    // ✅ Proper time format: HH:MM:SS
    Serial.printf("%02d:%02d:%02d\n", gps.time.hour(), gps.time.minute(), gps.time.second());
  } else {
    Serial.println("Invalid");
  }

  Serial.println(F("-------------------------------------"));
}
