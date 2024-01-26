#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */


CapacitiveSensor   cs_52_2 = CapacitiveSensor(53,51);        // 10M resistor between pins 22 & 2, pin 2 is sensor pin, add a wire and or foil if desired

void setup()                    
{
   cs_52_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);
}

void loop()                    
{
    long start = millis();
    long total1 =  cs_52_2.capacitiveSensor(30);
    Serial.println(total1);
    if(total1 > 500) {
      digitalWrite(8, HIGH);
    } else {
      digitalWrite(8, LOW);
    }

    delay(100);                             // arbitrary delay to limit data to serial port 
}
