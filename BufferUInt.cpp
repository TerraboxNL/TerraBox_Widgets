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
