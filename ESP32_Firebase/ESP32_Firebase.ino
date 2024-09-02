#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define SSID "test"
#define PASSWORD "plmoknij"
#define API_KEY "AIzaSyC7ANtQZ2tjZ85Yqea3dF3ImeL52s6oy1o"
#define DATABASE_URL "https://esp32-first-6425d-default-rtdb.asia-southeast1.firebasedatabase.app/"

#define LED_PIN 2

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData stream;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int statusLed = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Signup OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Set up a stream to listen for changes in "Led/status"
  if (!Firebase.RTDB.beginStream(&stream, "Led/status")) {
    Serial.printf("Stream begin error, %s\n", stream.errorReason().c_str());
  } else {
    Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
  }
}

void loop() {
  // Send random data to Firebase every 5 seconds
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.setInt(&fbdo, "Sensor/value", random(1, 101))) {
      Serial.println();
      Serial.print("Saved Successfully");
    } else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
}

// Callback function for stream changes
void streamCallback(FirebaseStream data) {
  if (data.dataType() == "boolean") {
    statusLed = data.boolData();
    digitalWrite(LED_PIN, statusLed ? HIGH : LOW); // Set LED state
    Serial.printf("Stream Data - Path: %s, Type: %s, Value: %s\n", data.dataPath().c_str(), data.dataType().c_str(), data.stringData().c_str());
  }
}

// Callback function for stream timeout
void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("Stream timeout, resuming...");
    if (!stream.httpConnected()) {
      Serial.printf("Error code: %d, reason: %s\n", stream.httpCode(), stream.errorReason().c_str());
    }
  }
}
