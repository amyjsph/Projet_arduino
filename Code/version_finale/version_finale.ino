#include <CapacitiveSensor.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

// Capaciting sensing part

#define CS CapacitiveSensor

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);
CapacitiveSensor capList[16] = {CS(53, 51), CS(53, 50), CS(53, 49), CS(53, 48), 
CS(53, 47), CS(53, 46), CS(53, 45), CS(53, 44), CS(53, 43), CS(53, 42), 
CS(53, 41), CS(53, 40), CS(53, 39),CS(53, 38),CS(53, 37),CS(53, 36)};

int capacitance[16];
int currentKey = -1;
bool isHolding = false;

// Led part
 
#define NUMPIXELS 50
#define PIN 10
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Ecran

Adafruit_SSD1306 display(128, 32, &Wire, -1);

#define SCREEN_ADDRESS 0x3C

enum State {
  MAIN, SELECT_MODE, SELECT_INSTRUMENT
};
enum Mode {
  INSTRUMENT, MUSIQUE
};

int32_t instr;
State current_state = MAIN;
Mode mode;
int piano_state = 0;

int b = random(NB_PIN);

int compteur = 0;

void setup() {
  Serial.begin(9600);
  mp3.begin(9600);
  delay(500);

  mp3_command(CMD_SEL_DEV, DEV_TF);  // select the TF card
  delay(200);

  mp3_command(CMD_SET_VOLUME,50);

  strip.begin();
  turnOffLed();

  strip.show();

  //ECRAN
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);  
  display.write("Press any button");

  display.setCursor(0, 7);
  display.write("to continue");

  display.display();

  for(int i = 22;i <= 26; i+=2) {
    pinMode(i, INPUT);
  }

}

void loop() {
  screen_update();
  if(piano_state) piano_update();
}

void screen_update() {
  if(current_state == MAIN) {
    for(int i = 22;i <= 26; i+=2) {
      if(digitalRead(i)) {
        turnOffLed();

        compteur = 0;
        b = random(NB_PIN);

        piano_state = 0;
        current_state = SELECT_MODE;

        display.clearDisplay();
        
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(1);

        display.setCursor(0, 0);
        display.write("1) Mode instrument");

        display.setCursor(0, 10);
        display.write("2) Mode guide");

        display.display();
        
        while(digitalRead(i)) {
          Serial.println(digitalRead(i));
        }
      }
    }
  } else if(current_state == SELECT_MODE) {
    if(digitalRead(22)) {
      mode = INSTRUMENT;
      current_state = SELECT_INSTRUMENT;
      display.clearDisplay();

      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);

      display.setCursor(0, 0);
      display.write("1) Piano");

      display.setCursor(0, 10);
      display.write("2) Batterie");
      
      display.setCursor(0, 20);
      display.write("3) Synth 80s");

      display.display();

      while(digitalRead(22)) {
        Serial.println(digitalRead(22));
      }
    } else if(digitalRead(24)) {
      turnOnLed(b);

      piano_state = 1;
      mode = MUSIQUE;
      current_state = MAIN;

      display.clearDisplay();

      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);

      display.setCursor(0, 0);
      display.write("Musique");

      display.display();

      while(digitalRead(24)) {
        Serial.println(digitalRead(24));
      }
    }
    
  } else if(current_state == SELECT_INSTRUMENT) {
    for(int i = 22;i <= 26; i+=2) {
      if(digitalRead(i)) {
        piano_state = 1;
        instr = (i - 22)/2 + 1;
        display.clearDisplay();
        
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(1);

        display.setCursor(0, 0);
        display.write("Playing : ");

        display.setCursor(0, 10);
        if(instr == 1) {
          display.write("Piano");
          turnEveryLedOn(255, 255, 0);
        } else if(instr == 2) {
          display.write("Batterie");
          turnEveryLedOn(255, 0, 0);
        } else if(instr == 3) {
          display.write("Synth 80s");
          turnEveryLedOn(0, 0, 255);
        }

        display.display();
        
        current_state = MAIN;
        while(digitalRead(i)) {
          Serial.println(digitalRead(i));
        }
      }
    }
  }
}

void piano_update() {
  if(mode == INSTRUMENT) {
    bool a = false;
    for(int i= 0; i<NB_PIN; i++) {
      capacitance[i] = capList[i].capacitiveSensor(30);
      if(capacitance[i] > 200) {
        if(!isHolding) {
          int64_t file = 0x0F00000 + (i+1);
          file = file | (instr << 8);
          int32_t text = file & 0xFFFF;
          mp3_command(CMD_PLAY_WITH_FOLDER, file);
        }
        a=true;
        break;
      }
    }
    // Serial.println("");
    isHolding = a;
   
  }  else {
    for(int i = 0; i < NB_PIN; i++) {
      bool a = false;
      capacitance[i] = capList[i].capacitiveSensor(30);
      // Serial.print(capacitance[i]);
      // Serial.print(" : ");
      
      if(capacitance[i] > 250) {
        if(!isHolding) {
          if(compteur > 39) {
            display.clearDisplay();

            display.setTextColor(SSD1306_WHITE);
            display.setTextSize(1);

            display.setCursor(0,0);
            display.write("Bravo !");

            display.display();

            for(int j = 0; j < 10; j++) {
              turnEveryLedOn(255, 0, 255);
              delay(60);
              turnOffLed();
              delay(60);
            }
            return;
          }
          if(i != b) {
            // turnOffLed();
            strip.show();
            Serial.print(b);
            Serial.print(" / ");
            Serial.println(capacitance[i]);
            break;
          }
          mp3_command(CMD_PLAY_WITH_FOLDER, 0x0F00400 + (compteur + 1));
          compteur++;
          b = random(NB_PIN);
          turnOffLed();
          turnOnLed(b);
          strip.show();
          break;
        }
        a = true;
      }
      // Serial.println("");
      isHolding = a;
    }
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

void turnOnLed(int i){
  int num_led;
  if(i < 8) {
    num_led = i*3;
  } else {
    num_led = 49 - (i-8)*3;
  }
  Serial.println(num_led);
  strip.setPixelColor(num_led, 255, 0, 150);
  strip.show();
}

void turnEveryLedOn(int red, int green, int blue) {
  for(int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, red, green, blue);
  }
  strip.show();
}
