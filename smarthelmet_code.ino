#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <DHT.h>

#define DHTPIN 27          // GPIO pin for DHT sensor
#define DHTTYPE DHT11    // Change to DHT22 if needed
#define TEMP_THRESHOLD 40.0
bool tempPrinted = false;
bool alcoholStatusPrinted = false;
bool sleepStatusPrinted = false;

DHT dht(DHTPIN, DHTTYPE);
bool highTempDetected = false;


// ---------- PIN DEFINITIONS ----------
#define MQ3_PIN 34           // MQ-3 alcohol sensor (analog)
#define SW420_PIN 25         // Vibration sensor (digital)
#define EYE_ANALOG_PIN 32    // Eye blink sensor (analog)
#define SAFETY_MARGIN 300
#define SAMPLE_COUNT 20

// ---------- OBJECTS ----------
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

// ---------- STATE / CALIBRATION ----------
int alcoholThreshold = 0;
bool alcoholDetected = false;
bool drowsinessDetected = false;
bool accidentDetected = false;

int EYE_CLOSED_THRESHOLD = 0;   // computed after calibration
const int EYE_MARGIN = 300;     // margin below open-eye average (tune if needed)
const int STABLE_CLOSE_COUNT = 10; // number of stable low readings before detection
int closeCount = 0;
bool eyeCalibrated = false;

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);
  pinMode(MQ3_PIN, INPUT);
  pinMode(SW420_PIN, INPUT);
  pinMode(EYE_ANALOG_PIN, INPUT);
  dht.begin();
  Serial.println("🌡️ Helmet Temperature Monitoring Enabled");


  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("\n🚗 SMART HELMET SYSTEM (Alcohol → Sleep → Accident → GPS)");
  Serial.println("--------------------------------------------------------");

  // ---------- MQ-3 CALIBRATION ----------
  Serial.println("Calibrating MQ-3 sensor in clean air...");
  long sum = 0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    sum += analogRead(MQ3_PIN);
    delay(150);
  }
  int baseline = sum / SAMPLE_COUNT;
  alcoholThreshold = baseline + SAFETY_MARGIN;
  Serial.print("MQ-3 Baseline: "); Serial.println(baseline);
  Serial.print("Alcohol Threshold: "); Serial.println(alcoholThreshold);

  // ---------- EYE SENSOR CALIBRATION ----------
  Serial.println();
  Serial.println("Eye Sensor Calibration: Please keep your EYES OPEN for 4 seconds...");
  delay(2000); // let user prepare

  long eyeSum = 0;
  const int CAL_SAMPLES = 40;
  for (int i = 0; i < CAL_SAMPLES; i++) {
    eyeSum += analogRead(EYE_ANALOG_PIN);
    delay(50);
  }
  int eyeOpenAvg = eyeSum / CAL_SAMPLES;
  EYE_CLOSED_THRESHOLD = eyeOpenAvg - EYE_MARGIN;
  if (EYE_CLOSED_THRESHOLD < 0) EYE_CLOSED_THRESHOLD = 0;

  Serial.print("Eye Open Average: "); Serial.println(eyeOpenAvg);
  Serial.print("Computed Eye Closed Threshold: "); Serial.println(EYE_CLOSED_THRESHOLD);
  Serial.println("Calibration complete. System ready.\n");

  eyeCalibrated = true;
  closeCount = 0;
  drowsinessDetected = false;
  alcoholDetected = false;
  accidentDetected = false;
}

// ---------- MAIN LOOP ----------
void loop() {
  detectAlcohol();          // Step 1
  if (!alcoholDetected) {   // Step 2
    detectSleep();
  }
  detectHelmetTemperature(); // 🌡️ NEW: Temperature check
  detectAccident();         // Step 3
  delay(100);
}

