#include <Terrabox_Widgets.h>

#define DEBUG		0

/*--------------------------------------------------------------------------------
 *
 *  Create a TouchEvent
 *
 *  timestamp
 *  event
 *  x
 *  y
 *  source
 *
 *------------------------------------------------------------------------------*/
TouchEvent::TouchEvent(
      uint16_t      pEvent,
      unsigned long pTimestamp,
      int16_t       pX,
      int16_t       pY,
      EventSource*  pSource) {

  #if DEBUG
    Serial.println("TouchEvent::TouchEvent(pEvent:" + String(pEvent) + 
                                            ", pX:" + String(pX) +
                                            ", pY:" + String(pY) + ")");
  #endif

  timestamp = pTimestamp;		// The timestamp at which the event was created.
  event     = pEvent;		// The type of event
  x         = pX;		// X coordinate
  y         = pY;		// Y coordinate
  source    = pSource;		// Set the touched Widget, which is seen as the source of the event

  passOn    = true;		// Pass up to chain of parents

  #if DEBUG
    Serial.println("TouchEvent::TouchEvent event=" + String(event) + 
                                            ", x=" + String(x) +
                                            ", y=" + String(y));
  #endif
}
