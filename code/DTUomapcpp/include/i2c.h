/*
 * i2c.h
 *
 *  Created on: 2015-7-28
 *      Author: Administrator
 */

#ifndef I2C_H_
#define I2C_H_

#define READ 1
#define WRITE 0

typedef struct I2CSLAVEADD{
	uint8 RW:1;
	uint8 SlaveAdd:3;
	uint8 rel:4;
}I2CSLAVEADD_T;
typedef union I2CSLAVEREG{
	I2CSLAVEADD_T bit;
	uint8 all;
}I2CSLAVEREG_U;
typedef struct FLASHREG{
	I2CSLAVEREG_U SlaveAdd;
	uint8 RegAdd;
}FLASHREG;

extern uint16 ReadI2cDevBypes(I2CSLAVEREG_U *SlaveMsg,uint8 RegAdd,int8 ReadDatatLength,uint8 *Data,int bus_id);
extern uint16 WriteI2cDevBypes(I2CSLAVEREG_U *SlaveMsg,uint8 RegAdd,int8 WriteDataLength,uint8 *wrptr,int bus_id);

#endif /* I2C_H_ */
