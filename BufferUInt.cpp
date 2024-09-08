/*-------------------------------------------------------------------------------------------------



       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                   A R D U I N O   G U I   W I D G E T S


                     (C) 2024, cor.hofman@terrabox.nl

                <BufferUInt.cpp> - Library for GUI widgets.
                             June 30, 2024
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

 *------------------------------------------------------------------------------------------------*
 *
 *  C H A N G E  L O G :
 *  ============================================================================================
 *  P0001 - Initial release
 *  ============================================================================================
 *
 *------------------------------------------------------------------------------------------------*/
#include <Terrabox_Widgets.h>

#define DEBUG		0

/*--------------------------------------------------------------------------
 * Create an unsigned integer buffer.
 *
 * x         X coordinate
 * y         Y coordinate
 * pSetup    The keyboard layout
 * fontSize  The font size of the key captions
 *------------------------------------------------------------------------*/
BufferUInt::BufferUInt() {
  nextOne  = 0;
  nextFree = 0;
  capacity = ARRAY_SIZE(buffer);
  overflow = 0;
}

/*-----------------------------------------------------------------------------------
 *
 *  Buffer a character. It returns the buffer capacity left.
 *  A negative return indicates the number of consequtive overflows.
 *  A greater or equal than zero return indicates the available capacity that remains.
 *
 *  c      The character to buffer
 *
 *---------------------------------------------------------------------------------*/
int16_t BufferUInt::put(uint16_t c) {

  //
  // If no capacity then return number of overflows so far
  //
  if (capacity < 0) {
    return -(++overflow);
  }

  //
  // Reset overflow
  //
  overflow = 0;

  //
  // Wrap around the next free if needed
  //
  if (nextFree >= ARRAY_SIZE(buffer)) {
    nextFree = 0;
  }

  //
  // Store the character and decrease the capacity
  //
  buffer[nextFree++] = c;
  capacity--;

  //
  // Return the available capacity
  //
  return capacity;

}

/*-----------------------------------------------------------------------------------
 *
 *  Gets a character from the buffer
 *
 *  c      The character to buffer
 *
 *---------------------------------------------------------------------------------*/
uint16_t BufferUInt::get() {

  if (nextOne > ARRAY_SIZE(buffer)) {
    nextOne = 0;
  }

  //
  //  Check if there are actually characters available
  //
  if (nextOne == nextFree) {
    return 0;
  }

  //
  //  Get the next character from the buffer,
  //  Increase the buffer capacity by 1
  //  Return the character.
  // 
  uint16_t c = buffer[nextOne++]; 
  capacity++;
  return c;
}
