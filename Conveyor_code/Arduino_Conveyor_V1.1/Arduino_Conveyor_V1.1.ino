
// 2 sides for each conveyor, L_16, R_16_error
#define totalStationsNum 20 // 20 stations on each side

double conveyor_Speed = 100;  // 10mm/sec
double RGB_distance = 100 / 3; // 33.3mm distance between each RGB module (mm)



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NeoPixel library
#include <Adafruit_NeoPixel.h>

#define PIN        1
#define NUMPIXELS totalStationsNum

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);




int pos_light[totalStationsNum]; // hold box position info
unsigned long last_RGB_delay = 0;

void setup() {
  pixels.begin();
  pixels.clear();

  for (int i = 0; i < totalStationsNum; i++)
  {
    pos_light[i] = -1; //empty place
  }
  pos_light[0] = 5;
  //  pos_light[0][0] = 0;
  //  pos_light[0][1] = 100;
  //  pos_light[0][2] = 0;
  //  for (int i = 0; i < 20; i++)
  //  {
  //    pixels.setPixelColor(i, pixels.Color(pos_light[i][0], pos_light[i][1], pos_light[i][2]));
  //  }

  pixels.show();

  Serial.begin(115200);
  //  while (!Serial) {
  //    ; // wait for port to be ready
  //  }


  //  unsigned int before = millis();
  //  DisplayRGB();
  //  Serial.println(millis() - before);

  //  Serial.begin(115200);
  //  while (!Serial) {
  //    ; // wait for port to be ready
  //  }
  pinMode(2, OUTPUT);
  //  digitalWrite(2, HIGH);
}

void loop() {
  int val = Serial_signal_retriever();
  if (val != 0)  Serial.println(val);
  //  else DisplayRGB();

  //  unsigned long RGB_cycle_time = 1000;
  unsigned long RGB_cycle_time = (RGB_distance * 1000) / conveyor_Speed ; // cycle time in ms

  //  Serial.println('1');
  if ((millis() - last_RGB_delay) >= RGB_cycle_time )
  {
    //    Serial.println("1");
    //      Serial.println(millis());
    for (int i = totalStationsNum - 2; i >= 0; i--)
    {
      pos_light[i + 1] = pos_light[i];
    }
    pos_light[0] = -1;

    //    unsigned int before = millis();
    digitalWrite(2, pixels.canShow());
    DisplayRGB();
    //    Serial.println(millis()-before);
    last_RGB_delay = millis();
    digitalWrite(2, pixels.canShow());
//    Serial.println(pixels.canShow());

  }

  //  unsigned int before = millis();
  //    DisplayRGB();
  //  Serial.println(millis() - before);
}

void DisplayRGB()
{

  for (int station = 0; station < 20; station++)
  {
    int pos_equal_station = (station == pos_light[station]);

    if (pos_equal_station)
    {
      pixels.setPixelColor(station, pixels.Color(0, 100 , 0 ));  //green
    }
    else if (pos_light[station] != -1)
    {
      pixels.setPixelColor(station, pixels.Color(0, 0 , 20 ));  //blue
    }
    else
    {
      pixels.setPixelColor(station, pixels.Color(0, 0 , 0 ));  //red
    }

    pixels.show();
  }
}



int Serial_signal_retriever()
{
  int val = 0;
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

    val = Position_from_string(Data);
    switch (Data[0]) {
      case 'L':
        val = -val;
        break;
      case 'R':
        val = val ;
        break;
      default:
        val = -100;
        break;


    }
  }
  //  else DisplayRGB();
  return val;
}

int Position_from_string(String Data)
{
  int index = Data.indexOf('_');
  int length = Data.length();
  String Pos_data = Data.substring(index + 1, length);
  int Pos_int = Pos_data.toInt();
  return Pos_int;
}
