/*-------------------------------------------------------------------------------------------------



       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                  A R D U I N O   G U I   W I D G E T S


                     (C) 2024, cor.hofman@terrabox.nl

               <ButtonWidget.cpp> - Library for GUI widgets.
                            June 30, 2024
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

 *------------------------------------------------------------------------------------------------*
 *
 *  C H A N G E  L O G :
 *  ============================================================================================
 *  P0001 - Initial release
 *  ============================================================================================
 *
 *------------------------------------------------------------------------------------------------*/
#include <TerraBox_Widgets.h>
#include <ButtonWidget.h>

#define DEBUG_ON_EVENT 0

/*--------------------------------------------------------------
 *
 * Create a button.
 *
 * parent  The parent from which this is a child
 * type    The type of button BUTTON_SQUARE, BUTTON_ROUNDED
 * pX      The X coordinate of the button
 * pY      The Y coordinate of the button
 * pWidth  The width of the button
 * pHeight The height of the button
 * pText   The caption of the button
 *
 *------------------------------------------------------------*/

ButtonWidget::ButtonWidget(
     Widget* parent,
     int16_t  px,     int16_t  py, 
     uint16_t pwidth, uint16_t pheight, 
     char* pText) 
      :  LabelWidget(
            parent,
		    BUTTON_SQUARE,
            px,     py,
            pwidth, pheight,
            3,      pText,
            WHITE,
            1,      GRAY_D,
         BLACK)
		 {

	strcpy(nameId, "ButtonWidget");
	widgetSize = sizeof(ButtonWidget);
}

ButtonWidget::ButtonWidget(
     Widget* parent,
	 uint16_t pType,
     int16_t  px,     int16_t  py,
     uint16_t pwidth, uint16_t pheight,
     char* pText)
      :  LabelWidget(
            parent,
		    pType,
            px,     py,
            pwidth, pheight,
            3,      pText,
            WHITE,
            1,      GRAY_D,
         BLACK)
		 {

	strcpy(nameId, "ButtonWidget");
	widgetSize = sizeof(ButtonWidget);
}

/*--------------------------------------------------------------
 *
 * Create a button.
 *
 * parent  The parent from which this is a child
 * type    The type of button BUTTON_SQUARE, BUTTON_ROUNDED
 * pX           The X coordinate of the button
 * pY           The Y coordinate of the button
 * pWidth       The width of the button
 * pHeight      The height of the button
 * pText        The caption of the button
 * bgColor      The background color
 * pStroke      The stroke width
 * pStrokeColor The stroke color
 * pFgColor     The foreground (i.e. text) color
 * pText        The text
 *
 *------------------------------------------------------------*/
ButtonWidget::ButtonWidget(
      Widget*  parent,
      int16_t  pX,       int16_t  pY,
      uint16_t pWidth,   uint16_t pHeight,
      uint16_t pTextSize,
      uint16_t pBgColor,
      uint16_t pStroke,  uint16_t pStrokeColor,
      uint16_t pFgColor, char* pText
) :   LabelWidget(
            parent,
			BUTTON_SQUARE,
            pX,        pY,
            pWidth,    pHeight,
            pTextSize, pText,
            pBgColor,
            pStroke,   pStrokeColor,
            pFgColor) {

	strcpy(nameId, "ButtonWidget");
	widgetSize = sizeof(ButtonWidget);
}

ButtonWidget::ButtonWidget(
      Widget*  parent,
	  uint16_t pType,
      int16_t  pX,       int16_t  pY,
      uint16_t pWidth,   uint16_t pHeight, 
      uint16_t pTextSize, 
      uint16_t pBgColor, 
      uint16_t pStroke,  uint16_t pStrokeColor, 
      uint16_t pFgColor, char* pText
) :   LabelWidget(
            parent,
			pType,
            pX,        pY, 
            pWidth,    pHeight,
            pTextSize, pText, 
            pBgColor, 
            pStroke,   pStrokeColor, 
            pFgColor) {

	strcpy(nameId, "ButtonWidget");
	widgetSize = sizeof(ButtonWidget);
}

/**----------------------------------------------------------------------------
 *
 *  Handle the touch event
 *
 * @param event  The event details for the touch
 *
 *---------------------------------------------------------------------------*/
void ButtonWidget::onTouch(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.print(F("Button touch")); Serial.println(id);
#endif
	drawInverted();
	action(event);
}


/**----------------------------------------------------------------------------
 *
 *  Handle the untouch event
 *
 * @param event  The event details for the untouch
 *
 *---------------------------------------------------------------------------*/
void ButtonWidget::onUntouch(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.print(F("Button untouch")); Serial.println(id);
#endif
	draw();
}

/**----------------------------------------------------------------------------
 *
 *  Perform the action associated with a button touch
 *
 * @param event  The event details for the touch
 *
 *---------------------------------------------------------------------------*/
void ButtonWidget::action(TouchEvent * event) {
#if DEBUG_ON_EVENT
	Serial.print(F("Aaaaaand....  ACTION!!!!!")); Serial.println(id);
#endif
}

/**----------------------------------------------------------------------------
 *
 *  Just draw the normal button, but do not trigger the action() method
 *
 * @param event
 *
 *---------------------------------------------------------------------------*/
void ButtonWidget::onInScope(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.print(F("onInScope")); Serial.println(id);
#endif

}

/**----------------------------------------------------------------------------
 *
 *  Just draw the normal button, but do not trigger the action() method
 *
 * @param event
 *
 *---------------------------------------------------------------------------*/
void ButtonWidget::onOutOfScope(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.print(F("onOutOfScope")); Serial.println(id);
#endif
}

