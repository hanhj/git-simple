/*
 * userlink.cmd
 *
 *  Created on: 2013-8-9
 *      Author: Administrator
 */

/*
MEMORY
{
   HFPGA (RWX) : org = 0x60000002, len = 0x0000000e//cs2
   SFPGA (RWX) : org = 0x60000020, len = 0x00000200
}*/
/*
A0 - A12
*/
/*
MEMORY
{
   YCMEM (RWX) : org = 0x60000000, len = 0x00000200//cs2
   YXMEM (RWX) : org = 0x60000400, len = 0x00000200
   YKMEM (RWX) : org = 0x60000800, len = 0x00000200
}*/

MEMORY
{
   YCMEM (RWX) : org = 0x60000300, len = 0x00000100//cs2
   /*BANK5_ (RWX) : org = 0x66000000, len = 0x0200000//cs5*/
   YXMEM (RWX) : org = 0x60000100, len = 0x00000100
   YKMEM (RWX) : org = 0x60000200, len = 0x00000100
}
/*
MEMORY
{
   YCMEM (RWX) : org = 0x60000c00, len = 0x00000100//cs2
   BANK5_ (RWX) : org = 0x66000000, len = 0x0200000//cs5
   YXMEM (RWX) : org = 0x60000800, len = 0x00000100
   YKMEM (RWX) : org = 0x60000000, len = 0x00000100
}*/
SECTIONS
{
	DATAINL1PSRAM:>L1PSRAM
CODEINL1PSRAM:>IRAM
/*CODEINL1PSRAM:>L1PSRAM*//*只要在L1放入代码就需要加载两遍*/
	DATAINL1DSRAM:>L1DSRAM
	CODEINL1DSRAM:>L1DSRAM
	CODEINIRAM:>IRAM
	DATAINIRAM:>IRAM
	CODEINDDR2:>DSP_PROG
	DATAINDDR2:>DSP_PROG
	DATAINSHARERAM:>L3_CBA_RAM
	.text:DSPF_sp_fftSPxSP:>IRAM
	.text:optasm:>IRAM
	/*	.text:DSPF_sp_fftSPxSP:>L1PSRAM
	.text:optasm:>L1PSRAM*/
	YCDATA:>YCMEM
	YXDATA:>YXMEM
	YKDATA:>YKMEM
/*	DATAINBANK5:>BANK5_*/
/*
     	TITARGETS :>IRAM
	{
		 ti.targets.rts6000.ae674(.text:xdc_runtime_System_doPrint__I)
	}
		SYSBIOSLIB :>IRAM
	{
		 sysbios.lib(.text:ti_sysbios_family_c64p_Exception_handler__I)
	}
*/
	/*SCADADATA :>L3_CBA_RAM
	{
		scada.obj(.text)
	//	fa.obj(.text)
	//	varm.obj(.text)
	//	data.obj(.text)
	}*/
   /* SYSBOIS :>L3_CBA_RAM
    {
       sysbios.lib (.text)
       syslink.obj (.text)
       ti.targets.rts6000.ae674 (.text)
    }*/
	CLOCKCPP :>IRAM
	{
		clock.obj(.text)
	}


}

  /*  SCADADATA
    {
		scada.obj(.text)
    }
	 LOAD = DSP_PROG,
	 RUN = L1PSRAM,
	 LOAD_START(_Scadadata_loadstart),
	 LOAD_END(_Scadadata_loadend),
	 RUN_START(_Scadadata_runstart)
	 SIZE(_flash_code_size)
*/
