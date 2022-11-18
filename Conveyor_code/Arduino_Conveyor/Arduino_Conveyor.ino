
// 2 sides for each conveyor, L_16, R_16_error
#define StationsNum 20 // 20 stations on each side

double conveyor_Speed = 100;  // 10mm/sec
double RGB_distance = 100 / 3; // 33.3mm distance between each RGB module (mm)



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NeoPixel library
#include <Adafruit_NeoPixel.h>

#define PIN        1
#define NUMPIXELS StationsNum

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500




int pos_light[StationsNum][3];
unsigned long last_RGB_delay = 0;

void setup() {
  pixels.begin();
  pixels.clear();
  Serial.begin(115200);

  pos_light[0][0] = 0;
  pos_light[0][1] = 100;
  pos_light[0][2] = 0;
  for (int i = 0; i < 20; i++)
  {
    pixels.setPixelColor(i, pixels.Color(pos_light[i][0], pos_light[i][1], pos_light[i][2]));
  }
}

void loop() {
  //  Serial_signal_retriever();

  unsigned long RGB_cycle_time = (RGB_distance * 1000) / conveyor_Speed ; // cycle time in ms

  //  Serial.println('1');
  if ((millis() - last_RGB_delay) >= RGB_cycle_time )
  {
    Serial.println(millis());

    for (int i = StationsNum - 2; i >= 0; i--)
    {
      pos_light[i + 1][0] = pos_light[i][0];
      pos_light[i + 1][1] = pos_light[i][1];
      pos_light[i + 1][2] = pos_light[i][2];
      pixels.setPixelColor(i + 1, pixels.Color(pos_light[i][0], pos_light[i][1], pos_light[i][2]));
    }
    pos_light[0][0] = 0;
    pos_light[0][1] = 0;
    pos_light[0][2] = 0;
    pixels.setPixelColor(0, pixels.Color(pos_light[0][0], pos_light[0][1], pos_light[0][2]));
    last_RGB_delay = millis();
  }


  pixels.show();

  //  for(int i=0; i<NUMPIXELS; i++) {
  //
  //    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
  //    pixels.show();
  //    delay(DELAYVAL);
  //  }



}





void Serial_signal_retriever()
{
  if (Serial.available() > 0) {
    String Data = Serial.readStringUntil('\n');
    Serial.print("You sent me: ");
    Serial.println(Data);

    int index = Data.indexOf('_');
    int length = Data.length();
    String sub_S1 = Data.substring(0, index);
    String sub_S2 = Data.substring(index + 1, length);

    Serial.println(Data[0]);
    Serial.println(Position_from_string(Data));
  }
}

int Position_from_string(String Data)
{
  int index = Data.indexOf('_');
  int length = Data.length();
  String Pos_data = Data.substring(index + 1, length);
  int Pos_int = Pos_data.toInt();
  return Pos_int;
}
