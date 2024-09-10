
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
	  //  Copy the text to be sure it will not be corrupted!!!
	  //  This happens for instance if the string is allocated on the stack.
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
	    if (textHeight < (height - 2 * stroke - 4) && textWidth < (width - 2*stroke - 4)) {
	      break;
	    }
	    size--;
	  }
}

/**----------------------------------------------------------------------------
 *
 *  Calculates the width and height needed for the text given a text size
 *
 *  @param text         The text
 *  @param size         The text size to calculate with
 *  @param align        Text alignment
 *
 *---------------------------------------------------------------------------*/
void LabelWidget::printText(char* text,
		   uint16_t size, uint16_t align = LABEL_CENTER) {

    Screen.setTextSize(size);
    Screen.setTextColor(inverted ? ~fgColor : fgColor);

    int16_t textLength = strlen(text);

    char buff[80];  //  Buffer containing a single line of text

    //
    //  Count the number of lines
    //
    uint16_t lines = 1;
    for (int i = 0; i < textLength; i++) {
    	if (text[i] == '\n')
    		lines++;
    }

    //
    //  Calculate the text width and text height
    //
    int16_t j = 0;
    uint16_t line = 0;
    while (j < textLength) {
      for (int i = 0; i <= textLength; i++) {

        //
    	//  Do we have an end of line?
    	//  In that case we calculate the width & height of this line
    	//
    	if (text[j] == '\n' || text[j] == '\0') {
          buff[i] = '\0';

          //
          //  Calculate the width and height
          //
          uint16_t xr, yr, tw, th;
          Screen.getTextBounds(buff, x, y, &xr, &yr, &tw, &th);

    	  //
    	  // If the text must be printed, then do that.
    	  //
  		  switch (align) {
  		    case LABEL_RIGHT: {  // Right justified
      	      Screen.setCursor((x + width - 2 * stroke) - tw, centerY - round((float)(line * th)/2.0) + lines * th);
 			  break;
  		    }
  		    case LABEL_CENTER: {  // Centered
  	    	  Screen.setCursor(centerX - tw/2, centerY - round((float)(lines * th)/2.0) + line * th);
  			  break;
  		    }
  		    default: { // Left justified (LABEL_LEFT)
    	      Screen.setCursor(x + 2* stroke,  centerY - round((float)(lines * th)/2.0) + line * th);
  		    }
  		  }

  	      Screen.print(buff);
  	      line++;

          //
          //  Return if we have reached the end of the text
          //
          if (text[j] == '\0')
        	  return;

          //
          //  Break out of this loop, to start with the next text line
          //
          j++;
          break;
    	}

    	//
    	//  Copy the character
    	//
        buff[i] = text[j];
        j++;
      }
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

    int16_t textWidth  = 0;
    int16_t textHeight = 0;

    //
    //  Calculate the text dimensions
    //
    printText(newText, size, LABEL_CENTER);

    //
    //  Save the text for possible refreshment if TFT wakes up from sleep mode
    //
    strncpy(text, newText, sizeof(text)-1);
  }
  else {
	clearText();
  	text[0] = '\0';
  }
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

