static bool flag=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // Initialize Serial (USB) for debugging
  Serial1.begin(115200);    // Initialize Serial2 (e.g., for UART communication)
}

void loop() {
  // Check if data is available on Serial2
  if (Serial.available()) {
    // Read the incoming data until the newline character '\n'
    String receivedData = Serial.readStringUntil('\n');
    
    // Print the received data to the Serial monitor
    Serial.println("Received: " + receivedData);

    if(flag==0){
      flag=1;
      Serial1.println('0');
    }else{
      flag=0;
      Serial1.println('1');
    }
  }
  // delay(1);
}
