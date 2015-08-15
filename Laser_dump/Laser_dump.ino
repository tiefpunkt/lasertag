#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    switch (results.value) {
      case 0x67A6E25B:
      case 0x839B0D18:
      case 0x81760AB3:
      case 0x7B1BEEC9:
      case 0x9436164D:
        Serial.print("blue");
        break;
      case 0x55206223:
      case 0x71148CE0:
      case 0x6EEF8A7B:
      case 0x68956E91:
      case 0x81AF9615:
        Serial.print("red");
        break;
      case 0xFCBAF4E1:
      case 0x18AF1F9E:
      case 0x4AF0F009:
      case 0xD9A64DF3:
      case 0x7DDC5D2F:
        Serial.print("yellow");
        break;
      case 0x1E16B32F:
      case 0x3A0ADDEC:
      case 0x37E5DB87:
      case 0x318BBF9D:
      case 0x4AA5E721:
        Serial.print("green");
        break;
      default:
        irrecv.resume();
        return;
    }

    Serial.print(" - ");

    switch (results.value) {
      case 0x67A6E25B:
      case 0x55206223:
      case 0xFCBAF4E1:
      case 0x1E16B32F:
        Serial.println("stealth");
        break;
      case 0x839B0D18:
      case 0x71148CE0:
      case 0x18AF1F9E:
      case 0x3A0ADDEC:
        Serial.println("pulse");
        break;
      case 0x81760AB3:
      case 0x6EEF8A7B:
      case 0x4AF0F009:
      case 0x37E5DB87:
        Serial.println("rail");
        break;
      case 0x7B1BEEC9:
      case 0x68956E91:
      case 0xD9A64DF3:
      case 0x318BBF9D:
        Serial.println("sonic");
        break;
      case 0x9436164D:
      case 0x81AF9615:
      case 0x7DDC5D2F:
      case 0x4AA5E721:
        Serial.println("laser");
    }
    irrecv.resume(); // Receive the next value
  }
}
