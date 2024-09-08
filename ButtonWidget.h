/*-------------------------------------------------------------------------------------------------



       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                  A R D U I N O   G U I   W I D G E T S


                     (C) 2024, cor.hofman@terrabox.nl

                <ButtonWidget.h> - Library for GUI widgets.
                            June 30, 2024
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

#define BUTTON_SQUARE     LABEL_SQUARE
#define BUTTON_ROUNDED    LABEL_ROUNDED

/*============================================================================
 *  B U T T O N  W I D G E T
 *===========================================================================*/
class ButtonWidget: public LabelWidget {

  public:

             ButtonWidget(
                 Widget* parent,
				 uint16_t pType,
                 int16_t px, int16_t py, uint16_t pwidth, uint16_t pheight,
                 uint16_t textSize, uint16_t pBgColor, uint16_t pStroke, uint16_t pStrokeColor,
                 uint16_t pFgColor, char* text);

             ButtonWidget(
                 Widget* parent,
                 int16_t px, int16_t py, uint16_t pwidth, uint16_t pheight,
                 uint16_t textSize, uint16_t pBgColor, uint16_t pStroke, uint16_t pStrokeColor,
                 uint16_t pFgColor, char* text);

             ButtonWidget(
                 Widget*  parent,
				 uint16_t pType,
                 int16_t  px,     int16_t  py, 
                 uint16_t pwidth, uint16_t pheight, 
                 char*    text);

             ButtonWidget(
                 Widget*  parent,
                 int16_t  px,     int16_t  py,
                 uint16_t pwidth, uint16_t pheight,
                 char*    text);

             virtual void onTouch(TouchEvent* event);     // If pressed
             virtual void onUntouch(TouchEvent* event);   // If released
             virtual void action(TouchEvent *event);      // Perform the action needed
             virtual void onInScope(TouchEvent* event);      // If touch slides over it
             virtual void onOutOfScope(TouchEvent* event);   // If touch slides out of it
};

#endif
