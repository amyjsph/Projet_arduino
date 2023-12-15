#include <Adafruit_NeoPixel.h>
 
// Parameter 32 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
int X=54;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(X, 6, NEO_GRB + NEO_KHZ800);
void setup() {
  strip.begin();
  strip.show(); // Initialise toute les led à 'off'
   
}
 
void loop() {
    for(int i = 0; i < X; i++ ) { // On fait une boucle pour définir la couleur de chaque led
        // setPixelColor(n° de led, Rouge, Vert, Bleu)
        strip.setPixelColor(i, random(0, 223), random(0, 223), random(0, 223));       
    }
    strip.show(); // on affiche 
    delay(1000);
    for(int i = 0; i < X; i++ ) { // On fait une boucle pour définir la couleur de chaque led
        // setPixelColor(n° de led, Rouge, Vert, Bleu)
    strip.setPixelColor(i, 223, 0, 0);       
    }
    strip.show(); // on affiche 
    delay(1000);
    for(int i = 0; i < X; i++ ) { // On fait une boucle pour définir la couleur de chaque led
        // setPixelColor(n° de led, Rouge, Vert, Bleu)
    strip.setPixelColor(i, 0, 223, 0);       
    }
    strip.show(); // on affiche 
    delay(1000);
    for(int i = 0; i < X; i++ ) { // On fait une boucle pour définir la couleur de chaque led
        // setPixelColor(n° de led, Rouge, Vert, Bleu)
    strip.setPixelColor(i, 0, 0, 223);       
    }
    strip.show(); // on affiche 
    delay(1000);
}
