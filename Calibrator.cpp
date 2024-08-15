/*-------------------------------------------------------------------------------------------------


       ///////  ////////  ///////   ///////    //////       //////     /////    //    //
         //    //        //    //  //    //  //    //      //   //   //    //   // //
        //    //////    ///////   ///////   ////////      //////    //    //     //
       //    //        //  //    //  //    //    //      //    //  //    //    // //
      //    ////////  //    //  //    //  //    //      ///////     /////    //   //

     
                           A R D U I N O   G U I   W I D G E T S


                             (C) 2024, cor.hofman@terrabox.nl

                       <Calibrator.cpp> - Library for GUI widgets.
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
#include <Calibrator.h>
#include <Math.h>

#define CALIBRATION_TOUCHES_NEEDED 3      // The number of calibration touches needed per marker
#define MAXIMUM_SPREAD             10     // The spread in calibration values

#define SHOW_ALL_MARKERS           1      // 1 = shows all markers 0 = shows only 1 marker at a time

/*--------------------------------------------------------------------------------------------------
 *
 *  Creates a calibration grid and handles the needed steps in calibrating your screen.
 *  The calibration data is permanently remembered by the Arduino you run it on.
 *
 *
 * 
 *                          G R I D  W I T H   M A R K E R S
 * 
 *                   0         0         0         3         3        3
 *                   0         1         2         0         1        1
 *                   012345678901234567890123   78901234567890123456789
 *                  00o===-------------===o===:::===o===-------------==o 
 *                   1|                   |         |                  | ----.   
 *                   2|                   |         |                  |   MarkerOffset
 *                   3|                   |         |                  | ____'
 *                    :                                                :
 *                    :                                                :
 *                    :                                                :
 *                   7|                   |         |                  |
 *                   8|                   |         |                  |
 *                   9|                   |         |                  |
 *                 240o===-------------===o===:::===o===-------------==o
 *                   1|                   |         |                  |
 *                   2|                   |         |                  |
 *                   3|                   |         |                  |
 *                    :                                                :
 *                    :                                                :
 *                    :                                                :
 *                   7|                   |         |                  |
 *                   8|                   |         |                  |
 *                 479o===-------------===o===:::===o===-------------==o
 *                    |                   |
 *                    |<---- Cell Size -->|
 *
 *
 *
 *-------------------------------------------------------------------------------------------------*/
