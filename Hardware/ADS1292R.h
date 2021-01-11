#ifndef __ADS1292R__H
#define __ADS1292R__H

#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#define ADS1292R_CS_PORT                GPIO_PORT_P4
#define ADS1292R_CS_PIN                 GPIO_PIN1
#define ADS1292R_START_PORT             GPIO_PORT_P4
#define ADS1292R_START_PIN              GPIO_PIN2
#define ADS1292R_PWDN_PORT              GPIO_PORT_P4
#define ADS1292R_PWDN_PIN               GPIO_PIN3
#define ADS1292R_ADC_RDY_PORT           GPIO_PORT_P4
#define ADS1292R_ADC_RDY_PIN            GPIO_PIN4

#define ADS1292R_CS_H				GPIO_setOutputHighOnPin(ADS1292R_CS_PORT,ADS1292R_CS_PIN)
#define ADS1292R_CS_L				GPIO_setOutputLowOnPin(ADS1292R_CS_PORT,ADS1292R_CS_PIN)

#define ADS1292R_PWDN_H			GPIO_setOutputHighOnPin(ADS1292R_PWDN_PORT,ADS1292R_PWDN_PIN)
#define ADS1292R_PWDN_L			GPIO_setOutputLowOnPin(ADS1292R_PWDN_PORT,ADS1292R_PWDN_PIN)

#define ADS1292R_START_H		GPIO_setOutputHighOnPin(ADS1292R_START_PORT,ADS1292R_START_PIN)
#define ADS1292R_START_L		GPIO_setOutputLowOnPin(ADS1292R_START_PORT,ADS1292R_START_PIN)

/*ADS1292R�����*/
/*ϵͳ����*/
#define ADS1292R_WAKEUP	        	0X02	//�Ӵ���ģʽ����
#define ADS1292R_STANDBY	        0X04	//�������ģʽ
#define ADS1292R_ADSRESET        	0X06	//��λADS1292R
#define ADS1292R_START	        	0X08	//������ת��
#define ADS1292R_STOP	        		0X0A	//ֹͣת��
#define ADS1292R_OFFSETCAL				0X1A	//ͨ��ƫ��У׼
/*���ݶ�ȡ����*/
#define ADS1292R_RDATAC	        	0X10	//�������������ݶ�ȡģʽ,Ĭ��ʹ�ô�ģʽ
#define ADS1292R_SDATAC	        	0X11	//ֹͣ���������ݶ�ȡģʽ
#define ADS1292R_RDATA	        	0X12	//ͨ�������ȡ����;֧�ֶ��ֶ��ء�
/*�Ĵ�����ȡ����*/
#define	ADS1292R_RREG	        		0X20	//��ȡ001r rrrr 000n nnnn  ���ﶨ���ֻ�и߰�λ����8λ�ڷ�������ʱ����
#define ADS1292R_WREG	        		0X40	//д��010r rrrr 000n nnnn
/*	r rrrr=Ҫ����д�ļĴ�����ַ
		n nnnn=Ҫ����д������		*/
/*ADS1292R�ڲ��Ĵ�����ַ����	*/
#define ADS1292R_ID								0X00	//ID���ƼĴ���
#define ADS1292R_CONFIG1					0X01	//���üĴ���1
#define ADS1292R_CONFIG2					0X02	//���üĴ���2
#define ADS1292R_LOFF							0X03	//����������ƼĴ���
#define ADS1292R_CH1SET						0X04	//ͨ��1���üĴ���
#define ADS1292R_CH2SET						0X05	//ͨ��2���üĴ���
#define ADS1292R_RLD_SENS					0X06	//��������ѡ��Ĵ���
#define ADS1292R_LOFF_SENS				0X07	//����������ѡ��Ĵ���
#define ADS1292R_LOFF_STAT				0X08	//����������״̬�Ĵ���
#define	ADS1292R_RESP1						0X09	//���������ƼĴ���1
#define	ADS1292R_RESP2						0X0A	//���������ƼĴ���2
#define	ADS1292R_GPIO							0X0B	//GPIO���ƼĴ���

extern uint8_t finish_flag;
void ADS1292R_Init(void); //��ʼ��ADS1292R
void ADS1292R_CMD(uint8_t cmd);
uint8_t ADS1292R_REG(uint8_t cmd, uint8_t data);//��ADS1292R�ڲ��Ĵ������в���
void ADS1292R_PowerOnInit(void);//ADS1292R�ϵ縴λ
void ADS1292R_Work(void);				//ADS1292R��������
void ADS1292R_Halt(void);				//ADS1292Rֹͣ����
void ADS1292R_ReadData(void);//��ȡ72λ������
extern uint8_t ads1292r_data_buff[9];
#endif //__ADS1292R__H