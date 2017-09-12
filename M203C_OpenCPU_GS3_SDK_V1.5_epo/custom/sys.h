#ifndef __SYS_H__
#define __SYS_H__


#include "ql_gpio.h"
#include "ql_adc.h"

//#define _USE_LED_MIS_


#ifdef _USE_LED_MIS_

#define LED1_L	 Ql_GPIO_SetLevel(LED1, PINLEVEL_LOW);
#define LED1_H	 Ql_GPIO_SetLevel(LED1, PINLEVEL_HIGH);
#define LED2_L	 Ql_GPIO_SetLevel(LED2, PINLEVEL_LOW);
#define LED2_H	 Ql_GPIO_SetLevel(LED2, PINLEVEL_HIGH);
#define LED3_L	 Ql_GPIO_SetLevel(LED3, PINLEVEL_LOW);
#define LED3_H	 Ql_GPIO_SetLevel(LED3, PINLEVEL_HIGH);

#else

#define LED1_H	 Ql_GPIO_SetLevel(LED1, PINLEVEL_LOW);
#define LED1_L	 Ql_GPIO_SetLevel(LED1, PINLEVEL_HIGH);
#define LED2_H	 Ql_GPIO_SetLevel(LED2, PINLEVEL_LOW);
#define LED2_L	 Ql_GPIO_SetLevel(LED2, PINLEVEL_HIGH);
#define LED3_H	 Ql_GPIO_SetLevel(LED3, PINLEVEL_LOW);
#define LED3_L	 Ql_GPIO_SetLevel(LED3, PINLEVEL_HIGH);


#endif


typedef struct 
{
	u32 HandInter;			//设置握手时间间隔
	u8 CallMode;				//设置自动上传时间间隔
	u32 Interval;
	u32 Length;
	u32 SpedLmt[2]; 			//设置超速
	u32 Spedtime;			//真正延迟时间
	u32 SysTime;			//系统总时间
	u16 ACCoffDelay;               //ACC断开后的延迟时间
	u8 CenterIP[16];			//IP地址			
	u8 CenterPort[6];			//端口号
	u8 SN[16];				//SN
	u8 TCPorUDP;			//设置：1是TCP，0是UDP
	u8 Passwd[20];			//设置密码
	u8 ProductID[20];			//产品ID
	u8 Centerapn[20];
	u8 LbsOpen;
	u8 saveflag;				//保存标志
}_system_setings;


extern _system_setings	systemset;

extern const u8 *modetbl[2];

extern Enum_PinName LED1;
extern Enum_PinName LED2;
extern Enum_PinName LED3;


typedef struct
{
	u8 packets_Send_sn[16];
	u8 packets_Send_snone[6];
	u8 num[2];
	u8 id[2];
}Protocol_Send_packets;

extern Protocol_Send_packets Send_packets;


extern volatile u8 SystemDebug;
extern volatile u8 SystemFlow;

extern u32 jishis;

void GpsDelay(u16 delaytime);
void LbsDelay(u16 delaytime);
void power_drv_init(void);
void LcdPin_init(void);
void Delayus(u32 data);
void SysSleep(void);
void  LoadDefualtCfg(void);
void SYS_Parameter_Init(void);
void ShowSysInfo(void);
void InternetsetSN_Analysis(u8 *snuf,u8 len);
void Callback_ChageInit(Enum_ADCPin adcPin, u32 adcValue, void *customParam);
void SysWakeUp(void);
void ADC_Program(void);
void DrvBell_Ring_R(u8 num);
#endif

