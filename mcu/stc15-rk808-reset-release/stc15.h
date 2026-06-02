#ifndef STC15_H
#define STC15_H

#include <8052.h>

/* STC15 GPIO mode registers.
 * PxM1/PxM0 bit encoding on STC15:
 *   00 quasi-bidirectional
 *   01 push-pull output
 *   10 high-impedance input
 *   11 open-drain
 */
__sfr __at (0x91) P1M1;
__sfr __at (0x92) P1M0;

#endif
