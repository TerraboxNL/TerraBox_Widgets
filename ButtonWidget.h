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
#include <LabelWidget.h>

#ifndef BUTTONWIDGET_h
#define BUTTONWIDGET_h

/*============================================================================
 *  B U T T O N  W I D G E T
 *===========================================================================*/
class ButtonWidget: public LabelWidget {
public:

             ButtonWidget(
                 Widget* parent,
                 int16_t px, int16_t py, uint16_t pwidth, uint16_t pheight, 
                 uint16_t textSize, uint16_t pBgColor, uint16_t pStroke, uint16_t pStrokeColor, 
                 uint16_t pFgColor, char* text);

             ButtonWidget(
                 Widget*  parent,
                 int16_t  px,     int16_t  py, 
                 uint16_t pwidth, uint16_t pheight, 
                 char*    text);

    virtual void onEvent(TouchEvent* event);
};

#endif
