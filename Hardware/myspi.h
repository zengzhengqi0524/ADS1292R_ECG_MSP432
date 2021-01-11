#ifndef __SPI__H
#define __SPI__H

#include <stdint.h>
#include <stdbool.h>
#include "driverlib.h"

void spi_init();
void spi_send(uint8_t TXData);
uint8_t SPI_RW(uint8_t TXData);
extern unsigned char Rx_data; 
#endif //__SPI__H