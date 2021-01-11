#include "ECG_process.h"
#include "ADS1292R.h"
#include "myusart.h"
#define   ADS1292_channel     0    //如果是紫色板子 选1  绿色板子选0
unsigned char showdata_flag=10;
signed long  ADS1292R_ECG_BUFFER[2];
signed short ECGRawData[2];
signed short fir_filter_buffer[filter_size];
unsigned char ecg_fre;
short CoeffBuf_40Hz_LowPass[filter_size] = {             

24,	38,	43,	38,	23,	0,	-24,	-46,	-58,	-57,	
-39,	-8,	29,	66,	92,	96,	75,	29,	-31,	-94,	
-141,	-157,	-133,	-68,	25,	127,	209,	247,	
224,	137,	0,	-155,	-291,	-366,	-354,	-244,	
-55,	173,	385,	521,	535,	408,	157,	-165,
-484,	-712,	-778,	-646,	-326,	118,	585,	953,	
1112,	997,	603,	0,	-677,	-1261,	-1585,	-1529,	
-1059,	-240,	758,	1702,	2334,	2437,	1900,	750,	-819,	
-2483,	-3824,	-4418,	-3922,	-2153,	859,	4856,	9374,
13820,	17568,	20070,	20948,	20070,	17568,	13820,	9374,	
4856,	859,	-2153,	-3922,	-4418,	-3824,	-2483,	-819,	750,	
1900,	2437,	2334,	1702,	758,	-240,	-1059,	-1529,	-1585,	-1261,	
-677,	0,	603,	997,	1112,	953,	585,	118,	-326,	-646,	-778,	-712,
-484,	-165,	157,	408,	535,	521,	385,	173,	-55,	-244,	-354,	-366,	
-291,	-155,	0,	137,	224,	247,	209,	127,	25,	-68,	-133,	-157,	-141,
-94,	-31,	29,	75,	96,	92,	66,	29,	-8,	-39,	-57,	-58,	-46,	-24,	0,	23,	38,	43,	38,	24
};


void data_tran()
{
	unsigned char number=0;
	for(number=0;number<2;number++)
	{
		ADS1292R_ECG_BUFFER[number]=(signed long)ads1292r_data_buff[3+3*number];
		ADS1292R_ECG_BUFFER[number]=ADS1292R_ECG_BUFFER[number]<<8;
		ADS1292R_ECG_BUFFER[number]|=ads1292r_data_buff[3+3*number+1];
		ADS1292R_ECG_BUFFER[number]=ADS1292R_ECG_BUFFER[number]<<8;
		ADS1292R_ECG_BUFFER[number]|=ads1292r_data_buff[3+3*number+2];		 
	}
	
	ADS1292R_ECG_BUFFER[0]=ADS1292R_ECG_BUFFER[0]>>4;
	ADS1292R_ECG_BUFFER[1]=ADS1292R_ECG_BUFFER[1]>>4;
	ADS1292R_ECG_BUFFER[0] &= 0xffff;
	ADS1292R_ECG_BUFFER[1] &= 0xffff;
	
	ECGRawData[0]=(short)ADS1292R_ECG_BUFFER[0];
	ECGRawData[1]=(short)ADS1292R_ECG_BUFFER[1];
	//到这里没问题
	
}
void  ECG_IIR_filter()
{
static unsigned short temp;	
	signed long fir_result;
	static short pre_dc_sample,pre_sample,ECG_bufCur=filter_size-1;
	short temp1,temp2,ECGData;
	static short ECG_WorkingBuff[2*filter_size];

	if(showdata_flag==0)
	{
	print_ecg_32byte(ECGRawData[ADS1292_channel]); 
	}
//IIR滤波器  滤除直流  效果还行
	temp1=0.992*pre_dc_sample;
	pre_dc_sample=(ECGRawData[ADS1292_channel]-pre_sample)+temp1;
pre_sample=ECGRawData[ADS1292_channel];
		
//右移2位后效果也还可以 基本是没有啥失真
	temp2 =  pre_dc_sample>>2;
	ECGData=(short) temp2;
	
	if(showdata_flag==1)
	{
	print_ecg_32byte(ECGData); 
	}
	
	fir_result=ECG_FIR_filter(ECGData, CoeffBuf_40Hz_LowPass);
			if(showdata_flag==2)
	{
		if(ADS1292_channel)
		print_ecg_32byte(fir_result); 
		else
		print_ecg_32byte(-1*fir_result); 				
	}
	
	cal_rate(-1*fir_result);
	
}
signed long  ECG_FIR_filter(short WorkingBuff, short * CoeffBuf)
{
	unsigned char i;
	signed long result=0;
	static unsigned char pointer=0;
	unsigned char pointer_1=pointer;
	fir_filter_buffer[pointer]=WorkingBuff;
		for(i=0;i<filter_size;i++)
	{

		result+=CoeffBuf[i]*fir_filter_buffer[pointer_1];
	if(pointer_1==0)
		pointer_1=160;
	else
					pointer_1--;
		}
		if(pointer==160)
			pointer=0;
		else
			pointer++;
		
		return result; 
}
void  cal_rate(long  Data)
{
static short time_label;
	static short ecg_pre[5];
	static unsigned char time_pointer1,time_pointer2;
	static short begin_flag=4;
	static short time_pointer;
	static short max_m1;
	static short max_der=0;
static short temp1,temp2;
	static short der_buffer[1500];
	//第一步输入数据转16位
	short  input_data=Data/256;	 
	short  data_der=input_data - temp1;	  //计算导数
	if(data_der<0)                        //绝对值
		//data_der=data_der*-1;
temp1 = temp2;                        //新的数据移入缓存区
temp2 = input_data;
	
	//缓存前1500个数据的BUFFER
	if(time_label<2499)
	{
			time_label++;
			//前1000个数据不要
			if(time_label>=1000 )
				der_buffer[time_label-1000]=data_der;
	}
	//求1500个数据的最大值
	if(time_label==2499  && max_der==0)
	{		
			for(int i=0;i<1500;i++)
				{				  
						if(der_buffer[i]>max_der)
							max_der=der_buffer[i];
						
				}
						max_der=0.7*max_der;
	}
	//求出最大值之后
	if(max_der)
	{
			if(data_der>max_der  &&  begin_flag==4)
			{
				begin_flag=1;
				time_pointer1=0;
				time_pointer2=0;
			}
			if(begin_flag==1)
			{
					if(time_pointer1<40)
					{
					time_pointer1++;
						if(max_m1< data_der)
						{
						max_m1=data_der;
							time_pointer2=time_pointer1;
						}
					}
					else
					{
							begin_flag=2;
						time_pointer1=0;		
					}
				}
			
				
			if(begin_flag==2)
					{
						for(int j=0;j<5;j++)
						{
				if(ecg_pre[j]==0)
				{
										ecg_pre[j]=time_pointer;
									break;
								}										
						}
			time_pointer=40-time_pointer2;
						begin_flag=3;
					}
					
			if(begin_flag==3)
			{						
					if(time_pointer1<100)
					{
					time_pointer1++;
					}
					else
					{
							begin_flag=4;	
					}
			}
			if(begin_flag!=1)
			time_pointer++;
			if(ecg_pre[4]!=0)
			{
				short time_num=0;
					for(int l=0;l<5;l++)
					time_num+=ecg_pre[l];
					time_num/=5;
					printf("平均时间为%d",60000/(2*(time_num)));
				
					for(int l=0;l<5;l++)
					ecg_pre[l]=0;							
			}
		}

}