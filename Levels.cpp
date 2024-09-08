/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <Levels.h> - Library forGUI Widgets.
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
#include <Arduino.h>
#include "Terrabox_Widgets.h"

/*==============================================================================
 *
 *  Levels is used to specify threshold levels for monitoring purposes
 *
 *============================================================================*/

Levels::Levels() {
      levels[0] = max;
      levels[1] = highhigh;
      levels[2] = high;
      levels[3] = low;
      levels[4] = lowlow;
      levels[5] = min;
    }

Levels::Levels(uint16_t mn, uint16_t ll, uint16_t l, uint16_t h, uint16_t hh, uint16_t mx) {
      levels[0] = max       = mx;
      levels[1] = highhigh  = hh;
      levels[2] = high      = h;
      levels[3] = low       = l;
      levels[4] = lowlow    = ll;
      levels[5] = min       = mn;
    }

