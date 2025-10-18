#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initializing DS3231 RTC...");

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, please set the time using command:");
    Serial.println("Example: SET 2025 10 16 13 50 00");
  }

  Serial.println("RTC Ready!");
  Serial.println("To set time, type:");
  Serial.println("SET YYYY MM DD HH MM SS");
  Serial.println("Example: SET 2025 10 16 13 50 00");
  Serial.println();
}

void loop() {
  // --- Display Current Time ---
  DateTime now = rtc.now();

  Serial.print("Date: ");
  Serial.print(now.year());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.day());

  Serial.print("  Time: ");
  if (now.hour() < 10) Serial.print('0');
  Serial.print(now.hour());
  Serial.print(':');
  if (now.minute() < 10) Serial.print('0');
  Serial.print(now.minute());
  Serial.print(':');
  if (now.second() < 10) Serial.print('0');
  Serial.println(now.second());

  // --- Listen for Serial Commands ---
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.startsWith("SET")) {
      int y, m, d, h, min, s;
      if (sscanf(input.c_str(), "SET %d %d %d %d %d %d", &y, &m, &d, &h, &min, &s) == 6) {
        rtc.adjust(DateTime(y, m, d, h, min, s));
        Serial.println("✅ Time updated successfully!");
      } else {
        Serial.println("⚠️ Invalid format! Use: SET YYYY MM DD HH MM SS");
      }
    }
  }

  delay(1000);
}
