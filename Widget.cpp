/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

                     <Widget.h> - Library forGUI Widgets.
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
#include <persistence.h>
#include <Dump.h>

#define DEBUG_CONTAINS	 0
#define DEBUG_MATCH	     0
#define DEBUG_BUILD_TREE 0
#define DEBUG_ON_EVENT   0

/*==============================================================================
 *
 *  Widget is a base class for visualizations in GUI's
 *  It holds the basic properties for coordinates and size of a widget.
 *  It also specifies the basic visualisation method draw().
 *
 *============================================================================*/
uint32_t Widget::idCount = 0;

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
	  // Generate unique widget id.
	  //
	  id = idCount++;

      //
      //  Initialize some pointers
      //
      child   = nullptr;
      sibling = nullptr;

      //
      // Add this widget to its parent, if it has one...
      //
      setParent(pParent);

      logMsg = "No messages at Widget creation time";

      visible = true;
}

/*-----------------------------------------------------------------------------
 *
 *  Add the widget as a child.
 *  Note that the child attribute holds a list of siblings, which are in fact
 *  all the children of the this widget. As this widget is their parent.
 *
 *  w    The child to add to this parent widget.
 *
 *---------------------------------------------------------------------------*/
void Widget::add(Widget* w) {
#if DEBUG_BUILD_TREE
	if (Serial) {Serial.print(F("Adding child 0x")); Serial.print((uint32_t)w); Serial.print(F(" to parent 0x")); Serial.println((uint32_t)this); }
#endif
   //
   // If there is already a child, make it a sibling of the new child
   //
   if (child) {
     w->sibling = child;  // Insert the new child as the head of the sibling chain.
   }

   //
   //  Make the last added child the first one in the sibling chain
   //
   child = w;      // Assign the head of the siblings to the paren child.
}

void Widget::tree() {
	Serial.println();
	Serial.println(F("Tree:"));

	dumpSerial.dumpRam((uint32_t)this, widgetSize);

	tree(0);

}

void Widget::tree(int level) {

	Serial.print(level); Serial.print(F(" SP=0x")); Serial.println(SP);

	//
	//  Print the indentation
	//
	int space = 4;
	int max = level * space;
	int spaces = (level-1) * space;
	for (int i = 0; i < max; i++) {
	  if (i % space == 0) {
		  if (sibling == nullptr && i + 4 >= max)
		    Serial.print(F("`"));
		  else
			Serial.print(F("|"));
	  }
	  else {
		if (i < spaces) {
		  Serial.print(F(" "));
		}
		else {
          Serial.print(F("-"));
		}
	  }
	}

	//
	//  Print the widget information
	//
	Serial.print(F("+-> ")); Serial.print(F("Visible: ")); Serial.print((isVisible() ? "Yes" : "No"));
	Serial.print(F(" @ 0x")); Serial.print((uint32_t)this, HEX);
	Serial.print(F(" id: ")); Serial.print(id);
	Serial.print(F(" xy: ("));Serial.print(x); Serial.print(F(",")); Serial.print(y); Serial.print(F(") X ("));
	Serial.print(x+width); Serial.print(F(",")); Serial.print(y+height);Serial.println(F(")"));
//    delay(1000);

	//
	//  Dump the children
	//
	for (Widget* w = child; w; w = w->sibling) {
		dumpSerial.dumpRam((uint32_t)w, widgetSize );
	}

	//
	//  Descend to the children
	//
	for (Widget* w = child; w; w = w->sibling) {
		w->tree(level+1);
	}
}

/**----------------------------------------------------------------------------
 *
 *  Prints out the path to the root
 *
 *---------------------------------------------------------------------------*/
void Widget::path() {
	Serial.println();
	Serial.println(F("Path:"));
	Serial.println(F("V"));
   	path(0);
}

/**----------------------------------------------------------------------------
 *
 *  Prints out the path to the root
 *
 *  @param level      The level at which a widget is
 *
 *---------------------------------------------------------------------------*/
