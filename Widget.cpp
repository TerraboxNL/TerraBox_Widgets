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

#define DEBUG_CONTAINS	0
#define DEBUG_MATCH	0

/*==============================================================================
 *
 *  Widget is a base class for visualizations in GUI's
 *  It holds the basic properties for coordinates and size of a widget.
 *  It also specifies the basic visualisation method draw().
 *
 *============================================================================*/


/*-------------------------------------------------------------------------------
 *
 *  Constructor creating a Widget.
 *
 *  pX     The x coordinate of the touch
 *  pY     The y coordinate of the touch
 *
 *-----------------------------------------------------------------------------*/
Widget::Widget(Widget* pParent, int16_t pX, int16_t pY, uint16_t pWidth, uint16_t pHeight)
     : Area(pX, pY, pWidth, pHeight) {
      //
      //  Initialize some pointers
      //
      child   = nullptr;
      sibling = nullptr;

      //
      // Add this widget to its parent, if it has one...
      //
      setParent(pParent);

      logMsg = "No messages at Widget creation time\0";

      visible = true;
}

/*-----------------------------------------------------------------------------
 *
 *  Add the widget as a child.
 *  Note that the child attribute holds a list of siblings, which are in fact
 *  all the children of the this widget. As this widget is their parent.
 *
 *  w    The child to add to this widget, which is the parent. See setParent()
 *
 *---------------------------------------------------------------------------*/
void Widget::add(Widget* w) {
   //
   // If no children yet, then this is the first one
   // Make it the head of the list of children.
   //
   if (! child) {
     child = w;
//     w->sibling = nullptr;
   }
   //
   // Otherwise the new child already has siblings
   //
   else {
     w->sibling = child;  // Insert the new child as the head of the sibling chain.
     child      = w;      // Assign the head of the siblings to the paren child.
   }
}

/*-----------------------------------------------------------------------------
 *
 *  Remove this widget as a child from the parent by removing it from the
 *  sibling chain.
 *  If this widget happend to be the first child in the chain then its
 *  sibling becomes the one the child pointer of parent points is pointing
 *  to afterwards.
 *
 *---------------------------------------------------------------------------*/
void Widget::remove(Widget* w) {
  Widget* prev = nullptr;
  for (Widget* c = child; c; c = c->sibling) {

    //
    // Found the Widget to be removed?
    //
    if (c == w) {

      //
      // If c is the head of the siblings list
      if (c == child) {
         child = c->sibling;          // Make the next sibling the head of the list
      }
      else {
        prev->sibling = c->sibling;   // Remove the sibling from the list.
      }

      //
      //  Mission accomplished
      //
      break;
    }

    //
    // Make the the current child the previous child.
    //
    prev = c;
  }
}

/*------------------------------------------------------------------------------
 *
 *  Assign the specified parent to this widget.
 *  It also adds this widget as a child of the parent.
 *  If this is the first child, then the parents child pointer will point to it.
 *  If more children are added afterwards then it will have its place further
 *  down the sibling chain.
 *
 *----------------------------------------------------------------------------*/
void Widget::setParent(Widget* pParent) {

      parent = pParent;

      if (parent) {
        parent->add(this);
      }
}

/*------------------------------------------------------------------------------
 *
 *  Returns the assigned parent of this widget.
 *
 *----------------------------------------------------------------------------*/
Widget* Widget::getParent() {
  return parent;
}

/*------------------------------------------------------------------------------
 *
 *  Returns the next child in the chain.
 *  Note that if you want all children you should
 *  1) get the child if this widget
 *  2) then get the child its sibling
 *  3) then consequently get the sibling its sibling for the all the children
 *
 *----------------------------------------------------------------------------*/
Widget* Widget::getChild() {
  return child;
}

/*------------------------------------------------------------------------------
 *
 *  Returns the next sibling in the chain.
 *  Note that siblings share the same hierarchical level in the widget tree
 *
 *----------------------------------------------------------------------------*/
Widget* Widget::getSibling() {
  return sibling;
}
    
/*-------------------------------------------------------------------------------
 *
 *  Clears the area the widget occupies.
 *
 *  pX     The x coordinate of the touch
 *  pY     The y coordinate of the touch
 *
 *-----------------------------------------------------------------------------*/
void Widget::clear() {
      Screen.tft->fillRect(x, y, width, height, BLACK);
}

/*---------------------------------------------------------------------------------
 *
 *  Returns true if the coordinates passed are part of the area the label occupies.
 *
 *  pX     The x coordinate
 *  pY     The y coordinate
 *
 *-----------------------------------------------------------------------------*/
