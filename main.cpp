#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>

#define WIFI_SSID "YOUR WIFI_SSID"
#define WIFI_PASSWORD "YOUR WIFI_PASSWORD"

#define RX_PIN 16
#define TX_PIN 17

HardwareSerial GM65(1); // UART1

const char* PROJECT_ID = "YOUR PROJECT_ID";

// Structure to store Firestore data
struct Barang {
  String id;
  String name;
  String weight;
  String length;
  String material;
  String color;
};

const int MAX_ITEMS = 20;
Barang itemList[MAX_ITEMS];
int itemCount = 0;

void fetchDataFromTA_BarcodeCollection() {
  String url = "https://firestore.googleapis.com/v1/projects/" + String(PROJECT_ID)
               + "/databases/(default)/documents/YOUR FIRESTORE_NAME/"; //YOUR FIRESTORE_NAME

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();

    const size_t capacity = 40 * 1024;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("Failed to parse JSON: ");
      Serial.println(error.c_str());
      return;
    }

    JsonArray documents = doc["documents"];
    for (JsonObject d : documents) {
      if (itemCount >= MAX_ITEMS) break;

      JsonObject fields = d["fields"];
      Barang item;

      item.id       = fields["id"]["stringValue"] | "-";
      item.name     = fields["nama"]["stringValue"] | "-";
      item.weight   = fields["berat"]["stringValue"] | "-";
      item.length   = fields["panjang"]["stringValue"] | "-";
      item.material = fields["material"]["stringValue"] | "-";
      item.color    = fields["warna"]["stringValue"] | "-";

      itemList[itemCount++] = item;
    }

    Serial.println("Data from Firestore loaded into memory!");
  } else {
    Serial.print("HTTP Error: ");
    Serial.println(httpCode);
  }

  http.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  fetchDataFromTA_BarcodeCollection();

  GM65.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("GM65 Barcode Scanner Ready. Please scan...");
}

void displayItemData(String scannedID) {
  for (int i = 0; i < itemCount; i++) {
    if (itemList[i].id == scannedID) {
      Serial.println("Item Found:");
      Serial.println("ID       : " + itemList[i].id);
      Serial.println("Name     : " + itemList[i].name);
      Serial.println("Weight   : " + itemList[i].weight);
      Serial.println("Length   : " + itemList[i].length);
      Serial.println("Material : " + itemList[i].material);
      Serial.println("Color    : " + itemList[i].color);
      Serial.println("------------------------");
      return;
    }
  }
  Serial.println("Barcode not found in the database.");
}

void loop() {
  if (GM65.available()) {
    String barcode = "";
    unsigned long timeout = millis();

    while (millis() - timeout < 200) {
      if (GM65.available()) {
        char c = GM65.read();
        if (c >= 32 && c <= 126) {
          barcode += c;
        }
        timeout = millis();
      }
    }

    if (barcode.length() > 0) {
      Serial.print("Scanned Barcode: ");
      Serial.println(barcode);
      String trimmedBarcode = barcode.substring(0, barcode.length() - 3);
      displayItemData(trimmedBarcode);
    }
  }
}
