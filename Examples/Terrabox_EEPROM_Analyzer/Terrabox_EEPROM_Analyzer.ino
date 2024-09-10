#include <TerraBox_Widgets.h>        // For Levels
#include <Calibrator.h>
#include <Persistence.h>

/**---------------------------------------------------
 *
 *  Fix a mistakenly cleared calibartion indicator
 *
 *--------------------------------------------------*/
void fixMistakenlyClearedCalibration() {
  Calibrator calibrator(nullptr, Screen.width, Screen.height, 20);
  calibrator.setCalibrated(true);
}

void setup() {
Serial.begin(115200);
while (!Serial);

//
//  Initialize the TFT screen
//
Screen.begin();

//
// Uncomment the appropriate code line to fix a mistakenly cleared calibrated state
//
//----- F I X E S ------------------------------------------------------------
//fixMistakenlyClearedCalibration();

}

void loop() {
  
  //
  //----- E E P R O M  M E M O R Y  A N A L Y Z E R ----------------------------
  //
  // Start analyzing the EEPROM memory contents
  //
  Screen.analyzeEEPROM();

}
