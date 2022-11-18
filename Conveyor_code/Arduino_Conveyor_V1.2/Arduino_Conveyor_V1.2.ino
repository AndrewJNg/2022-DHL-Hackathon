
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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Servo.h>
Servo swingArm;  // create servo object to control a servo
# define servoPin  18


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int pos_light[totalStationsNum]; // hold box position info
unsigned long last_RGB_delay = 0;

void setup() {
  pixels.begin();
  pixels.clear();

  swingArm.attach(servoPin);  // attaches the servo on pin 13 to the servo object
  for (int i = 0; i < totalStationsNum; i++)
  {
    pos_light[i] = -1; //empty place
  }
  pos_light[0] = 5;

  pixels.show();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for port to be ready
  }

  pinMode(2, OUTPUT);
  //  digitalWrite(2, HIGH);
}

void loop() {
  int val = Serial_signal_retriever();
//  if (val != 0)
//  {
//    Serial.println(val);
//  }




  //  unsigned long RGB_cycle_time = 1000;
  unsigned long RGB_cycle_time = (RGB_distance * 1000) / conveyor_Speed ; // cycle time in ms

  //  Serial.println('1');
  if ((millis() - last_RGB_delay) >= RGB_cycle_time )
  {
    for (int i = totalStationsNum - 2; i >= 0; i--)
    {
      pos_light[i + 1] = pos_light[i];
    }
    if (val != 0)
    {
      pos_light[0] = abs(val);
    }
    else
    {
      pos_light[0] = -1;
    }

    digitalWrite(2, pixels.canShow());
    last_RGB_delay = millis();
    DisplayRGB();
    digitalWrite(2, pixels.canShow());
    
    // servo control
    if(val<0)
    {
      swingArm.write(0);
    }
    else if(val>0)
    {
      swingArm.write(180);
    }
    else
    {
      swingArm.write(90);
    }
  }

}

void DisplayRGB()
{

  for (int station = 0; station < 20; station++)
  {
    int pos_equal_station = (station == pos_light[station]);

    if (pos_equal_station)
    {
      pixels.setPixelColor(station, pixels.Color(0, 10 , 0 ));  //green for pickup
    }
    else if (pos_light[station] != -1)
    {
      pixels.setPixelColor(station, pixels.Color(0, 0 , 10 ));  //blue for package location
    }
    else if (pos_light[station] != -1)
    {
      pixels.setPixelColor(station, pixels.Color(10, 0 , 0 ));  //red for error package location
    }
    else
    {
      pixels.setPixelColor(station, pixels.Color(0, 0 , 0 ));  //black for empty
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

    val = Position_from_string(Data)+1;
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
  //    DisplayRGB();
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
