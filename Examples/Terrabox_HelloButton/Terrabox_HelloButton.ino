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
