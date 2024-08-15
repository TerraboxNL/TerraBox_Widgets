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
#include <RectangleWidget.h>

#ifndef LABELWIDGET_h
#define LABELWIDGET_h

/*============================================================================
 *  L A B E L  W I D G E T
 *===========================================================================*/
class  LabelWidget : public RectangleWidget {
  private:
private:
    int16_t xMax;	// The x coordinate of the labels lower right corner  
    int16_t yMax;	// The y coordinate of the labels lower right corner. 

  public:
    uint16_t  size;
    uint16_t  fgColor;
    char      text[64];

             LabelWidget(
                 Widget* parent,
                 int16_t px, int16_t py, uint16_t pwidth, uint16_t pheight, 
                 uint16_t textSize, char* pText, uint16_t pBgColor, uint16_t pStroke, uint16_t pStrokeColor, 
                 uint16_t pFgColor);

    void         setFontSize(uint16_t fontSize);
    void         clear();
    void         clearPercent();
    void         clearInverted();
    void         setText(char* text);
    void         setTextPercent(char* text);
    void         setTextInverted(char* text);
    void         setTextInverterPercent(String text);
    void         setText(char* text, int16_t color);
    void         setTextPercent(char* text, int16_t color);

    uint16_t     getFgColor();

    virtual void draw();
    virtual void drawInverted();
    virtual void redraw();
//    virtual void onEvent(TouchEvent* event);
};

#endif
