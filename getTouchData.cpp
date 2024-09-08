/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <getTouchData.cpp> - Library forGUI Widgets.
                               16 Aug 2024
                      Released into the public domain
                as GitHub project: TerraboxNL/TerraBox_Widgets
                   under the GNU General public license V3.0

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <https://www.gnu.org/licenses/>.

 *---------------------------------------------------------------------------*
 *
 *  C H A N G E  L O G :
 *  ==========================================================================
 *  P0001 - Initial release
 *  ==========================================================================
 *
 *--------------------------------------------------------------------------*/
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
	//
	// TODO Loop around to measure the time is was pressed.
	// This feature is needed to distinguish between a short and
	// a long press.
	//
	// TODO Create the XYPress class, which is suited toward touch data.
	// Current the XY can pnly return x and y position. To support
	// a long touch, the time pressed and the pressure must be returned.
	// Returning the pressure could additinally distinguish between
	// a SOFT press and a HARD press and everithing in between.
	//
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
 *  bool result = getRawTouchData(&data);
 *  if (result) {
 *  }
 *
 *
 *  theTouch     The struct of type XY within which the raw touch data is passed,
 *               The data is only valid if getRawTouchData() returns true.
 *  
 *---------------------------------------------------------------------------------*/
bool     pressedState   = false;
uint32_t unpressedStart = 0;
TSPoint  pLast(0, 0, 0);
bool getRawTouchData(XY* theTouch)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);

    boolean pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);

    //
    //  Filter for short dips in the pressed state
    //
    if (! pressed) {

      uint32_t now = millis();

      if (pressedState) {
    	  pressedState   = false;
    	  unpressedStart = now;
      }

      if (now - unpressedStart < 15) {
    	pressed = true;
    	p.x     = pLast.x;
    	p.y     = pLast.y;
    	p.z     = pLast.z;
      }
    }
    else {
    	if (!pressedState) {
    		pressedState = true;
    	}

    	pLast = p;
    }

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
      theTouch->z = p.z;  // Pressure

    }
    else {

      theTouch->x = -1;	// No X coordinate
      theTouch->y = -1;	// No Y coordinate 
      theTouch->z = 0;

    }

    return pressed;
}

/**---------------------------------------------------------------------------
 *
 *  Wait for a full tap. Implying detect low/high and high/low transition
 *
 *--------------------------------------------------------------------------*/
void waitForATap() {
	XY xy;
	while (!getRawTouchData(&xy));
	while ( getRawTouchData(&xy));
}

/**----------------------------------------------------------------------------
 *
 *  Wait for a tap with a visual count down.
 *
 * @param   seconds     Time out in seconds.
 * @return              True implies a tap within time out. False no tap.
 *
 *---------------------------------------------------------------------------*/
bool countDownWait(uint16_t seconds) {
    //
    // Time out loop of 3.25 seconds
    //
    unsigned long timeout = 1000;
    uint16_t      counter = 0;
    uint32_t      now     = millis();
    XY xy;

    while (!getRawTouchData(&xy)) {

      delay(20);

      unsigned long waiting = millis() - now;
      if (waiting > timeout) {
        Screen.tft->print(++counter);
        Screen.tft->print(F(" "));
        timeout += 1000;
      }

      //
      // Timeout so return
      //
      if (counter > seconds) {
        return false;
      }
    }

    //
    // Wait for high/low edge
    //
    while (getRawTouchData(&xy));

    return true;
}
