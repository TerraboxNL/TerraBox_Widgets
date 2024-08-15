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