Calibrator::Calibrator(Widget* parent, uint16_t pWidth, uint16_t pHeight, uint16_t pCellSize) 
          : Widget(parent, 0, 0, pWidth, pHeight) {

  cellSize     = pCellSize;        // Save the requested cell size
  markerSize   = pCellSize / 1.25;	// Cell size / 3
  markerSize  |= 0x0001;		// Make sure we have an odd number of pixels as the size


  markerOffset = markerSize >> 1;	// The marker offset is half the (markerSize - 1)

  xMarkers     = nullptr;
  yMarkers     = nullptr;

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Clears the screen and print help information about the calibration procedure
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::explain() {


  do {
    long now = 0;

    Screen.tft->fillScreen(BLACK);
    Screen.tft->setCursor(0, 0);

    Screen.tft->println(F(".^-^."));
    Screen.tft->println(F(" o o"));
    Screen.tft->println(F("  |"));
    Screen.tft->println(F(" \\_/"));
    Screen.tft->println();
    Screen.tft->println(F("This TFT screen needs to be calibrated only once."));
    Screen.tft->println(F("Recalibration can be started from within your application as well."));
    Screen.tft->println(F("Below the calibration procedure is explained."));
    Screen.tft->println();
    Screen.tft->println(F("Calibration procedure explanation."));
    Screen.tft->println(F("----------------------------------"));
    Screen.tft->println(F("A grid of lines with RED markers will be drawn."));
    Screen.tft->println(F("One of the markers will be WHITE."));
    Screen.tft->println(F("ONLY tap on WHITE markers. Tap at its centre."));
    Screen.tft->println(F("Be as accurate as possible."));
    Screen.tft->println();
    Screen.tft->println(F("Most markers looks like a '+'. Screen edge markers look like a 'T',"));
    Screen.tft->println(F("or this '|-', or this '-|' or like an upside down 'T'"));
    Screen.tft->println(F("Tap markers close to where the two lines cross."));
    Screen.tft->println();
    Screen.tft->println(F("After a tap, a WHITE marker will turn RED, signalling you tapped,"));
    Screen.tft->println(F("A little later it will turn WHITE again. Signalling you need to tap it again."));
    Screen.tft->println(F("A marker must be tapped 3 times successfully in a row, for accuracy reasons."));
    Screen.tft->println(F("Then a marker is calibrated and a next marker will turn WHITE."));
    Screen.tft->println();
    Screen.tft->println(F("You can tap anywhere and still the marker turns RED... or BLUE."));
    Screen.tft->println(F("A BLUE marker implies a calibration error and will restart for that marker."));
    Screen.tft->println(F("Staying BLUE for about 1 second, then the marker becomes WHITE again."));
    Screen.tft->println();
    Screen.tft->println(F("After having calibrated all markers, the grid will automatically disappear."));
    Screen.tft->println(F("And your application will start running. After some time :-)"));
    Screen.tft->println(F("That's all there is to know..."));
    Screen.tft->println();
    Screen.tft->println(F("Ready reading this? Just tap to start calibrating..."));

    //
    // Wait for the touch
    //
    XY xy;
    while (!getRawTouchData(&xy))
      ;

    delay(3000);   // Ignore any jitter

    Screen.tft->println(F("Calibration will start in 3 seconds..."));
    Screen.tft->println(F("Tap again to postpone the calibration..."));


    //
    // Time out loop of 3.25 seconds
    //
    unsigned long timeout = 1000;   
    uint16_t      counter = 0;
    now                   = millis();
    while (!getRawTouchData(&xy)) {

      delay(10);

      unsigned long waiting = millis() - now;
      if (waiting > timeout) {
        Screen.tft->print(++counter);
        Screen.tft->print(" ");
        timeout += 1000;
      }

      //
      // Timeout so return
      //
      if (counter >= 3) {
        Screen.tft->fillScreen(BLACK);
        return;
      }
    }

    //
    //
  } while ( true );

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Draw a test grid with calibration markers.
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::draw() {

  if (!isCalibrated()) {

    //
    // Draw all the markers needed
    //
    drawCalibrationMarkers();

  }

}

/*--------------------------------------------------------------------------------------------------
 *
 *  drawInverted is not supported, hence has the same effect as draw()
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::drawInverted() {
  draw();
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Same as draw()
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::redraw() {
  draw();
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Draw a test grid with squares of cellSize pixels.
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::drawGrid() {

  for (int x = 0; x < width; x += cellSize) {
    Screen.tft->drawFastVLine(x, 0, height, WHITE);
  }  
  Screen.tft->drawFastVLine(width-1, 0, height, WHITE);

  for (int y = 0; y < height; y += cellSize) {
    Screen.tft->drawFastHLine(0, y, width, WHITE);
  }  
  Screen.tft->drawFastHLine(0, height-1, width, WHITE);

}

/*--------------------------------------------------------------------------------------------------
 *
 *            |    
 *  Draw  a --+-- as calibration marker.  
 *            |    
 *
 *  x         Central X coordinate of the marker
 *  y         Central Y coordinate of the marker
 *  color     The color to draw the marker with 
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::drawCalibrationMarker(int16_t x, int16_t y, uint16_t color) {
  int16_t xMarker = 0;
  uint16_t xSize  = 0;

  int16_t yMarker = 0;
  uint16_t ySize  = 0;

  //
  // Calculate the values for the horizontal cross line
  //

  if (x == width) {

    xMarker = width-1; // - markerOffset;
    xSize   = -(markerOffset);

  }
  else if (x == 0) {
    xMarker = 0;
    xSize   = markerOffset;//+1;

  } else {

    xMarker = x-markerOffset;// - (markerSize<<1);
    xSize   = markerSize;

  }

  yMarker = y;
  if (y == height)
    yMarker = height-1;

  Screen.tft->drawFastHLine(xMarker, yMarker, xSize, color);


  //
  // Calculate the values for the vertical cross line
  //


  //
  //  Are we drawing on the maximal Y screen coordinate ?
  //
  if (y == height) {
    yMarker = height-1;// - markerOffset - 1;  // Slightly different Y coordinate
    ySize   = -(markerOffset-2);               // Only the size of a half size cross
  }
  else  if (y == 0) {
    yMarker = 0;
    ySize   = markerOffset;
  }
  else {
    yMarker = y - markerOffset;
    ySize   = markerSize;
  }

  xMarker = x;
  if (x >= width)
    xMarker = width-1;

  //
  //  Draw the cross marker
  //
  Screen.tft->drawFastVLine(xMarker, yMarker, ySize, color);

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Draw alle the needed calibration markers.
 *
 *  event      The TOUCH event to handle
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::drawCalibrationMarkers() {
#if SHOW_ALL_MARKERS
  int y = height >> 1;
  for (int x = 0; x <= width; x += cellSize) {
    drawCalibrationMarker(x, y, RED);
  }

  int16_t x = width >> 1;
  for (int y = 0; y <= height; y += cellSize) {
    drawCalibrationMarker(x, y, RED);
  }
#endif
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Return the appropriate raw X or Y coordinate.
 *
 *  theTouch      raw coordinates to use.
 *  axis          the axis for which the calibration is going on.
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t Calibrator::getAxisValue(XY* theTouch, char axis) {

    switch (axis) {
      case 'x':
      case 'X':
        return theTouch->x;

      case 'y':
      case 'Y':
        return theTouch->y;
    }

    return 0;
} 

/*--------------------------------------------------------------------------------------------------
 *
 *  Calibrate based on the raw touch data coming from getRawTouchData() method.
 *  This data is generated by the onTouchEvent() method. It is available
 *  in the markerRawData class attribute.
 *
 *  prevResult      The prevResult, which must be smaller then the new marker value
 *  xy              Identifies the marker to be used for this calibration
 *  axis            Contains an 'x' or 'X' or 'y' or 'Y for the x or y-axis calibration
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t Calibrator::calibrateMarker(uint16_t prevResult, XY* xy, char axis) {

  //
  //  As long as we do not have a valid calibration value,
  //  keep repeating the calibration of this marker.
  //
  do {
    //
    //  Get the marker coordinates
    //
    int16_t x = xy->x;
    int16_t y = xy->y;

    uint16_t markerTouchedCount = 0;
    uint16_t calibrationValue   = 0;

    //
    //  As long as the calibration marker values are too wide apart (min and max values!),
    //  we keep trying to calibrate the current marker.
    //
    uint16_t min = 0xffff;
    uint16_t max = 0;
    do {
    
      //
      //  If minimum and maximum of marker coordinate are too much apart,
      //  give a visual clue and retry the same marker again
      //
      if ((max - min ) > MAXIMUM_SPREAD) {
        drawCalibrationMarker(x, y, BLUE);
        delay(1000);
      }
      min = 0xffff;
      max = 0;

      //
      // Reset the touch counter for this marker and the cumulative calibrationValue
      //
      markerTouchedCount = 0;
      calibrationValue   = 0;

      //
      // Make sure we have the required number of touches per  marker
      //
      while (markerTouchedCount < CALIBRATION_TOUCHES_NEEDED) {

        //
        //  Show same marker as white (again), for the (repeated) touch.
        //
        drawCalibrationMarker(x, y, WHITE);

        //
        // Wait for the touch
        //
        while (!getRawTouchData(xy))
          ;

        //
        //  Display raw x, y
        //
        char s[16] = "";
        int dX = 0;
        int dY = 0;
        Screen.tft->setCursor(dX, dY);
        Screen.tft->fillRect(dX, dY, 80, 10, BLACK);
        sprintf(s, "(%4d, %4d)", xy->x, xy->y);
        Screen.tft->println(s);

        //
        //  As it has been touched,
        //  redraw the calibration marker. 
        //  As confirmation that the input is recognized.
        //
        drawCalibrationMarker(x, y, RED);

        //
        // Avoid any jitter or disturbance
        // By accumulating the coordinate as along as the press takes
        //
        int counter = 0;
        uint16_t tmpValue = 0;
      
        do {
          displayXYTouch(xy);
          if (counter < 20 && tmpValue < 30000) {
            uint16_t v = getAxisValue(xy, axis);
            tmpValue  += v;
            counter++;
            delay(50);  // Check again over 50ms and avoid getting overflow
          }
        }
        while (getRawTouchData(xy));

        uint16_t roundedValue = round((float)tmpValue/(float)counter);

        if (roundedValue < min)
          min = roundedValue;
        if (roundedValue > max)
          max = roundedValue;

        //
        //  If illegal value, then force a retry
        //
        if(roundedValue == 0) {
          max = MAXIMUM_SPREAD + 1;
          min = 0;
        }

        //
        //  If the value is more than MAXIMUM_SPREAD, this triggers an immediate abort.
        //  And consequently a calibration retry of the current marker.
        //
        if ((max - min) >= MAXIMUM_SPREAD) {
          break;
        }

        calibrationValue += roundedValue;
        markerTouchedCount++;

        delay(500);  // Wait 500ms before asking again.
      }

      //
      // Keep trying to calibrate the current marker if the spread in calibration values is greater than the MAXIMUM_SPREAD.
      //
    } while( (max - min) >= MAXIMUM_SPREAD);
  
    //
    // The average result of the number of required touches
    //
    uint16_t result = round(((float)calibrationValue / (float)CALIBRATION_TOUCHES_NEEDED));

    //
    //  If the calibration value of this marker < previous marker
    //  then this is an error
    //
    if (result <= prevResult) {
       drawCalibrationMarker(x, y, BLUE);
       delay(1000);
    }
    else {
      return result;
    }
  }
  while (1);  // An error occurred, try again if we end up here.
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Calibrate the screen on a specific marker.
 *
 *  prevResult      The previous marker value
 *  x               The X-center coordinate of the marker used
 *  y               The Y-center coordinate of the marker used 
 *  axis            contains an 'x' or 'X' or 'y' or 'Y' depending on the axis to calibrate
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t Calibrator::calibrate(uint16_t prevResult, int16_t x, int16_t y, char axis) {

    //
    // Clip x
    //
    if (x <= 0)
      x = 0;
    else if (x >= width)
      x = width-1;

    //
    // Clip y
    //
    if (y < 0)
      y = 0;
    else if (y >= height)
      y = height-1;

    //
    //  Gather calibration data
    //
    XY xy;
    xy.x = x;
    xy.y = y;

    uint16_t data = calibrateMarker(prevResult, &xy, axis);
    delay(1000);

    return data;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Calibrate the entire screen using all markers for the X and Y axis.
 *  On return the data is captured in the arrays passed as an argument.
 *  Note that the arrays should be sufficiently large.
 *
 *  xCalibrationData      An array with (width/cellSize)+1 elements
 *  yCalibrationData      An array with (height/cellSize)+1 elements
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::calibrate(uint16_t* xCalibrationData, uint16_t* yCalibrationData) {

  //
  //  Check if some calibration parameters have changed.
  //  If so recalibration will be triggered.
  //
  checkParameters();

  uint16_t      xSize      = EEPROM_RD_INT(EPR16_TFT_CALIBR_X_S);
  uint16_t      xSizeBytes = xSize * sizeof(uint16_t);
 
  uint16_t      ySize      = EEPROM_RD_INT(EPR16_TFT_CALIBR_Y_S);
  uint16_t      ySizeBytes = ySize * sizeof(uint16_t);

  //
  // If already calibrated, then return the persisted calibration data, which is used to
  // convert the raw touch coordinates into proper screen coordinates if touched.
  //
  if (isCalibrated()) {
    //
    //  Read in the earlier persisted calibration data
    //
    persistentRead(ADR_TFT_CALIBR_X, (unsigned char*)xCalibrationData, xSizeBytes);
    persistentRead(ADR_TFT_CALIBR_Y, (unsigned char*)yCalibrationData, ySizeBytes);

    //
    // Make the calibration data available to the TouchHandler
    //
    Touch.setMarkerDistance(cellSize);
    Touch.setXCalibration(xSize, xCalibrationData);
    Touch.setYCalibration(ySize, yCalibrationData);     

    //
    // And we are ready
    //
    return;
  }

  //
  //  Before starting explain the calibration procedure
  //
  explain();

  //
  //  Draw the calibration grid
  //
  draw();

  //
  //
  //  Calibrate the X-axis
  //
  int y = height >> 1;
  uint16_t result = 0;
  for (int x = 0, xCalibration = 0; x <= width; x += cellSize, xCalibration++) {
    result = calibrate(result, x, y, 'x');
    xCalibrationData[xCalibration] = result;
  }

  //
  //  Persist the X-axis calibration data in EEPROM
  //
  persistentStore(ADR_TFT_CALIBR_X, (unsigned char*)xCalibrationData, xSizeBytes);

  //
  //  Calibrate the Y-axis
  //
  int x = width >> 1;
  result = 0;
  for (int y = height, yCalibration = 0; y >= 0; y -= cellSize, yCalibration++) {
    result = calibrate(result, x, y, 'y');
    yCalibrationData[yCalibration] = result;
  }

  //
  //  Persist the Y-axis calibration data in EEPROM
  //
  persistentStore(ADR_TFT_CALIBR_Y, (unsigned char*)yCalibrationData, ySizeBytes);

  //
  // We are done calibrating
  //
  setCalibrated(true);
  Screen.tft->fillScreen(BLACK);


  //
  // Now make the calibration data available to the TouchHandler
  //
  Touch.setMarkerDistance(cellSize);
  Touch.setXCalibration(xSize, xCalibrationData);
  Touch.setYCalibration(ySize, yCalibrationData);     
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Returns true if calibrated, otherwise false.
 *
 *------------------------------------------------------------------------------------------------*/
bool Calibrator::isCalibrated() {

  //
  // Explicitly test on the value true, because
  // the C-test:
  //
  //   if (EEPROM_RD_BYTE(EPR8_TFT_CALIBRATED))
  //
  // would translate to 
  //
  //   if (0xff)
  //
  // which results in true.
  //
  // Clearly this is not what we intend with this method.
  //

  unsigned char b = EEPROM_RD_BYTE(EPR8_TFT_CALIBRATED);
  return (b == true);
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Indicate that the TFT is uncalibrated.
 *  The next call to calibrate will start the calibration
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::recalibrate() {

  setCalibrated(false);

  //
  //  Release the possibly already allocated calibration value buffers
  //
  if (xMarkers) {
    free(xMarkers);
    xMarkers = nullptr;
  }

  if (yMarkers) {
    free(yMarkers);
    yMarkers = nullptr;
  }
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Set the calibrated indicator.
 *
 *  b      true implies calibrated, false uncalibrated.
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::setCalibrated(bool b) {

  unsigned char state = EEPROM_RD_BYTE(EPR8_TFT_CALIBRATED);

  //
  // If stored state differs from the requested state
  //
  if (state != b) {
    EEPROM_WR_BYTE(EPR8_TFT_CALIBRATED, (unsigned char)b);  // Change state

    //
    // If written state differs from read back state, then Error
    //
    if (EEPROM_RD_BYTE(EPR8_TFT_CALIBRATED) != b) {
      if (Serial) Serial.println(F("ERROR **** EEPROM Could not write TFT calibrated state"));
    }
    else {
      if (Serial) Serial.println(F("INFO **** EEPROM Changed TFT calibration state successfully"));
    }
  }

  //
  // Report state
  //
  if (Serial) {
    Serial.print(F("INFO *** TFT Calibrated set to: ")); Serial.println(b ? F("Yes") : F("No"));
  }

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Check is some calibration parameters have changed
 *  - Screen width
 *  - Screen height
 *  - calibration cell size
 *  - Number of calibration X-axis makers
 *  - Number of calibration Y-axis makers
 *
 *  If so a recalibration will be triggered 
 *
 *------------------------------------------------------------------------------------------------*/
bool Calibrator::checkParameters() {

  //
  //  Check if the screen width or height have changed
  //
  uint16_t eepromWidth    = EEPROM_RD_INT(EPR16_TFT_X_W);
  uint16_t eepromHeight   = EEPROM_RD_INT(EPR16_TFT_Y_H);

  uint8_t  eepromCellSize = EEPROM_RD_BYTE(EPR8_CELL_S);

  uint16_t xSize          = (width  / cellSize) + 1;
  uint16_t ySize          = (height / cellSize) + 1;
  uint16_t eepromXSize    = EEPROM_RD_INT(EPR16_TFT_CALIBR_X_S);
  uint16_t eepromYSize    = EEPROM_RD_INT(EPR16_TFT_CALIBR_Y_S);

  //
  //  If some parameters have changed
  //
  if ( eepromWidth    != width    ||
       eepromHeight   != height   ||
       eepromCellSize != cellSize ||
       eepromXSize    != xSize    ||
       eepromYSize    != ySize) {

    //
    //  Yes some have changed, so we need to recalibrate the screen
    //
    if (isCalibrated()) {
      setCalibrated(false);	// Set, so we recalibrate
    }

    //
    //  Persist the screen width, complain if EEPROM doesn't work
    // 
    if (eepromWidth != width) {
      EEPROM_WR_INT(EPR16_TFT_X_W, width);

      if (EEPROM_RD_INT(EPR16_TFT_X_W) != width) {
        if (Serial) Serial.println(F("ERROR **** EEPROM write failure: TFT screen width "));
      }
      else {
        if (Serial) {
          Serial.print(F("INFO **** Stored TFT Screen width: ")); Serial.println(width);
        }
      }
    }        

    //
    // Persist the screen height, complain if the EEPROM doesn't work
    //
    if (eepromHeight != height) {
      EEPROM_WR_INT(EPR16_TFT_Y_H, height);

      if (EEPROM_RD_INT(EPR16_TFT_Y_H) != height) {
        if (Serial) Serial.println(F("ERROR **** EEPROM write failure: TFT screen height "));
      }
      else {
        if (Serial) {
          Serial.print(F("INFO **** Stored TFT Screen height: ")); Serial.println(height);
        }
      }
    }        

    //
    //  Remember the cell size if it differs
    //
    if (cellSize != eepromCellSize) {
      EEPROM_WR_BYTE(EPR8_CELL_S, cellSize);

      if (EEPROM_RD_BYTE(EPR8_CELL_S) != cellSize) {
        if (Serial) Serial.println(F("ERROR **** EEPROM write failure: calibration cell size"));
      }
      else {
        if (Serial) {
          Serial.print(F("INFO **** Stored calibration cell size: ")); Serial.println(cellSize);
        }
      }
    }

    //
    //  Remember the X-axis number of calibration points differs
    //
    if (eepromXSize != xSize) {
      EEPROM_WR_INT(EPR16_TFT_CALIBR_X_S, xSize);

      if (EEPROM_RD_INT(EPR16_TFT_CALIBR_X_S) != xSize) {
        if (Serial) Serial.println(F("ERROR **** EEPROM write failure: Number of X-axis markers"));
      }
      else {
        if (Serial) {
          Serial.print(F("INFO **** Stored number of X-axis markers: ")); Serial.println(xSize);
        }
      }
   }

    //
    //  Remember the Y-axis number of calibration points differs
    //
    if (eepromYSize != ySize) {
      EEPROM_WR_INT(EPR16_TFT_CALIBR_Y_S, ySize);

      if (EEPROM_RD_INT(EPR16_TFT_CALIBR_Y_S) != ySize) {
        if (Serial) Serial.println(F("ERROR **** EEPROM write failure: Number of Y-axis markers"));
      }
      else {
        if (Serial) {
          Serial.print(F("INFO **** Stored number of Y-axis markers: ")); Serial.println(ySize);
        }
      }
    }

    return true;
  }

  return false;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Returns the number of markers needed for the Y-axis
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t Calibrator::getXWSize() {
  return (width  / cellSize) + 1;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Returns the number of markers needed for the Y-axis
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t Calibrator::getYHSize() {
  return (height / cellSize) + 1;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Returns a uint16_t array with getXWAxis() elements
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t* Calibrator::getXAxisBuffer() {

  if (! xMarkers)
    xMarkers = (uint16_t*)malloc(getYHSize() * sizeof(uint16_t));

  return xMarkers;

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Returns a uint16_t array with getYHAxis() elements
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t* Calibrator::getYAxisBuffer() {

  if (! yMarkers)
    yMarkers = (uint16_t*)malloc(getYHSize() * sizeof(uint16_t));

  return yMarkers;

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Throws a way any event that is matched in a normal way.
 *  Since this widget has its private handling of screen touch.
 *  See the calibrateOnMarker() method and its use of the getRawTouchData() method.
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::onEvent(TouchEvent* event) {

  //
  //  Ignore any event that is passed via the regular touch event mechanism
  //

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Print a marker buffer
 *
 *  title      X or Y
 *  buffer     The marker point values
 *  size       The number of marker points
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::printMarkerBuffer(char* title, uint16_t* buffer, uint16_t size) {
  if (!Serial) 
    return;

  Screen.tft->fillScreen(BLACK);
  Screen.tft->setCursor(0,0);
  char s[16];

  Screen.tft->print(title); Screen.tft->print(F(" markers: ")); Screen.tft->println(size);
  Screen.tft->println(F("\n-IDX---PIX----RAW--"));
  for (int i = 0, pix = 0; i < size; i++, pix+=cellSize) {

    sprintf(s, "  %2d:  ", i);
    Screen.tft->print(s);

    sprintf(s, "%3d:  ", pix);
    Screen.tft->print(s);

    sprintf(s, "%4d", buffer[i]);
    Screen.tft->println(s);
  }

  Screen.tft->println(F("-------------------\n"));
  Screen.tft->println(F("Tap the screen to continue..."));

  XY xy;
  while(!getTouchData(&xy))
        ;

  Screen.tft->fillScreen(BLACK);
  
}

/*-------------------------------------------------------------------------------------------------
 * 
 *  Showss data stored in EEPROM on the TFFT screen as diagnostics information.
 *
 *------------------------------------------------------------------------------------------------*/
void Calibrator::eepromData() {

  delay(1000);
  Screen.tft->fillScreen(BLACK);
  Screen.tft->setCursor(0,0);
  Screen.tft->println("Tap within 3 seconds \nto see EEPROM data...");
  if (!Touch.tapOrTimeout((unsigned long) 3000)) {
    Screen.tft->fillScreen(BLACK);
    return;
  }

  Screen.tft->fillScreen(BLACK);
  //
  //  If not virgin storage, then show some calibration data areas
  //
  bool virgin = isPersistentStorageVirgin();
  if (!virgin) {
    uint8_t rot = Screen.tft->getRotation();
    Screen.tft->setRotation(1);
    Screen.tft->println("\nEEPROM data areas:");

    Screen.tft->println(F("\n      EPR16_TFT_X_W (screen width):\n"));
    persistentDump(EPR16_TFT_X_W, 2);
    Screen.tft->fillScreen(BLACK);
    Screen.tft->setCursor(0,0);

    Screen.tft->println(F("\n      EPR16_TFT_Y_H (screen height):\n"));            // Contains 16 bit screen height
    persistentDump(EPR16_TFT_Y_H, 2);
    Screen.tft->fillScreen(BLACK);
    Screen.tft->setCursor(0,0);
 
    Screen.tft->println(F("\n      EPR8_TFT_CALIBRATED (0 if not calibrated yet, non-zero otherwise):\n")); // Contains 0 if not calibrated yet, non-zero otherwise
    persistentDump(EPR8_TFT_CALIBRATED, 1);
    Screen.tft->fillScreen(BLACK);
    Screen.tft->setCursor(0,0);

    Screen.tft->println(F("\n      EPR8_CELL_S (Cell size used to calibrate):\n")); // Cell size used to calibrate
    persistentDump(EPR8_CELL_S, 1);
    Screen.tft->fillScreen(BLACK);
    Screen.tft->setCursor(0,0);

    Screen.tft->print(F("\nEPR16_TFT_CALIBR_X_S (X-Axis calibration data size in # markers uint16_t): ")); Screen.tft->println(EEPROM_RD_INT(EPR16_TFT_CALIBR_X_S)); Screen.tft->println(); // Calibration data size in bytes for the X-axis 
    persistentDump(EPR16_TFT_CALIBR_X_S, 2);
    Screen.tft->fillScreen(BLACK);
    Screen.tft->setCursor(0,0);
 
    Screen.tft->print(F("\nEPR16_TFT_CALIBR_Y_S (Y-Axis calibration data size in # markers uint16_t): ")); Screen.tft->println(EEPROM_RD_INT(EPR16_TFT_CALIBR_Y_S)); Screen.tft->println();   // Calibration data size in bytes for the Y axis
    persistentDump(EPR16_TFT_CALIBR_Y_S, 2);
    Screen.tft->fillScreen(BLACK);
    Screen.tft->setCursor(0,0);

    Screen.tft->print(F("\n      &ADR_TFT_CALIBR_X (X-axis calibration data)= 0x")); Screen.tft->print(ADR_TFT_CALIBR_X, HEX); Screen.tft->print(F(", size: ")); Screen.tft->print(sizeof(uint16_t) * EEPROM_RD_INT(EPR16_TFT_CALIBR_X_S)); Screen.tft->println(F(" bytes\n"));
    persistentDump(ADR_TFT_CALIBR_X, EEPROM_RD_INT(EPR16_TFT_CALIBR_X_S) * sizeof(uint16_t));
    Screen.tft->fillScreen(BLACK);
    Screen.tft->setCursor(0,0);

    Screen.tft->print(F("\n      &ADR_TFT_CALIBR_Y (Y-axis calibration data)= 0x")); Screen.tft->print(ADR_TFT_CALIBR_Y, HEX); Screen.tft->print(F(", size: ")); Screen.tft->print(sizeof(uint16_t) * EEPROM_RD_INT(EPR16_TFT_CALIBR_Y_S)); Screen.tft->println(F(" bytes\n"));
    persistentDump(ADR_TFT_CALIBR_Y, EEPROM_RD_INT(EPR16_TFT_CALIBR_Y_S) * sizeof(uint16_t));
    Screen.tft->fillScreen(BLACK);
    Screen.tft->setCursor(0,0);

    Screen.tft->setRotation(rot);
  }
}

/*------------------------------------------------------------------------------
 *
 *  Prints Prints the coordinates. 
 *
 *----------------------------------------------------------------------------*/
void Calibrator::displayXYTouch(XY* xy) {
        char s[32];
        Screen.tft->setCursor(0, 0);
        Screen.tft->fillRect(0, 0, 80, 10, BLACK);
        sprintf(s, "(%4d, %4d)", xy->x, xy->y);
        Screen.tft->println(s);
}

/*------------------------------------------------------------------------------
 *
 *  Prints diagnostic data or execute diagnostics tests. 
 *
 *----------------------------------------------------------------------------*/
#define DIAGNOSTICS_TIMEOUT  1000     // In ms. Touching longer ends a test.

void Calibrator::diagnostics() {

  //
  // Do we want to enter diagnostics?
  //
  Screen.tft->setCursor(0,0);
  Screen.tft->println("Tap within 3 seconds \nto enter diagnostics mode...");
  if (! Touch.tapOrTimeout((unsigned long)3000)) {
    delay(200);
    Screen.tft->fillScreen(BLACK);
    return;  // No diagnostics :-)
  }

  //
  // Show EEPROM data
  //
  eepromData();

  //
  // Show the calibration data
  //
  printMarkerBuffer("Calibration data X-axis", xMarkers, getXWSize());
  printMarkerBuffer("Calibration data Y-axis", yMarkers, getYHSize());

  //
  //  Test raw coordinates in several screen rotations
  //
  for (int r=0; r < 1; r++) {
 
    Screen.tft->fillScreen(BLACK);
    Screen.tft->setRotation(r);
    Screen.tft->setCursor(0,0);
    Screen.tft->print(F("Screen WxH: ")); Screen.tft->print(Screen.tft->width()); Screen.tft->print(F("x"));Screen.tft->println(Screen.tft->height());
    Screen.tft->print(F("Rotation: ")); Screen.tft->println(r);
    Screen.tft->println(F("Tap to enter RAW X,Y tapping mode..."));
    if (Touch.tapOrTimeout((unsigned long)3000)) {
      Screen.tft->println(F("\nHold tap down longer to exit this test."));
      delay(3000);

      Screen.tft->setCursor(0,0);
      Screen.clear();


      drawCalibrationMarkers();



      Screen.tft->print(F("Start tapping..."));
      XY xy;
      char s[32];
      int height = -10;

      while (1) {
        while (!getRawTouchData(&xy));  // Wait for touch
        displayXYTouch(&xy);
//        Screen.tft->setCursor(0, 0);
//        Screen.tft->fillRect(0, 0, 80, 10, BLACK);
//        sprintf(s, "(%4d, %4d)", xy.x, xy.y);
//        Screen.tft->println(s);

        long now = millis();
        bool exitNow = false;
        while (! exitNow && getRawTouchData(&xy)) { // Wait for untouch
           if (millis() - now > DIAGNOSTICS_TIMEOUT)
             exitNow = true;
        }
        if (exitNow)
         break; 

//        Screen.tft->drawFastVLine(-1, 0, 1, WHITE);
//        height += 1;
//        Screen.tft->fillRect(0,Screen.tft->height() - height, 
//                             Screen.tft->width(), Screen.tft->height(), WHITE);
//        height += 50;
        while (getRawTouchData(&xy));  // Wait for untouch
      }
    }
  }

  //
  //  Test screen coordinates
  //
  Screen.tft->fillScreen(BLACK);
  Screen.tft->setRotation(0);
  Screen.tft->setCursor(0,0);
  Screen.tft->println(F("Tap to enter \nSCREEN X,Y tapping mode..."));
  if (Touch.tapOrTimeout((unsigned long)3000)) {
    Screen.tft->println(F("\nHold tap down longer to exit this test."));
    delay(3000);

    Screen.clear();
    drawGrid();
    Screen.tft->setCursor(0,0);
    Screen.tft->print(F("Start tapping..."));
    XY xy;
    char s[32];
    int height = 0;

    while (1) {

      //
      // Obtain the screen coordinates of a touch
      //
      while (!Touch.getTouch(&xy));  // Wait for touch
      Screen.tft->setCursor(0, 0);
      Screen.tft->fillRect(0, 0, 160, 20, BLACK);
      sprintf(s, "Screen: (%4d, %4d)", xy.x, xy.y);
      Screen.tft->println(s);

      getRawTouchData(&xy);
      sprintf(s, "Raw   : (%4d, %4d)", xy.x, xy.y);
      Screen.tft->println(s);

      long now = millis();
      bool exitNow = false;
      while (! exitNow && getRawTouchData(&xy)) { // Wait for untouch
         if (millis() - now > DIAGNOSTICS_TIMEOUT)
           exitNow = true;
      }
      if (exitNow)
       break;

      while (getRawTouchData(&xy));  // Wait for untouch
    }

  }

  Screen.tft->fillScreen(BLACK);

}
