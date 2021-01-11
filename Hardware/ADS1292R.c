#include "ADS1292R.h"
#include "myspi.h"
#include "mydelay.h"
#include "stdint.h"
uint8_t ads1292r_data_buff[9] = {0};
uint8_t device_id ;
uint8_t finish_flag=0;
void ADS1292R_Init(void)
{	
  MAP_GPIO_setAsOutputPin(ADS1292R_START_PORT, ADS1292R_START_PIN);
  MAP_GPIO_setAsOutputPin(ADS1292R_PWDN_PORT, ADS1292R_PWDN_PIN);	
  MAP_GPIO_setAsOutputPin(ADS1292R_CS_PORT, ADS1292R_CS_PIN);
//	MAP_GPIO_setAsInputPinWithPullUpResistor(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
//	MAP_GPIO_clearInterruptFlag(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
//  MAP_GPIO_enableInterrupt(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
//	
//	MAP_Interrupt_enableInterrupt(INT_PORT4);

	ADS1292R_CS_H;
	ADS1292R_START_H;
	ADS1292R_PWDN_L;
	
}

uint8_t ADS1292R_SPI_RW(uint8_t data)
{
//	uint8_t rx_data = 0;
	//rx_data = 
	SPI_RW(data);
	return Rx_data;
}
/**ADS1292R上电复位 **/
void ADS1292R_PowerOnInit(void)
{
	//uint8_t device_id ;

	ADS1292R_START_H;
	ADS1292R_CS_H;
	ADS1292R_PWDN_L;//进入掉电模式
	ADS1292R_PWDN_H;
	delay_ms(200);//等待稳定	
	ADS1292R_PWDN_L;//发出复位脉冲
	delay_us(10);
	ADS1292R_PWDN_H;
	delay_ms(200);//等待稳定
	ADS1292R_START_L;	
	ADS1292R_CMD(ADS1292R_SDATAC);//发送停止连续读取数据命令
	
	delay_ms(10);
	while(device_id!=83)       //识别芯片型号，1292r为0x73
	{
		device_id=ADS1292R_REG(ADS1292R_RREG|ADS1292R_ID,0X00);
	  printf("错误ID%d",device_id);
		delay_ms(200);
		
	}
	printf("正确ID%d",device_id);
	
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_CONFIG2,    0XE0);	//使用内部参考电压
	delay_ms(10);//等待内部参考电压稳定
	
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_CONFIG1,    0X02);	//设置转换速率为500SPS
	//ADS1292R_REG(ADS1292R_WREG|ADS1292R_CONFIG1,    0X03);	//设置转换速率为1kSPS
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_LOFF,       0XF0);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_CH1SET,     0X00);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_CH2SET,     0x00);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_RLD_SENS,   0x30);
	//	ADS1292R_REG(ADS1292R_WREG|ADS1292R_RLD_SENS,   0x3C);	//使用通道2提取共模电压
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_LOFF_SENS,  0x3F);
	//  ADS1292R_REG(ADS1292R_WREG|LOFF_STAT,  0X00);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_RESP1,      0xDE);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_RESP2,      0x07);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_GPIO,       0x0C);
}

void ADS1292R_Work(void)
{
	//OLED_PrintfString(0,2,"ADS1292R Working");
	ADS1292R_CMD(ADS1292R_RDATAC);//回到连续读取数据模式，检测噪声数据
	ADS1292R_START_H;//启动转换
	//开启中断
	MAP_GPIO_setAsInputPinWithPullUpResistor(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
  GPIO_interruptEdgeSelect(ADS1292R_ADC_RDY_PORT,ADS1292R_ADC_RDY_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	MAP_GPIO_clearInterruptFlag(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
  MAP_GPIO_enableInterrupt(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
	MAP_Interrupt_enableInterrupt(INT_PORT4);

	
}
void ADS1292R_Halt(void)
{
	//OLED_PrintfString(0,2,"ADS1292R Halt");
	ADS1292R_START_L;//停止转换
	ADS1292R_CMD(ADS1292R_SDATAC);//发送停止连续读取数据命令
  //关闭中断线1	
	MAP_Interrupt_disableInterrupt(INT_PORT4);
}
//对ADS1292R写入指令
void ADS1292R_CMD(uint8_t cmd)
{
	ADS1292R_CS_L;
	delay_us(10);
	ADS1292R_SPI_RW(cmd);
	delay_us(10);
	ADS1292R_CS_H;
}
//对ADS1292R内部寄存器进行操作
uint8_t ADS1292R_REG(uint8_t cmd, uint8_t data)	//只读一个数据
{
	ADS1292R_CS_L;
	delay_us(10);
	ADS1292R_SPI_RW(cmd);	//读写指令
	ADS1292R_SPI_RW(0X00);	//需要写几个数据（n+1个）
	if((cmd&0x20)==0x20)	//判断是否为读寄存器指令
    ADS1292R_SPI_RW(0X00);	//返回寄存器值
	else
    ADS1292R_SPI_RW(data);	//写入数值
	delay_us(10);
	ADS1292R_CS_H;
	return Rx_data;
}


/*读取72位的数据1100+LOFF_STAT[4:0]+GPIO[1:0]+13个0+2CHx24位，共9字节*/
void ADS1292R_ReadData(void)
{
	
	int i,k;
  ADS1292R_CS_L;
	for(k=0;k<50;k++);
	ads1292r_data_buff[i]=SPI_RW(0X00);
	for(i=0;i<9;i++)
	{
	   ads1292r_data_buff[i]=SPI_RW(0X00);
	}
	for(k=0;k<50;k++);
  ADS1292R_CS_H;
	
//			MAP_UART_transmitData(EUSCI_A0_BASE, 0x03);
//		  MAP_UART_transmitData(EUSCI_A0_BASE, 0xfc); 
//	
//	
//			MAP_UART_transmitData(EUSCI_A0_BASE, 0x00);
//		  MAP_UART_transmitData(EUSCI_A0_BASE, ads1292r_data_buff[5]); 
//		  MAP_UART_transmitData(EUSCI_A0_BASE, ads1292r_data_buff[4]); 	
//		  MAP_UART_transmitData(EUSCI_A0_BASE, ads1292r_data_buff[3]); 
//	

//			MAP_UART_transmitData(EUSCI_A0_BASE, 0x00); 
//			MAP_UART_transmitData(EUSCI_A0_BASE, ads1292r_data_buff[8]); 
//		  MAP_UART_transmitData(EUSCI_A0_BASE, ads1292r_data_buff[7]); 	
//		  MAP_UART_transmitData(EUSCI_A0_BASE, ads1292r_data_buff[6]); 

//	
//		  MAP_UART_transmitData(EUSCI_A0_BASE, 0xfc);
//	    MAP_UART_transmitData(EUSCI_A0_BASE, 0x03);	

}

void PORT4_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P4, status);
    /* Toggling the output on the LED */
    if(status & GPIO_PIN4)
    {
        //MAP_GPIO_disableInterrupt(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
			  finish_flag=1;
        //MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }

}