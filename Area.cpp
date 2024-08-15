/*-------------------------------------------------------------------------------------------------


       ///////  ////////  ///////   ///////    //////       //////     /////    //    //
         //    //        //    //  //    //  //    //      //   //   //    //   // //
        //    //////    ///////   ///////   ////////      //////    //    //     //
       //    //        //  //    //  //    //    //      //    //  //    //    // //
      //    ////////  //    //  //    //  //    //      ///////     /////    //   //

     
                           A R D U I N O   G U I   W I D G E T S


                             (C) 2024, cor.hofman@terrabox.nl

                       <Source file name> - Library for GUI widgets.
                          Created by Cor Hofman, June 30, 2024
                            Released into the public domain
                              as GitHub project: TBH_A_GUI
                       under the GNU General public license V3.0
                          

 *------------------------------------------------------------------------------------------------*
 *
 *  C H A N G E  L O G :
 *  ============================================================================================
 *  P0001 - Initial release 
 *  ============================================================================================
 *
 *------------------------------------------------------------------------------------------------*/
#include <TerraBox_Widgets.h>

Area::Area(int16_t pX, int16_t pY, uint16_t pWidth, uint16_t pHeight) 
      : EventSource( ) {

      //
      // Remember the X, Y coordinates
      //
      x = pX;
      y = pY;

      //
      //      Remember the height and width
      //
      width = pWidth;
      height = pHeight;

      //
      //      Calculate the centre coordinates.
      //
      calculateCenter();

      //
      // Initialize ul and lr
      //
      ul.x = x;
      ul.y = y;
      lr.x = x + width;
      lr.y = y + height;
}

/*-------------------------------------------------------------------------------
 *
 *  Calculates the center coordinates of a widget
 *
 *  pX     The x coordinate of the touch
 *  pY     The y coordinate of the touch
 *
 *-----------------------------------------------------------------------------*/
void Area::calculateCenter() {
       //
       //  Calculate the center coordinates
       //
       centerX = x + width/2;
       centerY = y + height/2;
}

/*-------------------------------------------------------------------------------
 *
 *  Returns the upper left coordinate
 *
 *-----------------------------------------------------------------------------*/
const XY* Area::getUL( ) {
  return &ul;
}

/*-------------------------------------------------------------------------------
 *
 *  Returns the lower right coordinate
 *
 *-----------------------------------------------------------------------------*/
const XY* Area::getLR( ) {
  return &lr;
} 

/*-------------------------------------------------------------------------------
 *
 *  Moves the widget to the position specified.
 *  Note this assume width and height of the widget are known.
 *
 *  pX     The x coordinate of the touch
 *  pY     The y coordinate of the touch
 *
 *-----------------------------------------------------------------------------*/
void Area::setPosition(int16_t pX, int16_t pY) {
  move(pX-x, pY-y);
}

/*-------------------------------------------------------------------------------
 *
 *  Moves the widget over an X and Y distance .
 *
 *  pX     The x coordinate of the touch
 *  pY     The y coordinate of the touch
 *
 *-----------------------------------------------------------------------------*/
void Area::move(int16_t deltaX, int16_t deltaY) {
      //
      //      Calculate the new X and Y coordinates
      //
      x += deltaX;
      y += deltaY;

      //
      // Update the ul and rl positions as well
      //
      ul.x = x;
      ul.y = y;
      lr.x = x + width;
      lr.y = y + height;

      //
      //      Calculate the new center coordinates of the widget.
      //      As adding the delta's to the current center is cheaper,
      //      calculateCenter() is not called to do this.
      //
      centerX += deltaX;
      centerY += deltaY;
}

/*-------------------------------------------------------------------------------
 *
 *  Moves the widget to the center of the .
 *
 *  pX     The x coordinate of the touch
 *  pY     The y coordinate of the touch
 *
 *-----------------------------------------------------------------------------*/
void Area::center(int16_t ulX, int16_t ulY, int16_t lrX, int16_t lrY) {

      //
      //	  Calculate the width, height and center of the area specified
      //
      int16_t widthR  = lrX - ulX;
      int16_t heightR = lrY - ulY;
      int16_t xCenterR = ulX + widthR/2;
      int16_t yCenterR = ulY + widthR/2;

      //
      //  Calculate distance between the widgets center and the area specified
      //
      int16_t deltaX = centerX - xCenterR;
      int16_t deltaY = centerY - yCenterR;

      //
      //  Center the widget in the specified area by moving it over the calculated distance.
      //    
      move(xCenterR - deltaX, yCenterR - deltaY);
}

/*-----------------------------------------------------------------------
 *
 *  Returns the object its class name
 *
 *---------------------------------------------------------------------*/
const char* Area::isType() {
  return "Area";
}
