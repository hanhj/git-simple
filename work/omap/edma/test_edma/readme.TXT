来自于edma3的例子程序.
1. 添加了外设驱动程序bsl
2. 将edma3的例子放入edma目录中
3. 将edma的main程序替换成do_emda,包括初始化,中断设置,调用原程序的edma3Test 
4. 替换了edma3Test程序为do_test,测试是否能成功
5. 添加了edma_interface.c文件,封装了接口,去掉原程序中不必要的文件.

经过试验:
1. 初始化时要enable PSC.
2. 要开全局中断,可以用sysbios的Hwi_enable
	相关寄存器:
		CSR			bit GIE
3. 要开cpuInt中断和相关Event中断.
	cpuInt 4-12可关联127个Event,
	相关寄存器:
		IER	:cpu interrupt enable 
		ICR	:cpu interrupt enable clear
		ISR	:cpu interrupt enable set 
		IFR	:cpu interrupt flag 
		EVTFLAGn	:event flag ,every have 32 event ,n:0-3 total 4
		EVTSETn		:event set 
		EVTCLRn		:event clear 
		EVTMASKn	:event mask ,1 disable event 
		MEVTFLAGn	:after mask enent flag 

		INTMUXn		:int map event ,every reg have 4 cpuInt ,from cpuInt4

		edma cc0 complete event is 8
		edma cc0 error event is 56

	edma cc相关寄存器:
		ER		:Event Flag,every bit is a channel ,32 channel 
		ECR		:event clear 
		ESR		:event set ,manual set event is set this register
		CER		:chained event flag 
		EER		:event enable flag 
		EECR	:event enable clear 
		EESR	:event enable set 
		IER		:event interrupt enable 
		IECR	:evnet interrupt enable clear 
		IESR	:event interrupt enable set 
		IPR		:interrupt Pending Flag 
		ICR		:interrupt Pending flag clear 

4.	初始化edma时,最好清ER和IPR
5.	每次传输前必须setParaRAM,即使每次都是一样的设置.
6.	如果传输没结束就退出,会导致寄存器错乱,导致下一次不能进edma中断,
	这一点在调试的时候经常出现,需要断电重启才能结束.
		


