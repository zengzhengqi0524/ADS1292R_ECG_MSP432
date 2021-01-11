/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "myusart.h"
#include "mysysclk.h"
#include "myspi.h"
#include "mydelay.h"
#include "ADS1292R.h"
#include "ECG_process.h"
#include "key.h"
int main(void)
{
	  int x;
    WDT_A_holdTimer();//关闭看门狗
		usart_init();//串口初始化 115200
    clk_init();//时钟初始化 配置为48M
	  spi_init();//SPI初始化
	  key_init(); //按键初始化
		delay_init(SYSTEM_CLOCK);//延时
	  MAP_Interrupt_enableMaster(); //开启总中断
    ADS1292R_Init();
    ADS1292R_PowerOnInit();
    ADS1292R_Work();
	  while(1)
	  {
			      while(!finish_flag);
			      MAP_GPIO_disableInterrupt(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
						ADS1292R_ReadData();
		        data_tran();
            ECG_IIR_filter();
						finish_flag=0;		
	          MAP_GPIO_clearInterruptFlag(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);
            MAP_GPIO_enableInterrupt(ADS1292R_ADC_RDY_PORT, ADS1292R_ADC_RDY_PIN);			
		}

}


