/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                  A R D U I N O   G U I   W I D G E T S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               Dump.h - Library forGUI Widgets.
                                6 Sep 2024
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

#ifndef DUMP_H_
#define DUMP_H_

#define DUMP_SCREEN  0  // Dumping on TFT screen
#define DUMP_SERIAL  1  // Dumping on Arduino Screen monitor

class Dump {
private:
	uint16_t type = DUMP_SERIAL;
	void printDumpHeader();
	unsigned char readMemory(uint32_t addr, bool eeprom);

	int16_t getRotation();
	void    setRotation(uint16_t r);
	int16_t getTextSize();
	void    setTextSize(uint16_t s);
	void    setCursor(int16_t x, int16_t y);

    void fillScreen(uint16_t color);

    size_t print(const __FlashStringHelper* s);
    size_t print(const char* (&s));
    size_t print(char* s);
    size_t print(char c);
    size_t print(unsigned int j, int i = DEC);
    size_t print(int j, int i = DEC);
    size_t print(unsigned long int j, int i = DEC);
    size_t print(long int j, int i = DEC);

    size_t println();
    size_t println(const __FlashStringHelper* s);
    size_t println(const char* (&s));
    size_t println(char* s);
    size_t println(char c);
    size_t println(unsigned int j, int i = DEC);
    size_t println(int j, int i = DEC);
    size_t println(unsigned long int j, int i = DEC);
    size_t println(long int j, int i = DEC);

    void waitForTap();

    void dump(uint32_t addr, uint16_t size, bool eeprom);

public:

	Dump(uint16_t type);

	void dumpRam(uint32_t addr, uint16_t size);
    void dumpEeprom(uint32_t addr, uint16_t size);

    void dumpAreaHeader(uint32_t addr);
    void dumpAreaData(uint32_t addr);
    void dumpPersistentAreas();
    void listPersistentAreas();
};

extern Dump dumpSerial;  // Dump to serial device
extern Dump dumpScreen;  // Dump to TFT screen

#endif