bool Widget::contains(int16_t pX, int16_t pY) {
      #if DEBUG_CONTAINS
      Serial.print(F("Widget::contains(pX:"));
      Serial.print(String(pX));
      Serial.print(F(", pY"));
      Serial.print(pY);
      Serial.print(F(")--> "));
      Serial.print(F("pX:"));
      Serial.print(pX);
      Serial.print(F(" >= x:"));
      Serial.print(x);
      Serial.print(F(" && pX:"));
      Serial.print(pX);
      Serial.print(F(" <= width:"));
      Serial.print((x+width));
      Serial.print(F(" && pY:"));
      Serial.print(pY);
      Serial.print(F(" >= y:"));
      Serial.print(y);
      Serial.print(F(" && pY:"));
      Serial.print(pY);
      Serial.print(F(" <= height:"));
      Serial.print((y+height));
      Serial.print(F(" ==> ");
      Serial.println(((pX >= x && pX <= (x+width) && pY >= y && pY <= (y+height)) ? "TRUE" : "FALSE"));
      #endif

      return (pX >= x && pX <= (x+width) && pY >= y && pY <= (y+height));
}

/*---------------------------------------------------------------------------------
 *
 *  Returns the deepest child in the hierarchy containing the coordinates.
 *
 *  pX     The x coordinate of the touch
 *  pY     The y coordinate of the touch
 *
 *-------------------------------------------------------------------------------*/
Widget* Widget::match(int16_t pX, int16_t pY) {

  //
  // If this widget contains the passed x,y coordinates, it matches.
  //
  #if DEBUG_MATCH
    Serial.println("Widget::match() X,Y = : " + String(pX) + "," + String(pY) + 
                             " in " + String(this->x) + "," + String(this->y) + 
                             " X " + String(this->x+this->width) + "," + String(this->y + this->height));
  #endif

  if (contains(pX, pY)) {
    #if DEBUG_MATCH
      Serial.println("Widget::match:  ---> YES!!!!");
    #endif
    //
    //  The try its child and siblings, which are in the z-order.
    //
    for (Widget* w = child;        // Start with the first child 
                 w;                // as long as we have a sibling
                 w = w->sibling) { // fetch the next sibling

       Widget* deepestMatchingWidget = w->match(pX, pY);
       if (deepestMatchingWidget)
         return deepestMatchingWidget;
         
       //
       // If no result from the depth first approach
       // Try another round with the current child its sibling.
       // 
    }

    //
    // No deeper matching child or child its sibling, so this is the deepest matching one.
    //
    return this;
  }

  //
  // No match
  //
  #if DEBUG_MATCH
    Serial.println("Widget::match:  ---> NO!!!!");
  #endif

  return nullptr;
}

/*----------------------------------------------------------------------
 *
 *  Processes Screen events that are delivered to this Widget
 *
 *  event      The event to process
 *
 *--------------------------------------------------------------------*/
void Widget::onEvent(TouchEvent* event) {
  switch (event->event) {
    case TouchEvents::TOUCH:
      onTouch(event);
      break;

    case TouchEvents::UNTOUCH:
      onUntouch(event);
      break;

    case TouchEvents::DRAW:
      onDraw(event);
      break;

    case TouchEvents::TTY_INSCOPE:
      onTtyInScope(event);
      break;

    case TouchEvents::TTY_OUTOFSCOPE:
      onTtyOutOfScope(event);
      break;

    case TouchEvents::INACTIVITY_TIMEOUT:
      onActivityTimeout(event);
      break;

    case TouchEvents::WAKEUP:
      onWakeUp(event);
      break;

    default:
      onUnsollicitedEvent(event);
  }

}

/*----------------------------------------------------------------------
 *
 *  Processes a TouchEvent type TOUCH
 *
 *  event      The touch event to process
 *
 *--------------------------------------------------------------------*/
void Widget::onTouch(TouchEvent* event) {
}

/*----------------------------------------------------------------------
 *
 *  Processes a TouchEvent type TOUCH
 *
 *  event      The untouch event to process
 *
 *--------------------------------------------------------------------*/
void Widget::onUntouch(TouchEvent* event) {
}

/*----------------------------------------------------------------------
 *
 *  Processes a TouchEvent type DRAW
 *
 *  event      The draw touch event to process
 *
 *--------------------------------------------------------------------*/
void Widget::onDraw(TouchEvent* event) {
}

/*----------------------------------------------------------------------
 *
 *  Processes a TouchEvent type TTY_INSCOPE
 *
 *  event      The draw touch event to process
 *
 *--------------------------------------------------------------------*/
void Widget::onTtyInScope(TouchEvent* event) {
}

/*----------------------------------------------------------------------
 *
 *  Processes a TouchEvent type TTY_OUTOFSCOPE
 *
 *  event      The draw touch event to process
 *
 *--------------------------------------------------------------------*/
void Widget::onTtyOutOfScope(TouchEvent* event) {
}

/*----------------------------------------------------------------------
 *
 *  Processes an unsollicited event. Thisis basically an event with
 *  an event type which is unknown.
 *
 *  event      The unknown event type to process
 *
 *--------------------------------------------------------------------*/
void Widget::onUnsollicitedEvent(TouchEvent* event) {
}

/*----------------------------------------------------------------------
 *
 *  After a specific interval of inactivity (i.e. no touches) this event
 *  is generated by the TouchHandler. The touchandel has an attribute
 *  inactivityTime which can be set to specify the inactivity interval.
 *
 *--------------------------------------------------------------------*/
void Widget::onActivityTimeout(TouchEvent* event) {
}

/*----------------------------------------------------------------------
 *
 *  After an ActivityTimeout event a WakeUp event will follow.
 *  As soon as a user starts to interact again by touching the
 *  TFT screen. Typically the
 *
 *--------------------------------------------------------------------*/
void Widget::onWakeUp(TouchEvent* event) {
}

/*----------------------------------------------------------------------
 *
 *  If set to true the widget becomes visible.
 *  I.e. calling draw() will result in a visible widget.
 *  If set to false calling draw() will have no effect.
 *
 *  visible      If true the widget becomes visible
 *               If false the widget becomes invisible
 *
 *--------------------------------------------------------------------*/
void Widget::setVisible(bool pVisible) {

  //
  // If going invisible
  //
  if (!visible && pVisible) {
    visible = true;
	redraw();
	return;
  }

  //
  //  If going visible
  //
  if (visible && !pVisible) {
	visible = false;
	clear();
	return;
  }

  //
  //  Otherwise we have true -> true or false -> false
  //  Which can be ignored...
  //
}

/*-----------------------------------------------------------------------
 *
 *  Returns true if the widget should be visible
 *
 *---------------------------------------------------------------------*/
bool Widget::isVisible() {
  return visible;
}

/*-----------------------------------------------------------------------
 *
 *  Returns the object its class name
 *
 *---------------------------------------------------------------------*/
const char* Widget::isType() {
  return "Widget";
}

