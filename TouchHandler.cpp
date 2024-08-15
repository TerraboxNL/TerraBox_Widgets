/*-------------------------------------------------------------------------------------------------


       ///////  ////////  ///////   ///////    //////       //////     /////    //    //
         //    //        //    //  //    //  //    //      //   //   //    //   // //
        //    //////    ///////   ///////   ////////      //////    //    //     //
       //    //        //  //    //  //    //    //      //    //  //    //    // //
      //    ////////  //    //  //    //  //    //      ///////     /////    //   //

     
                           A R D U I N O   G U I   W I D G E T S


                             (C) 2024, cor.hofman@terrabox.nl

                       <TouchHandler.cpp> - Library for GUI widgets.
                          Created by Cor Hofman, June 30, 2024
                            Released into the public domain
                              as GitHub project: TBH_A_GUI
                       under the GNU General public license V3.0
                          

 *------------------------------------------------------------------------------------------------*
 *
 *  C H A N G E  L O G :
 *  ============================================================================================
 *  P0001 - Initial release 
 *  ============================================================================================
 *
 *------------------------------------------------------------------------------------------------*/
#include <TerraBox_Widgets.h>

#define DEBUG	          0
#define DEBUG_DIGEST       0

#define XY_DELTA_THRESHOLD 5	// Ignore touch XY's which are LEQ than this

/*-----------------------------------------------------------------------------
 *
 * Create a touch handler.
 *  If scheduled as a Task its task name is TouchHandler, its cycleTim is 10ms
 *
 *---------------------------------------------------------------------------*/
