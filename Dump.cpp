/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                  A R D U I N O   G U I   W I D G E T S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

    Dump.cpp - Library managing Arduino persistent memory usage.
                               6 Sep 2024
                       Released into the public domain
              as GitHub project: TerraboxNL/TerraBox_Persistence
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
#include <dump.h>
#include <EEPROM.h>
#include <persistence.h>

Dump::Dump(uint16_t theType) {
      type = theType;
}

int16_t Dump::getRotation() {
	switch(type) {
	case DUMP_SCREEN:
		return Screen.getRotation();
	}

	return -1;
}

void Dump::setRotation(uint16_t r) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.setRotation(r);
		break;
	}
}
void Dump::setTextSize(uint16_t s) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.setTextSize(s);
		break;
	}
}
void Dump::setCursor(int16_t x, int16_t y) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.setCursor(x, y);
		break;
	}
}

void Dump::fillScreen(uint16_t color) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.fillScreen(color);
		break;
	}

}

size_t Dump::print(const __FlashStringHelper* s){
	switch(type) {
	case DUMP_SCREEN:
		Screen.print(s);
		break;
	case DUMP_SERIAL:
		Serial.print(s);
		break;
	}
}

size_t Dump::print(const char* (&s)) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.print(s);
		break;
	case DUMP_SERIAL:
		Serial.print(s);
		break;
	}
}
size_t Dump::print(char* s) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.print(s);
		break;
	case DUMP_SERIAL:
		Serial.print(s);
		break;
	}
}
size_t Dump::print(char c) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.print(c);
		break;
	case DUMP_SERIAL:
		Serial.print(c);
		break;
	}
}
size_t Dump::print(unsigned int j, int i = DEC) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.print(j, i);
		break;
	case DUMP_SERIAL:
		Serial.print(j, i);
		break;
	}
}
size_t Dump::print(int j, int i = DEC) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.print(j, i);
		break;
	case DUMP_SERIAL:
		Serial.print(j, i);
		break;
	}
}
size_t Dump::print(unsigned long int j, int i = DEC) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.print(j, i);
		break;
	case DUMP_SERIAL:
		Serial.print(j, i);
		break;
	}
}
size_t Dump::print(long int j, int i = DEC) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.print(j, i);
		break;
	case DUMP_SERIAL:
		Serial.print(j, i);
		break;
	}
}

size_t Dump::println() {
	switch(type) {
	case DUMP_SCREEN:
		Screen.println();
		break;
	case DUMP_SERIAL:
		Serial.println();
		break;
	}
}

size_t Dump::println(const __FlashStringHelper* s){
	switch(type) {
	case DUMP_SCREEN:
		Screen.println(s);
		break;
	case DUMP_SERIAL:
		Serial.println(s);
		break;
	}
}

size_t Dump::println(const char* (&s)) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.println(s);
		break;
	case DUMP_SERIAL:
		Serial.println(s);
		break;
	}
}
size_t Dump::println(char* s) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.println(s);
		break;
	case DUMP_SERIAL:
		Serial.println(s);
		break;
	}
}
size_t Dump::println(char c) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.println(c);
		break;
	case DUMP_SERIAL:
		Serial.println(c);
		break;
	}
}
size_t Dump::println(unsigned int j, int i = DEC) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.println(j, i);
		break;
	case DUMP_SERIAL:
		Serial.println(j, i);
		break;
	}
}
size_t Dump::println(int j, int i = DEC) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.println(j, i);
		break;
	case DUMP_SERIAL:
		Serial.println(j, i);
		break;
	}
}
size_t Dump::println(unsigned long int j, int i = DEC) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.println(j, i);
		break;
	case DUMP_SERIAL:
		Serial.println(j, i);
		break;
	}
}
size_t Dump::println(long int j, int i = DEC) {
	switch(type) {
	case DUMP_SCREEN:
		Screen.println(j, i);
		break;
	case DUMP_SERIAL:
		Serial.println(j, i);
		break;
	}
}

