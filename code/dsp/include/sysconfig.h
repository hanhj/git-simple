/*
 * sysconfig.h
 *
 *  Created on: 2013-9-22
 *      Author: Administrator
 */

#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_
#include "types.h"
//-----------------------------------------------------------------------------
// Register Structure & Defines//OMAPL_138datasheet P78
//-----------------------------------------------------------------------------

typedef struct
{
   volatile uint32 REVID;            // 0x0000
   volatile uint32 RSVD0;            // 0x0004
   volatile uint32 DIEIDR[4];        // 0x0008
   volatile uint32 RSVD1[2];         // 0x0018
   volatile uint32 BOOTCFG;          // 0x0020
   volatile uint32 RSVD2[5];         // 0x0024
   volatile uint32 KICKR[2];         // 0x0038
   volatile uint32 HOST0CFG;         // 0x0040
   volatile uint32 HOST1CFG;         // 0x0044
   volatile uint32 RSVD3[38];        // 0x0048
   volatile uint32 IRAWSTAT;         // 0x00E0
   volatile uint32 IENSTAT;          // 0x00E4
   volatile uint32 IENSET;           // 0x00E8
   volatile uint32 IENCLR;           // 0x00EC
   volatile uint32 EOI;              // 0x00F0
   volatile uint32 FLTADDRR;         // 0x00F4
   volatile uint32 FLTSTAT;          // 0x00F8
   volatile uint32 RSVD4[5];         // 0x00FC
   volatile uint32 MSTPRI[3];        // 0x0110
   volatile uint32 RSVD5;            // 0x011C
   volatile uint32 PINMUX[20];       // 0x0120 PIN 脚复用管理
   volatile uint32 SUSPSRC;          // 0x0170
   volatile uint32 CHIPSIG;          // 0x0174
   volatile uint32 CHIPSIG_CLR;      // 0x0178
   volatile uint32 CFGCHIP[5];       // 0x017C
} sysconfig_regs_t;

// define the one and only system config module.
#define SYSCONFIG          ((sysconfig_regs_t *)SYSCONFIG_REG_BASE)

// unlock/lock kick registers defines.
#define KICK0R_UNLOCK      (0x83E70B13)
#define KICK1R_UNLOCK      (0x95A4F1E0)
#define KICK0R_LOCK        (0x00000000)
#define KICK1R_LOCK        (0x00000000)

// bitmask defines for cfgchip[0].
#define PLL0_MASTER_LOCK   (0x00000010)   // bit 4

// bitmask defines for cfgchip[3].
#define CLK2XSRC           (0x00000080)   // bit 7
#define PLL1_MASTER_LOCK   (0x00000020)   // bit 5
#define DIV4P5ENA          (0x00000004)   // bit 2
#define EMA_CLKSRC         (0x00000002)   // bit 1

// defines for hostcfg.
#define BOOTRDY            (0x00000001)   // bit 0

// defines for pinmux0.
#define MASK_EMB_WE        (0xF0000000) // bits 31-28
#define MASK_EMB_RAS       (0x0F000000) // bits 27-24


#endif /* SYSCONFIG_H_ */
