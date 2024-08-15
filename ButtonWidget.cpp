#include <TerraBox_Widgets.h>
#include <ButtonWidget.h>


/*--------------------------------------------------------------
 *
 * Create a button.
 *
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
         px,     py, 
         pwidth, pheight,
         3,      pText, 
         WHITE,  
         1,      GRAY_D, 
         BLACK) {

     // Nothing to do

}

/*--------------------------------------------------------------
 *
 * Create a button.
 *
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
            pX,        pY, 
            pWidth,    pHeight,
            pTextSize, pText, 
            pBgColor, 
            pStroke,   pStrokeColor, 
            pFgColor) {

	// Nothing to do

}
