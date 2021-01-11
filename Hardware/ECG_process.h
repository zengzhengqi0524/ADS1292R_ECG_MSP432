#ifndef __ecf_prc_
#define __ecf_prc_

#define  filter_size  161
#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
extern unsigned char showdata_flag;
void data_tran();
void  ECG_IIR_filter();
signed long  ECG_FIR_filter(short WorkingBuff, short * CoeffBuf);
void  cal_rate(long  Data);
#endif //__ecf_prc_
