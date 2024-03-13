#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 32, &Wire, -1);

#define SCREEN_ADDRESS 0x3C

enum State {
  MAIN, SELECT_MENU
};

int instrument;

State current_state = MAIN;

void setup() {
  Serial.begin(9600);

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
  if(current_state == MAIN) {
    for(int i = 22;i <= 26; i+=2) {
      if(digitalRead(i)) {
        current_state = SELECT_MENU;
        display.clearDisplay();
        
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(1);

        display.setCursor(0, 0);
        display.write("1) Piano");

        display.setCursor(0, 10);
        display.write("2) Clarinette");
        
        display.setCursor(0, 20);
        display.write("3) Trompette");
        
        display.display();
        while(digitalRead(i)) {
          Serial.print(digitalRead(i));
        }
      }
    }
  } else if(current_state == SELECT_MENU) {
    for(int i = 22;i <= 26; i+=2) {
      if(digitalRead(i)) {
        instrument = i - 22;
        display.clearDisplay();
        
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(1);

        display.setCursor(0, 0);
        display.write("Playing : ");

        display.setCursor(0, 10);
        if(instrument == 0) {
          display.write("Clarinette");
        } else if(instrument == 2) {
          display.write("Trompette");
        } else if(instrument == 4) {
          display.write("Piano");
        }

        display.display();
        
        current_state = MAIN;
        while(digitalRead(i)) {
          Serial.print(digitalRead(i));
        }
      }
    }
  }
}