void Dump::waitForTap() {
	switch(type) {
	  case DUMP_SCREEN: {
		Screen.println(F("Tap to continue..."));

		waitForATap();
		break;
	  }
	  case DUMP_SERIAL: {
		Serial.println(F("Hit Enter to continue..."));

		while (!Serial.available())
			;

		while (Serial.available())
			Serial.read();

		break;
	  }
	}
}

/**------------------------------------------------------------------------------------------------
 *
 *  Reads a memory address
 *
 * @param addr
 * @param eeprom
 * @return        Unsigned byte
 *
 *-----------------------------------------------------------------------------------------------*/
unsigned char Dump::readMemory(uint32_t addr, bool eeprom) {
	if (eeprom) {
		return EEPROM.read(addr);
	}

	return *((unsigned char*)addr);
}


/**----------------------------------------------------------------------------
 *
 *  Prints the header for a memory dump
 *
 *---------------------------------------------------------------------------*/
void Dump::printDumpHeader() {
    println();
    println(F("      00 01 02 03  04 05 06 07  08 09 0a 0b  0c 0d 0e 0f   0123 4567 89ab cdef"));
    println(F("    . -----------  -----------  -----------  ----------- . ---- ---- ---- ---- ."));
}


/**---------------------------------------------------------------------------------------------------
 *
 *  Dumps a EEPROM memory area in hex and ASCII for inspection
 *
 *  @param dump      The device to write the dump to
 *  @param addr      The EEPROM address to start
 *  @param size      The number of bytes to dump
 *  @param eeprom    If true addresses are seen as EEPROM addresses
 *
 *-------------------------------------------------------------------------------------------------*/
