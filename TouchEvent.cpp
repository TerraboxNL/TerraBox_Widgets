/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <TouchEvent.cpp> - Library forGUI Widgets.
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
#include <Terrabox_Widgets.h>

#define DEBUG		0

/**--------------------------------------------------------------------------------
 *
 *  Create a TouchEvent
 *
 *  @param event        The event code
 *  @param timestamp    The time stamp of the event
 *  @param x            The x-coordinate of the event
 *  @param y            The y-coordinate of the event
 *  @param source       The widget receiving the event
 *
 *------------------------------------------------------------------------------*/
TouchEvent::TouchEvent(
      uint16_t      pEvent,
      unsigned long pTimestamp,
      int16_t       pX,
      int16_t       pY,
      EventSource*  pSource) {

	init(pEvent, pTimestamp, pX, pY, pSource, false);
}

/**--------------------------------------------------------------------------------
 *
 *  Create a TouchEvent
 *
 *  @param event        The event code
 *  @param timestamp    The time stamp of the event
 *  @param x            The x-coordinate of the event
 *  @param y            The y-coordinate of the event
 *  @param source       The widget receiving the event
 *  @param passOn       true: event must be passed on to the source its parent
 *
 *------------------------------------------------------------------------------*/
TouchEvent::TouchEvent(
      uint16_t      pEvent,
      unsigned long pTimestamp,
      int16_t       pX,
      int16_t       pY,
      EventSource*  pSource,
	  bool          pPassOn) {

	init(pEvent, pTimestamp, pX, pY, pSource, pPassOn);
}

/**--------------------------------------------------------------------------------
 *
 *  Common constructor initialization code.
 *
 *  @param event        The event code
 *  @param timestamp    The time stamp of the event
 *  @param x            The x-coordinate of the event
 *  @param y            The y-coordinate of the event
 *  @param source       The widget receiving the event
 *  @param passOn       true: event must be passed on to the source its parent
 *
 *------------------------------------------------------------------------------*/
void TouchEvent::init(
      uint16_t      pEvent,
      unsigned long pTimestamp,
      int16_t       pX,
      int16_t       pY,
      EventSource*  pSource,
	  bool          pPassOn) {

  #if DEBUG
    Serial.println("TouchEvent::TouchEvent(pEvent:" + String(pEvent) +
                                            ", pX:" + String(pX) +
                                            ", pY:" + String(pY) + ")");
  #endif

  timestamp = pTimestamp;   // The timestamp at which the event was created.
  event     = pEvent;		// The type of event
  x         = pX;		    // X coordinate
  y         = pY;		    // Y coordinate
  source    = pSource;		// Set the touched Widget, which is seen as the source of the event

  passOn    = pPassOn;		// Pass up to chain of parents

  #if DEBUG
    Serial.println("TouchEvent::TouchEvent event=" + String(event) + 
                                            ", x=" + String(x) +
                                            ", y=" + String(y));
  #endif
}
