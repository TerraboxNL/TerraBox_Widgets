/*-------------------------------------------------------------------------------------------------


       ///////  ////////  ///////   ///////    //////       //////     /////    //    //
         //    //        //    //  //    //  //    //      //   //   //    //   // //
        //    //////    ///////   ///////   ////////      //////    //    //     //
       //    //        //  //    //  //    //    //      //    //  //    //    // //
      //    ////////  //    //  //    //  //    //      ///////     /////    //   //

     
                           A R D U I N O   G U I   W I D G E T S


                             (C) 2024, cor.hofman@terrabox.nl

                       <Calibrator.h> - Library for GUI widgets.
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
#include <persistence.h>

#ifndef CALIBRATOR_h
#define CALIBRATOR_h

class Calibrator : public Widget {

  private:
    uint16_t cellSize;                             // Spacing between markers in pixels
    uint16_t markerSize;                           // The size of a marker, must be < than cellSize
    uint16_t markerOffset;                         // Half of the markerSize - 1.

    uint16_t* xMarkers = nullptr;
    uint16_t* yMarkers = nullptr;

    void drawGrid();                               // Draw the entire grid of calibration markers
    void drawCalibrationMarker(                    // Draw a calibration marker at the indicated x, y
                            int16_t  x, 
                            int16_t  y, 
                            uint16_t color);
    void drawCalibrationMarkers();

    void setCalibrated(bool b);                    // Set the calibration indicator

    uint16_t getAxisValue(   XY*     theTouch,     // Get the specific data either raw x or y position 
                             char    axis);
    uint16_t calibrateMarker(
                             uint16_t prevResult,  // Get raw calibration data for a specific x, y position
                             XY*     xy,         
                             char    axis);
    uint16_t calibrate(
                             uint16_t prevResult,  // Get raw calibration data for the x or y position
                             int16_t x,            
                             int16_t y, 
                             char    axis);

  public:
    Calibrator(Widget* parent, uint16_t pWidth, uint16_t pHeight, uint16_t pCellSize);

    void explain();                                   // Provides explanation
    bool isCalibrated();                              // Returns true if the TFT has been calibrated
    void recalibrate();                               // Makes calibrate recalibrate the TFT screen
    void draw();                                      // Draws the calibration grid
    void drawInverted();                              // Not used but must be there
    void redraw();                                    // Redraws the calibration grid
    void calibrate(uint16_t* xCalibrationData,        // Fill arrays with calibration data
                   uint16_t* yCalibrationData);       // If uncalibrated or recalibration is desired it calibrates first
    void tapToCalibrate(uint16_t* xCalibrationData,   // Fill arrays with calibration data
                   uint16_t* yCalibrationData);       // If uncalibrated or recalibration is desired it calibrates first

    uint16_t  getXWSize();                            // The number of markers on the X-axis (screen width)
    uint16_t  getYHSize();                            // The number of markers on the Y-axis (screen height)
    uint16_t* getXAxisBuffer();                       // Buffer array for calibration marker values of the X-axis
    uint16_t* getYAxisBuffer();                       // Buffer array for calibration marker values of the Y-axis

    bool     checkParameters();                       // Checks calibration parameters and persists them, if changed.

    void     printMarkerBuffer(char*     title,       // Prints the contents of a marker buffer
                               uint16_t* buffer, 
                               uint16_t  size);
    void     eepromData();                            // Prints data stored in EEPROM
    void     displayXYTouch(XY* xy);                  // Displays the coordinates in the left top corner
    void     diagnostics();                           // Executes some diagnostics tests

    virtual void onEvent(TouchEvent* event);          // Override Widget onEvent() method
};

#endif
