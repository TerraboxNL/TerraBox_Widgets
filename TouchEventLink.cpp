/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <TouchEventLink.cpp> - Library forGUI Widgets.
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

/*--------------------------------------------------------------------------------------------------
 *
 *  Create a double linked event list with these chain elements.
 *
 *  pPrev      Points the previous element in the list.
 *  pEvent     Points to the event
 *  pNext      Points to the next element in the list.
 *
 *------------------------------------------------------------------------------------------------*/
TouchEventLink::TouchEventLink(TouchEventLink* pPrev, TouchEvent* pEvent, TouchEventLink* pNext) {
  prev  = pPrev;
  event = pEvent;
  next  = pNext;
}

/*------------------------------------------------------------------------------------------------
 *
 *  Append the new event after this event and return the new event link.
 *  With which the first or last anchor can be updated.
 *
 *  newEvent      The event to add.
 *
 *----------------------------------------------------------------------------------------------*/
TouchEventLink* TouchEventLink::append(TouchEvent* newEvent) {

  //
  //                       B E F O R E   A P P E N D
  // .-------------.
  // | S C R E E N |
  // +-------------+
  // | last:  2    |--------------------------------.                 .-------------------.
  // +-------------+   .-----------.   .-----------. |  .-- this ---. |   .--- new1 ---.  |
  // | first: 0    |-->| addr:  0  |   | addr:  1  | `->| addr:  2 >--' .--< addr:  3  |  |
  // +-------------+   +-----------+   +-----------+    +-----------+   | +------------+  |
  // |             |   |           |   |           |    |      <--------' |            |  |
  // |             |   | next:  1  |-->| next:  2  |--->| next: Nil >---------->       |  |
  // :             :   +-----------+   +-----------+    +-----------+     +------------+  |
  // :             :   | prev:  Nil|<--| prev:  0  |<---| prev:  1  |     | this       <--'
  // :             :   +-----------+   +-----------+    +-----------+     +------------+
  // |             |   | event: 1  |   | event: 2  |    | event: 3  |     | newEvent   |
  // |             |   `-----------'   `-----------'    `-----------'     `------------'
  // |             |         |               |               |
  // |             |         V               V               V
  // |             |   .-----------.   .-----------.   .-----------.
  // `-------------'   | event: 1  |   | event: 2  |   | event: 3  |
  //                   |           |   |           |   |           |
  //
  //
  //
  //                          A F T E R  A P P E N D
  // .-------------.
  // | S C R E E N |
  // +-------------+
  // | last:  3    |-------------------------------------------------.
  // +-------------+   .-----------.   .-----------.   .-- this ---. |  .--- NEW ---.
  // | first: 0    |-->| addr:  0  |   | addr:  1  |   | addr:  2  | '->| addr:  3  |
  // +-------------+   +-----------+   +-----------+   +-----------+    +-----------+
  // |             |   | next:  1  |-->| next:  2  |-->| next:  3  |--->| next: nil |
  // :             :   +-----------+   +-----------+   +-----------+    +-----------+
  // :             :   | prev:  Nil|<--| prev:  0  |<--| prev:  1  |<---| prev:  2  |
  // :             :   +-----------+   +-----------+   +-----------+    +-----------+
  // |             |   | event: 0  |   | event: 1  |   | event: 2  |    | event: 3  |
  // |             |   `-----------'   `-----------'   `-----------'    `-----------'
  // |             |         |               |              |                 |
  // |             |         V               V              V                 V
  // |             |   .-----------.   .-----------.   .-----------.    .-----------.
  // `-------------'   | event: 0  |   | event: 1  |   | event: 2  |    | event: 3  | 
  //                   :           :   :           :   :           :    :           :
  //
  //

  TouchEventLink* new1 = new TouchEventLink(this, newEvent, this->next);
  this->next = new1;  
  return new1;
}

/*------------------------------------------------------------------------------------------------
 *
 *  Append the new event after this event and return the new event link.
 *  With which the first or last anchor can be updated.
 *
 *  newEvent      The event to add.
 *
 *----------------------------------------------------------------------------------------------*/
