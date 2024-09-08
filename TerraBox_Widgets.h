/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <TerraBox_Widgets.h> - Library forGUI Widgets.
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
#ifndef TERRABOX_WIDGETS_h
#define TERRABOX_WIDGETS_h

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

//#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <TaskScheduler.h>

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#include <Arduino.h>


////////////////////////////////////////////////////////////////////////////////////
/////                                                                          /////
/////               Some external touch logic which would work                 /////
/////               if wrapped in an object                                    /////
/////                                                                          /////
/////                                                                          /////

        /*-----------------------------------------------------------------
        *                                                                   
        *  Little struct wrapping the XY coordinates and the pressed status. 
        *  It is used as a vehicle to get all the back from the function    
        *  in an organised way.                                             
        *                                                                   
        *-----------------------------------------------------------------*/
        struct  XY {                                                        

          int16_t  x;
          int16_t  y;
          int16_t  z;  // Pressure

         };

         extern bool        getTouchData(XY* data);
         extern bool        getRawTouchData(XY* data);
         extern void        waitForATap();
         extern bool        countDownWait(uint16_t seconds);
         extern TouchScreen ts;

/////                                                                          /////
/////                                                                          /////
////////////////////////////////////////////////////////////////////////////////////



#define ARRAY_SIZE(array)		(sizeof(array)/sizeof(array[0]))

//
//  Colors...
//  Assign human-readable names to some common 16-bit color values:
//
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY_L  0xBDF7
#define GRAY_D  0x7BEF

#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define ORANGE      0xFDA0      /* 255, 180,   0 */
#define GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define PINK        0xFC9F

//
//  Some macro's to to manipulate the brightness of color.
//
#define RED_MSK   0xF800
#define BLUE_MSK  0x001F
#define GREEN_MSK 0x07E0

#define RED_BRIGHTNESS(color, factor)         (((color & RED_MSK)/factor) & RED_MSK)
#define BLUE_BRIGHTNESS(color, factor)        (((color & BLUE_MSK)/factor) & BLUE_MSK)
#define GREEN_BRIGHTNESS(color, factor)       (((color & GREEN_MSK)/factor) & GREEN_MSK)
#define BRIGTNESS(color, factor)              (RED_BRIGHTNESS(color, factor) | BLUE_BRIGHTNESS(color, factor) | GREEN_BRIGHTNESS(color, factor))

/*============================================================================
 *  B U F E R  U I N T
 *===========================================================================*/
class BufferUInt {
  private:
    uint16_t buffer[128];

    int nextOne;	// Index to next one to be returned
    int nextFree;	// Index to next free position in the buffer
    int capacity;	// Capacity in terms of not occupied slots
    int overflow;	// The amount of overflows that occurred

  public:
    BufferUInt();

    int16_t  put(uint16_t i);	// Store a value
    uint16_t get();		        // Get a value, destructively
};

/*============================================================================
 *  L E V E L S
 *===========================================================================*/
class Levels {

  public:
    uint16_t max              = 100;
    uint16_t highhigh         = 95;
    uint16_t high             = 90;
    uint16_t low              = 10;
    uint16_t lowlow           = 5;
    uint16_t min              = 0;
    uint16_t levels[6];
    uint16_t nrLevels         = 6;

             Levels();
             Levels(uint16_t mn, uint16_t ll, uint16_t l, uint16_t h, uint16_t hh, uint16_t mx);
//    virtual ~Levels();
};

/*============================================================================
 *  E V E N T   S O U R C E
 *===========================================================================*/
class EventSource {
  public:
    EventSource();
    // This is just a place holder.
};

/*============================================================================
 *  T O U C H  E V E N T S
 *===========================================================================*/
class TouchEvents {
  public:
    static const uint16_t NONE                = 0;  // No event was recognized
    static const uint16_t TOUCH               = 1;  // The screen is touched
    static const uint16_t UNTOUCH             = 2;  // The screen is not touched anymore, i.e. end of the touch.
    static const uint16_t DRAW                = 3;  // The screen is still touched and the X, Y differ from the previous touch.
    static const uint16_t TTY_INSCOPE         = 4;  // A widget is in scope of the TTY
    static const uint16_t TTY_OUTOFSCOPE      = 5;  // A widget is out of scope of the TTY
    static const uint16_t GOTO_SLEEP          = 6;  // Inactivity timeout
    static const uint16_t WAKEUP              = 7;  // Wake up event
    static const uint16_t IN_SCOPE            = 8;  // A touch was detected in of scope of a widget, which was formerly out of scope.
    static const uint16_t OUT_OF_SCOPE        = 9;  // A touch was detected out of scope for a formerly in scope widget.
};

/*============================================================================
 *  T O U C H  E V E N T
 *===========================================================================*/
