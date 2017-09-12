#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "sys.h"
#include "ql_socket.h"
#include "tiem.h"
#include "uart.h"
#include "pro.h"
#include "new_socket.h"
#include "lcd.h"
#include "ql_adc.h"
#include "flash.h"


const u8 *modetbl[2]={"TCP","UDP"};

Protocol_Send_packets Send_packets;

_system_setings	systemset;

volatile u8 SystemDebug=2;
volatile u8 SystemFlow=0;

u8 LbsOpen=0;

u32 jishis=0;


Enum_PinName LED1=PINNAME_RI;
Enum_PinName LED2=PINNAME_DCD;
Enum_PinName LED3=PINNAME_CTS;

void power_drv_init(void)
{
	 Ql_GPIO_Init(LED1, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(LED2, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(LED3, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);

	 LED1_L;
	 LED2_L;
	 LED3_L;
}


void Delayus(u32 data)
{
	u16 i;
	while(data--)
	{
		for(i=0;i<3;i++){}
	}
}

void InternetsetSN_AnalysisFor808(u8 *snuf)
{
	u8 k;
	u8 snlength,i,j=0;
	u8 duanbuf[3]={0};
	snlength=Ql_strlen((const char*)snuf);
	i=snlength/2;
	j=snlength%2;
	if(i>6)return;
	if(j==0)
		{
			for(k=1;k<(i+1);k++)
				{
					mid(duanbuf,snuf,2,snlength-2*k);
					Send_packets.packets_Send_snone[6-k]=strtol(duanbuf,NULL,16);
				}
		}
	if(j==1)
		{
			for(k=1;k<(i+1);k++)
				{
					mid(duanbuf,snuf,2,snlength-2*k);
					Send_packets.packets_Send_snone[6-k]=strtol(duanbuf,NULL,16);
				}
			mid(duanbuf,snuf,1,snlength-2*k);
			Send_packets.packets_Send_snone[6-k]=strtol(duanbuf,NULL,16);
		}
}



void  LoadDefualtCfg(void)
{
	systemset.HandInter=10; 
	systemset.CallMode=1; 
	systemset.ACCoffDelay=20;
	systemset.Interval=60;
	systemset.Length=500;
	systemset.SpedLmt[0]=255;
	systemset.SpedLmt[1]=255;
	systemset.TCPorUDP=0;
	systemset.LbsOpen=1;
	systemset.SysTime=0;
	systemset.saveflag=0x0A;
	Ql_memset(systemset.Passwd, 0, sizeof(systemset.Passwd));
	Ql_sprintf((char*)systemset.CenterIP,"101.201.114.211");
	Ql_sprintf((char*)systemset.CenterPort,"8100");
	Ql_sprintf((char*)systemset.Centerapn,"cmnet");
	Ql_sprintf((char*)systemset.SN,"FFFFFFFFFFFF");
	Ql_sprintf((char*)systemset.ProductID,"000000");
}


void ShowSysInfo(void)
{
	GetSysTime(&time);
	mprintf("GSM_GPS_V1.4.1\r\n");
	mprintf("ProductNumber:%s\r\n",systemset.ProductID);
	mprintf("CenterIP:%s\r\n",systemset.CenterIP);
	mprintf("CenterPort:%s\r\n",systemset.CenterPort);
	mprintf("TCP/IP:%s\r\n",modetbl[systemset.TCPorUDP]);
	mprintf("SN:%s\r\n",systemset.SN);
	mprintf("Passwd:%s\r\n",systemset.Passwd);
	mprintf("HandInter:%d\r\n",systemset.HandInter);
	mprintf("LbsSta:%d\r\n",systemset.LbsOpen);
	mprintf("+DataTime: %04d-%02d-%02d %02d:%02d:%02d\r\n",time.year, time.month, time.day, time.hour, time.minute, time.second);

}


void IsSnHavaSet(void)
{
	u8 *p=NULL;
	p=(u8*)strstr((const char *)systemset.SN,"FFFFFFFF");
	if(p!=NULL)
		{
			BootInfo=0x02;
		}
	else
		{
			BootInfo=0xFF;
		}
}

void SYS_Parameter_Init(void)
{
	ReadFlashParamsNew(&systemset);
	ShowSysInfo();
	IsSnHavaSet();
	InternetsetSN_AnalysisFor808(systemset.SN);
	
}

