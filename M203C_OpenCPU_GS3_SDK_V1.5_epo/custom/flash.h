#ifndef __FLASH_H__
#define __FLASH_H__


#define  MYPATH_ROOT    ((u8 *)"myroot")  
#define  SYSFILE 		"sys2_config.txt"

#define FILEPATH   "myroot\\sys2_config.txt\0"


extern u8 BootInfo;

void ReadFlashParamsNew(_system_setings *sysset);
void SaveFlashParamsNew(_system_setings *sysset);
#endif
