/*
 * userlink.cmd
 *
 *  Created on: 2013-8-9
 *      Author: Administrator
 */


MEMORY
{
   HFPGA (RWX) : org = 0x62000002, len = 0x0000000e
   SFPGA (RWX) : org = 0x62000020, len = 0x00000200

}

SECTIONS
{
	codenmifxn:>IRAM
	codeFFT:>IRAM
	codeGPIO_setOutput:>IRAM
	codeHwiDynamic:>IRAM
	.text:DSPF_sp_fftSPxSP:>IRAM
	.text:optasm:>IRAM
   .ADBUDATA:>IRAM
   .FFTOUTDATA:>IRAM
   .ADBUFDATA:>IRAM
   .FpgaHostData:>HFPGA
   .FpgaSlaveData:>SFPGA
	SCADADATA :>L1PSRAM
	    {
			scada.obj(.text)
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
