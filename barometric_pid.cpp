//I am using the Adafruit_BMP180 as my Barometric Sensor. The Adafruit_BMP085 library is compatible with the Adafruit_BMP180.
#include <Adafruit_BMP085.h>
#define T0 288.15f
#define L 0.0065f
#define M 0.0289644f
#define R 8.3144598f
bool measuredBasePressure = false;
bool BMPconnected = false;

const float exponent = (R * L) / (M * g);

Adafruit_BMP085 BMP;
float heightNow = 0;
float height = 0;
float initialPressure;
constexpr float confidence = 0.05;


void setup() {
    Serial.begin(9600);

    //I'm guessing BMP.begin() is bool
    if (BMP.begin()) Serial.println("BMP180 found and connected.");
    else Serial.println("BMP180 not found! Check connections.");
}


void loop() {
    if (!measuredBasePressure) 
    {
        Serial.println("Place device at base and type go.");
        while (!Serial.available())
        {
            //wait
        }
        initialPressure = BMP.readPressure();
        measuredBasePressure = true;
    }
    else
    {
        heightNow = findHeight(initialPressure);
        //low pass filter smoothening
        height += confidence * (heightnow - height);
        //-3 and 3 are the range of graph I want to display. The graph will also display the height and heightNow lines.
        Serial.print(-3);
        Serial.print(",");
        Serial.print(3);
        Serial.print(",");
        Serial.print(height);
        Serial.print(",");
        Serial.println(heightNow);
        delay(25);
    }


}




//barometric formula for height: h = (T0/L)*(1-(Ph/P0)^((R*L)/(M*g)))
/*Returns the current height of the rocket compared to the reference point in feet */
float findHeight(float initialPressure)
{
    float Ph = BMP.readPressure();
    //gravity in m/s^2
    float P0 = initialPressure;

    float height = (T0 / L) * (1 - pow((Ph / P0), exponent));
    //convert to feet
    height *= 3.28084;
    return height;
}