void Dump::dump(uint32_t addr, uint16_t size, bool eeprom) {

  uint8_t rot = getRotation();
  char s[16];  // Buffer for sprintf formatting

  if (rot != 1 && rot != 3) {
    setRotation(1);
  }

  setTextSize(1);

  //
  //  Assign the start address
  //
  unsigned long memA = addr;
  int spacing        = (memA & 0xf);

  //
  //  If  not EEPROM, then show RAM page
  //
  if (!eeprom) {
		print(F("      RAM page: 0x"));
	    sprintf(s, "%4x: ", (uint16_t)((memA & 0xffff0000) >> 16));
	    print(s);
  }

  //
  //  Print an initial header, if not at a 0x100 boundary
  //
  if (memA % 100 != 0) {
	  printDumpHeader();
  }

  //
  // Print initial fillers if address does not start at multiple of 16
  //
  if (memA % 16 > 0) {
	  sprintf(s, "%4x: ", (memA & 0xffff));
	  print(s);

	  for (int i = 0; i < spacing; i++) {
	    print(F("   "));
	    if ((i+1) % 16 != 0) {
	      if ((i+1) % 4 == 0) {
	        print(F(" "));
	      }
	    }
	  }
  }


  for (long i = spacing; i < size+spacing; i++, memA++) {

    //
    // Start of new page
    //
	if (memA % 0x100 == 0) {
    	printDumpHeader();
    }

    //
    // Print address
    //
    if (memA % 16 == 0) {
      sprintf(s, "%4x: ", (memA & 0xffff));
      print(s);
    }

    //
    // Print the byte in hex
    //
    unsigned char b = readMemory(memA, eeprom);
    sprintf(s, "%02x ", b);
    print(s);

    if ((i+1) % 16 != 0) {
      if ((i+1) % 4 == 0) {
       print(F(" "));
      }
    }

    //
    //  At the end of 16 bytes of HEX printing
    //  Print the ASCII readable characters
    //  TODO First row with starting address prints too many characters.
    //
    //       Example:   00 01 02 03 04 05  etc        0123 56 etc
    //                 -------------------------...-.------------...
    //       e.g. 0x4:              31 32, etc      | .... 12 etc
    //                                                --v-
    //                                                  Should no be printed!
    //
    if ((memA+1) % 16 == 0) {

      print(F("| "));

      memA -= 15;
      for (int j = i-15; j <= i; j++, memA++) {
        if (memA < addr) {
          print(F(" "));
        }
        else {
          unsigned char b = readMemory(memA, eeprom);

          if (b < 0x20 || b >= 0x7f) {
            print(F("."));
          }
          else {
            print((char)b);
          }
        }

        //
        // Print some filler space
        //
        if ((j+1) % 4 == 0) {
          print(F(" "));
        }
      }
      memA--; // Correct the address as it is 1 byte too high!!!!
      println(F("|"));
    }

  }

  //---------------------------------------------------------------------------
  //
  //  Print space for any non-requested bytes up to the 16th
  //  Print the character for the requested bytes and spaces for the remaining.
  //
  uint16_t rest = (memA % 16);
  if (rest > 0) {
    for (int i = rest; i < 16; i++) {

      //
      //
      // Print the remaining bytes up to 16 as spaces
      //
      print(F("   ")); // 3 spaces (normally 2 hex digits + 1 space)

      //
      // While not at the end print extra formatting spaces
      //
      if (i < 15) {
        // Print spacing 1 extra
        if ((i+1) % 4 == 0) {
          print(F(" "));
        }
      }
    }

    //
    //  dump the remaining characters
    //
    print(F("| "));

    //
    //  Assume the remaining addresses to dump start at mod 16
    //  Which is normallly the case, expect if we have to dump
    //  an address range < 16 bytes, which does not start at a mod 16 address
    //
    int jStart = 0;  // Assume we have to start at mod 16 address

    //
    //  Find out if we have a short address range that fits on a single line
    //  and does not start on a 16 byte boundary.
    //  In that case we have to print filler spaces first up to the start address
    //
    if ((memA - addr) < (16 - (addr % 16))) {
      jStart = (addr % 16);
      for (int j = 0; j < jStart; j++ ) {
    	  print(F(" "));
    	  if ((j+1) % 4 == 0)
    		print(F(" "));
      }
      memA = addr;
      jStart += memA - addr;
    }
    else {
      memA -= rest;
      jStart = 0;
    }

    for (int j = jStart; j < 16; j++, memA++) {

      //
      // Print the ASCII characters if within the rest
      //
      if (j < rest) {
        unsigned char b = readMemory(memA, eeprom);

        if (b < 0x20 || b >= 0x7f) {
          print(F("."));
        }
        else {
          print((char)b);
        }

      }

      //
      // Only print spaces as filler for out of range bytes
      //
      else {
        print(F(" "));
      }

      if ((j+1) % 4 == 0) {
        print(F(" "));
      }

    }

    println(F("|"));
  }

  println(F("\n                   *** E N D   O F   D U M P ***\n"));


  waitForTap();

  if (getRotation() != rot) {
    setRotation(rot);
  }

  fillScreen(BLACK);
  setCursor(0,0);
}

void Dump::dumpRam(uint32_t addr, uint16_t size) {
	dump(addr, size, false);
}

void Dump::dumpEeprom(uint32_t addr, uint16_t size) {
	dump(addr, size, true);
}

/**----------------------------------------------------------------------------
 *
 *  Print the area header for the name specified
 *
 * @param name  The area name to look for.
 *        -1    name is unknown
 *         1    printed
 *
 *---------------------------------------------------------------------------*/
int16_t dumpAreaHeader(char* name) {
	uint32_t addr = getPersistentHeaderAddress(name);
	if (addr == 0)
		return -1;

	persistentDump(addr, sizeof(PERSISTENT_AREA_PREFIX_SIZE));
	return 1;
}


/**----------------------------------------------------------------------------
 *
 *  Dumps the contents of the data area (header + data part).
 *
 *  @param addr  Start address of the header. (Note: NOT the normal Area address)
 *
 *---------------------------------------------------------------------------*/
