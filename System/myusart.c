#include  "myusart.h"
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        6,                                     // BRDIV = 78
        8,                                       // UCxBRF = 2
        0x20,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
};
//![Simple UART Config]

void usart_init(void)
{

    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Setting DCO to 12MHz */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    //![Simple UART Example]
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

    /* Enabling interrupts */
    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);

}

/* EUSCI A0 UART ISR - Echoes data back to PC host */
void EUSCIA0_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    UART_clearInterruptFlag(EUSCI_A0_BASE, status);
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        MAP_UART_transmitData(EUSCI_A0_BASE, 'x');//MAP_UART_receiveData(EUSCI_A0_BASE));
    }

}
int fputc(int ch, FILE *f)
{
   MAP_UART_transmitData(EUSCI_A0_BASE, (uint8_t) ch);
	 return ch;
}
void print_ecg_16byte(short  data)
{
		MAP_UART_transmitData(EUSCI_A0_BASE, 0x03);
		MAP_UART_transmitData(EUSCI_A0_BASE, 0xfc); 

		MAP_UART_transmitData(EUSCI_A0_BASE, (unsigned char)data); 
		MAP_UART_transmitData(EUSCI_A0_BASE, (unsigned char)(data>>8)); 

		MAP_UART_transmitData(EUSCI_A0_BASE, 0xfc);
		MAP_UART_transmitData(EUSCI_A0_BASE, 0x03);	

}
void print_ecg_32byte(long  data)
{
		MAP_UART_transmitData(EUSCI_A0_BASE, 0x03);
		MAP_UART_transmitData(EUSCI_A0_BASE, 0xfc); 

		MAP_UART_transmitData(EUSCI_A0_BASE, (unsigned char)data); 
		MAP_UART_transmitData(EUSCI_A0_BASE, (unsigned char)(data>>8)); 
		MAP_UART_transmitData(EUSCI_A0_BASE, (unsigned char)(data>>16)); 
		MAP_UART_transmitData(EUSCI_A0_BASE, (unsigned char)(data>>24)); 

		MAP_UART_transmitData(EUSCI_A0_BASE, 0xfc);
		MAP_UART_transmitData(EUSCI_A0_BASE, 0x03);	

}
// ±÷”∑¢ÀÕ
/*
		while(1)
		{
				printf("ACLK%d\n",CS_getACLK());
				printf(	"dcof%d\n",CS_getDCOFrequency());
			  printf( "BCLK%d\n",CS_getBCLK());
			  printf( "HSMCLK%d\n",CS_getHSMCLK());
				printf(  "MCLK%d\n",CS_getMCLK());
				printf(  "SMCLK%d\n",	CS_getSMCLK());
		}
*/