# 🪖 Multi-Sensor Smart Helmet for Real-Time Rider Safety Monitoring

An embedded IoT-based smart helmet system designed to enhance rider safety by monitoring physiological conditions, environmental factors, and vehicle dynamics in real-time. The system detects unsafe conditions such as alcohol consumption, drowsiness, and accidents, and sends emergency alerts with GPS location.

---

## 🚀 Project Overview

This project implements a **multi-sensor integrated embedded system** using ESP32 to ensure rider safety through continuous monitoring and intelligent decision-making.

The system performs:
- Pre-ride safety validation (alcohol detection)
- Real-time monitoring (drowsiness, temperature, motion)
- Accident detection using multi-sensor fusion
- Emergency alert with GPS location via GSM

---

## 🎯 Key Features

- 🛑 Alcohol detection before ride initiation  
- 😴 Drowsiness detection using eye-blink monitoring  
- 🚨 Accident detection using vibration + motion validation  
- 📍 Real-time GPS tracking of accident location  
- 📩 GSM-based emergency alert system  
- 🌡️ Temperature monitoring inside helmet  
- ⚙️ Dynamic threshold calibration for improved accuracy  

---

## 🧠 System Architecture

The system is built around the **ESP32 microcontroller**, which processes real-time data from multiple sensors and executes safety validation logic.

### 🔧 Components Used
- ESP32 Microcontroller  
- MQ-3 Alcohol Sensor  
- IR Sensor (Eye Blink Detection)  
- MPU6050 (Accelerometer & Gyroscope)  
- SW-420 Vibration Sensor  
- Neo-6M GPS Module  
- SIM800L GSM Module  
- DHT11 Temperature Sensor  

---

## 🔄 System Workflow

1. **Sensor Calibration**
   - Establish baseline values for accurate detection

2. **Alcohol Detection**
   - Prevents system activation if alcohol is detected

3. **Drowsiness Monitoring**
   - Tracks eye-blink patterns using IR sensor

4. **Environmental Monitoring**
   - Monitors temperature inside helmet

5. **Accident Detection**
   - Uses vibration + motion sensor validation

6. **Emergency Response**
   - Retrieves GPS location
   - Sends alert via GSM module



---

## 📊 Results

- Reliable detection of alcohol, drowsiness, and accidents  
- Reduced false positives using multi-sensor fusion  
- GPS location retrieved within **3–5 seconds**  
- Real-time alert system successfully tested  

---

## 💻 Technologies Used

- Embedded C  
- ESP32 Programming  
- Sensor Integration  
- IoT Systems  
- Real-Time Data Processing  

---

## 🧪 Testing & Validation

- Tested under multiple simulated scenarios  
- Verified sensor accuracy and response time  
- Evaluated GPS tracking and alert system performance  

---





## 🧠 Skills Demonstrated

- Embedded Systems Development  
- Sensor Integration & Data Fusion  
- Real-Time Control Systems  
- Hardware Prototyping  
- System-Level Debugging  

---

## 🚀 Future Improvements

- Cloud-based IoT integration  
- Machine learning for predictive safety  
- Engine interlock system  
- Advanced health monitoring sensors  

---

## 👨‍💻 Author

**Mukil S**  
B.Tech Electronics and Communication Engineering  
Vellore Institute of Technology  

---

## ⭐ If you found this project useful, consider giving it a star!
