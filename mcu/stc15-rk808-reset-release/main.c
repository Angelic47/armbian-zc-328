#include "stc15.h"

/* Board signal name from the original firmware/project.
 * On RK808 reference design, VPPOTP is also used as high-active RESET input:
 *   VCCRTC -- 10K -- RESET button -- VPPOTP
 * Therefore:
 *   VPPOTP high  = reset asserted
 *   VPPOTP low/Z = reset released / normal operation
 */
#define RK808_RESET_VPPOTP      P1_3
#define RK808_RESET_VPPOTP_MASK 0x08

static void release_rk808_reset(void)
{
    /* Safest idle state: high-impedance input.
     * This avoids holding VPPOTP high, and also does not fight the physical
     * RESET button path through the 10K resistor.
     */
    RK808_RESET_VPPOTP = 1;              /* release port latch before Hi-Z */
    P1M1 |=  RK808_RESET_VPPOTP_MASK;    /* mode bit 1 */
    P1M0 &= ~RK808_RESET_VPPOTP_MASK;    /* mode bit 0 => 10: high-Z input */
}

void main(void)
{
    release_rk808_reset();

    /* Enter STC power-down. The P1.3 pin remains released instead of
     * permanently asserting the RK808 reset input.
     */
    PCON |= 0x02;

    while (1) {
        ;
    }
}
