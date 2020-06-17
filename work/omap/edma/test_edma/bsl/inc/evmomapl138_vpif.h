//-----------------------------------------------------------------------------
// \file    evmomapl138_vpif.h
// \brief   OMAP-L138 vpif registers, bit definitions, and
//          function prototypes.
//
//-----------------------------------------------------------------------------

#ifndef EVMOMAPL138_VPIF_H
#define EVMOMAPL138_VPIF_H

#if defined (__cplusplus)
extern "C" {
#endif
//-----------------------------------------------------------------------------
// Register Structure & Defines
//-----------------------------------------------------------------------------
typedef struct
{
   volatile uint32_t PID;           // 0x0000
   volatile uint32_t CH0CTRL;       // 0x0004
   volatile uint32_t CH1CTRL;       // 0x0008
   volatile uint32_t CH2CTRL;       // 0x000C
   volatile uint32_t CH3CTRL;       // 0x0010
   volatile uint32_t RSVD0[3];      // 0x0014
   volatile uint32_t INTEN;         // 0x0020
   volatile uint32_t INTENSET;      // 0x0024
   volatile uint32_t INTENCLR;      // 0x0028
   volatile uint32_t INTSTAT;       // 0x002C
   volatile uint32_t INTSTATCLR;    // 0x0030
   volatile uint32_t EMUCTRL;       // 0x0034
   volatile uint32_t REQSIZE;       // 0x0038
} vpif_regs_t;

#define VPIF            ((vpif_regs_t *)VPIF_REG_BASE)

// bitmask defines for CHnCTRL (all).
#define CLKEDGE               (0x80000000)   // bit 31
#define FIELDFRAME            (0x00001000)   // bit 12
#define VANC                  (0x00000200)   // bit 9
#define HANC                  (0x00000100)   // bit 8
#define INTFRAME_TOP_ONLY     (0x00000000)   // bit 6-7
#define INTFRAME_BOTTOM_ONLY  (0x00000040)   // bit 6-7
#define INTFRAME_TOP_BOTTOM   (0x00000080)   // bit 6-7
#define YCMUX                 (0x00000008)   // bit 3
#define CHANEN                (0x00000001)   // bit 0

// bitmask defines for CHnCTRL (0-1).
#define DATAWIDTH_8BPP        (0x00000000)   // bit 28-29
#define DATAWIDTH_10BPP       (0x10000000)   // bit 28-29
#define DATAWIDTH_12BPP       (0x20000000)   // bit 28-29
#define FIDINV                (0x00008000)   // bit 15
#define VVINV                 (0x00004000)   // bit 14
#define HVINV                 (0x00002000)   // bit 13
#define VVINV                 (0x00004000)   // bit 14
#define CH01_INTRPROG         (0x00000400)   // bit 10
#define CAPMODE               (0x00000004)   // bit 2
#define INTLINE_SHIFT         (16)

// bitmask defines for CHnCTRL (2-3).
#define CLIPANC               (0x00004000)   // bit 14
#define CLIPVID               (0x00002000)   // bit 13
#define CH23_INTRPROG         (0x00000800)   // bit 11
#define PIXEL_DATA            (0x00000400)   // bit 10
#define CLKEN                 (0x00000002)   // bit 1

// bitmask defines for INT*.
#define INT_FRAME_CH0         (0x00000001)   // bit 0
#define INT_FRAME_CH1         (0x00000002)   // bit 1
#define INT_FRAME_CH2         (0x00000004)   // bit 2
#define INT_FRAME_CH3         (0x00000008)   // bit 3

typedef struct
{
   volatile uint32_t TLUMA;         // 0x0000
   volatile uint32_t BLUMA;         // 0x0004
   volatile uint32_t TCHROMA;       // 0x0008
   volatile uint32_t BCHROMA;       // 0x000C
   volatile uint32_t THANC;         // 0x0010
   volatile uint32_t BHANC;         // 0x0014
   volatile uint32_t TVANC;         // 0x0018
   volatile uint32_t BVANC;         // 0x001C
   volatile uint32_t SUBPIC_CFG;    // 0x0020
   volatile uint32_t IMGOFFSET;     // 0x0024
   volatile uint32_t HANCOFFSET;    // 0x0028
   volatile uint32_t HCFG;          // 0x002C
   volatile uint32_t VCFG0;         // 0x0030
   volatile uint32_t VCFG1;         // 0x0034
   volatile uint32_t VCFG2;         // 0x0038
   volatile uint32_t VSIZE;         // 0x003C
} vpif_cap_regs_t;

#define VPIF_CAP_CH0    ((vpif_cap_regs_t *)VPIF_CAP_CH0_REG_BASE)
#define VPIF_CAP_CH1    ((vpif_cap_regs_t *)VPIF_CAP_CH1_REG_BASE)

// bitmask defines for HCFG.
#define EAV2SAV_SHIFT   (16)

typedef struct
{
   volatile uint32_t TLUMA;         // 0x0000
   volatile uint32_t BLUMA;         // 0x0004
   volatile uint32_t TCHROMA;       // 0x0008
   volatile uint32_t BCHROMA;       // 0x000C
   volatile uint32_t THANC;         // 0x0010
   volatile uint32_t BHANC;         // 0x0014
   volatile uint32_t TVANC;         // 0x0018
   volatile uint32_t BVANC;         // 0x001C
   volatile uint32_t SUBPIC_CFG;    // 0x0020
   volatile uint32_t IMGOFFSET;     // 0x0024
   volatile uint32_t HANCOFFSET;    // 0x0028
   volatile uint32_t HCFG;          // 0x002C
   volatile uint32_t VCFG0;         // 0x0030
   volatile uint32_t VCFG1;         // 0x0034
   volatile uint32_t VCFG2;         // 0x0038
   volatile uint32_t VSIZE;         // 0x003C
   volatile uint32_t THANCPOS;      // 0x0040
   volatile uint32_t THANCSIZE;     // 0x0044
   volatile uint32_t BHANCPOS;      // 0x0048
   volatile uint32_t BHANCSIZE;     // 0x004C
   volatile uint32_t TVANCPOS;      // 0x0050
   volatile uint32_t TVANCSIZE;     // 0x0054
   volatile uint32_t BVANCPOS;      // 0x0058
   volatile uint32_t BVANCSIZE;     // 0x005C
} vpif_disp_regs_t;

#define VPIF_DISP_CH2   ((vpif_disp_regs_t *)VPIF_DISP_CH2_REG_BASE)
#define VPIF_DISP_CH3   ((vpif_disp_regs_t *)VPIF_DISP_CH3_REG_BASE)

//-----------------------------------------------------------------------------
// Public Defines and Macros
//-----------------------------------------------------------------------------
#define ADDR_VIDEO_DATA_BASE        (0xC1000000)

//-----------------------------------------------------------------------------
// Public Typedefs
//-----------------------------------------------------------------------------
typedef enum
{
   VIDEO_CONN_COMPOSITE = 0,
   VIDEO_CONN_SVIDEO,
   VIDEO_CONN_CAMERA
} video_connector_e;

//-----------------------------------------------------------------------------
// Public Function Prototypes
//-----------------------------------------------------------------------------
uint32_t VPIF_initReceive(video_connector_e in_conn);
uint32_t VPIF_initTransmit(video_connector_e in_conn);

#if defined (__cplusplus)
}
#endif
#endif

