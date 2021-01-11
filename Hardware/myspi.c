#include "myspi.h"
unsigned char Rx_data; 
const eUSCI_SPI_MasterConfig spiMasterConfig =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
        12000000,                                   // SMCLK = DCO = 12MHZ
        2000000,                                    // SPICLK = 500khz
        EUSCI_B_SPI_MSB_FIRST,                     // MSB First
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW, // High polarity
        EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
};
//![Simple SPI Config]
void spi_init()
{
    //![Simple SPI Example]
    /* Selecting P1.5 P1.6 and P1.7 in SPI mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	  //修改片选线PI 0
     MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    /* Configuring SPI in 3wire master mode */
    SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);

    /* Enable SPI module */
    SPI_enableModule(EUSCI_B0_BASE);
	
	  //MAP_Interrupt_enableMaster(); 

    /* 发送中断使能 */
    //SPI_enableInterrupt(EUSCI_B0_BASE, EUSCI_SPI_TRANSMIT_INTERRUPT);	
	
    /* 中断接收使能 */
    SPI_enableInterrupt(EUSCI_B0_BASE, EUSCI_SPI_RECEIVE_INTERRUPT);
	
	  /* 开启中断 */
    Interrupt_enableInterrupt(INT_EUSCIB0);
		
		MAP_Interrupt_enableMaster(); 
	
	  /*中断睡眠模式*/
    //Interrupt_enableSleepOnIsrExit();
		
		
}
void spi_send(uint8_t TXData)
{
	  //保证寄存器为空
    while (!(SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_SPI_TRANSMIT_INTERRUPT)));
    //发送数据	  
    SPI_transmitData(EUSCI_B0_BASE, TXData);
}
uint8_t SPI_RW(uint8_t TXData)
{
	 
		//保证寄存器为空
	  while (!(SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_SPI_TRANSMIT_INTERRUPT)));
	
    SPI_transmitData(EUSCI_B0_BASE, TXData);
	

	  while (!(SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_SPI_TRANSMIT_INTERRUPT))); 	
    return Rx_data;
}

//******************************************************************************
//
//This is the EUSCI_B0 interrupt vector service routine.
//
//******************************************************************************
void EUSCIB0_IRQHandler(void)
{
    uint32_t status = SPI_getEnabledInterruptStatus(EUSCI_B0_BASE);
    //清除中断标志位
    SPI_clearInterruptFlag(EUSCI_B0_BASE,status);
//		if(status & EUSCI_SPI_TRANSMIT_INTERRUPT)
//		{
//			  TR_flag=1;
//		}
     if(status & EUSCI_SPI_RECEIVE_INTERRUPT)
    {	
			  while (!(SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_SPI_TRANSMIT_INTERRUPT)));
			  Rx_data=SPI_receiveData(EUSCI_B0_BASE);
    }

}