#include <Adafruit_NeoPixel.h>
 
#define NUMPIXELS 44
#define PIN 31
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int j = 0;


void setup() {
  strip.begin();
  strip.show(); // Initialise toute les led à 'off'
}
 
void loop() {
    for(int i = 0; i < j; i++ ) { // On fait une boucle pour définir la couleur de chaque led
        strip.setPixelColor(i, 0, 0, 255);       
    }
    j++;
    strip.show();
    delay(500);
}