TouchEventLink* TouchEventLink::insert(TouchEvent* newEvent) {

  //
  //                       B E F O R E   I N S E R T
  // .-------------.
  // | S C R E E N |
  // +-------------+
  // | last:  2    |--------------------------------.                 .-------------------.
  // +-------------+   .-----------.   .-----------. |  .-- this ---. |   .--- new1 ---.  |
  // | first: 0    |-->| addr:  0  |   | addr:  1  | `->| addr:  2 >--' .--< addr:  3  |  |
  // +-------------+   +-----------+   +-----------+    +-----------+   | +------------+  |
  // |             |   |           |   |           |    |      <--------' |            |  |
  // |             |   | next:  1  |-->| next:  2  |--->| next: Nil >---------->       |  |
  // :             :   +-----------+   +-----------+    +-----------+     +------------+  |
  // :             :   | prev:  Nil|<--| prev:  0  |<---| prev:  1  |     | this       <--'
  // :             :   +-----------+   +-----------+    +-----------+     +------------+
  // |             |   | event: 1  |   | event: 2  |    | event: 3  |     | newEvent   |
  // |             |   `-----------'   `-----------'    `-----------'     `------------'
  // |             |         |               |               |
  // |             |         V               V               V
  // |             |   .-----------.   .-----------.   .-----------.
  // `-------------'   | event: 1  |   | event: 2  |   | event: 3  |
  //                   |           |   |           |   |           |
  //
  //
  //
  //                          A F T E R  I N S E R T
  // .-------------.
  // | S C R E E N |
  // +-------------+
  // | last:  3    |-------------------------------------------------.
  // +-------------+   .-----------.   .-----------.   .-- new1 ---. |  .-- this ---.
  // | first: 0    |-->| addr:  0  |   | addr:  1  |   | addr:  3  | '->| addr:  2  |
  // +-------------+   +-----------+   +-----------+   +-----------+    +-----------+
  // |             |   | next:  1  |-->| next:  2  |-->| next:  2  |--->| next: nil |
  // :             :   +-----------+   +-----------+   +-----------+    +-----------+
  // :             :   | prev:  Nil|<--| prev:  0  |<--| prev:  1  |<---| prev:  3  |
  // :             :   +-----------+   +-----------+   +-----------+    +-----------+
  // |             |   | event: 0  |   | event: 1  |   | event: 3  |    | event: 2  |
  // |             |   `-----------'   `-----------'   `-----------'    `-----------'
  // |             |         |               |              |                 |
  // |             |         V               V              V                 V
  // |             |   .-----------.   .-----------.   .-----------.    .-----------.
  // `-------------'   | event: 0  |   | event: 1  |   | event: 3  |    | event: 2  | 
  //                   :           :   :           :   :           :    :           :
  //
  //
  
  TouchEventLink* new1 = new TouchEventLink( prev=this->prev, event=newEvent, next=this);
  this->prev = new1;
  return new1;
}

/*---------------------------------------------------------------------------------------------------
 *
 *  Remove this element from the linked list. The element itself is not modified.
 *  On return one might need to destroy the TouchEventLink if the links have been created using 
 *  the new TouchEventLink( ... )
 *
 *-------------------------------------------------------------------------------------------------*/
TouchEventLink* TouchEventLink::remove() {
  prev->next = next;
  next->prev = prev;

  return this;
}

/*---------------------------------------------------------------------------------------------------
 *
 *  Returns the next pointer.
 *
 *-------------------------------------------------------------------------------------------------*/
TouchEventLink* TouchEventLink::getNext() {
  return next;
}

/*---------------------------------------------------------------------------------------------------
 *
 *  Returns the event pointer.
 *
 *-------------------------------------------------------------------------------------------------*/
TouchEvent* TouchEventLink::getEvent() {
  return event;
}

/*---------------------------------------------------------------------------------------------------
 *
 *  Returns the previous pointer.
 *
 *-------------------------------------------------------------------------------------------------*/
TouchEventLink* TouchEventLink::getPrev() {
  return prev;
}

/*---------------------------------------------------------------------------------------------------
 *
 *  Assign the next pointer
 *
 *-------------------------------------------------------------------------------------------------*/
void TouchEventLink::setNext(TouchEventLink* n) {
  next = n;
}

/*---------------------------------------------------------------------------------------------------
 *
 *  Assign the previous pointer
 *
 *-------------------------------------------------------------------------------------------------*/
void TouchEventLink::setPrev(TouchEventLink* p) {
  prev = p;
}

/*---------------------------------------------------------------------------------------------------
 *
 *  Assign the event pointer
 *
 *-------------------------------------------------------------------------------------------------*/
void TouchEventLink::setEvent(TouchEvent* e) {
  event = e;
}