class TouchEvent {
  private:

  public:
    TouchEvent*   next;           // The next event in the chain
    TouchEvent*   prev;           // The previous event in the chain
    unsigned long timestamp;	// The creation time
    uint16_t      event;		// The event code
    int16_t       x;		// X Screen coordinate of the touch
    int16_t       y;		// Y Screen coordinate of the touch
    EventSource*  source;		// The source of the event.

    bool          passOn;		// True if the event must be passed on to the parent

    TouchEvent(
               uint16_t      pEvent,
               unsigned long timeStamp,
               int16_t       pX,
               int16_t       pY,
               EventSource*  source 
             );

    TouchEvent(
               uint16_t      pEvent,
               unsigned long timeStamp,
               int16_t       pX,
               int16_t       pY,
               EventSource*  source,
			   bool          passOn
             );

    void init(
               uint16_t      pEvent,
               unsigned long timeStamp,
               int16_t       pX,
               int16_t       pY,
               EventSource*  source,
			   bool          passOn
    		);

    EventSource* getSource();

    void         setPassOn(bool b);
    bool         isPassOn( );   
};


/*============================================================================
 *  T O U C H  E V E N T  L I N K
 *===========================================================================*/
class TouchEventLink {

  private:
    TouchEventLink* prev;
    TouchEventLink* next;
    TouchEvent*     event;

  public:

    TouchEventLink(
                    TouchEventLink* prev, 
                    TouchEvent* event, 
                    TouchEventLink* next
                  );

    TouchEventLink* remove();
    TouchEventLink* append(TouchEvent* event);
    TouchEventLink* insert(TouchEvent* event);

    TouchEvent*     getEvent();
    TouchEventLink* getNext();
    TouchEventLink* getPrev();

    void            setNext(TouchEventLink* n);
    void            setPrev(TouchEventLink* p);
    void            setEvent(TouchEvent* event);
};

/*============================================================================
 *  A R E A
 *===========================================================================*/
class Area : public EventSource {
  private:
    void calculateCenter();


  public:
    //
    // Position
    //
    int16_t  x;
    int16_t  y;
    int16_t  centerX; 
    int16_t  centerY;

    //
    //  Corners
    //
    XY ul;
    XY lr;

    //
    //  Sizing
    //
    uint16_t height;
    uint16_t width;



    Area(int16_t x, int16_t y, uint16_t width, uint16_t height);

    const   XY*  getUL( );
    const   XY*  getLR( );
    virtual void setPosition(int16_t pX, int16_t pY);
            void move(int16_t deltaX, int16_t deltaY);
            void center(int16_t ulX, int16_t ulY, int16_t lrX, int16_t lrY);

    //
    // Return the type of the object
    //
    virtual const char* isType();
};

/*============================================================================
 *  W I D G E T
 *===========================================================================*/
class Widget : public Area {
  protected:
    static uint32_t idCount;  // Initialized in the Widget.cpp file !!!!

    void calculateCenter();

  public:
    //
    // Unique widget id.
    uint32_t id;

    char      nameId[16]  = "Widget";
    uint16_t  widgetSize  = sizeof(Widget);

    //
    //  Drawing
    //
    bool     visible  = false;
    bool     inverted = false;

    //
    //  Widget tree
    //
    Widget* parent  = nullptr;	// The parent widget
    Widget* child   = nullptr;	// List of children
    Widget* sibling = nullptr;	// List of siblings

    //
    //  Facilitary
    //
    char*   logMsg;

             Widget(Widget* parent,
                 int16_t  px,     int16_t  py, 
                 uint16_t pwidth, uint16_t pheight);
//    virtual ~Widget();

    //
    //  Positions
    //
//    void            move(int16_t deltaX, int16_t deltaY);
//    void            setPosition(int16_t pX, int16_t pY);
//    void            center(int16_t ulX, int16_t ulY, int16_t llX, int16_t llY);

    //
    //  Sizes
    //
    void            setDimensions(uint16_t width, uint16_t height);

    //
    //  Widget tree
    //
    void            add(Widget* w);
    void            remove(Widget* w);
    void            remove();
    Widget*         getParent();
    void            setParent(Widget* pParent);
    Widget*         getChild();
    Widget*         getSibling();

    //
    //  Widget tree querying
    //
    virtual   bool  contains(int16_t pX, int16_t pY);
    virtual Widget* match(int16_t pX, int16_t pY);

    //
    //  Visualization
    //
    virtual void    clear();
    virtual void    draw()         = 0;
    virtual void    drawInverted() = 0;
    virtual bool    isVisible();
    virtual void    redraw()       = 0;
    virtual void    setVisible(bool visible);

