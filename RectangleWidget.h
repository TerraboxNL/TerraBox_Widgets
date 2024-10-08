/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <RectangleWidget.h> - Library forGUI Widgets.
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

#ifndef RECTANGLEWIDGET_h
#define RECTANGLEWIDGET_h

#define RECTANGLE_SQUARE   1
#define RECTANGLE_ROUNDED  2

#define RECTANGLE_RADIUS   4

/*============================================================================
 *  R E C T A N G L E  W I D G E T
 *===========================================================================*/
class  RectangleWidget : public Widget {
  public:
	uint16_t  type;             // form factor
    uint16_t  stroke;           // Stroke size
    uint16_t  bgColor;          // Background color
    int16_t   strokeColor;      // Stroke color

             RectangleWidget(
                 Widget*  parent,
				 uint16_t pType,
                 int16_t  px,       int16_t py,
                 uint16_t pWidth,   uint16_t pHeight,
                 uint16_t pBgColor,
                 uint16_t pStroke,  uint16_t pStrokeColor);

             RectangleWidget(
                 Widget*  parent,
                 int16_t  px,       int16_t py, 
                 uint16_t pWidth,   uint16_t pHeight, 
                 uint16_t pBgColor, 
                 uint16_t pStroke,  uint16_t pStrokeColor);

    void         init(uint16_t pType,
             	      uint16_t pBgColor,
             	      uint16_t pStroke,
             		  uint16_t pStrokeColor);

    uint16_t     getStroke();
    uint16_t     getBgColor();

    virtual void draw();
    virtual void drawInverted();
    virtual void redraw();
//    virtual void onEvent(TouchEvent* event);
};

#endif
