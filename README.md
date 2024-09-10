

       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                  A R D U I N O   G U I   W I D G E T S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <README.md> - Library forGUI Widgets.
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

Releases
========

1.0.1-beta.1
The first release with some basis support for TFT screen with touch and a number of GUI widgets.
The TFT support is based upon the MCUFRIEND_kbv library. This library depends on it and will not work without it.

Installation
============
Locate the Arduino/library folder on your system. Create the folder TerraBox_Widgets in the library folder and copy the contents of this release into it.
Do not forget to download the dependent packages: MCUFRIEND_kbv, Adafruit_BusIO, Adafruit_GFX_Library, Adafruit_TFTLCD_Library, Adafruit_TouchScreen, TerraBox_Persistence and TerraBox_Scheduler.

Abstract
========
Wanting to make an Arduino application which uses a TFT screen with touch was the trigger to create this library of basic GUI widgets.

First of all it supports setting up TFT screens with resistive touch panels. The trick with these TFT's is that they need calibration. Which is fully integrated in the screen startup logic. And it offers some diagnostic functions for exploring persistent memory usage and calibration tests as well.

The GUI part supply a handful of basic widgets and supports a way of working commonly seen in main stream Windowing systems using event handling.

It also contains an inactivity timer which triggers sleep and wakeup function of the screen.


Screen initialisation
=====================
The screen setup is made every easy. The code below shows an example of how to start up using a TFT screen. It only needs an include file and a single line of code in the Arduino setup function.

``` C++

  #include <TerraBox_Widgets.h>
  :
  :

  void setup() {

    Screen.beginFull();  // Fully initialise the screen including calibration 

  }

```

Screen and Touch
================
By including the header file <TerraBox_widgets.h> two objects are automatically created that will manage the GUI you create. Touch will manage the touch panel interaction. This results in touch coordinates identifying the screen spot touched. The Touch object is said to digest the detected touches. Once the touch coordinates are swallowed by Touch, it will create and send a TouchEvent about it to the Screen object. And at that point Touch is said to have digested the Touch. It is now up to the Screen to dispatch this event to the proper widget for further processing. Before that is possible, the touched widget needs to be identified by matching the coordinates to the targeted Widget. As the screen knows where widgets are on the screen and the touch coordinates correlate to the screen coordinates in a known way (thanks to the screen calibration), Screen can now match the touch coordinates with the widget touched. Finally Screen can now pass the TouchEvent to the proper widget. The touched widget is called the source widget of the touch. After receiving the event in one of its on*() functions, the designated source widget will further process the TouchEvent. 

We now can complete the earlier example with the digesting of touches. Since there is no way to have a touch trigger an interrupt we are left with the only other option. Which is polling. So we use Arduinos loop() function for that.

``` C++

  #include <TerraBox_Widgets.h>
  :
  :

  void setup() {

    Screen.beginFull();  // Fully initialise the screen including calibration 

  }

  void loop() {

    Touch.digest();  // Swallow the touches and digest them

  }

```

Last but not least offers Screen a number of primitives to draw basic shapes, like lines, rectangles  and print text and numerical information.

Using widgets
=============
Now as a GUI creator you will only have to do two things to make the above magic happen:

1. Create a widget tree
2. Write code reacting on TouchEvents

The widget tree is exactly what is missing in the last version of our example.
So let us create a widget tree. And then one other thing has to be mentioned about the Screen object.
Which is that the most basic Widget is the Screen object. So because of this the Screen itself can receive and process TouchEvents as well. Which comes in handy in implementing a sleep and wake up function in order to darken the screen after some inactivity period. And another use for this is that it provides a hook for implementing a context menu. Which is currently not yet implemented. 
A third use of Screen is that it is the root of our widget tree. In short any Widget that is placed on the screen has the Screen as its parent. No notice that we also can have widgets within widgets. For instance if we have a form with a button. In that case the form is a child of the Screen and the button is child of the form. But the button can also directly be placed on the Screen. Which is what constitutes the Hello World example of the TerraBox Widget library.


