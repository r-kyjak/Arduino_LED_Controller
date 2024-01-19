#include <IRremote.hpp>
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS  148
#define LED_PIN   5
#define IR_RECEIVE_PIN 11

#define INIT_BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t value;
uint32_t previousVal;

int state = 0; // 0-off 1-on
int brightness = INIT_BRIGHTNESS;

enum colors { RED, GREEN, BLUE, WHITE, DARK_ORANGE, CHARTREUSE, DARK_BLUE, ORANGE, ROYAL_BLUE, DARK_VIOLET, GOLDEN_ROD, LIGHT_SEA_GREEN, PURPLE, YELLOW, TEAL, MAGENTA };
enum actions { FLASH, STROBE, FADE, SMOOTH };

colors lastColor = WHITE;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver

  strip.begin();
  while (!IrReceiver.isIdle());
  if (IrReceiver.isIdle()) {
    strip.show();
  }
  strip.setBrightness(brightness);

  strip.clear();
  while (!IrReceiver.isIdle());
  if (IrReceiver.isIdle()) {
    strip.show();
  }
}

void loop() {
  Serial.println("Heartbeat");
  Serial.print("State:");
  Serial.println(state);
  while (!IrReceiver.isIdle());
  if (IrReceiver.decode()) {
    value = IrReceiver.decodedIRData.decodedRawData;

    if (value == 0x0) value = previousVal;
    Serial.println(value, HEX);

    switch (value) {
      case 0xFF00EF00:
        intensityUp();
        break;
      case 0xFE01EF00:
        intensityDown();
        break;
      case 0xFD02EF00:
        turnOnOff(0);
        break;
      case 0xFC03EF00:
        turnOnOff(1);
        break;
      case 0xFB04EF00:
        setColor(RED);
        break;
      case 0xFA05EF00:
        setColor(GREEN);
        break;
      case 0xF906EF00:
        setColor(DARK_BLUE);
        break;
      case 0xF807EF00:
        setColor(WHITE);
        break;
      case 0xF708EF00:
        setColor(DARK_ORANGE);
        break;
      case 0xF609EF00:
        setColor(CHARTREUSE);
        break;
      case 0xF50AEF00:
        setColor(BLUE);
        break;
      case 0xF40BEF00:

        break;
      case 0xF30CEF00:
        setColor(ORANGE);
        break;
      case 0xF20DEF00:
        setColor(ROYAL_BLUE);
        break;
      case 0xF10EEF00:
        setColor(PURPLE);
        break;
      case 0xF00FEF00:
        break;
      case 0xEF10EF00:
        setColor(GOLDEN_ROD);
        break;
      case 0xEE11EF00:
        setColor(LIGHT_SEA_GREEN);
        break;
      case 0xED12EF00:
        setColor(DARK_VIOLET);
        break;
      case 0xEC13EF00:
        break;
      case 0xEB14EF00:
        setColor(YELLOW);
        break;
      case 0xEA15EF00:
        setColor(TEAL);
        break;
      case 0xE916EF00:
        setColor(MAGENTA);
        break;
      case 0xE817EF00:
        break;
      default:
        Serial.println("UNKNOWN");
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        break;
    }
    previousVal = value;
    IrReceiver.resume(); // Receive the next value
  }
  delay(250); // small delay to prevent reading errors
}

void setColor(colors val) {
  if (state == 0) return;
  lastColor = val;

  uint32_t color;
  switch (val) {
    case RED:
      Serial.println("Setting color: RED");
      color = strip.Color(255, 0, 0);
      break;
    case GREEN:
      Serial.println("Setting color: GREEN");
      color = strip.Color(0, 255, 0);
      break;
    case BLUE:
      Serial.println("Setting color: BLUE");
      color = strip.Color(0, 0, 255);
      break;
    case WHITE:
      Serial.println("Setting color: WHITE");
      color = strip.Color(255, 255, 255);
      break;
    case DARK_ORANGE:
      Serial.println("Setting color: DARK_ORANGE");
      color = strip.Color(255, 140, 0);
      break;
    case CHARTREUSE:
      Serial.println("Setting color: CHARTREUSE");
      color = strip.Color(127, 255, 0);
      break;
    case DARK_BLUE:
      Serial.println("Setting color: DARK_BLUE");
      color = strip.Color(0, 0, 139);
      break;
    case ORANGE:
      Serial.println("Setting color: ORANGE");
      color = strip.Color(255, 165, 0);
      break;
    case ROYAL_BLUE:
      Serial.println("Setting color: ROYAL_BLUE");
      color = strip.Color(65, 105, 225);
      break;
    case DARK_VIOLET:
      Serial.println("Setting color: DARK_VIOLET");
      color = strip.Color(148, 0, 211);
      break;
    case GOLDEN_ROD:
      Serial.println("Setting color: GOLDEN_ROD");
      color = strip.Color(218, 165, 32);
      break;
    case LIGHT_SEA_GREEN:
      Serial.println("Setting color: LIGHT_SEA_GREEN");
      color = strip.Color(32, 178, 170);
      break;
    case PURPLE:
      Serial.println("Setting color: PURPLE");
      color = strip.Color(128, 0, 128);
      break;
    case YELLOW:
      Serial.println("Setting color: YELLOW");
      color = strip.Color(255, 255, 0);
      break;
    case TEAL:
      Serial.println("Setting color: TEAL");
      color = strip.Color(0, 128, 128);
      break;
    case MAGENTA:
      Serial.println("Setting color: MAGENTA");
      color = strip.Color(255, 0, 255);
      break;
    default:
      Serial.println("UNKNOWN");
      lastColor = WHITE;
      return;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }

  while (!IrReceiver.isIdle());
  if (IrReceiver.isIdle()) {
    strip.show();
  }
}

void turnOnOff(int val) {
  if (val == 1) {
    if (state == 1) return;
    Serial.println("Turning LED on.");
    state = 1;
    setColor(lastColor);
    Serial.print("Setting brightness: ");
    Serial.println(brightness);
    strip.setBrightness(brightness);
    while (!IrReceiver.isIdle());
    if (IrReceiver.isIdle()) {
      strip.show();
    }
  } else if (val == 0) {
    if (state == 0) return;
    Serial.println("Turning LED off.");
    brightness = INIT_BRIGHTNESS;
    state = 0;
    strip.clear();
    while (!IrReceiver.isIdle());
    if (IrReceiver.isIdle()) {
      strip.show();
    }
  }
}

void intensityUp() {
  if (state == 0) return;
  if (brightness < 200) {
    brightness = brightness + 5;
  } else {
    return;
  }
  Serial.print("Setting brightness: ");
  Serial.println(brightness);
  strip.setBrightness(brightness);
  while (!IrReceiver.isIdle());
  if (IrReceiver.isIdle()) {
    strip.show();
  }
}

void intensityDown() {
  if (state == 0) return;
  if (brightness >= 10) {
    brightness = brightness - 5;
  } else {
    return;
  }
  Serial.print("Setting brightness: ");
  Serial.println(brightness);
  strip.setBrightness(brightness);
  while (!IrReceiver.isIdle());
  if (IrReceiver.isIdle()) {
    strip.show();
  }
}

void perform(actions val) {
  if (state == 0) return;
  // Implement your perform function based on NeoPixel actions
  // ...
}