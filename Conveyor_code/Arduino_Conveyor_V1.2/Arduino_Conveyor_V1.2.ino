
// 2 sides for each conveyor, L_16, R_16_error
#define totalStationsNum 10 // 10 stations on each side
#define laneNum 2 // 2 lanes for package collection

double conveyor_Speed = 100;  // 10mm/sec
double RGB_distance = 100 / 3; // 33.3mm distance between each RGB module (mm)



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NeoPixel library
#include <Adafruit_NeoPixel.h>

#define RGB_left_PIN        19
#define RGB_right_PIN        1
#define NUMPIXELS totalStationsNum

Adafruit_NeoPixel left_RGB_Lane(NUMPIXELS, RGB_left_PIN , NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel right_RGB_Lane(NUMPIXELS, RGB_right_PIN, NEO_GRB + NEO_KHZ800);


byte package_RGB_output[4][3] = {
  {0, 10, 0} //green for pickup
  , {0, 0, 10} //blue for package location
  , {10, 0, 0} //red for error package location
  , {0, 0, 0} //black for empty
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Servo.h>
Servo swingArm;  // create servo object to control a servo
# define servoPin  18


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int pos_light[totalStationsNum][laneNum]; // hold box position info       {-1 for empty space, 0-9 for positions, from 1-10 (station number) }
unsigned long last_RGB_delay = 0;

void setup() {
  left_RGB_Lane.begin();
  left_RGB_Lane.clear();

  right_RGB_Lane.begin();
  right_RGB_Lane.clear();

  swingArm.attach(servoPin);  // attaches servo pin

  // set all to 0
  for (int i = 0; i < totalStationsNum; i++)
    for (int j = 0; j < laneNum; j++)
      pos_light[i][j] = -1; //empty place

  pos_light[0][0] = 5 - 1; //set test as station 5 and 8 place
  pos_light[0][1] = 8 - 1;

  left_RGB_Lane.show();
  right_RGB_Lane.show();

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for port to be ready
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  int serial_input = Serial_signal_retriever();

  //  unsigned long RGB_cycle_time = 1000;
  unsigned long RGB_cycle_time = (RGB_distance * 1000) / conveyor_Speed ; // cycle time in ms

  //  Serial.println('1');
  if ((millis() - last_RGB_delay) >= RGB_cycle_time )
  {
    for (int j = 0; j < laneNum; j++)
    {
      for (int i = totalStationsNum - 2; i >= 0; i--)
      {
        pos_light[i + 1][j] = pos_light[i][j];
      }
    }


    // decide next input from serial input
    if (serial_input != 0)
    {
      if (serial_input < 0)      //right lane
      {
        pos_light[0][1] = abs(serial_input) - 2;
      }
      else if (serial_input > 0)  //left lane
      {
        pos_light[0][0] = abs(serial_input) - 2;
      }
    }
    else
    {
      pos_light[0][0] = -1;
      pos_light[0][1] = -1;
    }

    digitalWrite(2, left_RGB_Lane.canShow());
    last_RGB_delay = millis();
    DisplayRGB();
    digitalWrite(2, left_RGB_Lane.canShow());

    // servo control
    if (serial_input < 0)
    {
      swingArm.write(0);
    }
    else if (serial_input > 0)
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

  int j = 0;
  for (int station = 0; station < totalStationsNum; station++)
  {
    int pos_equal_station = (station == pos_light[station][j]);

    if (pos_equal_station)
    {
      left_RGB_Lane.setPixelColor(station, left_RGB_Lane.Color(0, 10 , 0 ));  //green for pickup
    }
    else if (pos_light[station][j] != -1)
    {
      left_RGB_Lane.setPixelColor(station, left_RGB_Lane.Color(0, 0 , 10 ));  //blue for package location
    }
    //    else if (pos_light[station][j] != -1)
    //    {
    //      left_RGB_Lane.setPixelColor(station, left_RGB_Lane.Color(10, 0 , 0 ));  //red for error package location
    //    }
    else
    {
      left_RGB_Lane.setPixelColor(station, left_RGB_Lane.Color(0, 0 , 0 ));  //black for empty
    }
  }

  j = 1;
  for (int station = 0; station < totalStationsNum; station++)
  {
    int pos_equal_station = (station == pos_light[station][j]);

    if (pos_equal_station)
    {
      right_RGB_Lane.setPixelColor(station, right_RGB_Lane.Color(0, 10 , 0 ));  //green for pickup
    }
    else if (pos_light[station][j] != -1)
    {
      right_RGB_Lane.setPixelColor(station, right_RGB_Lane.Color(0, 0 , 10 ));  //blue for package location
    }
    //    else if (pos_light[station][j] != -1)
    //    {
    //      right_RGB_Lane.setPixelColor(station, right_RGB_Lane.Color(10, 0 , 0 ));  //red for error package location
    //    }
    else
    {
      right_RGB_Lane.setPixelColor(station, right_RGB_Lane.Color(0, 0 , 0 ));  //black for empty
    }
  }
  left_RGB_Lane.show();
  right_RGB_Lane.show();
}



int Serial_signal_retriever()
{
  int serial_input = 0;
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

    serial_input = Position_from_string(Data) + 1;
    switch (Data[0]) {
      case 'L':
        serial_input = -serial_input;
        break;
      case 'R':
        serial_input = serial_input ;
        break;
      default:
        serial_input = -100;
        break;
    }
  }
  //    DisplayRGB();
  return serial_input;
}

int Position_from_string(String Data)
{
  int index = Data.indexOf('_');
  int length = Data.length();
  String Pos_data = Data.substring(index + 1, length);
  int Pos_int = Pos_data.toInt();
  return Pos_int;
}