``` C++

  #include <TerraBox_Widgets.h>
  #include <ButtonWidget.h>

  class HelloButton : public ButtonWidget {
    public:

    HelloButton(Widget* parent) :
             ButtonWidget(parent, 
                 120, 20,             // x, y coordinates 
                 80,  20,             // button width and height
                 "Hello world") {     // Text on the button

        Screen.setCursor(0, 40);      // Place text cursor under the button

    }

    //
    //  Process the touch event...
    //  which was generated by Touch 
    //  and dispatched by Screen.
    //
    void action(TouchEvent* event) {

      Screen.setCursor(120, 60);                // Position text cursor below button

      Screen.println("Hello World");            // Print hello world

      delay(5000);                              // Wait..
      Screen.fillRect(120, 60, 80, 20, BLACK);  // Clear the text

    }
  
  };

  //
  //  Create the Hello button widget
  //
  HelloButton hello(&Screen);

  void setup() {

    Screen.beginFull();  // Fully initialise the screen including calibration
    Screen.draw();       // Draw the widget tree 

  }

  void loop() {

    Touch.digest();      // Swallow the touches and digest them

  }


```
What was added to the sketch? Only three things:
- The HelloButton class
- The instantiation of a HelloButton object
- A line to draw the entire widget tree

The extra class is needed to create a button with code to process the TouchEvent, performed by the action(TouchEvent* event) function of the HelloButton class. The only thing it does is print the ever famous "Hello World" message. It looks a bit more complex however, because the printing is somewhat enhanced by repositioning the text cursor to a position where it doesn't damage the buttons appearance. And there is some code erasing the text after 5 seconds. In this way clicking the button prints the Hello world message every time.

Then the HelloButton needs to be instantiated, which is performed by this code line:

``` C++
  HelloButton hello(&Screen);
```

Note that the parent of the button is the Screen object. So the entire widget tree exists of
a root, being "Screen", and its child the HelloButton instance named hello.

Then the last extra line of code in the setup function.

``` C++
  Screen.draw();
```

This line of code is needed to actually draw the widget tree. The fact that a widget tree has been created does not imply that it appears on the screen. The programmer decides when it is time for the widget tree to make its appearance by explicitly drawing it.


By the way, did you notice the effect the delay(5000) has?
Because of the polling for touches no touches are digested or events are sent during the 5 seconds delay. This is also the reason why the button stays in a pressed state during this period.
The action() function is invoked right after the button was touched touch on the screen, a TOUCH type event was dispatched. Right after the action() function has finished an UNTOUCH event will be generated and dispatched. Where the TOUCH even triggers a button to appear as pressed, is it the UNTOUCH event type that triggers the button to return to its unpressed appearance. The action() function is executed in between. As a result the UNTOUCH type event will not be processed until the action() function has returned. 

And did you also notice, that if you waited a few minutes, the screen is being cleared.
Try touching the screen after that has happened.
You will see your button reappear. And it will still react if you click on it.

Other widgets
=============
Next the other widgets will be briefly described.

The RectangleWidget visualizes as a rectangle. It can have a stroke around it in a different colour. It is a base class for the LabelWidget, which adds the capability of presenting a text within the rectangle. The ButtonWidget is on its turn based on the LabelWidget adding the capability to mimic two visual states. Pressed and unpressed. The three widgets support all the possible on*() event handling functions. They distinguish themselves on visualisation aspects and the introduction of showing a text.

The BarWidget has the RectangleWidget as its base class too. It offers support for dynamically visualising a level represented by an integer number in the range 0 - 100. The levels low, lowlow, high and highhigh are visualised using different colours and can be specified using the Levels object.

The BarWidget can be used together with the ValueConverter, which can map any numerical range onto a 0-100 value range.

A more dressed up version of a level indicator is available in the TerraBox_LevelIndicator library. This widget offers an additional title capability and a numerical representation in terms of 0%-100%.
 