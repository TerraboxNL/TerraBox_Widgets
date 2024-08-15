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

Splash::Splash()
       : Widget(nullptr,
                 0,     0, 
                 Screen.tft->width(), Screen.tft->height())
{

}

/*-------------------------------------------------------------------------------------------------
 *
 *  Splash screen
 *
 *-----------------------------------------------------------------------------------------------*/
void Splash::draw() 
{
  Screen.tft->setTextSize(1);

  uint8_t rot = Screen.tft->getRotation();

  if (rot == 1 || rot == 3) {
    Screen.tft->fillRect(x, y, width, height, BLACK);
    Screen.tft->setCursor(0, 25);
    Screen.tft->println(F("           /////// ////// //////  //////   /////     /////    ////  //   //"));
    Screen.tft->println(F("             //   //     //   // //   // //   //    //  //  //   // // //"));
    Screen.tft->println(F("            //   ////   //////  //////  ///////    /////   //   //   //"));
    Screen.tft->println(F("           //   //     // //   // //   //   //    //   // //   //  // //"));
    Screen.tft->println(F("          //   ////// //   // //   // //   //    //////    ////  //   //"));
    Screen.tft->println();
    Screen.tft->println();
    Screen.tft->println(F("       This program is free software: you can redistribute it and/or modify"));
	Screen.tft->println(F("       it under the terms of the GNU General Public License as published by"));
	Screen.tft->println(F("       the Free Software Foundation, either version 3 of the License, or"));
	Screen.tft->println(F("       (at your option) any later version."));

	Screen.tft->println(F("       This program is distributed in the hope that it will be useful,"));
	Screen.tft->println(F("       but WITHOUT ANY WARRANTY; without even the implied warranty of"));
    Screen.tft->println(F("       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"));
	Screen.tft->println(F("       GNU General Public License for more details."));

	Screen.tft->println(F("       You should have received a copy of the GNU General Public License"));
	Screen.tft->println(F("       along with this program.  If not, see <https://www.gnu.org/licenses/>."));
    Screen.tft->println();
    Screen.tft->println();

  }
  else {
    Screen.tft->fillRect(x, y, height, width, BLACK);
    Screen.tft->setCursor(0, 50);
    Screen.tft->println(F("        /////// ////// //////  //////   ///// "));
    Screen.tft->println(F("          //   //     //   // //   // //   // "));
    Screen.tft->println(F("         //   ////   //////  //////  ///////  "));
    Screen.tft->println(F("        //   //     // //   // //   //   //   "));
    Screen.tft->println(F("       //   ////// //   // //   // //   //    "));
    Screen.tft->println();
    Screen.tft->println(F("              /////    ////  //   //"));
    Screen.tft->println(F("             //  //  //   // // //"));
    Screen.tft->println(F("            /////   //   //   //"));
    Screen.tft->println(F("           //   // //   //  // //"));
    Screen.tft->println(F("          //////    ////  //   //"));
    Screen.tft->println();
    Screen.tft->println();
    Screen.tft->println();
    Screen.tft->println();
    Screen.tft->println();
    Screen.tft->println();
    Screen.tft->println(F("This program is free software: you can redistribute"));
    Screen.tft->println(F("it and/or modify it under the terms of the "));
    Screen.tft->println(F("GNU General Public License as published by the"));
    Screen.tft->println(F(" Free Software Foundation, either version 3 of "));
    Screen.tft->println(F("the License, or (at your option) any later version."));
    Screen.tft->println();
    Screen.tft->println(F("This program is distributed in the hope that it will"));
    Screen.tft->println(F("be useful, but WITHOUT ANY WARRANTY; without even the"));
    Screen.tft->println(F("implied warranty of MERCHANTABILITY or FITNESS FOR A"));
    Screen.tft->println(F("PARTICULAR PURPOSE. See the GNU General Public License"));
    Screen.tft->println(F("for more details."));
    Screen.tft->println();
    Screen.tft->println(F("You should have received a copy of the GNU General"));
    Screen.tft->println(F("Public License along with this program."));
    Screen.tft->println(F("If not, see<https://www.gnu.org/licenses/>."));
    Screen.tft->println();

  }

}

void Splash::redraw() {
  Screen.tft->fillRect(x, y, width, height, BLACK);
  Screen.tft->setCursor(x, y);

  draw();
}

void Splash::drawInverted() {
  draw();
}

/*-------------------------------------------------------------------------------------------------
 *
 *  Returns the class name
 *
 *-----------------------------------------------------------------------------------------------*/
const char* Splash::isType() {
  return "Splash";
}
