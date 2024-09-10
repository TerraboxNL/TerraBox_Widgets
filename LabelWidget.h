/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <LabelWidget.h> - Library forGUI Widgets.
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
#include <RectangleWidget.h>

#ifndef LABELWIDGET_h
#define LABELWIDGET_h

#define LABEL_SQUARE    1       // Draw with square corners
#define LABEL_ROUNDED   2       // Draw with rounded corners

#define LABEL_LEFT      1		// Left justified
#define LABEL_RIGHT     2		// Right justified
#define LABEL_CENTER    3		// Centered

/*============================================================================
 *  L A B E L  W I D G E T
 *===========================================================================*/
class  LabelWidget : public RectangleWidget {
  private:
    int16_t  xMax;	// The x coordinate of the labels lower right corner
    int16_t  yMax;	// The y coordinate of the labels lower right corner.
    uint16_t type = LABEL_SQUARE;  // Default type is SQUARE

  public:
    uint16_t  size;
    uint16_t  fgColor;
    char      text[64];

             LabelWidget(
                 Widget* parent,
                 int16_t px, int16_t py, uint16_t pwidth, uint16_t pheight, 
                 uint16_t textSize, char* pText, uint16_t pBgColor, uint16_t pStroke, uint16_t pStrokeColor, 
                 uint16_t pFgColor);

             LabelWidget(
                 Widget* parent,
				 uint16_t type,
                 int16_t px, int16_t py, uint16_t pwidth, uint16_t pheight,
                 uint16_t textSize, char* pText, uint16_t pBgColor, uint16_t pStroke, uint16_t pStrokeColor,
                 uint16_t pFgColor);

    void     init(
                 uint16_t textSize,
				 char*    pText,
	             uint16_t pFgColor);

    void         setFontSize(uint16_t fontSize);
    void         clearText();
    void         clearPercentText();
    void         clearInverted();
    const char*  getText();
    void         setText(char* text);
    void         setTextPercent(char* text);
    void         setTextInverted(char* text);
    void         setTextInverterPercent(String text);
    void         setText(char* text, int16_t color);
    void         setTextPercent(char* text, int16_t color);
    void         printText(char* text, uint16_t size, uint16_t align);

    uint16_t     getFgColor();

    virtual void draw();
    virtual void drawInverted();
    virtual void redraw();
//    virtual void onEvent(TouchEvent* event);
};

#endif
