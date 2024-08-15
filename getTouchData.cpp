#include <Arduino.h>
#include <Terrabox_Widgets.h>
#include <TouchScreen.h>

#define DEBUG_SCREEN	0

/*==================================================================================
 *
 *  getTouch gets touch data, like is it pressed and the press its X, Y coordinates
 *
 *================================================================================*/

//
// ALL Touch panels and wiring is DIFFERENT
// Some magic numbers that worked for my development
//
const int TS_LEFT = 153, TS_RT = 993, TS_TOP = 948, TS_BOT = 169;
const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341

/*-----------------------------------------------------------------------------------
 *
 *  Create the touch screen driver, with data that works for me
 *
 *  TODO Make a way to calibrate the screen in a setup procedure.
 *  TODO Safe the calibration data in flash memory
 *  TODO Retrieve the calibration data for use with the touch screen initialization
 *
 *---------------------------------------------------------------------------------*/
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  // The AdaFruit touch screen!!!

/*-----------------------------------------------------------------------------------
 *
 *  This conventional C-function will get the desired touch data for you.
 *  In order to do so you have to use code similar to the code below.
 *
 *  struct XY data;
 *
 *  bool result = getTouchData(&data);
 *  if (result) {
 *  }
 *
 *
 *  theTouch     The struct of type XY within which the touch data is passed,
 *               The data is only valid if getTouchData() returns true.
 *  
 *---------------------------------------------------------------------------------*/
bool getTouchData(XY* theTouch)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);

    boolean pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);

    if (pressed) {

      #if DEBUG_SCREEN
        Serial.print(F("getTouchData() Screen width, height = "));
        Serial.print(Screen.width);
        Serial.print(F(", "));
        Serial.println(Screen.height);
      #endif

      theTouch->x    = map(p.x, TS_LEFT, TS_RT, 0, Screen.width);
      theTouch->y    = map(p.y, TS_TOP, TS_BOT, 0, Screen.height);

    }
    else {

      theTouch->x = 0;	// No X coordinate
      theTouch->y = 0;	// No Y coordinate 

    }

    return pressed;
}

/*-----------------------------------------------------------------------------------
 *
 *  This conventional C-function will gets raw touch data for you.
 *  In order to do so you have to use code similar to the code below.
 *
 *  struct XY data;
 *
 *  bool result = getTouchData(&data);
 *  if (result) {
 *  }
 *
 *
 *  theTouch     The struct of type XY within which the raw touch data is passed,
 *               The data is only valid if getRawTouchData() returns true.
 *  
 *---------------------------------------------------------------------------------*/
bool getRawTouchData(XY* theTouch)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);

    boolean pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {

      #if DEBUG_SCREEN
        Serial.print(F("getRawTouchData() Screen width, height = "));
        Serial.print(Screen.width);
        Serial.print(F(", "));
        Serial.println(Screen.height);
        Serial.print(F("getRawTouchData() (x, y) = "));
        Serial.print(p.x);
        Serial.print(F(", "));
        Serial.println(p.y);
    #endif

      theTouch->x = p.x;
      theTouch->y = p.y;

    }
    else {

      theTouch->x = -1;	// No X coordinate
      theTouch->y = -1;	// No Y coordinate 

    }

    return pressed;
}

