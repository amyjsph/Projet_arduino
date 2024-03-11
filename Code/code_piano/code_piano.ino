#include <CapacitiveSensor.h>
#include <SoftwareSerial.h>


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





void setup()
{
  Serial.begin(9600);
  mp3.begin(9600);
  delay(500);

  mp3_command(CMD_SEL_DEV, DEV_TF);  // select the TF card
  delay(200);

  mp3_command(CMD_SET_VOLUME,50);

  //mp3_command(CMD_PLAY_WITH_FOLDER, 0x0F00102);       // Play mp3
  // mp3_command(CMD_PAUSE, 0x0000);      // Pause mp3
  //mp3_command(CMD_PLAY_NEXT, 0x0000);  // Play next mp3
  //mp3_command(CMD_PLAY_PREV, 0x0000);  // Play previous mp3
  

}

void loop()                    
{
  bool a = false;
  for(int i= 0; i<NB_PIN; i++) {
    capacitance[i] = capList[i].capacitiveSensor(30);
    Serial.print(capacitance[i]);
    Serial.print(" / ");
    
    if(capacitance[i] > 200) {
      if(!isHolding) {
        mp3_command(CMD_PLAY_WITH_FOLDER, 0x0F00200 + (i+1));
      }
      a=true;
      break;
    }
  }
  Serial.println("");
  isHolding = a;
   
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


