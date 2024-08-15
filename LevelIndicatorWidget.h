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
#include <BarWidget.h>
#include <LabelWidget.h>

#ifndef LEVELINDICATORWIDGET_h
#define LEVELINDICATORWIDGET_h


/*============================================================================
 *  L E V E L  I N D I C A T O R  W I D G E T
 *===========================================================================*/
class LevelIndicatorWidget : public Widget {
  private:
    LabelWidget *title;
    LabelWidget *value;
    uint16_t    currentHeight;
    const String unit;

  public:

    BarWidget   *bar;

             LevelIndicatorWidget(
                   Widget* parent, const char *name, uint16_t labelSize, 
                   int16_t px, int16_t py, uint16_t pwidth, uint16_t pheight, 
                   uint16_t pBgColor, uint16_t pStroke, uint16_t pStrokeColor, 
                   uint16_t pTickLength, uint16_t pTickStroke, Levels* pLevels,
                   String pUnit);
//    virtual ~LevelIndicatorWidget();

    virtual void draw();
    virtual void redraw();
    virtual void drawInverted();
    void         update(uint16_t newValue);
//    virtual void onEvent(TouchEvent* event);
};

#endif
