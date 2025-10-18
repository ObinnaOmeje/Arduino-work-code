const int sensorPin = 34;        // ADC input (Analog to Digital converter)
const float Vcc = 5.0;           // ACS758 supply voltage
const float adcMax = 4095.0;     // 12-bit ADC (ESP32 ADC range)
const float sensitivity = 0.020; // 20mV/A (model dependent)
const float QUI = 0.5;           // Quiescent Constant (bidirectional model)
float voltageDividerRatio = (10.0 + 20.0) / 20.0; // 1.5 voltage divider ratio
float QOV = (Vcc * QUI);         // Quiescent Output Voltage (2.5V at 5V supply)
float cutOffLimit = 0.20;        // 0.2A threshold for noise filtering
float cutOff = sensitivity * cutOffLimit;

const int sampleCount = 1000;    // Number of samples per RMS cycle
const int sampleDelay = 1;       // Delay between samples in ms (~1kHz sampling)

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ACS758 AC/DC RMS Current Measurement");
  Serial.println("----------------------------------");
}

void loop() {
  float sumOfSquares = 0.0;

  // Take multiple samples to calculate RMS
  for (int i = 0; i < sampleCount; i++) {
    int adcValue = analogRead(sensorPin);
    float sensorVoltage_adc = (adcValue / adcMax) * 3.3;             // voltage at ADC pin
    float actualVoltage = sensorVoltage_adc * voltageDividerRatio;   // corrected voltage
    float voltage = actualVoltage - QOV + 0.353;                     // zero-offset correction
    float current = voltage / sensitivity;                           // convert to current (A)
    sumOfSquares += current * current;
    delay(sampleDelay);
  }

  // Compute RMS current
  float rmsCurrent = sqrt(sumOfSquares / sampleCount);
  float offSetCurrent = rmsCurrent - 3;

  // Display only when current exceeds cutoff
  if (rmsCurrent > cutOffLimit) {
    Serial.print("RMS Current: ");
    Serial.print(rmsCurrent, 3);
    Serial.print(" _ OffSet Current: ");
    Serial.print(offSetCurrent, 3);
    Serial.println(" A");
  } else {
    Serial.println("No Significant Current Detected");
  }

  delay(500); // Small pause before next RMS block
}