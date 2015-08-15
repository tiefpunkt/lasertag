#include <IRremote.h>
#include <IRremoteInt.h>
#include <FlexiTimer2.h>

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(66, PIN, NEO_GRB + NEO_KHZ800);

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);
decode_results results;

void checkIR() {
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
        irrecv.resume();
        return;
    }
    Serial.print("!");
    FlexiTimer2::stop();
    colorWipe(strip.Color(255, 255, 255), 0); // Red
    delay(100);
    if (color == 1) {
      colorWipe(strip.Color(0,0,255),0);
    } else if (color == 2) {
      colorWipe(strip.Color(255,0,0),0);
    } else if (color == 3) {
      colorWipe(strip.Color(255,255,0),0);
    } else if (color == 4) {
      colorWipe(strip.Color(0,255,0),0);
    }
    delay(300);
    FlexiTimer2::start();
    irrecv.resume();
  }
}

void setup() {
Serial.begin(9600);


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  irrecv.enableIRIn();
  FlexiTimer2::set(100, checkIR);
  FlexiTimer2::start();
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();

        delay(wait);

        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
