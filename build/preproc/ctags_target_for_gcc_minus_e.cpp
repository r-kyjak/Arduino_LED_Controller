# 1 "/Users/radovankyjak/Temp/arduino/arduino.ino"
# 2 "/Users/radovankyjak/Temp/arduino/arduino.ino" 2
# 3 "/Users/radovankyjak/Temp/arduino/arduino.ino" 2
int IR_RECEIVE_PIN = 11; // define input pin on Arduino 



CRGB leds[15];
decode_results results;
IRrecv irrecv(IR_RECEIVE_PIN);

uint32_t value;
uint32_t previousVal;

int state = 0; //0-off 1-on
int brightness = 50;

enum colors {RED, GREEN, BLUE, WHITE, DARK_ORANGE, CHARTREUSE, DARK_BLUE, ORANGE, ROYAL_BLUE, DARK_VIOLET, GOLDEN_ROD, LIGHT_SEA_GREEN, PURPLE, YELLOW, TEAL, MAGENTA};
enum actions {FLASH, STROBE, FADE, SMOOTH};

void setup() {
 Serial.begin(9600);
 //IrReceiver.begin(IR_RECEIVE_PIN);
 IrReceiver.enableIRIn();

 FastLED.addLeds<WS2812B, 5, GRB>(leds, 15);
 delay(200);
   FastLED.setBrightness(brightness);

 FastLED.clear();
 FastLED.show();
}

void loop() {
 if (IrReceiver.decode(&results)) {
  value = results.value;
  Serial.println(value, 16);

  if(value == 0xFFFFFFFF) value = previousVal;
  switch (value){
            case 0xF700FF:
    intensityUp();
    break;
   case 0xF7807F:
    intensityDown();
    break;
   case 0xF740BF:
    turnOnOff(0);
    break;
   case 0xF7C03F:
    turnOnOff(1);
    break;
   case 0xF720DF:
    setColor(RED);
    break;
   case 0xF7A05F:
    setColor(GREEN);
    break;
   case 0xF7609F:
    setColor(DARK_BLUE);
    break;
   case 0xF7E01F:
    setColor(WHITE);
    break;
   case 0xF710EF:
    setColor(DARK_ORANGE);
    break;
   case 0xF7906F:
    setColor(CHARTREUSE);
    break;
   case 0xF750AF:
    setColor(BLUE);
    break;
   case 0xF7D02F:

    break;
   case 0xF730CF:
    setColor(ORANGE);
    break;
   case 0xF7B04F:
    setColor(ROYAL_BLUE);
    break;
   case 0xF7708F:
    setColor(PURPLE);
    break;
   case 0xF7F00F:
    break;
   case 0xF708F7:
    setColor(GOLDEN_ROD);
    break;
   case 0xF78877:
    setColor(LIGHT_SEA_GREEN);
    break;
   case 0xF748B7:
    setColor(DARK_VIOLET);
    break;
   case 0xF7C837:
    break;
   case 0xF728D7:
    setColor(YELLOW);
    break;
   case 0xF7A857:
    setColor(TEAL);
    break;
   case 0xF76897:
    setColor(MAGENTA);
    break;
   case 0xF7E817:
    break;
   default:
    Serial.println("UNKNOWN");
    Serial.println(results.value, 16);
    break ;
        }
  previousVal = value;
  IrReceiver.resume(); // Receive the next value 
 }
 delay (200); // small delay to prevent reading errors
}

void setColor(colors val){
 if (state == 0) return;

 switch (val){
  case RED:
   Serial.println("Setting color: RED");
   fill_solid( leds, 15, CRGB::Red);
   break;
  case GREEN:
   Serial.println("Setting color: GREEN");
   fill_solid( leds, 15, CRGB::Green);
   break;
  case BLUE:
   Serial.println("Setting color: BLUE");
   fill_solid( leds, 15, CRGB::Blue);
   break;
  case WHITE:
   Serial.println("Setting color: WHITE");
   fill_solid( leds, 15, CRGB::White);
   break;
  case DARK_ORANGE:
   Serial.println("Setting color: DARK_ORANGE");
   fill_solid( leds, 15, CRGB::DarkOrange);
   break;
  case CHARTREUSE:
   Serial.println("Setting color: CHARTREUSE");
   fill_solid( leds, 15, CRGB::Chartreuse);
   break;
  case DARK_BLUE:
   Serial.println("Setting color: DARK_BLUE");
   fill_solid( leds, 15, CRGB::DarkBlue);
   break;
  case ORANGE:
   Serial.println("Setting color: ORANGE");
   fill_solid( leds, 15, CRGB::Orange);
   break;
  case ROYAL_BLUE:
   Serial.println("Setting color: ROYAL_BLUE");
   fill_solid( leds, 15, CRGB::RoyalBlue);
   break;
  case DARK_VIOLET:
   Serial.println("Setting color: DARK_VIOLET");
   fill_solid( leds, 15, CRGB::DarkViolet);
   break;
  case GOLDEN_ROD:
   Serial.println("Setting color: GOLDEN_ROD");
   fill_solid( leds, 15, CRGB::Goldenrod);
   break;
  case LIGHT_SEA_GREEN:
   Serial.println("Setting color: LIGHT_SEA_GREEN");
   fill_solid( leds, 15, CRGB::LightSeaGreen);
   break;
  case PURPLE:
   Serial.println("Setting color: PURPLE");
   fill_solid( leds, 15, CRGB::Purple);
   break;
  case YELLOW:
   Serial.println("Setting color: YELLOW");
   fill_solid( leds, 15, CRGB::Yellow);
   break;
  case TEAL:
   Serial.println("Setting color: TEAL");
   fill_solid( leds, 15, CRGB::Teal);
   break;
  case MAGENTA:
   Serial.println("Setting color: MAGENTA");
   fill_solid( leds, 15, CRGB::Magenta);
   break;
  default:
   Serial.println("UNKNOWN");
   break ;
    }
 FastLED.show();
}

void turnOnOff(int val){
 if(val == 1){
  if (state == 1) return;
  Serial.println("Turning LED on.");
  state = 1;
  setColor(WHITE);
 } else if (val == 0){
  if (state == 0) return;
  Serial.println("Turning LED off.");
  state = 0;
  FastLED.clear();
  FastLED.show();
 }
}

void intensityUp(){
 if (state == 0) return;
 if(brightness != 100){
  brightness += 5;
 } else {
  return;
 }
 FastLED.setBrightness(brightness);
 FastLED.show();
}

void intensityDown(){
 if (state == 0) return;
 if(brightness != 5){
  brightness -= 5;
 } else {
  return;
 }
 FastLED.setBrightness(brightness);
 FastLED.show();
}

void perform(actions val){
 if (state == 0) return;
}
