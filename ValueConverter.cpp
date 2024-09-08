/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <ValueConverter.cpp> - Library forGUI Widgets.
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
#include <ValueConverter.h>

ValueConverter::ValueConverter() {
    setConversionData(0, 100, 0, 100);
}

/**--------------------------------------------------------------------------------------------
 *
 *  Constructor for a ValueConverter, which converts a raw value into a target value
 *
 *  @param pMinRaw      minimum Raw value
 *  @param pMaxRaw      maximum Raw value
 *  @param pMinTarget   minimum Target value
 *  @param pMaxTarget   maximum Target value
 *
 *------------------------------------------------------------------------------------------*/
ValueConverter::ValueConverter(int16_t pMinRaw,    int16_t pMaxRaw,
		                       int16_t pMinTarget, int16_t pMaxTarget) {

        setConversionData(pMinRaw, pMaxRaw, pMinTarget, pMaxTarget);
}

 /**--------------------------------------------------------------------------------------------
  *
  *  Calculate the conversion factors and other numbers needed.
  *
  *  @param pMinRaw      minimum Raw value
  *  @param pMaxRaw      maximum Raw value
  *  @param pMinTarget   minimum Target value
  *  @param pMaxTarget   maximum Target value
  *
  *------------------------------------------------------------------------------------------*/
void ValueConverter::setConversionData(int16_t pMinRaw,    int16_t pMaxRaw,
		                               int16_t pMinTarget, int16_t pMaxTarget) {

        //
        //  Copy the min and max raw values
        //  And calculate the raw value range.
        //
        int16MinRaw       = pMinRaw;
        int16MaxRaw       = pMaxRaw;

        //
        //  Copy the specified min and max target values.
        //  And calculate the range
        //
        int16MinTarget    = pMinTarget;
        int16MaxTarget    = pMaxTarget;

        calculateConversionFactors();
}

/**----------------------------------------------------------------------------
 *
 *  Update with a new minimum raw value.
 *
 * @param min  The new minimum raw value
 *
 *---------------------------------------------------------------------------*/
void ValueConverter::updateMinRaw(int16_t min) {
	int16MinRaw = min;
	calculateConversionFactors();
}

/**----------------------------------------------------------------------------
 *
 *  Update with a new maximum raw value.
 *
 * @param max  The new max raw value
 *---------------------------------------------------------------------------*/
void ValueConverter::updateMaxRaw(int16_t max) {
	int16MaxRaw = max;
	calculateConversionFactors();
}

/**----------------------------------------------------------------------------
 *
 *  Calculate a new set of conversion factors.
 *
 *---------------------------------------------------------------------------*/
void ValueConverter::calculateConversionFactors() {
        //
        //  Calculate ranges
        //
        int16RangeRaw     = int16MaxRaw - int16MinRaw;
        int16RangeTarget  = int16MaxTarget - int16MinTarget;

        //
        // The raw min and max values can be inverted from 100 - 0 instead of 0 - 100
        // but the raw value still is only valid in the range 0 - 100.
        // Because of this we have to actually pick the lowest and highest raw value
        // for clipping the offered raw value
        //
        if (int16MinRaw < int16MaxRaw) {
          int16MinClipRaw = int16MinRaw;
          int16MaxClipRaw = int16MaxRaw;
        }
        else {
          int16MinClipRaw = int16MaxRaw;
          int16MaxClipRaw = int16MinRaw;
        }

        //
        // The raw min and max values can be inverted from 100 - 0 instead of 0 - 100
        // but the raw value still is only valid in the range 0 - 100.
        // Because of this we have to actually pick the lowest and highest raw value
        // for clipping the offered raw value
        //
        if (int16MinTarget < int16MaxTarget) {
          int16MinClipTarget = int16MinTarget;
          int16MaxClipTarget = int16MaxTarget;
        }
        else {
          int16MinClipTarget = int16MaxTarget;
          int16MaxClipTarget = int16MinTarget;
        }

        //
        //  Calulate the conversions factors
        //
        conversionFactor    = (float)int16RangeTarget / (float)int16RangeRaw;     // for raw -> target
        conversionFactorInv = (float)int16RangeRaw    / (float)int16RangeTarget;  // for target -> raw
}

/**--------------------------------------------------------------------------------------------
 *
 *  Implements a linear conversion of a raw value to a target value.
 *
 *  @param valueRaw      The raw value to be converted
 *
 *  @return              The calculated target value
 *
 *------------------------------------------------------------------------------------------*/
uint16_t ValueConverter::convert2Target(int16_t valueRaw) {

        //
        //  Clip the offered raw value to the minimum and maximum values
        //
        if (valueRaw < int16MinClipRaw)
          valueRaw = int16MinClipRaw;
        else if (valueRaw > int16MaxClipRaw)
          valueRaw = int16MaxClipRaw;

        //
        // Normalize the raw value to a value within the raw value range.
        // Convert that raw value to a target value using the conversionFactor
        // and offset it with the target minimum value.
        //
        uint16_t result = (valueRaw - int16MinRaw) * conversionFactor + int16MinTarget;
        return result;
}

/**--------------------------------------------------------------------------------------------
 *
 *  Implements a linear conversion of a target value to a raw value.
 *
 *  @param valueTarget   The target value to be converted
 *
 *  @return              The calculated raw value
 *
 *------------------------------------------------------------------------------------------*/
uint16_t ValueConverter::convert2Raw(int16_t valueTarget) {
        //
        //  Clip the offered raw value to the minimum and maximum values
        //
        if (valueTarget < int16MinClipTarget)
          valueTarget = int16MinClipTarget;
        else if (valueTarget> int16MaxClipTarget)
          valueTarget - int16MaxClipTarget;

        //
        // Normalize the raw value to a value within the raw value range.
        // Convert that raw value to a target value using the conversionFactor
        // and offset it with the target minimum value.
        //
        return (valueTarget - int16MinTarget) * conversionFactorInv + int16MinRaw;
}

