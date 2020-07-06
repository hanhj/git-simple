/*
 * at24c02.h
 *
 *  Created on: 2015-7-29
 *      Author: Administrator
 */

#ifndef AT24C02_H_
#define AT24C02_H_

extern uint16 FlashWriteData(uint8 SlotNum,uint8 StartAdd,uint8 *ExpDataAdd ,uint16 ExpDataLen);
extern uint16 FlashReadData(uint8 SlotNum,uint8 StartAdd,uint8 *ExpDataAdd ,uint16 ExpDataLen);
extern uint16 RW_FlashData(uint8 RW,uint8 SlotNum,uint8 StartAdd,uint8 *ExpDataAdd ,uint16 ExpDataLen);
#endif /* AT24C02_H_ */
