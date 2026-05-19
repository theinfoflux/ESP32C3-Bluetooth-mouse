#include <BleMouse.h>

BleMouse bleMouse("XIAO_C3_MOUSE");

void setup() {
  Serial.begin(115200);

  delay(3000);  // IMPORTANT for ESP32-C3 boot stability

  Serial.println("Starting BLE Mouse...");

  bleMouse.begin();

  Serial.println("BLE Started");
}

void loop() {
  if (bleMouse.isConnected()) {
    Serial.println("Connected");

    bleMouse.move(5, 0);
    delay(1000);
  }
}