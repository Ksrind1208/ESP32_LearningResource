#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define LED_PIN 2  // Define the pin for LED 2

BLECharacteristic *pCharacteristic;

// Cài đặt cho BLE Server
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Client connected.");
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Client disconnected.");
    // Start advertising again when client disconnects
    BLEDevice::startAdvertising();
    Serial.println("Restarted advertising.");
  }
};

// Callback cho BLE Characteristic
class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    String value = pCharacteristic->getValue();

    if (value == "ON") {
      Serial.println("Received: ON");
      digitalWrite(LED_PIN, HIGH);  // Turn LED 2 ON
    } else if (value == "OFF") {
      Serial.println("Received: OFF");
      digitalWrite(LED_PIN, LOW);  // Turn LED 2 OFF
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  pinMode(LED_PIN, OUTPUT);  // Set the LED pin as output
  digitalWrite(LED_PIN, LOW); // Ensure LED is initially OFF

  BLEDevice::init("Long name works now");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());  // Set the callback for the characteristic

  pCharacteristic->setValue("Hello World says Neil");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // Kiểm tra xem có dữ liệu nào từ Serial Monitor không
  if (Serial.available() > 0) {
    String receivedValue = Serial.readStringUntil('\n'); // Đọc chuỗi từ Serial Monitor

    // Set giá trị này vào BLE Characteristic
    pCharacteristic->setValue(receivedValue.c_str()); // Set giá trị từ Serial Monitor
    pCharacteristic->notify(); // Thông báo giá trị mới đến GATT client

    Serial.print("Set value to: ");
    Serial.println(receivedValue);
  }

  delay(200); // Độ trễ để giảm tải cho vi điều khiển
}
