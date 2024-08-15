
#include <Terrabox_Widgets.h>
#include <LabelWidget.h>

#define DEBUG 0
/*==============================================================================
 *  Creates a text label with a specific background color and an optional stroke
 *  around the label area with another color of choice.
 *  Note that the height of the widget can be adjusted depending on the
 *  font size of the text.
 *============================================================================*/
LabelWidget::LabelWidget( Widget* parent,
		                  int16_t px,       int16_t py,  uint16_t pwidth, uint16_t pheight,
		                 uint16_t textSize, char* pText, uint16_t pBgColor,
						 uint16_t pStroke,  uint16_t pStrokeColor,
						 uint16_t pFgColor) :
  RectangleWidget(parent, px, py, pwidth, pheight, pBgColor, pStroke, pStrokeColor) {

  //
  //  Set the text
  //
  strcpy(text, pText);

  //
  //  Font size
  //
  size = textSize;
  // Serial.println("(1) LabelWidget::Textsize: " + String(textSize));

  //
  //  Set the colors
  //
  bgColor = pBgColor;
  fgColor = pFgColor;

  //
  //  Shrink the text if it will not fit in the space of the are specified.
  //
  while (size > 1) {
    Screen.setTextSize(size);
    uint16_t xr, yr, textWidth, textHeight;
    Screen.getTextBounds("W", x, y, &xr, &yr, &textWidth, &textHeight);
//    Serial.println("label height vs. text height [" + String(size) + "]: " + (height) + "/" + textHeight);
    if (textHeight < (height - 2 * stroke - 4)) {
//      Serial.println("Selected text size: " + String(size));
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
   clear();

  //
  //  If no text, then skip right to the end.
  //
  if (newText != nullptr && newText[0] != '\0') {
    // Establish the width
    Screen.setTextSize(size);
    uint16_t xr, yr, textWidth, textHeight;
    Screen.getTextBounds(newText, x, y, &xr, &yr, &textWidth, &textHeight);

    //  Center the text and show it.
    Screen.setTextSize(size);
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
void LabelWidget::clear() {
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
void LabelWidget::clearPercent() {
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
  //  First pf all set the text size
  //
  Screen.setTextSize(size);

  //
  //  Clear the current text
  //
  clearPercent();

  //
  //  If no text, then skip right to the end.
  //
  if (newText != nullptr && newText[0] != '\0') {
    // Establish the width
    uint16_t xr, yr, textWidth, textHeight;
    Screen.getTextBounds(newText, x, y, &xr, &yr, &textWidth, &textHeight);

    Screen.setTextSize(size);
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

/*-------------------------------------------------------------------------------
 *
 *  Draws the widget including the currenr or last text that was displayed.
 *  Effectively it refreshes the widget by redrawing it.
 *
 *-----------------------------------------------------------------------------*/
void LabelWidget::draw() {

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
  if (!inverted) {
	  draw();
  }
  else {
	  drawInverted();
  }
}

void LabelWidget::drawInverted() {

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

/*
void LabelWidget::cpyTxt(char *s1, char *s2) {
	while (s2) {
		*s1++ = *s2++;
	}
}
*/

