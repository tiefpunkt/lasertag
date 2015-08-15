#include <FastLED.h>

#include <IRremote.h>
#include <FlexiTimer2.h>

#define LED_PIN 6
#define NUM_LEDS 20
CRGB leds[NUM_LEDS];


int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);
decode_results results;

bool checkIR() {
  Serial.print(".");
  uint8_t color = 0;
  if (irrecv.decode(&results)) {
    Serial.print("?");
    switch (results.value) { 
      case 0x67A6E25B:
      case 0x839B0D18:
      case 0x81760AB3:
      case 0x7B1BEEC9:
      case 0x9436164D:
        color = 1; //blue");
        break;
      case 0x55206223:
      case 0x71148CE0:
      case 0x6EEF8A7B:
      case 0x68956E91:
      case 0x81AF9615:
        color = 2; //red");
        break;
      case 0xFCBAF4E1:
      case 0x18AF1F9E:
      case 0x4AF0F009:
      case 0xD9A64DF3:
      case 0x7DDC5D2F:
        color = 3; //yellow");
        break;
      case 0x1E16B32F:
      case 0x3A0ADDEC:
      case 0x37E5DB87:
      case 0x318BBF9D:
      case 0x4AA5E721:
        color = 4; //green");
        break;
      default:
        Serial.println(results.value, HEX);
        irrecv.resume();
        return false;
    }
    Serial.print("!");
 //   FlexiTimer2::stop();
    setColor(CRGB::Black); // Red
    delay(20);
    if (color == 1) {
      setColor(CRGB::Blue);
    } else if (color == 2) {
      setColor(CRGB::Red);
    } else if (color == 3) {
      setColor(CRGB::Yellow);
    } else if (color == 4) {
      setColor(CRGB::Green);
    }
    delay(10);
   // FlexiTimer2::start();
    irrecv.resume();
    
    return true;
  }
  return false;
}

void setup() {
Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  irrecv.enableIRIn();
  //FlexiTimer2::set(100, checkIR);
  //FlexiTimer2::start();
}

void loop() {
  checkIR();
  delay(50);
  // Some example procedures showing how to display to the pixels:
  /*colorWipe(0xff0000, 50); // Red
  colorWipe(0x00ff00, 50); // Green
  colorWipe(0x0000ff, 50); // Blue

  rainbow(50);
  rainbowCycle(5  0);*/
}

void ir_delay(uint16_t ms) {
  if (checkIR()) {
    for (int i; i < 40; i++) {
      delay(50);
      if (checkIR()) {
        i = 0;
      }
    }
  }
  delay(ms);
}

void setColor(CRGB c) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      leds[i] = c;
  }
  FastLED.show();
}

// Fill the dots one after the other with a color
void colorWipe(CRGB c, uint8_t wait) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      leds[i] = c;
      FastLED.show();
      ir_delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<NUM_LEDS; i++) {
      leds[i] = Wheel((i+j) & 255);
    }
    FastLED.show();
    ir_delay(wait);
  }
}  

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
    }
    FastLED.show();
    ir_delay(wait);
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return (255 - WheelPos * 3) * 256 * 256 + WheelPos * 3;
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return WheelPos * 3 * 256 + 255 - WheelPos * 3;
  } else {
   WheelPos -= 170;
   return WheelPos * 3 * 256 * 256 + (255 - WheelPos * 3) *256;
  }
}

