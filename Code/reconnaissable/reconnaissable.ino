#include <CapacitiveSensor.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define COMMON_PIN 53
#define NB_PIN 16

#define CMD_PLAY_NEXT 0x01
#define CMD_PLAY_PREV 0x02
#define CMD_PLAY_W_INDEX 0x03
#define CMD_SET_VOLUME 0x06
#define CMD_SEL_DEV 0x09
#define CMD_PLAY_W_VOL 0x22
#define CMD_PLAY 0x0D
#define CMD_PAUSE 0x0E
#define CMD_SINGLE_CYCLE 0x19
#define CMD_PLAY_WITH_FOLDER 0x0F
#define DEV_TF 0x02
#define SINGLE_CYCLE_ON 0x00
#define SINGLE_CYCLE_OFF 0x01

#define ARDUINO_RX 7  // Arduino Pin connected to the TX of the Serial MP3 Player module
#define ARDUINO_TX 6  // Arduino Pin connected to the RX of the Serial MP3 Player module
#define CS CapacitiveSensor

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);
CapacitiveSensor capList[16] = {CS(53, 51), CS(53, 50), CS(53, 49), CS(53, 48), 
CS(53, 47), CS(53, 46), CS(53, 45), CS(53, 44), CS(53, 43), CS(53, 42), 
CS(53, 41), CS(53, 40), CS(53, 39),CS(53, 38),CS(53, 37),CS(53, 36)};

int capacitance[16];
int currentKey = -1;
bool isHolding = false;

// Led part
 
#define NUMPIXELS 44
#define PIN 31
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int b = random(NB_PIN/2);

void setup()
{
  Serial.begin(9600);
  mp3.begin(9600);
  delay(500);

  mp3_command(CMD_SEL_DEV, DEV_TF);  // select the TF card
  delay(200);

  mp3_command(CMD_SET_VOLUME,1000);

  //mp3_command(CMD_PLAY_WITH_FOLDER, 0x0F00102);       // Play mp3
  // mp3_command(CMD_PAUSE, 0x0000);      // Pause mp3
  //mp3_command(CMD_PLAY_NEXT, 0x0000);  // Play next mp3
  //mp3_command(CMD_PLAY_PREV, 0x0000);  // Play previous mp3

  strip.begin();
  turnOffLed();
  strip.setPixelColor(b, 0, 0, 255);
  strip.show();
}

void loop()                    
{
  for(int i = 0; i < NB_PIN; i++) {
    bool a = false;
    capacitance[i] = capList[i].capacitiveSensor(30);
    // Serial.print(capacitance[i]);
    // Serial.print(" : ");
    
    if(capacitance[i] > 250) {
      if(!isHolding) {
        if(i != b) {
          for(int j = 0; j < 3; j++) {
            //  turnRedLed();
            //  delay(100);
             turnOffLed();
             delay(100);
          }
          strip.setPixelColor(b*3, 0, 0, 255);
          strip.show();
          Serial.print(b);
          Serial.print(" / ");
          Serial.println(capacitance[i]);
          break;
        }
        mp3_command(CMD_PLAY_WITH_FOLDER, 0x0F00100 + (i+1));
        turnOffLed();
        b = random(NB_PIN / 2);
        strip.setPixelColor(i*3, 0, 0, 255);
        strip.show();
        break;
      }
      a = true;
    }
    // Serial.println("");
    isHolding = a;
    // long start = millis();
    // long total2 =  second.capacitiveSensor(30);

    // Serial.print(millis() - start);        // check on performance in milliseconds
    // Serial.print("\t");                    // tab character for debug windown spacing

                     // print sensor output 1
    // Serial.print("\t");
    // Serial.println(total2);

    // if(total1 > 500 and total2 > 500) {
    //   selected_sound = 0;
    //   mp3_command(CMD_PAUSE, 0x0000);
    // } else if(total1 > 500) {
    //   if(selected_sound != 1) {
    //     selected_sound = 1;
    //     mp3_command(CMD_PLAY_WITH_FOLDER, 0x0F00101);
    //   }
    // } else if(total2 > 500) {
    //   if(selected_sound != 2) {
    //     selected_sound = 2;
    //     mp3_command(CMD_PLAY_WITH_FOLDER, 0x0F00102);
    //   }
    // }
    // delay(10);                             // arbitrary delay to limit data to serial port 
  }
}

void mp3_command(int8_t command, int16_t dat) {
  int8_t frame[8] = { 0 };
  frame[0] = 0x7e;                // starting byte
  frame[1] = 0xff;                // version
  frame[2] = 0x06;                // the number of bytes of the command without starting byte and ending byte
  frame[3] = command;             //
  frame[4] = 0x00;                // 0x00 = no feedback, 0x01 = feedback
  frame[5] = (int8_t)(dat >> 8);  // data high byte
  frame[6] = (int8_t)(dat);       // data low byte
  frame[7] = 0xef;                // ending byte
  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(frame[i]);
  }
}

void turnOffLed() {
  for(int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}

void turnRedLed() {
  for(int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 255, 0, 0);
  }
  strip.show();
}