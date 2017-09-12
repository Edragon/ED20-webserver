

#ifdef __EXAMPLE_MULTITASK__

#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "sys.h"
#include "lcd.h"
#include "ql_uart.h"
#include "uart.h"
#include "ril_network.h"
#include "ql_gprs.h"
#include "ql_socket.h"
#include "custom_feature_def.h"
#include "ril.h"
#include "new_socket.h"
#include "pro.h"
#include "gps.h"
#include "ql_timer.h"
#include "ql_time.h"
#include "tiem.h"
#include "flash.h"





void proc_main_task(s32 taskId)
{

   u8 i;
   ST_MSG msg;
   power_drv_init();
   UartInit();
   SYS_Parameter_Init();
    while (1)
    {

       Ql_OS_GetMessage(&msg);
        switch(msg.message)
        {
            case MSG_ID_RIL_READY:
                Ql_RIL_Initialize();
                mprintf("RIL is ready\r\n");
            break;
            case MSG_ID_USER_START:
                break;

            default:
                break;
        
    	}
    }
}


void proc_subtask1(s32 TaskId)
{
	u8 setime=0;
	u8 res=0;
	u8 initflow=0,pregps=0;
	u8 errort=0,initres=0;
	if(BootInfo==0xFF)
		{
			Ql_Sleep(3000);
		}
	while(1)
	    {    
	         Ql_Sleep(20);
		if(BootInfo==0xFF)
			{
				if(SystemFlow==0)
					{
						M203C_init();
						M203C_gprs_init();
						res=M203C_tcpudp_conncet((u8)systemset.TCPorUDP,systemset.CenterIP,systemset.CenterPort,0);
						if(res==1)
							{
								mprintf("\r\n++++Reset System++++\r\n");
								SaveFlashParamsNew(&systemset);
								Ql_Sleep(300);
								Ql_Reset(0);
							}
						SystemFlow=1;
					}
				if(SystemFlow==1&&initflow==0)
					{
						 M203CIsBusy=1;
#ifdef _USE_LBS_
						  if(gpsx.useorno!=65)
						  	{
						  		Ql_Sleep(8000);
						  		GetLbsData();
						  	}
#endif
						  	CoreDataInitNew2(&res,0x4E02);
						 	setime=M203CSendDataNoAck(PROBUF,res,0);
							if(setime)goto  OVER;
							setime=M203CSendDataCheckOK(500);
							if(setime)goto  OVER;
						  M203CIsBusy=0;
						  initflow=1;
					}
				if(SystemFlow==1&&initflow==1)
					{
						M203CIsBusy=1;
						Ql_Sleep(1000);
						 if(gpsx.useorno!=65)
						 	{
						 		pregps=0;
						 		Ql_Sleep(3000);
								 GetLbsData();
								 M203C_EPO_Init(Jdbuf,Wdbuf);
								 M203C_Ch2Gprs();
						 	}
						  CoreDataInitNew2(&res,0x4E02);
						  setime=M203CSendDataNoAck(PROBUF,res,0);
						  if(setime)goto  OVER;
						   setime=M203CSendDataCheckOK(500);
						   if(setime)goto  OVER;
						   Ql_Sleep(1000);
						  M203CIsBusy=0;
						  initflow=2;
						  SystemFlow=2;
						 
					}
				
				if(SystemFlow==2&&initflow==2)
					{
						jishis++;
						if(gpsx.useorno==65&&pregps==0)
							{
								jishis=0;
								pregps=1;
								M203CIsBusy=1;
								Ql_Sleep(1000);
								 CoreDataInitNew2(&res,0x4E02);
								 setime=M203CSendDataNoAck(PROBUF,res,0);
								 if(setime)goto  OVER;
								setime=M203CSendDataCheckOK(500);
								 if(setime)goto  OVER;
								 Ql_Sleep(1000);
								  M203CIsBusy=0;
								
							}
						if(jishis==(systemset.HandInter*50))
							{
								  jishis=0;
								  M203CIsBusy=1;
								  Ql_Sleep(1000);
								  if(systemset.LbsOpen)
								  	{
										  if(gpsx.useorno!=65)
										  	{
										  		pregps=0;
												 GetLbsData();
										  	}
								  	}
								 CoreDataInitNew2(&res,0x4E02);
								 setime=M203CSendDataNoAck(PROBUF,res,0);
								 if(setime)goto  OVER;
								setime=M203CSendDataCheckOK(500);
								 if(setime)goto  OVER;
								 Ql_Sleep(1000);
								  M203CIsBusy=0;
							}
					}
OVER:
				if(setime)
					{
						mprintf("\r\n++++Reset System++++\r\n");
						SaveFlashParamsNew(&systemset);
						Ql_Sleep(300);
						Ql_Reset(0);
					}
				if(SystemFlow==1||SystemFlow==2)
					{
						Recive_OutData();
					}
			}
		else
			{
				if(initres==0)
					{
						initres=1;
						mprintf("\r\n模块未设置SN号码,请先设置SN号码!!\r\n");
						mprintf("设置命令: $setsn xxxxxxxxxx\r\n");
						mprintf("提示:SN号码贴在模块的标签上面\r\n");
					}
				errort++;
				if(errort>100)
					{
						errort=0;
						mprintf("\r\n模块未设置SN号码,请先设置SN号码!!\r\n");
						mprintf("设置命令: $setsn xxxxxxxxxx\r\n");
						mprintf("提示:SN号码贴在模块的标签上面\r\n");
					}
			}
	    }    
}


void proc_subtask2(s32 TaskId)
{

    u16 cent=0;
    while(1)
    {
    	Ql_Sleep(100);
	if(SystemFlow==2)
		{
			cent++;
			if(cent>=15)
				{
					if(!M203CIsBusy)
						{
							GetGsmCsq();
							GetModuVol();
							GetGpsData();
						}
					cent=0;
				}
		}
    }
	
}



void proc_subtask3(s32 TaskId)
{

     while(1)
    {
    	Ql_Sleep(1000);
	systemset.SysTime++;
	  if(gpsx.useorno!=65)
	  	{
	  		LED3_L;
	  	}
	  else
	  	{
	  		LED3_H;
	  	}
	
    }
}


void proc_subtask4(s32 TaskId)
{
    
     while(1)
    {
    	Ql_Sleep(300);
	LED2_H;
	Ql_Sleep(300);
	LED2_L;
	Ql_Sleep(300);
	LED2_H;
	Ql_Sleep(300);
	LED2_L;
	Ql_Sleep(1000);
    }
}



void proc_subtask5(s32 TaskId)
{
    u16 i;
    while(1)
    {
    	Ql_Sleep(10);
	if(SystemFlow==1||SystemFlow==2)
		{
			while(M203CIsBusy)
				{
					LED1_H;
					Ql_Sleep(50);
					LED1_L;
					Ql_Sleep(50);
				}
			LED1_H;
			Ql_Sleep(500);
			LED1_L;
			Ql_Sleep(500);
		}
    }
}



void proc_subtask6(s32 TaskId)
{
     while(1)
    {
    	Ql_Sleep(100);
    }
}



void proc_subtask7(s32 TaskId)
{
   
    while(1)
    {
    	Ql_Sleep(100);
	
    }
    
}


void proc_subtask8(s32 TaskId)
{

    while(1)
    {
    	Ql_Sleep(100);
    } 
}

#endif 