    //
    // Touch function which are not screen agnostic.
    //
    virtual void    onEvent(TouchEvent* event);
    virtual void    onTouch(TouchEvent* event);
    virtual void    onUntouch(TouchEvent* event);
    virtual void    onDraw(TouchEvent* event);
    virtual void    onTtyInScope(TouchEvent* event);
    virtual void    onTtyOutOfScope(TouchEvent* event);
    virtual void    onUnsollicitedEvent(TouchEvent* event);
    virtual void    onGotoSleep(TouchEvent* event);
    virtual void    onWakeUp(TouchEvent* event);
    virtual void    onInScope(TouchEvent* event);
    virtual void    onOutOfScope(TouchEvent* event);

    virtual void    tree();
    virtual void    tree(int level);
    virtual void    path();
    virtual void    path(int level);

    virtual const char* isType();

};

/*============================================================================
 *  S P L A S H
 *===========================================================================*/
class Splash : public Widget {
  public:
    Splash();

    //
    //  Visualisation
    //
    virtual void    draw();
    virtual void    drawInverted();
    virtual void    redraw();

    virtual const char* isType();
};

/*============================================================================
 *  S C R E E N
 *===========================================================================*/
class ScreenHandler : public Widget {

  private:
	uint32_t  lastDispatch = 0;
    TouchEventLink* laterQueue;       // Event that need to be dispatched later.
    TouchEventLink* laterQueueLast;   // The last event in the queue.

  Widget* dispatchOnly(TouchEvent* event);

  public:

    MCUFRIEND_kbv* tft;

    ScreenHandler(MCUFRIEND_kbv* pTft);

            void    begin();                           // Bare bones TFT begin
            void    beginFull();                       // Productized TFT begin
            void    analyzeEEPROM();                   // Analyze EEPROM memory
            Widget* dispatch(TouchEvent* event);       // Dispatches pending later events and then offered event
            void    dispatchLater(TouchEvent* event);  // Dispatches pending later events and then offered event
            boolean dispatch();                        // Dispatch a single later event from the queue		
            void    dispatchAll();                     // Dispatch all the queued later events
            void    draw();
            void    redraw();
            void    drawInverted();
            void    drawSibling(Widget* child);

    virtual void    onTouch(TouchEvent* event);
    virtual void    onUntouch(TouchEvent* event);
    virtual void    onDraw(TouchEvent* event);
    virtual void    onGotoSleep(TouchEvent* event);
    virtual void    onWakeUp(TouchEvent* event);

//    Widget* match(int16_t x, int16_t y);
    virtual const char*   isType();

    void clear(uint16_t fgColor, uint16_t bgColor);    // Clears the screen with an edge
    void clear();                                      // Clears the screen BLAck and the edge WHITE

    //-------------------------------------------------------------------------
    //
    //  GFX methods
    //
    //-------------------------------------------------------------------------
    void fillRect(      int16_t  x,      int16_t y,
    		           uint16_t  width, uint16_t height,
				       uint16_t  color);

    void fillRoundRect( int16_t  x,       int16_t y,
    		            uint16_t width,  uint16_t height,
    					int16_t  radius, uint16_t color);

    void fillScreen(uint16_t color);

    size_t print(const char* (&s));
    size_t print(const __FlashStringHelper* s);
    size_t print(char* s);
    size_t print(char c);
    size_t print(unsigned int j, int i = DEC);
    size_t print(int j, int i = DEC);
    size_t print(unsigned long int j, int i = DEC);
    size_t print(long int j, int i = DEC);

    size_t println();
    size_t println(const char* (&s));
    size_t println(const __FlashStringHelper* s);
    size_t println(char* s);
    size_t println(char c);
    size_t println(unsigned int j, int i = DEC);
    size_t println(int j, int i = DEC);
    size_t println(unsigned long int j, int i = DEC);
    size_t println(long int j, int i = DEC);

/*
    size_t print(const __FlashStringHelper *);
    size_t print(const String & s);
    size_t print(const char* s);
    size_t print(unsigned char, int i = DEC);
    size_t print(int j, int = DEC);
    size_t print(long l, int i = DEC);
    size_t print(unsigned long l, int i = DEC);
    size_t print(double d, int i = 2);
    size_t print(const Printable& p);

    size_t println(char* s);
    size_t println(const __FlashStringHelper * s);
    size_t println(const String &s);
    size_t println(const char c);
//    size_t println(char c);
    size_t println(unsigned char c, int i = DEC);
    size_t println(int j, int i = DEC);
    size_t println(unsigned int j, int i = DEC);
    size_t println(long l, int i = DEC);
    size_t println(unsigned long l, int i = DEC);
    size_t println(double s, int i = 2);
    size_t println(const Printable& p);
    size_t println(void);
*/
    int16_t getRotation();
    void    setRotation(int16_t s);
    int16_t getTextSize();
    void    setTextSize(int16_t s);
    void    setCursor(int16_t x, int16_t y);