void Dump::dumpAreaData(uint32_t addr) {
	struct persistentAreaHeader header;
	persistentReadHeader(addr+PERSISTENT_AREA_PREFIX_SIZE, &header);

	println(F("Data area: H E A D E R   P A R T (interpreted) @ = address, + = Offset, x = Void"));
	println();
	print(F("Header @: 0x")); println(addr, HEX);
	print(F("Next   +: 0x")); print(header.next, HEX); print(F(", Next @: 0x")); println((addr + header.next), HEX);
	if (header.data != 0xffff) {
	  print(F("Data   +: 0x")); print(header.data, HEX); print(F(", Data @: 0x")); println((addr + header.data), HEX);
	  print(F("Name    : ")); println(header.name);
	}
	else {
	  print(F("Data   x: 0x")); println(header.data, HEX);
	  print(F("Name   x: "));
	  for (int i = 0; i < sizeof(header.name); i++) {
		  if ((unsigned char)header.name[i] < 0x20) {
			  print(F(" 0x"));print((unsigned char)header.name[i], HEX); print(F(" "));
		  }
		  else {
			  print(header.name[i]);
		  }
	  }
	  println();
	}

	println();

	println(F("Data area: H E A D E R   P A R T (raw)"));
	dumpEeprom(addr, sizeof(struct persistentAreaHeader));

	println(F("Data area: D A T A   P A R T (raw)"));
	dumpEeprom(addr+header.data, header.next - header.data);
}

/**----------------------------------------------------------------------------
 *
 *  Dump the persistent data areas
 *
 *---------------------------------------------------------------------------*/
void Dump::dumpPersistentAreas() {
	  uint8_t rot = Screen.tft->getRotation();
	  if (rot != 1 && rot != 3) {
	    Screen.tft->setRotation(1);
	  }

	  setCursor(0,0);

	//
	//  For as long as there is allocatable EEPROM memory,
	//
	struct persistentAreaHeader header;

	int i = 0;
	for (uint32_t addr = EPR_START_FREE; addr < EPR_END_FREE; addr += header.next) {

		//
		//  Read in the header
		//
        persistentReadHeader(addr + PERSISTENT_AREA_PREFIX_SIZE, &header);

        //
        //  If the next contains 0xffff, that is the end of the list
        //
        if (header.next == 0xffff) {
      	  break;
        }

        fillScreen(BLACK);

        //
        //  Dump the data area
        //
        dumpScreen.dumpAreaData(addr);

        i++;
     }

}

/**----------------------------------------------------------------------------
 *
 *  Lists the data areas
 *
 *---------------------------------------------------------------------------*/
void Dump::listPersistentAreas() {
	//
	//  Initial println
	//
    println();
    println(F("Data areas:"));
    println(F("---------------------------------------"));
    println(F("         Area name Addr Next Data"));
    println(F("- ---------------- ---- ---- ---- -----"));

	//
	//  For as long as there is allocatable EEPROM memory,
	//
	struct persistentAreaHeader header;

	int i = 0;
	for (uint32_t addr = EPR_START_FREE; addr < EPR_END_FREE; addr += header.next) {

		//
		//  Read in the header
		//
        persistentReadHeader(addr + PERSISTENT_AREA_PREFIX_SIZE, &header);

        //
        //  If the next contains 0xffff, that is the end of the list
        //
        if (header.next == 0xffff) {

           println(F("---------------------------------------"));
          print(F("Found "));print(i);println(F(" data area(s)"));
          println();
      	  break;
        }

        char buffer[64];
        char freeOrNot = (header.data == 0xffff) ? 'F' : 'O';
        char* name     = (freeOrNot == 'O') ? header.name : "-- Free --";
        sprintf(buffer, "%c %16s %04x %04x %04x ", freeOrNot, name, (uint16_t)addr, (uint16_t)header.next, (uint16_t)header.data);
        println(buffer);

        i++;
     }

	waitForTap();
}

Dump dumpSerial(DUMP_SERIAL);
Dump dumpScreen(DUMP_SCREEN);

