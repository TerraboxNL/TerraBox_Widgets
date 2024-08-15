/*--------------------------------------------------------------------------------------------------

       ///////  ////////  ///////   ///////    //////       //////     /////    //    //
         //    //        //    //  //    //  //    //      //   //   //    //   // //
        //    //////    ///////   ///////   ////////      //////    //    //     //
       //    //        //  //    //  //    //    //      //    //  //    //    // //
      //    ////////  //    //  //    //  //    //      ///////     /////    //   //

     
                           A R D U I N O   G U I   W I D G E T S


                             (C) 2024, cor.hofman@terrabox.nl

                       <Source file name> - Library for GUI widgets.
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

//#include <SoftKeyboardWidget.h>

#define DEBUG_DISPATCH	0
#define DEBUG_BEGIN	0

/*==============================================================================
 *
 * The Screen class is an abstraction of the physical screen.
 *
 *============================================================================*/

/*-----------------------------------------------------------------------------
 *
 * Create a screen handler
 *
 * pTft    The driver for the screen.
 *
 *---------------------------------------------------------------------------*/
ScreenHandler::ScreenHandler(MCUFRIEND_kbv* pTftScreen) 
      : Widget(nullptr, 0, 0, 0, 0) {

  //
  //  Remember the TFT screen driver.
  //
  tft             = pTftScreen;

  //
  //  Initialize the queue for dispatching later events
  //
  laterQueue = nullptr;

  //
  //  Add the keyboard to the widget tree.
  //
  #ifdef SOFTKEYBOARDWIDGET_h
    tty.setParent(this);
  #endif
}

/*-----------------------------------------------------------------------------
 *
 *  This method has to called during the setup() function of Arduino.
 *  It initialises the the Screen object driving the screen.
 *  To call this method use :
 *
 *  Example:
 *  ------------------------------------ 
 *  setup () {
 *  //  Some code .....
 *
 *      Screen.begin();
 *
 *  //  Some more code ....
 *  }
 *
 *---------------------------------------------------------------------------*/
void ScreenHandler::begin() {

  #if DEBUG_BEGIN
    if (child)
      Serial.println(F("ScreenHandler::begin() YES it has a child"));
    else
      Serial.println(F("ScreenHandler::begin() NO it has no child"));
  #endif


  //
  // Initialize TFT screen
  //
  uint16_t ID = tft->readID();

  #if DEBUG_BEGIN
    Serial.print(F("ScreenHandler::begin ID = 0x"));
    Serial.println(ID, HEX);
  #endif

  if (ID == 0xD3D3) ID = 0x9481; // write-only shield
  tft->begin(ID);

  width  = tft->width();
  height = tft->height();

  //
  //  Clear the screen
  //
  tft->fillScreen(BLACK);


  //=============================================================================
  //  Start up the screen and calibration
  //=============================================================================
  #define CELLSIZE 20

  Screen.tft->setRotation(0);
 
  //
  // Create the calibrator
  //
  Calibrator calibrator(&Screen, Screen.tft->width(), Screen.tft->height(), CELLSIZE);

  //
  // If virgin memory, then request a calibration
  //
  bool virgin = isPersistentStorageVirgin();
  if (virgin) {
    calibrator.recalibrate();
  }

  //
  //  Show Splash screen
  //
  Splash splash;
  splash.draw();

  //
  // If already calibrated, ask if it must  be recalibrated
  //
  if (!virgin && calibrator.isCalibrated()) {
    uint8_t rot = tft->getRotation();
    if (rot == 1 || rot == 3) {
      Screen.tft->println(F("               TAP THE SCREEN WITHIN 6 SECONDS TO RECALIBRATE IT"));
      Screen.tft->print(F("                           ... "));
    }
    else {
      Screen.tft->println(F("\n\n\n\n\n"));
      Screen.tft->println(F("  TAP THE SCREEN WITHIN 6 SECONDS TO RECALIBRATE IT"));
      Screen.tft->print(  F("  ... "));
    }

    if (Touch.tapOrTimeout((unsigned long)6000)) {
      calibrator.recalibrate();
    }
  }
  //
  // Otherwise show the splash screen for 3 seconds
  //
  else {
    delay(3000);
  }

  //
  // Clear the splash screen
  //
  Screen.tft->fillScreen(BLACK);

  //
  // Set the rotation back to what we need, since the splash screen changes it
  //
  Screen.tft->setRotation(0);

  //
  // Allocate the marker calibration buffers
  //
  uint16_t xMarkerBuffer = calibrator.getXAxisBuffer();
  uint16_t yMarkerBuffer = calibrator.getYAxisBuffer();

  //
  //  Start the calibration procedure
  //
  calibrator.calibrate(xMarkerBuffer, yMarkerBuffer);

  calibrator.diagnostics();

}

