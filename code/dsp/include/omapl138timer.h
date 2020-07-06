/*
 * omapl138timer.h
 *
 *  Created on: 2013-10-14
 *      Author: Administrator
 */

#ifndef OMAPL138TIMER_H_
#define OMAPL138TIMER_H_
#include "omapl138.h"
typedef struct
{
    volatile uint32 REV;             // 0x0000
    volatile uint32 EMUMGT;          // 0x0004
    volatile uint32 GPINT_GPEN;      // 0x0008
    volatile uint32 GPDATA_GPDIR;    // 0x000C
    volatile uint32 TIM12;           // 0x0010
    volatile uint32 TIM34;           // 0x0014
    volatile uint32 PRD12;           // 0x0018
    volatile uint32 PRD34;           // 0x001C
    volatile uint32 TCR;             // 0x0020
    volatile uint32 TGCR;            // 0x0024
    volatile uint32 WDTCR;           // 0x0028
    volatile uint32 RSVD0[2];        // 0x002C
    volatile uint32 REL12;           // 0x0034
    volatile uint32 REL34;           // 0x0038
    volatile uint32 CAP12;           // 0x003C
    volatile uint32 CAP34;           // 0x0040
    volatile uint32 INTCTLSTAT;      // 0x0044
    volatile uint32 RSVD1[6];        // 0x0048
    volatile uint32 CMP0;            // 0x0060
    volatile uint32 CMP1;            // 0x0064
    volatile uint32 CMP2;            // 0x0068
    volatile uint32 CMP3;            // 0x006C
    volatile uint32 CMP4;            // 0x0070
    volatile uint32 CMP5;            // 0x0074
    volatile uint32 CMP6;            // 0x0078
    volatile uint32 CMP7;            // 0x007C
} timer_regs_t;

// define all the available timer peripherals for the processor.
#define TMR0            ((timer_regs_t *)TIMER0_REG_BASE)
#define TMR1            ((timer_regs_t *)TIMER1_REG_BASE)

#define TMR2            ((timer_regs_t *)TIMER2_REG_BASE)
#define TMR3            ((timer_regs_t *)TIMER3_REG_BASE)

// bitmask defines for GPINT_GPEN.
#define GPENO34         (0x02000000)   // bit 25
#define GPENI34         (0x01000000)   // bit 24
#define GPENO12         (0x00020000)   // bit 17
#define GPENI12         (0x00010000)   // bit 16
#define GPINT34INVO     (0x00002000)   // bit 13
#define GPINT34INVI     (0x00001000)   // bit 12
#define GPINT34ENO      (0x00000200)   // bit 9
#define GPINT34ENI      (0x00000100)   // bit 8
#define GPINT12INVO     (0x00000020)   // bit 5
#define GPINT12INVI     (0x00000010)   // bit 4
#define GPINT12ENO      (0x00000002)   // bit 1
#define GPINT12ENI      (0x00000001)   // bit 0

// bitmask defines for GPDATA_GPDIR.
#define GPDIRO34        (0x02000000)   // bit 25
#define GPDIRI34        (0x01000000)   // bit 24
#define GPDIRO12        (0x00020000)   // bit 17
#define GPDIRI12        (0x00010000)   // bit 16
#define GPDATAO34       (0x00000200)   // bit 9
#define GPDATAI34       (0x00000100)   // bit 8
#define GPDATAO12       (0x00000002)   // bit 1
#define GPDATAI12       (0x00000001)   // bit 0

// bitmask defines for TCR.
#define ENAMODE34                (0x00C00000)   // bit 22,23
#define ENAMODE34_ONETIME        (0x00400000)   // bit 22
#define ENAMODE34_CONT           (0x00800000)   // bit 23
#define ENAMODE34_CONT_RELOAD    (0x00C00000)   // bit 22,23
#define ENAMODE12                (0x000000C0)   // bit 6,7
#define ENAMODE12_ONETIME        (0x00000040)   // bit 6
#define ENAMODE12_CONT           (0x00000080)   // bit 7
#define ENAMODE12_CONT_RELOAD    (0x000000C0)   // bit 6,7

// bitmask defines for TGCR.
#define PRESCALER(n)             ((n) << 8)
#define PLUSEN                   (0x00000010)   // bit 4
#define TIMMODE_64BIT            (0x0000000C)   // bit 2,3
#define TIMMODE_32BIT_UNCHAINED  (0x00000004)   // bit 2
#define TIMMODE_64BIT_WDOG       (0x00000008)   // bit 3
#define TIMMODE_32BIT_CHAINED    (0x0000000C)   // bit 2,3
#define TIM34RS                  (0x00000002)   // bit 1
#define TIM12RS                  (0x00000001)   // bit 0

// bitmask defines for INTCTLSTAT.
#define PRDINTSTAT34             (0x00020000)   // bit 17
#define PRDINTEN34               (0x00010000)   // bit 16
#define PRDINTSTAT12             (0x00000002)   // bit 1
#define PRDINTEN12               (0x00000001)   // bit 0

#define DELAY_10TH_SEC     (100000)   // in us
#define DELAY_QUARTER_SEC  (250000)    // in us
#define DELAY_HALF_SEC     (500000)    // in us
#define DELAY_1_SEC        (1000000)   // in us


#endif /* OMAPL138TIMER_H_ */
