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
#include <TerraBox_widgets.h>
#include <Persistence.h>

/*--------------------------------------------------------------------------------------------------
 *
 *  Returns not 0 if there is persistent storage available.
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t hasPersistentStorage() {
   return EEPROM_SIZE;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Returns true if the persistence store is virgin, i.e. not written in before.
 *
 *------------------------------------------------------------------------------------------------*/
bool isPersistentStorageVirgin() {

   for (int i = 0; i < EEPROM_SIZE; i++) {
      if (EEPROM.read(i) != 0xff)
        return false;
   }

   return true;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Store data persistently
 *
 *  addr      Address of the persistent memory where the data is to be stored
 *  data      data to be stored
 *  size      The size of the data in bytes
 *
 *------------------------------------------------------------------------------------------------*/
void persistentStore(unsigned long addr, unsigned char* data, unsigned long size) {

   for (unsigned long i = 0; i < size; i++) {
     //
     // Only write if data differs from what has already bee stored
     //
     if (EEPROM.read(addr) != data) {
       EEPROM.write((int)addr++, *data);
       data++;
     }
   }

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Read persisted data
 *
 *  addr      Address of the persistent memory where the data is stored
 *  data      memory area where the persisted data must be copied to
 *  size      The size of the data in bytes
 *
 *------------------------------------------------------------------------------------------------*/
void persistentRead(unsigned long addr, unsigned char* data, unsigned long size) {

   for (unsigned long i = 0; i < size; i++) {
     *data = EEPROM.read((int)addr++);
     data++;
   }

}

/*---------------------------------------------------------------------------------------------------
 *
 *  Dumps a EEPROM memory area in hex and ASCII for inspection
 *
 *  addr      The EEPROM address to start
 *  size      The number of bytes to dump
 *
 *-------------------------------------------------------------------------------------------------*/
void persistentDump(unsigned long addr, unsigned long size) {

  uint8_t rot = Screen.tft->getRotation();

  if (rot != 1 && rot != 3) {
    Screen.tft->setRotation(1);
  }

  unsigned long memA = addr;

  char s[16];  // Buffer for sprintf formatting

  for (long i = 0; i < size; i++, memA++) {

    //
    // Start of new page
    //
    if (i % 0x100 == 0) {
      Screen.tft->println(F("      00 01 02 03  04 05 06 07  08 09 0a 0b  0c 0d oe 0f   0123 4567 89ab cdef"));
      Screen.tft->println(F("    . -----------  -----------  -----------  ----------- . ---- ---- ---- ---- ."));
    }

    //
    // Print address
    //
    if (i % 16 == 0) {
      sprintf(s, "%4x: ", memA);
      Screen.tft->print(s);
    }

    //
    //
    // Print the byte in hex
    //
    unsigned char b = EEPROM.read(memA);
    sprintf(s, "%02x ", b);
    Screen.tft->print(s);

    if ((i+1) % 16 != 0) {
      if ((i+1) % 4 == 0) {
        Screen.tft->print(F(" "));
      }
    }

    //
    //  At the end of 16 bytes of HEX printing
    //  Print the ASCII readable characters
    //
    if ((i+1) % 16 == 0) {

      Screen.tft->print("| ");

      memA -= 15;
      for (int j = i-15; j <= i; j++, memA++) {
        unsigned char b = EEPROM.read(memA);

        if (b < 0x20 || b >= 0x7f) {
          Screen.tft->print(F("."));
        }
        else {
          Screen.tft->print((char)b);
        }

        if ((j+1) % 4 == 0) {
          Screen.tft->print(F(" "));
        }
      }
      memA--; // Correct the address as it is 1 byte too high!!!!
      Screen.tft->println(F("|"));
    }
   
  }

  //---------------------------------------------------------------------------
  //
  //  Print space for any non-requested bytes up to the 16th
  //  Print the character for the requested bytes and spaces for the remaining. 
  //
  uint16_t rest = size % 16;
  if (rest > 0) {
    memA = memA-rest;
    for (int i = rest; i < 16; i++) {

      //
      //
      // Print the remaining bytes up to 16 as spaces
      //
      Screen.tft->print("   "); // 3 spaces (normally 2 hex digits + 1 space)

      //
      // While not at the end print extra formatting spaces
      //
      if (i < 15) {
        // Print spacing 1 extra
        if ((i+1) % 4 == 0) {
          Screen.tft->print(F(" "));
        }
      }
    }

    //
    //  dump the remaining characters
    //
    Screen.tft->print("| ");
    
    for (int j = 0; j < 16; j++, memA++) {

      //
      // Print the ASCII characters if within the rest
      //
      if (j < rest) {
        unsigned char b = EEPROM.read(memA);

        if (b < 0x20 || b >= 0x7f) {
          Screen.tft->print(F("."));
        }
        else {
          Screen.tft->print(" ");
        }

      }

      //
      // Only print spaces as filler for out of range bytes
      //
      else {
        Screen.tft->print(F(" "));
      }

      if ((j+1) % 4 == 0) {
        Screen.tft->print(F(" "));
      }

    }
   
    Screen.tft->println(F("|"));
  }

  Screen.tft->println(F("                   *** E N D   O F   D U M P ***\n"));



  Screen.tft->println(F("Tap the screen to continue..."));

  XY xy;
  while(!getTouchData(&xy))
      ;

  if (Screen.tft->getRotation() != rot) {
    Screen.tft->setRotation(rot);
  }

  Screen.tft->fillScreen(BLACK);
}