/*------------------------------------------------------------------------------
 * 
 * Since the Screen object contains a the full tree of Widgets of which
 * Screen is the root. This draw() method will draw it all. 
 *
 *----------------------------------------------------------------------------*/
void ScreenHandler::draw() {

  //
  //  Draw the screens stuff first.
  //  By its nature the screen has a Z-order which is infinite.
  //  I.e. it is positioned behind any other widget.
  //

  /*   Currently the screen does not have any graphical features   */

  //
  //  Now draw all the widget with a smaller z-order.
  // 
  for (Widget* sibling = child; sibling; sibling = sibling->getSibling()) {
    sibling->draw( );
  }

}

/*------------------------------------------------------------------------------
 * 
 * Unsupported, hence it does nothing. 
 *
 *----------------------------------------------------------------------------*/
void ScreenHandler::drawInverted( ){
	// Not supported for Screen
}

/*------------------------------------------------------------------------------
 * 
 *  Redraw the entire widget tree.
 *  The Screen kicks this process off by clearing the entire screen. 
 *
 *----------------------------------------------------------------------------*/
void ScreenHandler::redraw( ){

  //
  //  Clear the entire screen
  //
  tft->fillScreen(BLACK);

  //
  //  Draw the entire widget tree
  //
  draw();

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Handle TOUCH events for the screen.
 *
 *------------------------------------------------------------------------------------------------*/
void ScreenHandler::onTouch(TouchEvent* event) {

  // Currently silently ignore the events.

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Handle UNTOUCH events for the screen.
 *
 *------------------------------------------------------------------------------------------------*/
void ScreenHandler::onUntouch(TouchEvent* event) {

  // Currently silently ignore the events.

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Handle DRAW events for the screen
 *
 *------------------------------------------------------------------------------------------------*/
void ScreenHandler::onDraw(TouchEvent* event) {

  // Currently silently ignore the events.

}

/*----------------------------------------------------------------------
 *
 *  After a specific interval of inactivity (i.e. no touches) this event
 *  is generated by the TouchHandler. The touchandel has an attribute
 *  inactivityTime which can be set to specify the inactivity interval.
 *
 *--------------------------------------------------------------------*/
void ScreenHandler::onActivityTimeout(TouchEvent* event) {
	  Serial.println("Go to sleep");
}

/*----------------------------------------------------------------------
 *
 *  After an ActivityTimeout event a WakeUp event will follow.
 *  As soon as a user starts to interact again by touching the
 *  TFT screen. Typically the
 *
 *--------------------------------------------------------------------*/
void ScreenHandler::onWakeUp(TouchEvent* event) {
  Serial.println("Wake up");
}

/*------------------------------------------------------------------------------
 * 
 *  Try to match a widget based on the coordinates.
 *  If the X, Y fall in the area occupied by the widget,
 *  the here is a match.
 *
 *  event      The event to be matched
 *
 *----------------------------------------------------------------------------*/
Widget* ScreenHandler::match(int16_t x, int16_t y) {
  //
  // Try to find the touched widget
  //
  Widget* widget = match(x, y);

  //
  // If a widget was not found, then return no object
  //
  if (! widget) {
    #if DEBUG_MATCH
      Serial.println(F("ScreenHandler::dispatch No widget matched :-("));
    #endif

    return nullptr;
  }
}

/*-----------------------------------------------------------------------------------------
 *
 *  Dispatches all pending event prior to executing the event passed as an argument.
 *
 *  event      Event to be executed after queued events have been executed.
 *
 *---------------------------------------------------------------------------------------*/
Widget* ScreenHandler::dispatch(TouchEvent* event) {

  //
  //  Empty the event queue with later events
  //
  dispatchAll();

  //
  //  Executed the event passed
  //
  dispatchOnly(event);

}

/*-----------------------------------------------------------------------------------------
 *
 *  Dispatches the GUI event offered and returns the Widget to which the event 
 *  was initially sent.
 *
 *  event      The event to be dispatched.
 *
 *---------------------------------------------------------------------------------------*/
Widget* ScreenHandler::dispatchOnly(TouchEvent* event) {
  #if DEBUG_DISPATCH
    if (!child) 
      Serial.println(F("ScreenHandler::dispatch() !!!!!!!! N O   C H I L D   W I D G E T S !!!!!!!!"));
    Serial.print(F("ScreenHandler::dispatch() Received an event 2B dispatched @ ("));
    Serial.print(event->x);
    Serial.print(F(","));
    Serial.print(event->y);
    Serial.println(F(")"));
  #endif

  //
  // If a widget was not found, then send it to the unsollicited event handler
  // and return no object
  //
  if (! event->source) {

    onUnsollicitedEvent(event);
    return nullptr;

  }

  #if DEBUG_DISPATCH
   String msg = "ScreenHandler::dispatch Found a widget, *** Start *** dispatching event...";
  #endif

  #if DEBUG_DISPATCH  
    int lvl        = 0;
  #endif

  Widget* widget = (Widget*)event->source;
  do {
    #if DEBUG_DISPATCH
      Serial.println(msg);
    #endif

    widget->onEvent(event);
    widget = widget->parent;
      
    #if DEBUG_DISPATCH
      msg = "ScreenHandler::dispatch Dispatch the event to its parent " + String(++lvl);
    #endif
  } while (widget && event->passOn);

  #if DEBUG_DISPATCH
    Serial.println(F("ScreenHandler::dispatch *** Finished *** event dispatching"));
  #endif

  //
  // Return the matched widget
  //
  return widget;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Dispatch the first later event in the queue. It returns false if no events are queued.
 *
 *------------------------------------------------------------------------------------------------*/
boolean ScreenHandler::dispatch() {

  if (!laterQueue) {
    return false;
  }

  TouchEventLink* link = laterQueue;
  laterQueue->remove();
  laterQueue = link->getNext();

  if (link->getEvent()) {
    dispatch(link->getEvent());
  }

  link->setPrev(nullptr);
  link->setEvent(nullptr);
  link->setNext(nullptr);

  delete link;
  return true;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Dispatch all the pending later events.
 *
 *------------------------------------------------------------------------------------------------*/
void ScreenHandler::dispatchAll() {

  //
  //  Dispatch as long as there are queued events
  //
  while (dispatch()) {
    // Do nothing
  }

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Dispatch an event that is handled at a later point in time.
 *  Handling an event later has the advantage that it limits stack growth.
 *  The events are queued on a FIFO queue and executed at its latest at two points in time.
 *    1) On entry of the dispatch() method
 *    2) Just before returning from the dispatch() method.
 *  This guarantees that there are no pending later events if dispatch is called and
 *  dispatches the event.
 *
 *  And
 *    3) After every poll for touches one later event is dispatched. This guarantees that
 *       later event execution is basically spread in time evenly, which is positive in
 *       spreading system load.
 *
 *  event      The event to dispatch a later point in time.
 *
 *------------------------------------------------------------------------------------------------*/
void ScreenHandler::dispatchLater(TouchEvent* event) {

  //
  // If no later event, then add the first event
  //
  if (! laterQueueLast) {

    TouchEventLink* link = new TouchEventLink(nullptr, event, nullptr);
    laterQueue           = link;
    laterQueueLast       = link;
    return;
  } 

  //
  // Add a later event at the end of the queue
  //
  laterQueueLast = laterQueueLast->append(event);
}

/*-------------------------------------------------------------
 *
 *  Return the object type, so we know.
 *
 *-----------------------------------------------------------*/
const char* ScreenHandler::isType() {
  return "ScreenHandler";
}

/*-------------------------------------------------------------
 *
 *  Clears the screen with an edge
 *
 *  fgColor      The edge color
 *  bgColor      The rest of the screen its color
 *
 *-----------------------------------------------------------*/
void ScreenHandler::clear(uint16_t fgColor, uint16_t bgColor) {
    
  tft->fillScreen(bgColor);
  Screen.tft->drawFastHLine(0, 0, tft->width()-1, fgColor);
  Screen.tft->drawFastHLine(0, tft->height()-1, tft->width()-1, fgColor);
  Screen.tft->drawFastVLine(0, 0, tft->height()-1, fgColor);
  Screen.tft->drawFastVLine(tft->width()-1, 0, tft->height()-1, fgColor);

}

/*-------------------------------------------------------------
 *
 *  Clears the screen (BLACK) with an edge (WHITE)
 *
 *-----------------------------------------------------------*/
void ScreenHandler::clear() {
  
  clear(BLACK, BLACK);

}


/*-------------------------------------------------------------
 *
 *  Fill a rectangle on the screen with a specific color
 *
 *  x      X-coordinate
 *  y      Y-coordinate
 *  width  The width (i.e. horizontal size)
 *  height The height(i.e. vertical size)
 *
 *-----------------------------------------------------------*/
void ScreenHandler::fillRect( int16_t x,      int16_t y,
		                     uint16_t width, uint16_t height,
							 uint16_t color) {
	if (!isVisible()) {
		return;
	}

	tft->fillRect(x, y, width, height, color);
}

void ScreenHandler::fillScreen(uint16_t color) {
    if (!isVisible()) {
    	return;
    }

    tft->fillScreen(color);
}

size_t ScreenHandler::print(char* s) {
	if (!isVisible()) {
		return;
	}

	return tft->print(s);
}


size_t ScreenHandler::print(char c) {
	if (!isVisible()) {
		return;
	}

	return tft->print(c);
}

size_t ScreenHandler::print(unsigned int j, int i = DEC) {
	if (!isVisible()) {
		return 0;
	}

	return tft->print(j, i);
}

/*

size_t ScreenHandler::print(const __FlashStringHelper * s) {
	if (!isVisible()) {
		return 0;
	}

	return tft->print(s);
}

size_t ScreenHandler::print(const String & s) {
	if (!isVisible()) {
		return 0;
	}

	return tft->print(s);
}

size_t ScreenHandler::print(const char* c) {
	if (!isVisible()) {
		return 0;
	}

	return tft->print(c);
}

size_t ScreenHandler::print(unsigned char c, int i = DEC) {
	if (!isVisible()) {
		return 0;
	}

	return tft->print(c, i);
}

size_t ScreenHandler::print(int j, int i = DEC) {
	if (!isVisible()) {
		return 0;
	}

	return tft->print(j, i);
}

size_t ScreenHandler::print(long j, int i = DEC) {
	if (!isVisible()) {
		return 0;
	}

	return tft->print(j, i);
}

size_t ScreenHandler::print(unsigned long j, int i = DEC) {
	if (!isVisible()) {
		return 0;
	}

	return tft->print(j, i);
}

size_t ScreenHandler::print(double d, int i = 2) {
	if (!isVisible()) {
		return 0;
	}

	return tft->print(d, i);
}

size_t ScreenHandler::print(const Printable& s) {
	if (!isVisible()) {
		return 0;
	}

	return tft->print(s);
}

size_t ScreenHandler::println(const __FlashStringHelper * s) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(s);
}

size_t ScreenHandler::println(const String &s) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(s);
}

size_t ScreenHandler::println(const char* c) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(c);
}

size_t ScreenHandler::println(char c) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(c);
}

size_t ScreenHandler::println(unsigned char c, int i = DEC) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(c, i);
}

size_t ScreenHandler::println(int j, int i = DEC) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(j, i);
}
size_t ScreenHandler::println(unsigned int j, int i = DEC) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(j, i);
}
size_t ScreenHandler::println(long l, int i = DEC) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(l, i);
}

size_t ScreenHandler::println(unsigned long l, int i = DEC) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(l, i);
}

size_t ScreenHandler::println(double d, int i = 2) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(d, i);
}

size_t ScreenHandler::println(const Printable& p) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println(p);
}
size_t ScreenHandler::println(void) {
	if (!isVisible()) {
		return 0;
	}

	return tft->println();

}
*/
void ScreenHandler::setTextSize(int16_t size) {
	if (!isVisible()) {
		return;
	}

	tft->setTextSize(size);
}

void ScreenHandler::setCursor(int16_t x, int16_t y) {
	if (!isVisible()) {
		return;
	}

    tft->setCursor(x, y);
}

void ScreenHandler::getTextBounds(char* s, int16_t x, int16_t y,
		            int16_t *xr,     int16_t *yr,
		           uint16_t *width, uint16_t *height) {

	if (!isVisible()) {
		return;
	}

	tft->getTextBounds(s, x, y, xr, yr, width, height);
}

void ScreenHandler::getTextBounds(String s, int16_t x, int16_t y,
		            int16_t *xr,     int16_t *yr,
		           uint16_t *width, uint16_t *height) {

	if (!isVisible()) {
		return;
	}

	tft->getTextBounds(s, x, y, xr, yr, width, height);
}

void ScreenHandler::setTextColor(uint16_t color) {
	if (!isVisible()) {
		return;
	}

    tft->setTextColor(color);
}

//
//  Initialize all the screen stuff
//
using namespace std;

MCUFRIEND_kbv tftScreen;			// Create a driver for the screen
ScreenHandler Screen(&tftScreen);	// Create Touch Screen encapsulation object.


