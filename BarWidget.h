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

#ifndef BARWIDGET_h
#define BARWIDGET_h

/*============================================================================
 *  B A R  W I D G E T
 *===========================================================================*/
class BarWidget : public RectangleWidget {
  private:
    uint16_t animated        = 0;
    uint16_t colorBackground = 0;
    uint16_t colorEdge       = 0xffff;
//    uint16_t percentage      = 0;		// The current percentage

    uint16_t level2y(uint16_t percentage);

    void         updateIncr(uint16_t percentage);  // Incremental updates

  public:
    uint16_t oldPercentage   = 0;
    uint16_t tickStroke;      // The stroke thickness of the ticks
    uint16_t tickLength;      // Length of the tick
    uint16_t ticks;           // The number of ticks on the bar scale
    Levels   *levels;
    String   unit;            // unit of the tick values

             BarWidget(
                 Widget* parent,
                 int16_t px, int16_t py, uint16_t pwidth, uint16_t pheight, 
                 uint16_t pBgColor, uint16_t pStroke, uint16_t pStrokeColor, uint16_t pTickLength, 
                 uint16_t pTickStroke, Levels* pLevels, String pUnit);
//    virtual ~BarWidget();

    virtual void draw();
    virtual void redraw();
    void         update(uint16_t percentage);

//    virtual void onEvent(TouchEvent* event);
};

#endif
