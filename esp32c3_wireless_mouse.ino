#include <BleMouse.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// BLE Mouse
BleMouse bleMouse("XIAO BLE Mouse", "ESP32C3", 100);

// Joystick pins
#define VRX A0
#define VRY A1
#define SW  D8

// YOUR REAL CENTER VALUES
int centerX = 3700;
int centerY = 3650;

#define DEADZONE 120
#define SPEED 6

bool oledOK = false;
bool bleStarted = false;

void setup() {

  Serial.begin(115200);

  pinMode(SW, INPUT_PULLUP);

  // YOUR WORKING OLED SETUP (UNCHANGED)
  Wire.begin(6, 7);

  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    oledOK = true;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(10, 20);
    display.println("Starting...");
    display.display();

  } else {
    Serial.println("OLED FAILED");
  }

  

  // IMPORTANT: delay BEFORE BLE (fix crash)
delay(4000);

  bleMouse.begin();
  bleStarted = true;

  Serial.println("BLE STARTED");
}

void loop() {

  bool connected = bleMouse.isConnected();

  // OLED (SAME STYLE YOU CONFIRMED WORKING)
  if (oledOK) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);

    display.println("XIAO BLE Mouse");

    display.setCursor(0, 20);

    if (connected) display.println("CONNECTED");
    else display.println("WAITING...");

    display.display();
  }

  // WAIT FOR BLE CONNECTION
  if (!connected) {
    delay(10);
    return;
  }

  // JOYSTICK READ
  int x = analogRead(VRX);
  int y = analogRead(VRY);

  int xOffset = x - centerX;
  int yOffset = y - centerY;

  int moveX = 0;
  int moveY = 0;

  if (abs(xOffset) > DEADZONE) {
    moveX = (xOffset > 0) ? -SPEED : SPEED;
  }

  if (abs(yOffset) > DEADZONE) {
    moveY = (yOffset > 0) ? SPEED : -SPEED;
  }

  moveY = -moveY;

  bleMouse.move(moveX, moveY);

  // CLICK
  if (digitalRead(SW) == LOW) {

    bleMouse.click(MOUSE_LEFT);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(30, 25);
    display.println("CLICK");
    display.display();

    delay(250);
  }

  delay(10);
}