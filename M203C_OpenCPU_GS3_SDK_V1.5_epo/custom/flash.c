#include "string.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_fs.h"
#include "ql_error.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "sys.h"
#include "flash.h"
#include "uart.h"


u8 BootInfo=0;

void ReadFlashParamsNew(_system_setings *sysset)
{
	 s32 ret;
	 s32 handle = -1;
	 u8 filePath[100] = {0};
	 ret = Ql_FS_CheckDir(MYPATH_ROOT);
	 if(ret != QL_RET_OK)
	   	{
	   		ret  = Ql_FS_CreateDir(MYPATH_ROOT);
	   	}
	 Ql_sprintf(filePath,"%s\\%s\0",MYPATH_ROOT,SYSFILE);
	 handle = Ql_FS_Open(filePath,QL_FS_READ_WRITE);
	 if(handle<0)
	 	{
	 		
	 		handle=Ql_FS_Open(filePath,QL_FS_CREATE_ALWAYS);
			if(handle>0)
				{
					LoadDefualtCfg();
					Ql_FS_Write(handle, sysset, sizeof(_system_setings),NULL);
					Ql_FS_Close(handle);
					BootInfo=0;
				}
	 	}
	 else
	 	{
	 		 Ql_FS_Read(handle, sysset, sizeof(_system_setings),NULL);
			 Ql_FS_Close(handle); 
			 BootInfo=1;
	 	}
}

void SaveFlashParamsNew(_system_setings *sysset)
{
	 s32 handle = -1;
    	 s32 ret;
    	 u8 filePath[100] = {0};
	Ql_sprintf(filePath,"%s\\%s\0",MYPATH_ROOT,SYSFILE);
	handle = Ql_FS_Open(filePath,QL_FS_READ_WRITE );
	Ql_FS_Write(handle, sysset, sizeof(_system_setings),NULL);
	Ql_FS_Close(handle); 
}

