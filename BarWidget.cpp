#include <Terrabox_Widgets.h>
#include <BarWidget.h>

/*==============================================================================
 *
 *  Create a text label in a specific color with a background color
 *  and an optional stroke.
 *  Note that the height of the widget can be adjusted depending on the
 *  font size of the text.
 *
 *============================================================================*/

/*------------------------------------------------------------------------------
 *
 *  Translate the level percentage to an actual y-coordinate.
 *
 *----------------------------------------------------------------------------*/
//uint16_t BarWidget::level2y(uint16_t percentage);
uint16_t BarWidget::level2y(uint16_t percentage) {
  return y + height - round((((float)height/100.0) * percentage));
}
#define LEVEL2Y(percentage)  (y + height - round((((float)height/100.0) * percentage)))
/*------------------------------------------------------------------------------
 *
 *  Create the level widget
 *
 */
BarWidget::BarWidget(Widget* parent, int16_t px, int16_t py, uint16_t pwidth, uint16_t pheight,
                     uint16_t pBgColor, uint16_t pStroke, uint16_t pStrokeColor, uint16_t pTickLength, 
                     uint16_t pTickStroke, Levels* pLevels, String pUnit) 
      : RectangleWidget(parent, px, py, pwidth, pheight, pBgColor, pStroke, pStrokeColor) {
  tickLength  = pTickLength;
  stroke      = pStroke;
  tickStroke  = pTickStroke;
  levels      = pLevels;
  unit        = pUnit;

  oldPercentage = 0;
}

void BarWidget::draw() {
  inverted = false;

  //
  // Draw the rectangle widget
  //
  RectangleWidget::draw();

  //
  //  Draw ticks for the levels
  //
  int16_t  centerCorrection = -(tickStroke>>1);
  uint16_t spacing = 5;
  Screen.setTextColor(strokeColor);

  //
  //  Draw the ticks
  //
  for (int i = 0; i < levels->nrLevels; i++) {
    int16_t  xTick = x + width + tickLength + spacing;
    uint16_t yTick = level2y(levels->levels[i]);

    Screen.fillRect(x + width, yTick + centerCorrection, tickLength,
                         stroke,    strokeColor);

    int16_t xr    = x + width + tickLength + spacing;
    int16_t yr    = yTick;
    uint16_t txtWidth;
    uint16_t txtHeight;

    Serial.println(levels->levels[i]);
    Screen.setTextSize(1);
    Screen.getTextBounds(String(levels->levels[i]),
                              xTick,    yTick,
                              &xr,       &yr,
                              &txtWidth, &txtHeight);
    Screen.setCursor(xTick, yTick - (txtHeight>>1));
    Screen.print(levels->levels[i]);
    Screen.print('%');
  }
}

void BarWidget::update(uint16_t percentage) {

	//
	// No update needed
	//
	if (oldPercentage == percentage)
		return;

	bool up = oldPercentage < percentage;
	uint16_t running;

	if (up)
		running = oldPercentage++;
	else
		running = oldPercentage--;

	while (running != percentage) {

		updateIncr(running);
		oldPercentage = running;

		if (up)
			running++;
		else
			running--;
	}

	updateIncr(running);

	oldPercentage = percentage;
	delay(500);
}

void BarWidget::updateIncr(uint16_t percentage) {

  uint16_t color = 0xFFFF;

  uint16_t updateWidth = width - 2*stroke;
  uint16_t updateX     = x + stroke;

  //
  //  Only update if the percentage has changed
  //

  if (oldPercentage == percentage) {
    return;
  }

  //
  //  New value, so modify the current bar so it represent it.
  //
  if (oldPercentage != percentage) {
    //
    //  If the new value is smaller we only have tor erase part of the bar.
    //
    if (percentage < oldPercentage) {

      //
      //  Black out only the part that is not needed
      //

       Screen.fillRect(updateX,     level2y(oldPercentage),
                           updateWidth,  level2y(percentage) - level2y(oldPercentage),
                           strokeColor);
       delay(50);
    }

    //
    //  Otherwise we have to increase the bar by drawing the "missing" part for the increasd value
    //
    else {
        //
        //  Color from zero to LowLow border
        //
        color = RED;
        if (oldPercentage < levels->lowlow && percentage > 0.0) {
          int16_t y = percentage <= levels->lowlow ? level2y(percentage) : level2y(levels->min);
          Screen.fillRect(updateX,     y,
                               updateWidth, level2y(levels->min) - y,
                               color);
        }

        //
        //  Color from LowLow to Low border
        //
        color = YELLOW;
        if (oldPercentage < levels->low && percentage > levels->lowlow) {
          int16_t y = percentage <= levels->low ? level2y(percentage) : level2y(levels->low);
          Screen.fillRect(updateX,     y,
                               updateWidth, level2y(levels->lowlow) - y,
                               color);
        }

        //
        //  Color up to High border
        //
        color = GREEN;
        if (oldPercentage < levels->high && percentage > levels->low) {
          int16_t y = percentage <= levels->high ? level2y(percentage) : level2y(levels->high);
          Screen.fillRect(updateX,     y,
                               updateWidth, level2y(levels->low) - y,
                               color);
        }

        //
        //  Color up to HighHigh border
        //
        color = BLUE;
        if (oldPercentage < levels->highhigh && percentage > levels->high) {
          int16_t y = percentage <= levels->highhigh ? level2y(percentage) : level2y(levels->highhigh);
          Screen.fillRect(updateX,     y,
                               updateWidth, level2y(levels->high) - y,
                               color);
        }

        //
        //  Color up to Max border
        //
        color = CYAN;
        if (percentage > levels->highhigh) {
          int16_t y = percentage <= levels->max ? level2y(percentage) : level2y(levels->max);
          Screen.fillRect(updateX,     y,
                               updateWidth, level2y(levels->highhigh) - y,
                               color);
        }
    }
  }

  //
  //  Make percentage the old percentage, because it will be at the next update invocation
  //
  oldPercentage = percentage;

}

void drawInverted() {
      /* Not supported */
}

/*---------------------------------------------------------------------------------
 *
 *  Redraw it as it was drawn the last time.
 *
 *-------------------------------------------------------------------------------*/
void BarWidget::redraw() {

  if (! inverted)
    draw();
  else
    drawInverted();

  //
  // Set to the last percentage
  //
  update(oldPercentage);
}
