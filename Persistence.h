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
#include <Arduino.h>
#include <EEPROM.h>

#ifndef PERSISTENCE_h
#define PERSISTENCE_h

/*================================================================================================*/

#ifdef ARDUINO_AVR_MEGA2560

/*================================================================================================*/

  #define EEPROM_SIZE    4096   // The number of bytes available in EEPROM

  #define EEPROM_RD_BYTE(addr) ((unsigned char)(EEPROM.read(addr)))
  #define EEPROM_RD_INT(addr)  ((unsigned int)((EEPROM.read(addr) << 8) + EEPROM.read(addr+1)))
  #define EEPROM_RD_LONG(addr) ((unsigned long)((EEPROM.READ_INT(addr) << 16) + EEPROM.read(addr+2)))

  #define EEPROM_WR_BYTE(addr, v)   EEPROM.write(addr, (unsigned char)(v & 0xff))
  #define EEPROM_WR_INT(addr, v)  \
                                    EEPROM_WR_BYTE(addr,   (unsigned char) ((v & 0xff00) >> 8) ); \
                                    EEPROM_WR_BYTE(addr+1, (unsigned char) ((v & 0x00ff)     ) )
  #define EEPROM_WR_LONG(addr, v) \
                                    EEPROM_WR_INT(addr,    (unsigned int) ((v & 0xffff0000) >> 16) ); \
                                    EEPROM_WR_INT(addr+2,  (unsigned int) ((v & 0x0000ffff)      ) )

/*================================================================================================*/

#else      // Otherwise platform unknown

/*================================================================================================*/
  #define EEPROM_SIZE    0      // Unknown board, assume no EEPROM.

#endif


//
//   EEPROM  m e m o r y  m a p 
//
//          +---------------+- 0
//          |               |
//          |     Fixed     |
//          |      size     |
//          |      data     |
//          |               |
//          +---------------+- EEPROM_START_FREE
//          |       |       |
//          |       V grows |
//          :       :       :
//          :       :       :
//          :       :       :
//          |       A grows |
//          |       |       |
//          +---------------+- EEPROM_END_FREE
//          |               |
//          |    Variable   |
//          :      size     :
//          |      data     |
//          |               |
//          +---------------+- EEPROM_SIZE
//
//
//
//
//

#define EPR16_TFT_X_W          0      // Contains 16 bit screen width
#define EPR16_TFT_Y_H          2      // Contains 16 bit screen height
#define EPR8_TFT_CALIBRATED    4      // Contains 0 if not calibrated yet, non-zero otherwise
#define EPR8_CELL_S            5      // Cell size used to calibrate
#define EPR16_TFT_CALIBR_X_S   6      // Calibration data size in bytes for the X-axis 
#define EPR16_TFT_CALIBR_Y_S   8      // Calibration data size in bytes for the Y axis

#define EPR_START_FREE         10     // Current free EEPROM

//
//  The following are are EEPROM memory areas that depend on the variable length data
//  Each of those memory areas are stored in a fixed order.
//  Since their size is dynamically specified, these macro's only provide sensible addresses
//  if the length of those areas is stored in their assigned EEPROM address.
//
#define ADR_TFT_CALIBR_X          ((unsigned long)(EEPROM_SIZE - \
                                                   (EEPROM_RD_INT(EPR16_TFT_CALIBR_X_S) * sizeof(uint16_t)) ) )
#define ADR_TFT_CALIBR_Y          ((unsigned long)((ADR_TFT_CALIBR_X) - \
                                                   (EEPROM_RD_INT(EPR16_TFT_CALIBR_Y_S) * sizeof(uint16_t)) ) )


#define EEPROM_END_FREEMEM ADR_CALWDTH  // The width calibration data is the first chuck of variable data

extern uint16_t hasPersistentStorage();
extern bool     isPersistentStorageVirgin();
extern void     persistentStore(unsigned long addr, unsigned char* data, unsigned long size);
extern void     persistentRead( unsigned long addr, unsigned char* data, unsigned long size);
extern void     persistentDump( unsigned long addr, unsigned long size);

#endif