TouchHandler::TouchHandler() :
              Task("TouchHandler", 10) {
  lastTimestamp = millis();
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Main entry point if managed as a scheduled task
 *
 *------------------------------------------------------------------------------------------------*/
void TouchHandler::exec() {
  digest();
}

/*--------------------------------------------------------------------------------------------------
 *
 *  If tapped it returns true, otherwise false.
 *
 *------------------------------------------------------------------------------------------------*/
bool TouchHandler::tapOrTimeout(long timeout) {

    //
    // Return the coordinates in
    //
    XY xy;

    //
    //  Remember when the timeout period started.
    //
    unsigned long now = millis();
    long      seconds = 0;
    
    //
    //  Stay in the loop waiting until the timeout
    //
    while (millis() - now < timeout) {

      //
      // 
      unsigned long now2 = millis();
      while (millis() - now2 < 1000){
        if (getRawTouchData(&xy))
          return true;
      }

      Screen.tft->print(++seconds);
      Screen.tft->print(" ");

    }

    //
    // If we are here then we timed out.
    //
    return false;
}

/*---------------------------------------------------------------------------------------
 *
 *  Assign the X axis arrays with calibration marker data
 * 
 *  size      The number of markers
 *  markers   The array with size markers
 *
 *-------------------------------------------------------------------------------------*/
void TouchHandler::setXCalibration(uint16_t size, uint16_t* markers) {

  xSize        = size;
  xCalibration = markers;

  rawAvgXMarkerDistance = (float)(markers[xSize-1] - markers[0])/ (float)xSize; 

}

/*---------------------------------------------------------------------------------------
 *
 *  Assign the Y axis arrays with calibration marker data
 * 
 *  size      The number of markers
 *  markers   The array with size markers
 *
 *-------------------------------------------------------------------------------------*/
void TouchHandler::setYCalibration(uint16_t size, uint16_t* markers) {

  ySize        = size;
  yCalibration = markers;

  rawAvgYMarkerDistance = (float)(markers[ySize-1] - markers[0])/ (float)ySize; 

}

/*---------------------------------------------------------------------------------------
 *
 *  Assign the distance between two markers expressed in pixels.
 *  The marker distance is equally sized for X and Y axises.
 *
 *  distance      The distance in pixels
 *
 *-------------------------------------------------------------------------------------*/
void TouchHandler::setMarkerDistance(uint16_t distance) {

  markerDistance = distance;

}

/*---------------------------------------------------------------------------------------
 *
 *  Normalize a single coordinate.
 *
 *  raw             The raw value to normalize
 *  data            The array of raw calibrated marker values for the pixel cells
 *  size            The number of markers
 *  avgRawCellSize  The average raw value range between two markers
 *
 *-------------------------------------------------------------------------------------*/
uint16_t TouchHandler::normalize(uint16_t raw, uint16_t* data, uint16_t size, float avgRawMarkerDistance) {
  Serial.print(F("Raw: ")); Serial.println(raw);

  //
  // Get the rotation
  //
  uint8_t rotation = Screen.tft->getRotation();

  //
  // find minimum and maximum values
  //
  uint16_t minimum = data[0];
  uint16_t maximum = data[size-1];

  //
  //  Limit the coordinate range to the lowest pixel coordinate 
  //
  if (raw <= minimum) {
    return 0;
  }

  //
  // Limit the coordinate range to the highest pixel coordinate
  // 
  if (raw >= maximum) {
    return (markerDistance * (size-1))-1;
  }

  //
  //  Estimate the marker index nearest to the touch.
  //
  int16_t markerIndex = round((float) (raw - minimum) / avgRawMarkerDistance);

  Serial.print(F("Raw: ")); Serial.println(raw);
  Serial.print(F("Calculated markerIndex: ")); Serial.println(markerIndex);

  //
  //  Check if we have the right cell.
  //  If not search for it.
  //
  //  But first keep the marker index from getting out of range
  //
  if (markerIndex >= size) {
    raw      = data[size-1];
    Serial.print(F("Raw corrected: ")); Serial.println(raw);
    markerIndex = size-1;
    Serial.print(F("markerIndex >= size: ")); Serial.println(markerIndex);
  }

  //
  // If we don not have an exact hit, the search up or downwards
  //
  else if (data[markerIndex] != raw) {

    //
    //  If the marker index is too low
    //  Find the first marker index where raw is >= data[markerIndex]
    //
    //  markerIndex = 1
    //          raw = 154
    //
    //   search
    //  direction index   marker
    //      |       0       54
    //      |       1      104    104 <= 154 (i.e. data[1]) -> true
    //      |       2      154    154 <= 154 (i.e. data[2]) -> true 
    //     \ /      3      204    204 <= 154 (i.e. data[3]) -> false  markerIndex--; -> markerIndex = 2
    //      V       4      254
    //
    if (data[markerIndex] <= raw) {
      while (markerIndex < size && data[markerIndex] <= raw) {
        markerIndex++;
      }
      markerIndex--;  // Adjust for the last one where raw < data[markerIndex] 
      Serial.print(F("Upwards search - index: ")); Serial.println(markerIndex);
    }

    //
    //  If the marker index is too high
    //  Find the first marker index where raw is >= data[markerIndex]
    //
    //  markerIndex = 4
    //          raw = 154
    //
    //   search
    //  direction index   marker
    //      .       0       54
    //     / \      1      104
    //      |       2      154    154 > 154  (i.e. data[2]) -> false -> marker Index = 2 
    //      |       3      204    204 > 154  (i.e. data[3]) -> true
    //      |       4      254    254 > 154  (i.e. data[4]) -> true
    //
    else if (data[markerIndex] > raw) {
      while (markerIndex >= 0 && data[markerIndex] > raw) {
        markerIndex--;
      }

      Serial.print(F("Downwards search - index: ")); Serial.println(markerIndex);
    }
  }

  //
  //  Special case
  //
  //  If at the end of the scale
  //  Then return highest possible pixel coordinate
  // 
  if (markerIndex == size - 1) {
     Serial.print(F("Result (Max coordinate): ")); Serial.println((markerDistance * (size-1))-1);
     return (markerDistance * (size-1))-1;
  }

  //  Otherwise calculate the pixel coordinate as follows:
  //
  //  raw values: 100  150  200  250  300  350  400  450   
  //  markerIndices:  0    1    2    3    4    5    6    7
  //                  |----|----|----|----|----|----|----|
  //                                  2222
  //                                  6789
  //                                  0000
  //                                   .
  //                                  /|\
  //                                   |
  //  raw touch value: 270 ------------'
  //  between marker index 3 and 4:
  //  lower boundary index is : 3
  //  rest of raw value between the indexed markers: 270-250 = 20
  //  raw value range between markers: 300 - 250 = 50 
  //  pixels between markers: 20
  //  formula: pixelCoordinate = (markerIndex * pixelsPerCel) + (pixelsBetweenMarkers * (restOfRawValue / celValueRange))
  //  pixel coordinate =         (    3    *     20      ) + (     20           *       (     20        /      50      ))
  //                   =                   60              + (     20           *                     0.4              )
  //                   =                   60              +                            8
  //                   =                                   68
  //                                                       .    
  //                                                      /|\
  //                                                       |
  //  returned normalised pixel coordinate = 68 -----------'  
  //

  Serial.print(F("data[markerIndex]  : ")); Serial.println(data[markerIndex]);
  Serial.print(F("data[markerIndex+1]: ")); Serial.println(data[markerIndex+1]);
  Serial.print(F("data[markerIndex+1] - data[markerIndex]")); Serial.println(data[markerIndex+1] - data[markerIndex]);
  Serial.print(F("raw - data[markerIndex]: ")); Serial.println(raw - data[markerIndex]);

  Serial.print(F("(A) markerDistance * markerIndex = ")); Serial.println(markerDistance * markerIndex);
  Serial.print(F("(B) raw - data[markerIndex] = ")); Serial.println(raw - data[markerIndex]);
  Serial.print(F("(C) data[markerIndex+1] - data[markerIndex] = ")); Serial.println(data[markerIndex+1] - data[markerIndex]);
  Serial.print(F("20 * ( B / C ) = ")); Serial.println(markerDistance * ( (float)(raw - data[markerIndex]) / (float)(data[markerIndex+1] - data[markerIndex]) ));
  uint16_t result = (markerDistance * markerIndex +                   // ( celIndex * 20 pixels) +            
           round( (float)markerDistance *                              //     ( 20 pixels *
             ( (float)(raw - data[markerIndex]) /                         //        ( raw part within the indexed raw data cel /    
                 (float)(data[markerIndex+1] - data[markerIndex]) ) ) ); //            raw value range of indexed raw data cel) )

  Serial.print(F(" A * 20 + (20 * (B / C)) = ")); Serial.println(result);

  return result;
}

/*---------------------------------------------------------------------------------------
 *
 *  Normalize so that the X,Y touch coordinates are equal to the screen coordinates.
 *
 *-------------------------------------------------------------------------------------*/
void TouchHandler::normalize(XY* touch) {

  Serial.println(F("X ---> "));
  touch->x = normalize(touch->x, xCalibration, xSize, rawAvgXMarkerDistance);
  Serial.println(F("Y ---> "));
  touch->y = normalize(touch->y, yCalibration, ySize, rawAvgYMarkerDistance);

}

/*---------------------------------------------------------------------------------------
 *
 *  Get touch data in screen coordinates, which is only valid if true is returned.
 *
 *  touchData      The XY structure within which the screen coordinates are returned.
 *
 *--------------------------------------------------------------------------------------*/
bool  TouchHandler::getTouch(XY* touchData) {
  bool pressedNow = getRawTouchData(touchData);	// Call C-function to gather the data

  if (getRawTouchData(touchData)) {
    normalize(touchData);
    return true;
  }

  touchData->x = 0;
  touchData->y = 0;

  return false;
}

/*----------------------------------------------------------------------------------------
 *
 *  Returns true if the screen is currently touched.
 *  Calling this method will also detect it an generate an UNTOUCH event and dispatches
 *  that event for it.
 *
 *--------------------------------------------------------------------------------------*/
TouchEvent* TouchHandler::digest( ){

  /**** G E T   F R E S H   T O U C H   D A T A *****************************************/

  XY touchData;			                    // Structure in which touch data is returned
  bool pressedNow = getTouch(&touchData);	// Call C-function to gather the data
   
  //
  // Ignore NON-PRESSED-> to NON-PRESSED state transitions
  //
  // Only allowing transitions:
  //   NON_PRESSED -> PRESSED
  //   PRESSED     -> PRESSED
  //   PRESSED     -> NON-PRESSED
  //
  //  Note that at this point:
  //  o pressed     STILL contains the PREVIOUS press state !!!!!!
  //  o touchData.pressed contains the CURRENT  press state !!!!!!
  //
  if (!pressed && !pressedNow) {

	//
	// Handle inactivity
	//
	if (Screen.isVisible() && millis() - lastTimestamp > inactivityTimeout) {
		Screen.setVisible(false);
		TouchEvent event = TouchEvent(TouchEvents::INACTIVITY_TIMEOUT, millis(), 0, 0, &Screen);
		Screen.dispatch(&event);
	}

    return nullptr;
  }
  else {
	 if (! Screen.isVisible()) {
	     TouchEvent event = TouchEvent(TouchEvents::WAKEUP, millis(), 0, 0, &Screen);
	     Screen.dispatch(&event);
	 }
  }

  //
  //  Calculate the de delta of the the current x and y, found in the touchData struct,
  //  and the last saved touch coordinates in x and y variables.
  //  If the delta (dx and dy) is too insignificant then we ignore this event
  //
  int16_t dx = x - touchData.x;
  if (dx < 0) dx = -dx;
  int16_t dy = y - touchData.y;
  if (dy < 0) dy = -dy;

  if (dx <= XY_DELTA_THRESHOLD && dy <= XY_DELTA_THRESHOLD) {
    return nullptr;
  }

  #if DEBUG_DIGEST
      Serial.print(F("TouchHandler::digest() Delta(s) calculated ("));
      Serial.print(x);
      Serial.print(F(","));
      Serial.print(y);
      Serial.print(F(")"));
      Serial.print(F(" dx: "));
      Serial.print(dx);
      Serial.print(F(" dy: "));
      Serial.println(dy);
  #endif

  //
  // Having filtered out the irrelevant press state changes,
  // we can do a saveState().
  // It is important to realise that the pressed, x, y still contains
  // the data of the PREVIOUS RELEVANT state we were in !!!!!
  //
  saveState();

  #if DEBUG_DIGEST
    Serial.print(F("TouchHandler::digest() L A S T         Pressed: "));
    Serial.print(lastPressed ? F("True") : F("False"));
    Serial.print(F(", lastEvent: "));
    Serial.print(lastEvent);
    Serial.print(F("  ("));
    Serial.print(lastX);
    Serial.print(F(", "));
    Serial.print(lastY);
    Serial.println(F(")"));
  #endif

  //
  //  Now, after filtering the stream of raw touch data,
  //  we take notice of the current touch state returned by getTouchData(&touchData)
  //  in the touchData struct
  //
  pressed = pressedNow;
  x       = touchData.x;
  y       = touchData.y;
   
  #if DEBUG_DIGEST
    Serial.print(F("TouchHandler::digest() C U R R E N T   Pressed: "));
    Serial.print(pressed ? "True" : "False");
    Serial.print(F(", Event: "));
    Serial.print(event);
    Serial.print(F("  ("));
    Serial.print(x);
    Serial.print(F(", "));
    Serial.print(y);
    Serial.println(F(")"));
  #endif

 /***** D I G E S T   T O U C H   D A T A **********************************************/

  event      = TouchEvents::NONE;	// By default we assume no event
  timestamp  = millis();		// Register a timestamp

  //------------------------------------------------------------------
  //
  //  If pressed handle it and generate events for it
  //
  //------------------------------------------------------------------  
  if (pressed) {

    source    = Screen.match(x, y);			// Obtain the source widget

    //
    //  T O U C H   E V E N T
    //  ---------------------
    //  Happens on a state transition NON-PRESSED -> PRESSED
    //
    if (! lastPressed) {
      #if DEBUG_DIGEST
        Serial.println(F("TouchHandler::digest() TOUCH detected"));
      #endif

      event    = TouchEvents::TOUCH;			// Set the TOUCH event type
    
      TouchEvent touch(event, timestamp, x, y, source);	// Create the TOUCH event
      Screen.dispatch(&touch);				// Dispatch it
      return &touch;
    }

    //
    //  D R A W   E V E N T
    //  -------------------
    //  Happens on a PRESSED -> PRESSED transition.
    //
      #if DEBUG_DIGEST
        Serial.println(F("TouchHandler::digest() DRAW detected"));
      #endif

    event     = TouchEvents::DRAW;			// Set the DRAW event type
    
    TouchEvent touch(event, timestamp, x, y, source);	// Create the TOUCH event
    Screen.dispatch(&touch);				// Dispatch it
    return &touch;
  }

  //------------------------------------------------------------------
  //  At this point the touch panel is currently not pressed
  //  So handle the no press situation.
  //------------------------------------------------------------------
  
  //
  //  U N T O U C H   E V E N T
  //  -------------------------
  //  Happens on a PRESSED -> NON-PRESSED transition.
  //  If the last event we generated was a TOUCH or a DRAW !!!!!
  //  Only in that case we generate an UNTOUCH event.
  //
  //  This event signals the fact that the source object is no longer pressed upon.
  //
  switch (lastEvent) {
    case TouchEvents::TOUCH:
    case TouchEvents::DRAW: {

      if (lastSource) {
        #if DEBUG_DIGEST
          Serial.println(F("TouchHandler::digest() UNTOUCH detected"));
        #endif
   
        event  = TouchEvents::UNTOUCH;	// Set the UNTOUCH event type       

        TouchEvent touch(event, timestamp, lastX, lastY, lastSource);
        Screen.dispatch(&touch);
        return &touch;
      }

      break;
    }

    default:
      // Do nothing
      break;
  }
  
  return nullptr;
}

/*------------------------------------------------------------------------------
 *
 *  Copies the current timestamp, event, x, y and source as the last event data
 *
 *----------------------------------------------------------------------------*/
void TouchHandler::saveState() {

  lastPressed     = pressed;
  lastTimestamp   = timestamp;
  lastEvent       = event;
  lastX           = x;
  lastY           = y;
  lastSource      = source;

}

/*------------------------------------------------------------------------------
 *
 *  Returns the class name of an object
 *
 *----------------------------------------------------------------------------*/
const char* TouchHandler::isType() {
  return "TouchHandler";
}


TouchHandler Touch;	// Create Touch Panel encapsulation object.