void Widget::path(int level) {

	//
	//  Print the indentation
	//
	int space = 4;
	int max = level * space;
	for (int i = 0; i < max; i++) {
	  if (i % space == 0) {
		  Serial.print(F("|"));
	  }
	  else {
        Serial.print(F("-"));
	  }
	}

	//
	//  Print the widget information
	//
	Serial.print(F("-> ")); Serial.print(F("Visible: ")); Serial.print((isVisible() ? "Yes" : "No"));
	Serial.print(F(" @ 0x")); Serial.print((uint32_t)this);
	Serial.print(F(" ("));Serial.print(x); Serial.print(F(",")); Serial.print(y); Serial.print(F(") X ("));
	Serial.print(x+width); Serial.print(F(",")); Serial.print(y+height);Serial.println(F(")"));


	//
	//  Ascend to the parent
	//
	if (parent) {
	  parent->path(level+1);
	}
}

/**-----------------------------------------------------------------------------
 *
 *  Remove this widget being a child from the parent by removing it from the
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
      // Yes, if w (the widget to be removed) is the head of the siblings list
      // then make w its successor the sibling in the list.
      //
      if (w == child) {
         child = w->sibling;          // Make the next sibling the head of the list
      }
      //
      //  Otherwise w is just a sibling, so let its prior sibling point
      //  to its successor. Which removes w as a sibling.
      //
      else {
        prev->sibling = w->sibling;   // Remove the sibling from the list.
      }

      //
      //  Mission accomplished
      //
      return;
    }

    //
    // Remember the current sibling the previous sibling.
    // Needed to remove w from the list. See the else branch above.
    //
    prev = c;
  }
}

/**-----------------------------------------------------------------------------
 *
 *  Remove this widget from the sibling list of its parent.
 *
 *----------------------------------------------------------------------------*/
void Widget::remove() {
	//
	//  If there is no parent, then we cannot remove this widget from anything
	//
	if (! parent)
		return;

	//
	//  Remove it from the parent its sibling list
	//
	parent->remove(this);
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
#if DEBUG_BUILD_TREE
	if (Serial) { Serial.print(F("Assign parent 0x"));Serial.print((uint32_t) pParent); Serial.print(F(" to child 0x")); Serial.println((uint32_t)this); }
#endif

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
      Serial.print(F(", pY:"));
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
      Serial.print(F(" ==> "));
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
	Serial.print(F("Widget address: 0x")); Serial.println((uint32_t)this);
    Serial.print(F("Widget::match() X,Y = : ")); Serial.print(pX); Serial.print(F(",")); Serial.print(pY);
                   Serial.print(F(" in ")); Serial.print(x); Serial.print(F(",")); Serial.print(y);
                   Serial.print(F(" X ")); Serial.print(x+width); Serial.print(F(",")); Serial.print(y + height);
    Serial.println();
    delay(1000);
  #endif

  //
  //  Does this widget contain the (x,y) coordinates?
  //
  if (contains(pX, pY)) {
    #if DEBUG_MATCH
      Serial.println(F("Widget::match:  ---> YES!!!!"));
   #endif
    //
    //  Then try its child and siblings, which are in the z-order.
    //
    int count = 1;
    for (Widget* w = child;        // Start with the first child 
                 w;                // as long as we have a sibling
                 w = w->sibling) { // fetch the next sibling

      if (w->isVisible()) {

       #if DEBUG_MATCH
    	 Serial.print(F("Try matching child: "));Serial.println(count);
       #endif

       Widget* deepestMatchingWidget = w->match(pX, pY);
       if (deepestMatchingWidget)
         return deepestMatchingWidget;
         
      }
      else {
        #if DEBUG_MATCH
	    Serial.print(F("Skipping invisible child: "));Serial.println(count);
        #endif
      }

      count++;
    }

    //
    // No deeper matching child or child its sibling, so this is the deepest matching one.
    //
    #if DEBUG_MATCH
    path();
    #endif
    return this;
  }

  //
  // No match
  //
  #if DEBUG_MATCH
    Serial.println(F("Widget::match:  ---> NO!!!!"));
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

    case TouchEvents::GOTO_SLEEP:
      onGotoSleep(event);
      break;

    case TouchEvents::WAKEUP:
      onWakeUp(event);
      break;

    case TouchEvents::IN_SCOPE:
      onInScope(event);
      break;

    case TouchEvents::OUT_OF_SCOPE:
      onOutOfScope(event);
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
#if DEBUG_ON_EVENT
	Serial.print(F("Override onTouch "));Serial.println(id);
#endif
}

