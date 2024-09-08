/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <RectangleWidget.cpp> - Library forGUI Widgets.
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
#include <TerraBox_Widgets.h>
#include <RectangleWidget.h>

#define DEBUG      0

/*==============================================================================
 *
 *  Rectangle creates a rectangle with a background color.
 *  Optionally there is a stroke around the rectangel with another color.
 *  Note that the stroke is drawn within the dimensions of the rectangle.
 *  The stroke can have a different color.
 *
 *============================================================================*/
RectangleWidget::RectangleWidget(
      Widget*  parent,
	  uint16_t pType,
      int16_t  px,       int16_t  py, 
      uint16_t pWidth,   uint16_t pHeight, 
      uint16_t pBgColor, 
      uint16_t pStroke,  uint16_t pStrokeColor)
      : Widget(
            parent,
            px,     py, 
            pWidth, pHeight) {

	init(pType, pBgColor, pStroke, pStrokeColor);
}

RectangleWidget::RectangleWidget(
      Widget*  parent,
      int16_t  px,       int16_t  py,
      uint16_t pWidth,   uint16_t pHeight,
      uint16_t pBgColor,
      uint16_t pStroke,  uint16_t pStrokeColor)
      : Widget(
              parent,
              px,     py,
              pWidth, pHeight)  {

  init(RECTANGLE_SQUARE, pBgColor, pStroke, pStrokeColor);
}

void RectangleWidget::init(uint16_t pType,
	                  uint16_t pBgColor,
	                  uint16_t pStroke,
					  uint16_t pStrokeColor) {

	  strcpy(nameId, "RectangleWidget");
	  widgetSize    = sizeof(RectangleWidget);
	  type          = pType;
	  bgColor       = pBgColor;
	  stroke        = pStroke;
	  strokeColor   = pStrokeColor;
}

/*---------------------------------------------------------------------------------
 *
 *  Returns the stroke width.
 *
 *-------------------------------------------------------------------------------*/
uint16_t RectangleWidget::getStroke() {
  return stroke;
}


/*---------------------------------------------------------------------------------
 *
 *  Draw the rectangle on the screen.
 *
 *-------------------------------------------------------------------------------*/
void RectangleWidget::draw() {
  inverted = false;

  if (! isVisible())
	  return;

  //
  //  Draw stroke around rectangle
  //
  if (stroke != 0) {
#if DEBUG
    Serial.print(F("RectangleWidget::draw strokeColor: 0x"));
    Serial.println(strokeColor, HEX);
#endif


    switch (type) {
    case RECTANGLE_ROUNDED:
  	  Screen.fillRoundRect(x, y, width, height, RECTANGLE_RADIUS, strokeColor);
  	  break;

    case RECTANGLE_SQUARE:
    default:
  	  Screen.fillRect(x, y, width, height, strokeColor);
    }
  }

  //
  //  Fill up the inner rectangle
  //

  #if DEBUG
  Serial.print(F("RectangleWidget::draw bgColor: 0x"));
  Serial.println(bgColor, HEX);
  #endif

  switch(type) {
  case RECTANGLE_ROUNDED:
    Screen.fillRoundRect(x + stroke, y + stroke, width-2*stroke, height-2*stroke, RECTANGLE_RADIUS, bgColor);
    break;

  case RECTANGLE_SQUARE:
  default:
   Screen.fillRect(x + stroke, y + stroke, width-2*stroke, height-2*stroke, bgColor);
  }
}

/*---------------------------------------------------------------------------------
 *
 *  Draw the rectangle on the screen with inverted colours
 *
 *-------------------------------------------------------------------------------*/
void RectangleWidget::drawInverted() {

  inverted = true;

  if (! isVisible())
	  return;

  uint16_t invStrokeColor = ~strokeColor;
  uint16_t invBgColor     = ~bgColor;

  //
  //  Draw stroke around rectangle
  //
  if (stroke != 0) {
#if DEBUG
    Serial.print(F("RectangleWidget::draw invStrokeColor: 0x"));
    Serial.println(invStrokeColor, HEX);
#endif

    switch (type) {
    case RECTANGLE_ROUNDED:
  	  Screen.fillRoundRect(x, y, width, height, RECTANGLE_RADIUS, invStrokeColor);
  	  break;

    case RECTANGLE_SQUARE:
    default:
  	  Screen.fillRect(x, y, width, height, invStrokeColor);
    }
  }

  //
  //  Fill up the inner rectangle
  //
  #if DEBUG
    Serial.print(F("RectangleWidget::draw invBgColor: 0x"));
    Serial.println(invBgColor, HEX);
  #endif

    switch(type) {
    case RECTANGLE_ROUNDED:
      Screen.fillRoundRect(x + stroke, y + stroke, width-2*stroke, height-2*stroke, RECTANGLE_RADIUS, invBgColor);
      break;

    case RECTANGLE_SQUARE:
    default:
      Screen.fillRect(x + stroke, y + stroke, width-2*stroke, height-2*stroke, invBgColor);
    }

}

/*---------------------------------------------------------------------------------
 *
 *  Redraw it as it was drawn the last time.
 *
 *-------------------------------------------------------------------------------*/
void RectangleWidget::redraw() {

  if (! isVisible())
	return;

  if (! inverted)
    draw();
  else
    drawInverted();

}
