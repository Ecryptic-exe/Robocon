#include <mbed.h>

#include "pinmap.h"
#include "communication_format.h"
#include "_74HC165.h"

int numBits = 16;
_74HC165 hc165(PB_5, CP, nPL, numBits);

int main(){
// Setup

  while (true){
    // Loop
    
  }
    return 0;
}

