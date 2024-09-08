/*-------------------------------------------------------------------------------------------------



       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                  A R D U I N O   G U I   W I D G E T S


                     (C) 2024, cor.hofman@terrabox.nl

                <BarWidget.cpp> - Library for GUI widgets.
                             June 30, 2024
                      Released into the public domain
               as GitHub project: TerraboxNL/TeraBox_Widgets
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

/**---------------------------------------------------------------------------
 *
 *  Create the Bar widget
 *
 * @param parent
 * @param px
 * @param py
 * @param pwidth
 * @param pheight
 * @param pBgColor
 * @param pStroke
 * @param pStrokeColor
 * @param pTickLength
 * @param pTickStroke
 * @param pLevels
 * @param pUnit
 *
 *---------------------------------------------------------------------------*/
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

  strcpy(nameId, "BarWidget");
  widgetSize = sizeof(BarWidget);
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
  if (isVisible()) {

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
}

/**----------------------------------------------------------------------------
 *
 *  Update the level of the bar
 *
 * @param percentage
 *
 *---------------------------------------------------------------------------*/
void BarWidget::update(uint16_t percentage) {
	if (!isVisible()) {
		return;
	}

	//
	// No update needed
	//
	if (oldPercentage == percentage)
		return;

	if (1) {
		updateIncr(percentage);
		oldPercentage = percentage;
	}
	else {
	  bool up = oldPercentage < percentage;
	  uint16_t running;

	  if (up)
		running = oldPercentage++;
	  else
		running = oldPercentage--;

	  //
	  //  Update only a slice
	  //
	  int i = 0;
	  while (i < 10 && running != percentage) {

		updateIncr(running);
		oldPercentage = running;

		if (up)
			running++;
		else
			running--;

		i++;
	  }

	  updateIncr(running);

	  oldPercentage = running; // save the running percentage
	}
//	delay(500);
}

void BarWidget::updateIncr(uint16_t runningPercentage) {

  if (!isVisible()) {
	  return;
  }

  uint16_t color = 0xFFFF;

  uint16_t updateWidth = width - 2*stroke;
  uint16_t updateX     = x + stroke;

  //
  //  Only update if the percentage has changed
  //

  if (oldPercentage == runningPercentage) {
    return;
  }

  //
  //  New value, so modify the current bar so it represent it.
  //
  if (oldPercentage != runningPercentage) {
    //
    //  If the new value is smaller we only have tor erase part of the bar.
    //
    if (runningPercentage < oldPercentage) {

      //
      //  Black out only the part that is not needed
      //

       Screen.fillRect(updateX,     level2y(oldPercentage),
                           updateWidth,  level2y(runningPercentage) - level2y(oldPercentage),
                           strokeColor);
//       delay(50);
    }

    //
    //  Otherwise we have to increase the bar by drawing the "missing" part for the increasd value
    //
    else {
        //
        //  Color from zero to LowLow border
        //
        color = RED;
        if (oldPercentage < levels->lowlow && runningPercentage > 0.0) {
          int16_t y = runningPercentage <= levels->lowlow ? level2y(runningPercentage) : level2y(levels->lowlow);
          Screen.fillRect(updateX,     y,
                               updateWidth, level2y(levels->min) - y,
                               color);
        }

        //
        //  Color from LowLow to Low border
        //
        color = YELLOW;
        if (oldPercentage < levels->low && runningPercentage > levels->lowlow) {
          int16_t y = runningPercentage <= levels->low ? level2y(runningPercentage) : level2y(levels->low);
          Screen.fillRect(updateX,     y,
                               updateWidth, level2y(levels->lowlow) - y,
                               color);
        }

        //
        //  Color up to High border
        //
        color = GREEN;
        if (oldPercentage < levels->high && runningPercentage > levels->low) {
          int16_t y = runningPercentage <= levels->high ? level2y(runningPercentage) : level2y(levels->high);
          Screen.fillRect(updateX,     y,
                               updateWidth, level2y(levels->low) - y,
                               color);
        }

        //
        //  Color up to HighHigh border
        //
        color = BLUE;
        if (oldPercentage < levels->highhigh && runningPercentage > levels->high) {
          int16_t y = runningPercentage <= levels->highhigh ? level2y(runningPercentage) : level2y(levels->highhigh);
          Screen.fillRect(updateX,     y,
                               updateWidth, level2y(levels->high) - y,
                               color);
        }

        //
        //  Color up to Max border
        //
        color = CYAN;
        if (runningPercentage > levels->highhigh) {
          int16_t y = runningPercentage <= levels->max ? level2y(runningPercentage) : level2y(levels->max);
          Screen.fillRect(updateX,     y,
                               updateWidth, level2y(levels->highhigh) - y,
                               color);
        }
    }
  }

  //
  //  Make percentage the old percentage, because it will be at the next update invocation
  //
  oldPercentage = runningPercentage;

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
  if (!isVisible())
	  return;

  if (! inverted)
    draw();
  else
    drawInverted();

  oldPercentage = 0;

}
