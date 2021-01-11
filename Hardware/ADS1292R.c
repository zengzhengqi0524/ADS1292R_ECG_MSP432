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
/**ADS1292R�ϵ縴λ **/
void ADS1292R_PowerOnInit(void)
{
	//uint8_t device_id ;

	ADS1292R_START_H;
	ADS1292R_CS_H;
	ADS1292R_PWDN_L;//�������ģʽ
	ADS1292R_PWDN_H;
	delay_ms(200);//�ȴ��ȶ�	
	ADS1292R_PWDN_L;//������λ����
	delay_us(10);
	ADS1292R_PWDN_H;
	delay_ms(200);//�ȴ��ȶ�
	ADS1292R_START_L;	
	ADS1292R_CMD(ADS1292R_SDATAC);//����ֹͣ������ȡ��������
	
	delay_ms(10);
	while(device_id!=83)       //ʶ��оƬ�ͺţ�1292rΪ0x73
	{
		device_id=ADS1292R_REG(ADS1292R_RREG|ADS1292R_ID,0X00);
	  printf("����ID%d",device_id);
		delay_ms(200);
		
	}
	printf("��ȷID%d",device_id);
	
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_CONFIG2,    0XE0);	//ʹ���ڲ��ο���ѹ
	delay_ms(10);//�ȴ��ڲ��ο���ѹ�ȶ�
	
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_CONFIG1,    0X02);	//����ת������Ϊ500SPS
	//ADS1292R_REG(ADS1292R_WREG|ADS1292R_CONFIG1,    0X03);	//����ת������Ϊ1kSPS
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_LOFF,       0XF0);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_CH1SET,     0X00);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_CH2SET,     0x00);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_RLD_SENS,   0x30);
	//	ADS1292R_REG(ADS1292R_WREG|ADS1292R_RLD_SENS,   0x3C);	//ʹ��ͨ��2��ȡ��ģ��ѹ
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_LOFF_SENS,  0x3F);
	//  ADS1292R_REG(ADS1292R_WREG|LOFF_STAT,  0X00);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_RESP1,      0xDE);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_RESP2,      0x07);
	ADS1292R_REG(ADS1292R_WREG|ADS1292R_GPIO,       0x0C);
}

void ADS1292R_Work(void)
{
	//OLED_PrintfString(0,2,"ADS1292R Working");
	ADS1292R_CMD(ADS1292R_RDATAC);//�ص�������ȡ����ģʽ�������������
	ADS1292R_START_H;//����ת��
	//�����ж�
	MAP_GPIO_setAsInputPinWithPullUpResistor(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
  GPIO_interruptEdgeSelect(ADS1292R_ADC_RDY_PORT,ADS1292R_ADC_RDY_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	MAP_GPIO_clearInterruptFlag(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
  MAP_GPIO_enableInterrupt(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
	MAP_Interrupt_enableInterrupt(INT_PORT4);

	
}
void ADS1292R_Halt(void)
{
	//OLED_PrintfString(0,2,"ADS1292R Halt");
	ADS1292R_START_L;//ֹͣת��
	ADS1292R_CMD(ADS1292R_SDATAC);//����ֹͣ������ȡ��������
  //�ر��ж���1	
	MAP_Interrupt_disableInterrupt(INT_PORT4);
}
//��ADS1292Rд��ָ��
void ADS1292R_CMD(uint8_t cmd)
{
	ADS1292R_CS_L;
	delay_us(10);
	ADS1292R_SPI_RW(cmd);
	delay_us(10);
	ADS1292R_CS_H;
}
//��ADS1292R�ڲ��Ĵ������в���
uint8_t ADS1292R_REG(uint8_t cmd, uint8_t data)	//ֻ��һ������
{
	ADS1292R_CS_L;
	delay_us(10);
	ADS1292R_SPI_RW(cmd);	//��дָ��
	ADS1292R_SPI_RW(0X00);	//��Ҫд�������ݣ�n+1����
	if((cmd&0x20)==0x20)	//�ж��Ƿ�Ϊ���Ĵ���ָ��
    ADS1292R_SPI_RW(0X00);	//���ؼĴ���ֵ
	else
    ADS1292R_SPI_RW(data);	//д����ֵ
	delay_us(10);
	ADS1292R_CS_H;
	return Rx_data;
}


/*��ȡ72λ������1100+LOFF_STAT[4:0]+GPIO[1:0]+13��0+2CHx24λ����9�ֽ�*/
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