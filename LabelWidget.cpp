
/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <LabelWidget.cpp> - Library forGUI Widgets.
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
#include <LabelWidget.h>

#define DEBUG 0

#define LABEL_SQUARE   RECTANGLE_SQUARE
#define LABEL_ROUNDED  RECTANGLE_ROUNDED

/*==============================================================================
 *  Creates a text label with a specific background color and an optional stroke
 *  around the label area with another color of choice.
 *  Note that the height of the widget can be adjusted depending on the
 *  font size of the text.
 *============================================================================*/
LabelWidget::LabelWidget( Widget* parent,
		                  uint16_t pType,
		                  int16_t  px,       int16_t py,  uint16_t pwidth, uint16_t pheight,
		                  uint16_t textSize, char* pText, uint16_t pBgColor,
						  uint16_t pStroke,  uint16_t pStrokeColor,
						  uint16_t pFgColor) :
  RectangleWidget(parent, pType, px, py, pwidth, pheight, pBgColor, pStroke, pStrokeColor) {

  init(textSize, pText, pFgColor);

}

LabelWidget::LabelWidget( Widget* parent,
		                  int16_t  px,       int16_t py,  uint16_t pwidth, uint16_t pheight,
		                  uint16_t textSize, char* pText, uint16_t pBgColor,
						  uint16_t pStroke,  uint16_t pStrokeColor,
						  uint16_t pFgColor) :
		  RectangleWidget(parent, LABEL_SQUARE, px, py, pwidth, pheight, pBgColor, pStroke, pStrokeColor) {

	  init(textSize, pText, pFgColor);
}

void LabelWidget::init(uint16_t textSize,
		               char*    pText,
		               uint16_t pFgColor) {

	  strcpy(nameId, "LabelWidget");
	  widgetSize = sizeof(LabelWidget);

	  //
	  //  Set the text
	  //
	  strncpy(text, pText, sizeof(text)-1);

	  //
	  //  Font size
	  //
	  size = textSize;

	  //
	  //  Set the colors
	  //
	  fgColor = pFgColor;

	  //
	  //  Shrink the text if it will not fit in the space of the are specified.
	  //
	  while (size > 1) {
	    Screen.setTextSize(size);
	    uint16_t xr, yr, textWidth, textHeight;
	    Screen.getTextBounds("W", x, y, &xr, &yr, &textWidth, &textHeight);
	    if (textHeight < (height - 2 * stroke - 4)) {
	      break;
	    }
	    size--;
	  }
}

/*-------------------------------------------------------------------------------
 *  Display a text with a color specific color.
 *  With this method it is possible to override the text color that was
 *  specified at construction time. The different color will not stick.
 *  The text will be displayed immediately. So calling draw() is not necesary.
 *
 *  tft     The screen driver
 *  text    The text to be displayed
 *-----------------------------------------------------------------------------*/
void LabelWidget::setText(char* newText) {

   //
   //  No update if value did not change
   //
   if (!strcmp(text, newText))
	 return;

   //
   //  Even though we are invisible, we still have to remember the text set!!!
   //
   if (!isVisible()) {
	 strncpy(text, newText, sizeof(text)-1);
	 return;
   }

   clearText();

  //
  //  If no text, then skip right to the end.
  //
  if (newText != nullptr && newText[0] != '\0') {
    // Establish the width
    Screen.setTextSize(size);
    uint16_t xr, yr, textWidth, textHeight;
    Screen.getTextBounds(newText, x, y, &xr, &yr, &textWidth, &textHeight);

    //  Center the text and show it.
    Screen.setTextColor(inverted ? ~fgColor : fgColor);
    Screen.setCursor(centerX - textWidth/2, centerY - round(float(textHeight/2.0)));
    Screen.print(newText);
  }

  if (newText == nullptr)
	text[0] = '\0';
  else
    strncpy(text, newText, sizeof(text)-1);

}

/*-------------------------------------------------------------------------------
 *
 *  Clears the text currently displayed.
 *  Effectively it redraws the background including the strokes around it.
 *
 *-----------------------------------------------------------------------------*/
