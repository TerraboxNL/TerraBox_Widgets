/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <ValueConverter.h> - Library forGUI Widgets.
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

#ifndef VALUE_CONVERTER_H_
#define VALUE_CONVERTER_H_

/*================================================================================================
 *
 *  Class with which converts field data values into numbers GUI widgets can crunch and represent
 *
 *==============================================================================================*/
class ValueConverter {
  private:
    int16_t int16MinRaw;           // The raw minimum
    int16_t int16MaxRaw;           // The raw maximum
    int16_t int16RangeRaw;         // The range of the raw value (e.g. max - min)

    int16_t int16MinClipRaw;       // The raw minimum
    int16_t int16MaxClipRaw;       // The raw maximum

    float    conversionFactor;     // The conversionFactor used for raw -> target

    int16_t int16MinTarget;        // The target minimum
    int16_t int16MaxTarget;        // The target maximum
    int16_t int16RangeTarget;      // The range of the target value (e.g. max - min)

    int16_t int16MinClipTarget;    // The target clip minimum
    int16_t int16MaxClipTarget;    // The target clip maximum

    float   conversionFactorInv;   // The inverted conversionFactor used for target -> raw

    public:

      ValueConverter();
      ValueConverter(int16_t pMinRaw,    int16_t pMaxRaw,
    		         int16_t pMinTarget, int16_t pMaxTarget);

      void     setConversionData(int16_t pMinRaw,    int16_t pMaxRaw,
    		                     int16_t pMinTarget, int16_t pMaxTarget);

      uint16_t convert2Target(int16_t valueRaw);
      uint16_t convert2Raw(int16_t valueTarget);

      void updateMinRaw(int16_t min);
      void updateMaxRaw(int16_t max);
      void calculateConversionFactors();

};

#endif
