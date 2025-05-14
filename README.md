# My Mini Project
# 📦 ESP32 Barcode Scanner with GM65 + Firebase
![GM65 Sensor](GM65_Sensor.jpg)

A lightweight barcode scanner system using **ESP32-WROOM-32 DevKit V1** and **GM65 module**, sending scanned data to **Firebase Firestore** via WiFi. No external server required, perfect for cloud-based inventory systems.

---

## 🔌 Hardware Requirements

- ESP32-WROOM-32 DevKit V1 
- GM65 Barcode Scanner (UART)
- USB to TTL Programmer
- Jumper Wires
- Firebase Account (Firestore enabled)

---

## 🧰 Wiring Diagram

| GM65 Pin | ESP32-WROOM-32 DevKit V1 |
|----------|--------------------------|
| TX       | RX0 (16)                 |
| RX       | TX0 (17)                 |
| VCC      | 5V                       |
| GND      | GND                      |

> ⚠️ Disconnect TXD during firmware upload to avoid interference with serial communication.

---

## 🌐 Firebase Setup

Follow these steps to set up Firebase and Firestore:

1. Go to the [Firebase Console](https://console.firebase.google.com/) and create a new project.
2. In the left sidebar, navigate to **Build → Firestore Database** and click **Create Database**.
   - Select **Start in test mode** (for development purposes)
   - Choose your preferred Cloud Firestore location and click **Enable**
3. Under **Project Settings → General**, find your **Project ID** (e.g., `your-project-id`)
4. Open **Firestore**, and create a new **collection** named `TA-Barcode`
   - You can manually add a dummy document to initialize the collection

You're now ready to send data from ESP32 to Firestore!
---

## 🧾 WiFi and Firebase Configuration

Replace the placeholders directly in the main sketch file:

```cpp
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

const char* PROJECT_ID = "YOUR PROJECT_ID"
```


---


## 🧪 Expected Output

### 🔎 Serial Monitor
![Result](result.jpg)


### 🔥 Firestore Entry
![Firestore Structure](FirestoreStructure.jpg)

---

## 📚 Reference

- GM65 Datasheet
- FirebaseESP32 library by @mobizt
- Arduino IDE with ESP32 board definitions

---

