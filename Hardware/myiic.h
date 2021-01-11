#ifndef __IIC_H__
#define __IIC_H__
/**
	***************************
	* msp432										
	***************************
	*/
#include "driverlib.h"
#include "delay.h"

/* 4 iic interfaces in total */
static uint16_t iic_list[4][4];
static uint8_t iic_num = 0;

/*------------ SDA I/O direction ------------*/
/* Sets SDA In mode */
#define SDA_IN()  (GPIO_setAsInputPin(iic_list[iic_num][2], iic_list[iic_num][3]))
/* Sets SDA Out mode */
#define SDA_OUT() (GPIO_setAsOutputPin(iic_list[iic_num][2], iic_list[iic_num][3]))
	 
/*------------ I/O operations ------------*/

/* SCL output */
#define WRITE_SCL(x)   ((x) ? GPIO_setOutputHighOnPin(iic_list[iic_num][0], iic_list[iic_num][1]) \
													  : GPIO_setOutputLowOnPin(iic_list[iic_num][0], iic_list[iic_num][1]))
/* SDA output */
#define WRITE_SDA(x)   ((x) ? GPIO_setOutputHighOnPin(iic_list[iic_num][2], iic_list[iic_num][3]) \
													  : GPIO_setOutputLowOnPin(iic_list[iic_num][2], iic_list[iic_num][3]))
/* SDA input */
#define READ_SDA()  	 (GPIO_getInputPinValue(iic_list[iic_num][2], iic_list[iic_num][3]))



#endif /* msp432 */



/**
	***************************
	* Public codes										
	***************************
	*/
	
/*------------ definitions ------------*/
#ifndef HIGH
#define HIGH 1u
#endif

#ifndef LOW
#define LOW	 0u
#endif

#ifndef BOOLEAN
#define BOOLEAN unsigned char
#endif

#ifndef TRUE
#define TRUE  1u
#endif

#ifndef FALSE
#define FALSE 0u
#endif

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned short int
#endif

/*------------ functions ------------*/
#ifdef __STM32F407ZGT6__
void iicInit();
#endif

#ifdef __MSP432P401R__
void iicInit(uint16_t scl_port, uint16_t scl_pin, uint16_t sda_port, uint16_t sda_pin);		
#define IIC_SHIFT() iic_num++
#endif

void iicStart(void);
void iicStop(void);
void iicSendByte(u8 txd);
void _iicSendAck(void);
void _iicSendNAck(void);
u8 iicReadByte(BOOLEAN is_ack);
BOOLEAN iicWaitAck(void);


#endif /* __IIC_H__ */ 

