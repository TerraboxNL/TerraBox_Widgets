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
      int16_t  px,       int16_t  py, 
      uint16_t pWidth,   uint16_t pHeight, 
      uint16_t pBgColor, 
      uint16_t pStroke,  uint16_t pStrokeColor)
      : Widget(
            parent,
            px,     py, 
            pWidth, pHeight) {

  stroke        = pStroke;
  bgColor       = pBgColor;
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

  //
  //  Draw stroke around rectangle
  //
  #if DEBUG
    Serial.print(F("RectangleWidget::draw strokeColor: 0x"));
    Serial.println(strokeColor, HEX);
  #endif

  Screen.tft->fillRect(x, y, width, height, strokeColor);

  //
  //  Fill up the inner rectangle
  //

  #if DEBUG
    Serial.print(F("RectangleWidget::draw bgColor: 0x"));
    Serial.println(bgColor, HEX);
  #endif

  Screen.tft->fillRect(x + stroke, y + stroke, width-2*stroke, height-2*stroke, bgColor);
}

/*---------------------------------------------------------------------------------
 *
 *  Draw the rectangle on the screen with inverted colours
 *
 *-------------------------------------------------------------------------------*/
void RectangleWidget::drawInverted() {

  inverted = true;

  uint16_t invStrokeColor = ~strokeColor;
  uint16_t invBgColor     = ~bgColor;

  //
  //  Draw stroke around rectangle
  //
  #if DEBUG
    Serial.print("RectangleWidget::draw invStrokeColor: 0x");
    Serial.println(invStrokeColor, HEX);
  #endif

  Screen.tft->fillRect(x, y, width, height, invStrokeColor);

  //
  //  Fill up the inner rectangle
  //
  #if DEBUG
    Serial.print("RectangleWidget::draw invBgColor: 0x");
    Serial.println(invBgColor, HEX);
  #endif

  Screen.tft->fillRect(x + stroke, y + stroke, width-2*stroke, height-2*stroke, invBgColor);

}

/*---------------------------------------------------------------------------------
 *
 *  Redraw it as it was drawn the last time.
 *
 *-------------------------------------------------------------------------------*/
void RectangleWidget::redraw() {

  if (! inverted)
    draw();
  else
    drawInverted();

}
