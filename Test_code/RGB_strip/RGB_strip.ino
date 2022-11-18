
// 2 sides for each conveyor, L_16, R_16_error
#define StationsNum 20 // 20 stations on each side



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NeoPixel library
#include <Adafruit_NeoPixel.h>

#define PIN        1
#define NUMPIXELS StationsNum 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500



double RGB_distance = 100/3; // 33.3mm distance between each RGB module (mm)



void setup() {
  Serial.begin(115200);
  
  pixels.begin();
}

void loop() {
  pixels.clear();

  for(int i=0; i<NUMPIXELS; i++) {

    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(DELAYVAL);
  }



}

void Serial_signal_retriever()
{

}
  
