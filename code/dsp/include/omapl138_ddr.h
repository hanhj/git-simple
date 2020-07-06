/*
 * omapl138_ddr.h
 *
 *  Created on: 2013-9-26
 *      Author: Administrator
 */

#ifndef OMAPL138_DDR_H_
#define OMAPL138_DDR_H_


//-----------------------------------------------------------------------------
// Register Structure & Defines
//-----------------------------------------------------------------------------
typedef struct
{
  volatile uint32 REVID;          // 0x0000
  volatile uint32 SDRSTAT;        // 0x0004
  volatile uint32 SDCR;           // 0x0008
  volatile uint32 SDRCR;          // 0x000C
  volatile uint32 SDTIMR1;        // 0x0010
  volatile uint32 SDTIMR2;        // 0x0014
  volatile uint32 RSVD1;          // 0x0018
  volatile uint32 SDCR2;          // 0x001C
  volatile uint32 PBBPR;          // 0x0020
  volatile uint32 RSVD2;          // 0x0024
  volatile uint32 VBUSMCFG1;      // 0x0028
  volatile uint32 VBUSMCFG2;      // 0x002C
  volatile uint32 RSVD3[36];      // 0x0030
  volatile uint32 IRR;            // 0x00C0
  volatile uint32 IMR;            // 0x00C4
  volatile uint32 IMSR;           // 0x00C8
  volatile uint32 IMCR;           // 0x00CC
  volatile uint32 RSVD4[4];       // 0x00D0
  volatile uint32 DDRPHYREV;      // 0x00E0
  volatile uint32 DDRPHYCTL1;     // 0x00E4
  volatile uint32 DDRPHYCTL2;     // 0x00E8
} ddr_regs_t;

#define DDR          ((ddr_regs_t*)DDR_REG_BASE)

#define VTPIO_CTL    (*(uint32 *)(0x1E2C0000))


#endif /* OMAPL138_DDR_H_ */
