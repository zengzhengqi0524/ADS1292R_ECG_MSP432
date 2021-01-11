#ifndef __usart_h
#define __usart_h
#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"
void usart_init(void);
void print_ecg_16byte(short  data);
void print_ecg_32byte(long  data);
#endif   //__usart_h