    void    getTextBounds(char* s,
                  int16_t x, int16_t y,
                  int16_t *xr, int16_t *yr,
                  uint16_t *width, uint16_t *height);

    void    getTextBounds(String s,
                  int16_t x, int16_t y,
                  int16_t *xr, int16_t *yr,
                  uint16_t *width, uint16_t *height);

    void    setTextColor(uint16_t strokeColor);
};

extern ScreenHandler Screen;

/*============================================================================
 *  T O U C H  H A N D L E R
 *===========================================================================*/
class TouchHandler : public Task {
    //=============================================================================================

    private:

    //=============================================================================================

    
    //----------------------------------------------------------------------------------------------
    //  Data and methods needed for TouchEvent synthesis.
    //----------------------------------------------------------------------------------------------
    bool           isPressed         = false;      // true as long as there is no end of touch detected.

    bool           pressed           = false;      // Whether the panel was pressed with the current event
    uint16_t       event             = 0;	         // The current event type
    unsigned long  timestamp         = millis();   // The point in time the current event occurred
    uint16_t       rawX              = 0;          // The current raw X coordinate
    uint16_t       rawY              = 0;          // the current raw X coordinate
    int16_t        x                 = 0;		// The X coordinate of the current event
    int16_t        y                 = 0;		// The Y coordinate of the current event
    Widget*        source            = nullptr;	// The Y coordinate of the current event

    bool           lastPressed       = false;      // Whether the panel was pressed with the last event.
    int16_t        lastEvent         = 0;          // The last event type
    unsigned long  lastTimestamp     = 0;          // The last point in time an event occurred
    uint16_t       lastRawX          = 0;          // The current raw X coordinate
    uint16_t       lastRawY          = 0;          // the current raw X coordinate
    int16_t        lastX             = 0;          // The X used in the last event
    int16_t        lastY             = 0;          // The Y used in the last event
    Widget*        lastSource        = nullptr;    // The source of the last detected touch

    void saveState();				// Save the Touch panel its state in terms of generated Touch events

    //----------------------------------------------------------------------------------------------
    //  Data and methods needed for the conversion from raw to screen coordinates
    //----------------------------------------------------------------------------------------------
    uint16_t       xSize;                          // The number of calibration marker points on the X-axis
    uint16_t       ySize;                          // The number of calibration marker points on the X-axis

    uint16_t*      xCalibration;                   // Raw X-coordinates of the marker points
    uint16_t*      yCalibration;                   // Raw Y-coordinates of the marker points
    uint16_t       markerDistance;                 // The distance between markers in pixels

    float          rawAvgXMarkerDistance;          // The average X marker distance in raw coordinates
    float          rawAvgYMarkerDistance;          // The average Y marker distance in raw coordinates

    uint16_t normalize(uint16_t  raw,              // Normalize a single coordinate X or Y. 
                       uint16_t* data, 
                       uint16_t  size, 
                       float     avgRawCellSize);
    void     normalize(XY*       touch);           // Normalize the raw X and Y coordinates

    
    //==============================================================================================

    public:

    uint32_t  inactivityTimeout  = 300000;         // Inactivity interval after which it is signaled

    //==============================================================================================

    //----------------------------------------------------------------------------------------------
    //  Constructor
    //----------------------------------------------------------------------------------------------
    TouchHandler();

    //----------------------------------------------------------------------------------------------
    //  Task control
    //----------------------------------------------------------------------------------------------
    virtual void    exec();

    //----------------------------------------------------------------------------------------------
    //  Touch detection
    //----------------------------------------------------------------------------------------------
    bool            digest();	                   // Translate touches into TouchEvents and dispatch them.

    bool            getTouch(XY* touchData);       // Returns touch position in screen coordinates

    bool            tapOrTimeout(long timeout);    // If tapped it returns true

    //----------------------------------------------------------------------------------------------
    //  Data for the coordinate conversion from raw to screen methods
    //----------------------------------------------------------------------------------------------
    void            setXCalibration(               // Assign the number of markers and the X-axis markers array
                          uint16_t size, 
                          uint16_t* markers);
    void            setYCalibration(               // Assign the number of markers and the Y-Axis markers array
                          uint16_t size, 
                          uint16_t* markers);
    void            setMarkerDistance(             // Assign the pixel distance between two markers, equal in X an Y
                          uint16_t pixels);
 
    //----------------------------------------------------------------------------------------------
    //  Getters for data attributes
    //----------------------------------------------------------------------------------------------
//    void            getRawXY(XY* xy);
//    void            getScreenXY(XY* xy);


    virtual const char*    isType();
};

extern TouchHandler Touch;

#endif
