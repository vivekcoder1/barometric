//I am using the Adafruit_BMP180 as my Barometric Sensor. The Adafruit_BMP085 library is compatible with the Adafruit_BMP180.
#include <Adafruit_BMP085.h>
bool measuredBasePressure = false;
bool BMPconnected = false;


Adafruit_BMP085 BMP;
float heightNow = 0;
float height = 0;
float initialPressure;
float confidence = 0.05;


void setup() {
    Serial.begin(9600);
    BMPconnected = BMP.begin();


    if (BMPconnected)
    {
        Serial.println("BMP180 found and connected.");
    }
    else
    {
        Serial.println("BMP180 not found! Check connections.");
    }
}


void loop() {
    if (measuredBasePressure == false)
    {
        Serial.println("Place device at base and type go.");
        while (Serial.available() == 0)
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
        height = height * (1 - confidence) + confidence * heightNow;
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
/**
*Returns the current height of the rocket compared to the reference point in feet
*/
float findHeight(float initialPressure)
{
    //reference tempertature at sea level in kelvin
    float T0 = 288.15;
    //temperature lapse rate in kelvin/m
    float L = 0.0065;
    //molar mass of air in KJ/mol
    float M = 0.0289644;
    //universal gas constant in J/mol*K
    float R = 8.3144598;
    //base pressure in pascals
    float P0 = initialPressure;
    //pressure at height in pascals
    float Ph = BMP.readPressure();
    //gravity in m/s^2
    float g = 9.8;
    //height above base in meters
    float height;


    float exponent = (R * L) / (M * g);
    height = (T0 / L) * (1 - pow((Ph / P0), exponent));
    //convert to feet
    height *= 3.28084;
    return height;
}