void LabelWidget::clearText() {
  if (text != nullptr && text[0] != '\0') {
	  uint16_t xr, yr, textWidth, textHeight;
	  Screen.getTextBounds(text, x, y, &xr, &yr, &textWidth, &textHeight);

	  Screen.fillRect(centerX - textWidth/2,
	  		               centerY - round(float(textHeight/2.0)),
						   textWidth, textHeight, bgColor);
  }
}

/*-------------------------------------------------------------------------------
 *
 *  Clears the text ending on a % currently displayed.
 *  Effectively it redraws the background including the strokes around it.
 *  This is a hack due to trouble printing the % sign
 *
 *-----------------------------------------------------------------------------*/
void LabelWidget::clearPercentText() {
  if (text != nullptr && text[0] != '\0') {
	  uint16_t xr, yr, textWidth, textHeight;
	  Screen.getTextBounds(text, x, y, &xr, &yr, &textWidth, &textHeight);
	  uint16_t textWidthPercent, textHeightPercent;
	  Screen.getTextBounds(text, x, y, &xr, &yr, &textWidthPercent, &textHeightPercent);

	  Screen.fillRect(centerX - (textWidth + textWidthPercent)/2,
	  		               centerY - round(float(textHeight/2.0)),
						   textWidth+textWidthPercent, textHeight, bgColor);
  }
}

/*-------------------------------------------------------------------------------
 *
 *  Set a text with a percent sign at the end
 *  Note that this is a hack, because I had problems just printing it..
 *
 *-----------------------------------------------------------------------------*/
void LabelWidget::setTextPercent(char* newText) {

  //
  //  No update if value did not change
  //
  if (!strcmp(text, newText))
	  return;

  //
  //  Even though we are invisible, we still have to remember the text set!!!
  //
  if (!isVisible()) {
	strncpy(text, newText, sizeof(text)-1);
    return;
  }

  //
  //  Clear the current text
  //
  clearPercentText();

  //
  //  First pf all set the text size
  //
//  Screen.setTextSize(size);

  //
  //  If no text, then skip right to the end.
  //
  if (newText != nullptr && newText[0] != '\0') {
    // Establish the width
	Screen.setTextSize(size);
    uint16_t xr, yr, textWidth, textHeight;
    Screen.getTextBounds(newText, x, y, &xr, &yr, &textWidth, &textHeight);

    Screen.setTextColor(inverted ? ~fgColor : fgColor);
    Screen.setCursor(centerX - textWidth/2, centerY - round(float(textHeight/2.0)));
    Screen.print(newText);
    Screen.print('%');
  }

  if (newText == nullptr)
	text[0] = '\0';
  else
    strncpy(text, newText, sizeof(text)-1);
}

/**----------------------------------------------------------------------------
 *
 *  Returns the current text of the label
 *
 *  @return   The current label text.
 *
 *---------------------------------------------------------------------------*/
const char * LabelWidget::getText() {
	return text;
}

/*-------------------------------------------------------------------------------
 *
 *  Draws the widget including the currenr or last text that was displayed.
 *  Effectively it refreshes the widget by redrawing it.
 *
 *-----------------------------------------------------------------------------*/
void LabelWidget::draw() {

  if (! isVisible())
	  return;

  RectangleWidget::draw();
  //
  //  Force a refresh of the text.
  //  Normally it will compare the oldText with the new text.
  //  If equal it will not update the text.
  //
  char newText[64];
  strncpy(newText, text, sizeof(text)-1);
  *text = '\0';

  //
  //  Now offer the same text again
  //
  setText(newText);
}

void LabelWidget::redraw() {

  if (! isVisible()) {
    return;
  }

  if (!inverted) {
	  draw();
  }
  else {
	  drawInverted();
  }

}

void LabelWidget::drawInverted() {

	if (! isVisible())
	  return;


	RectangleWidget::drawInverted();

	//
	//  Force a refresh of the text.
	//  Normally it will compare the oldText with the new text.
	//  If equal it will not update the text.
	//
	char newText[64];
	strncpy(newText, text, sizeof(text)-1);
	text[0] = '\0';

	//
	//  Now offer the same text again
	//
	setText(newText);

}