/*----------------------------------------------------------------------
 *
 *  Processes a TouchEvent type TOUCH
 *
 *  event      The untouch event to process
 *
 *--------------------------------------------------------------------*/
void Widget::onUntouch(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.print(F("Override onUntouch "));Serial.println(id);
#endif
}

/*----------------------------------------------------------------------
 *
 *  Processes a TouchEvent type DRAW
 *
 *  event      The draw touch event to process
 *
 *--------------------------------------------------------------------*/
void Widget::onDraw(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.print(F("Override onDraw "));Serial.println(id);
#endif
}

/*----------------------------------------------------------------------
 *
 *  Processes a TouchEvent type TTY_INSCOPE
 *
 *  event      The draw touch event to process
 *
 *--------------------------------------------------------------------*/
void Widget::onTtyInScope(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.println(F("Override onTtyInScope"));
#endif
}

/*----------------------------------------------------------------------
 *
 *  Processes a TouchEvent type TTY_OUTOFSCOPE
 *
 *  event      The draw touch event to process
 *
 *--------------------------------------------------------------------*/
void Widget::onTtyOutOfScope(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.println(F("Override onTtyOutOfScope"));
#endif
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
#if DEBUG_ON_EVENT
	Serial.print(F("onUnsollicitedEvent!!!!! "));Serial.println(id);
#endif
}

/*----------------------------------------------------------------------
 *
 *  After a specific interval of inactivity (i.e. no touches) this event
 *  is generated by the TouchHandler. The touchandel has an attribute
 *  inactivityTime which can be set to specify the inactivity interval.
 *
 *--------------------------------------------------------------------*/
void Widget::onGotoSleep(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.println(F("Override onGotoSleep"));
#endif
}

/*----------------------------------------------------------------------
 *
 *  After an ActivityTimeout event a WakeUp event will follow.
 *  As soon as a user starts to interact again by touching the
 *  TFT screen. Typically the
 *
 *--------------------------------------------------------------------*/
void Widget::onWakeUp(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.println(F("Override onWakeUp"));
#endif
}

/*----------------------------------------------------------------------
 *
 *  After an ActivityTimeout event a WakeUp event will follow.
 *  As soon as a user starts to interact again by touching the
 *  TFT screen. Typically the
 *
 *--------------------------------------------------------------------*/
void Widget::onInScope(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.print(F("Override onOnInscope ")); Serial.println(id);
#endif
}

/*----------------------------------------------------------------------
 *
 *  After an ActivityTimeout event a WakeUp event will follow.
 *  As soon as a user starts to interact again by touching the
 *  TFT screen. Typically the
 *
 *--------------------------------------------------------------------*/
void Widget::onOutOfScope(TouchEvent* event) {
#if DEBUG_ON_EVENT
	Serial.print(F("Override onOutOfScope ")); Serial.println(id);
#endif
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
  // If going visible
  //
  if (!visible && pVisible) {
    visible = true;
    /*
    for (Widget* w = child; w; w = w->sibling)
    	w->setVisible(true);
    */
	draw();
	return;
  }

  //
  //  If going invisible
  //
  if (visible && !pVisible) {
	visible = false;
	/*
    for (Widget* w = child; w; w = w->sibling)
    	w->setVisible(false);
    */
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

