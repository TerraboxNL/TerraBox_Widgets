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

#ifndef RECTANGLEWIDGET_h
#define RECTANGLEWIDGET_h

/*============================================================================
 *  R E C T A N G L E  W I D G E T
 *===========================================================================*/
class  RectangleWidget : public Widget {
  public:
    uint16_t  stroke;           // Stroke size
    uint16_t  bgColor;          // Background color
    int16_t   strokeColor;      // Stroke color

             RectangleWidget(
                 Widget*  parent,
                 int16_t  px,       int16_t py, 
                 uint16_t pWidth,   uint16_t pHeight, 
                 uint16_t pBgColor, 
                 uint16_t pStroke,  uint16_t pStrokeColor);

    uint16_t     getStroke();
    uint16_t     getBgColor();

    virtual void draw();
    virtual void drawInverted();
    virtual void redraw();
//    virtual void onEvent(TouchEvent* event);
};

#endif
