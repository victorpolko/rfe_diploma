/*
 * Heart and Breath rate counter
 *
 * Chip type           : ATmega8
 * Program type        : Application
 * AVR Core Clock freq : 2,000000 MHz
 * Memory model        : Small
 * External RAM size   : 0
 * Data Stack size     : 256 B
 *
 * by Polkó Victor © 2014
 */

#include <includes.h>

void main(void) {
  initialize();

  while (1) {
    breath_processing();
    heart_processing();
  }
}
