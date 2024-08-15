#include <TerraBox_Widgets.h>
#include <LevelIndicatorWidget.h>

/*==============================================================================
 *
 *  The LevelIndicatorWidget is a composite class, which combines
 *  an indicator title label, a bar dynamically visualizing a rising and falling
 *  (fluid) level and a label displaying level as a numerical value.
 *
 *============================================================================*/

/*------------------------------------------------------------------------------
 *
 *  Create the level widget
 *
 *----------------------------------------------------------------------------*/
LevelIndicatorWidget::LevelIndicatorWidget(
                         Widget* parent,
                         const char *name, uint16_t labelSize, 
                         int16_t px, int16_t py, uint16_t pwidth, uint16_t pheight,
                         uint16_t pBgColor, uint16_t pStroke, uint16_t pStrokeColor, 
                         uint16_t pTickLength, uint16_t pTickStroke, Levels* pLevels,
                         String pUnit) 
            : Widget(parent, px, py, pwidth, pheight) {

  unit = pUnit;

  // Set the text size
  Screen.tft->setTextSize(labelSize);

  //
  // Calculate the text height for the title
  //
  uint16_t xr, yr, titleWidth, titleHeight, valueHeight;
  Screen.tft->getTextBounds(name, x, y, &xr, &yr, &titleWidth, &titleHeight);
  valueHeight = titleHeight;    //  Same test size, so same height.

  //
  //  Create the title label and set the title text.
  //
  title = new LabelWidget(this,
                       x,         y, 
                       pwidth,    titleHeight + 2*pStroke, 
                       labelSize, "",         GRAY_D,   
                       pStroke,   WHITE,      WHITE);

  title->setText(name);
  currentHeight  = title->height;

  //
  //  Create the Bar widget.
  //
  bar = new BarWidget(this,
                      x,           y + currentHeight, 
                      width,       height+2*pStroke, pBgColor, 
                      pStroke,     pStrokeColor, 
                      pTickLength, pTickStroke, 
                      pLevels,     unit);
  currentHeight += bar->height;

//  bar->draw();

  //
  //  Create the value label and set the initial value
  //
  value = new LabelWidget(this, x,         y + currentHeight,
                       width,     valueHeight + 2*pStroke, 
                       labelSize, "", GRAY_D,   
                       pStroke,   WHITE,        WHITE);

  value->setText("0");
  currentHeight += value->height;

}

/*------------------------------------------------------------------------------
 *
 *  Draw the widget
 *
 *----------------------------------------------------------------------------*/
void LevelIndicatorWidget::draw() {
  title->draw();
  bar  ->draw();
  value->draw();
}

/*---------------------------------------------------------------------------------
 *
 *  Redraw it as it was drawn the last time.
 *
 *-------------------------------------------------------------------------------*/
void LevelIndicatorWidget::redraw() {

  if (!inverted)
    draw();
  else
	drawInverted();

}

/*---------------------------------------------------------------------------------
 *
 *  Redraw it as it was drawn the last time.
 *
 *-------------------------------------------------------------------------------*/
void LevelIndicatorWidget::drawInverted() {

  draw();

}

/*------------------------------------------------------------------------------
 *
 *  Update the level indicator displaying the newly measured value
 *
 *  newValue    The newly measured value
 *
 *----------------------------------------------------------------------------*/
void LevelIndicatorWidget::update(uint16_t newValue) {
  
  char buff [16];  
  //
  //  Show the new percentage explicitly
  //
  sprintf(buff, "%d", newValue);
  value->setTextPercent(buff);

  //
  //  Show the new percentage on the bar as well
  //
  bar->update(newValue);
}