// ---------- ALCOHOL DETECTION ----------
void detectAlcohol() {
  int val = analogRead(MQ3_PIN);
  bool isDrunk = val > alcoholThreshold;

  // 🔹 PRINT STATUS ONCE AFTER CALIBRATION
  if (!alcoholStatusPrinted) {
    Serial.println("🍺 Alcohol Status Check (After Calibration)");
    Serial.print("🍺 MQ-3 Sensor Value: ");
    Serial.println(val);
    Serial.print("🍺 Alcohol Threshold: ");
    Serial.println(alcoholThreshold);

    if (isDrunk) {
      Serial.println("🚨 STATUS: RIDER IS DRUNK");
    } else {
      Serial.println("✅ STATUS: RIDER IS NOT DRUNK");
    }

    Serial.println("----------------------------------");
    alcoholStatusPrinted = true;
  }

  // 🔹 NORMAL STATE-CHANGE LOGIC
  if (isDrunk && !alcoholDetected) {
    alcoholDetected = true;
    Serial.println("🚨 Alcohol detected — rider is drunk!");
  }
  else if (!isDrunk && alcoholDetected) {
    alcoholDetected = false;
    Serial.println("✅ Rider is sober — alcohol-free.");
  }
}



// ---------- SLEEP / DROWSINESS DETECTION ----------
void detectSleep() {
  if (!eyeCalibrated) return;

  long t = 0;
  const int SAMPLES = 5;
  for (int i = 0; i < SAMPLES; i++) {
    t += analogRead(EYE_ANALOG_PIN);
    delay(5);
  }
  int sensorValue = t / SAMPLES;

  // 👁️ Eyes CLOSED
  if (sensorValue < EYE_CLOSED_THRESHOLD) {
    if (closeCount < STABLE_CLOSE_COUNT) closeCount++;

    if (closeCount >= STABLE_CLOSE_COUNT && !drowsinessDetected) {
      drowsinessDetected = true;
      sleepStatusPrinted = false; // allow printing

      Serial.println("😴 STATUS: RIDER IS DROWSY (Eyes Closed)");
    }
  }
  // 👁️ Eyes OPEN
  else {
    closeCount = 0;

    if (drowsinessDetected) {
      drowsinessDetected = false;
      sleepStatusPrinted = false; // allow printing
    }

    if (!sleepStatusPrinted) {
      Serial.println("👁️ STATUS: RIDER IS AWAKE (Not Drowsy)");
      sleepStatusPrinted = true;
    }
  }
}


// ---------- ACCIDENT DETECTION ----------
void detectAccident() {
  int vibrationState = digitalRead(SW420_PIN);
  if (vibrationState == HIGH && !accidentDetected) {
    accidentDetected = true;
    Serial.println("💥 Accident Detected!");
    getAccidentLocation();  // GPS only for accidents
  } else if (vibrationState == LOW && accidentDetected) {
    accidentDetected = false;
  }
}

// ---------- GPS LOCATION (only for accident) ----------
void getAccidentLocation() {
  Serial.println("📡 Getting GPS coordinates for accident...");
  unsigned long start = millis();
  bool found = false;

  while (millis() - start < 5000) { // wait up to 5 seconds
    while (gpsSerial.available() > 0) gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()) {
      Serial.print("Latitude: "); Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: "); Serial.println(gps.location.lng(), 6);
      found = true;
      break;
    }
  }

  if (!found) {
    Serial.println("⚠️ GPS location not found (no fix)");
  }
}
void detectHelmetTemperature() {
  static unsigned long lastRead = 0;
  if (millis() - lastRead < 2000) return; // DHT needs 2s gap
  lastRead = millis();

  float temp = dht.readTemperature(); // Celsius

  if (isnan(temp)) {
    Serial.println("❌ Temperature sensor error");
    return;
  }

  // ✅ PRINT TEMPERATURE ONLY ONCE
  if (!tempPrinted) {
    Serial.print("🌡️ Helmet Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");
    tempPrinted = true;
  }

  // ⚠️ Warning logic still works
  if (temp >= TEMP_THRESHOLD && !highTempDetected) {
    highTempDetected = true;
    Serial.println("⚠️ WARNING: High helmet temperature!");
    Serial.println("⚠️ Rider may feel dizzy. Please stop and rest.");
  }
  else if (temp < TEMP_THRESHOLD && highTempDetected) {
    highTempDetected = false;
    Serial.println("✅ Helmet temperature back to safe level.");
  }
}


