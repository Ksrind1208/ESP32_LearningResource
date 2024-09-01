#include "BluetoothSerial.h"  // Include Bluetooth Serial library

BluetoothSerial SerialBT;  // Create an instance of the BluetoothSerial class

const int ledPin = 2;  // Define the LED pin
String receivedMessage;  // To store the received message
String serialInput;  // To store the input from the Serial Monitor

void setup() {
  Serial.begin(115200);  // Initialize the Serial monitor at 115200 baud rate
  SerialBT.begin("ESP32_BT");  // Initialize Bluetooth with the device name "ESP32_BT"
  pinMode(ledPin, OUTPUT);  // Set the LED pin as an output
  Serial.println("Bluetooth is ready to pair...");
}

void loop() {
  // Check if there's any data available from the Bluetooth connection
  if (SerialBT.available()) {
    receivedMessage = SerialBT.readStringUntil('\n');  // Read the incoming message until a newline character
    receivedMessage.trim();  // Remove any leading/trailing whitespace

    Serial.println("Received: " + receivedMessage);  // Print the received message to the Serial Monitor

    // Control the LED based on the received message
    if (receivedMessage == "led_on") {
      digitalWrite(ledPin, HIGH);  // Turn on the LED
      SerialBT.println("LED is ON");
      Serial.println("LED is ON");
    } else if (receivedMessage == "led_off") {
      digitalWrite(ledPin, LOW);  // Turn off the LED
      SerialBT.println("LED is OFF");
      Serial.println("LED is OFF");
    } else {
      SerialBT.println("Unknown command: " + receivedMessage);  // Send an error message for unknown commands
      Serial.println("Unknown command: " + receivedMessage);
    }
  }

  // // Check if there's any data available from the Serial Monitor
  // if (Serial.available()) {
  //   serialInput = Serial.readStringUntil('\n');  // Read the input from the Serial Monitor until a newline character
  //   serialInput.trim();  // Remove any leading/trailing whitespace

  //   if (serialInput.length() > 0) {
  //     SerialBT.println(serialInput);  // Send the input from the Serial Monitor to the connected app via Bluetooth
  //     Serial.println("Sent to app: " + serialInput);  // Print the sent message to the Serial Monitor
  //   }
  // }

}
