#include "ql_system.h"
#include "ql_gpio.h"
#include "ql_stdlib.h"
#include "ql_uart.h"
#include "sys.h"
#include "pro.h"
#include "uart.h"
#include "gps.h"
#include "tiem.h"
#include "new_socket.h"
#include "lcd.h"
#include "stdlib.h"



u8 PROBUF[PORALEN];
u16 ProNum=0;
u8 CurrtLen=0;

u8 TlvIsBusy=0;

tlv CenterIDTlV;
tlv ReCenterTlV;


_pro_setings Pro;


u16	Center2ModuleInfo=0; 
u8    Center2ModuleLen=0;

static unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};
static unsigned char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};


u16 SDI_CRC16(unsigned char *puchMsg, unsigned short usDataLen)
{
	unsigned char uchCRCHi = 0xFF ; /* ��ʼ�����ֽ�*/
	unsigned char uchCRCLo = 0xFF ; /* ��ʼ�����ֽ�*/
	unsigned int uIndex ; /*��CRC��*/
	while(usDataLen--)
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; /*����CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCLo << 8 | uchCRCHi) ;
}



u8 Dec2Hex(u8 num)
{
	u8 numbuf[2]={0};
	u8 hex;
	Ql_sprintf((char*)numbuf,"%02d",num);
	hex=strtol((const char*)numbuf,NULL,16);
	return hex;
}

u8 Hex2Dec(u8 num)
{
	u8 numbuf[2]={0};
	u8 dec;
	Ql_sprintf((char*)numbuf,"%02x",num);
	dec=strtol((const char*)numbuf,NULL,10);
	return dec;
}

// bvol, hvol
void ProInit(void)
{
	Pro.bvol=0xff;
	Pro.hvol=0xff;
}

// time 
void ProTimeInit(void)
{
	GetSysTime(&time);
	Pro.time[0]=Dec2Hex(time.year-2000);
	Pro.time[1]=Dec2Hex(time.month);
	Pro.time[2]=Dec2Hex(time.day);
	Pro.time[3]=Dec2Hex(time.hour);
	Pro.time[4]=Dec2Hex(time.minute);
	Pro.time[5]=Dec2Hex(time.second);
}
// pro data, sn number is 0123456
void ProDataInit(void)
{
	u8 i;
	for(i=0;i<6;i++)Pro.subsn[i]=i;
	Pro.paswd[0]=0x00;
	Pro.paswd[1]=0x00;
	Pro.ChargIn=0x00;
	Pro.ChargOut=0x00;
	Pro.tem=0;
	Pro.hum=0;
	Pro.bvol=0;
	
}


void Head_Init(u16 func)
{
	u8 i;
	ProNum++;
	PROBUF[0]=0x7E;
	PROBUF[1]=0x7E; // header start 0-1
	PROBUF[2]=(PROVERSION&0xff000000)>>24;
	PROBUF[3]=(PROVERSION&0xff0000)>>16;
	PROBUF[4]=(PROVERSION&0xff00)>>8;
	PROBUF[5]=PROVERSION&0xff; // version 2-5
	PROBUF[6]=(func&0xff00)>>8;
	PROBUF[7]=func&0xff; // function 6-7
	PROBUF[8]=(ProNum&0xff00)>>8;
	PROBUF[9]=ProNum&0xff; // packet times 8-9
	for(i=0;i<6;i++)PROBUF[10+i]=Pro.time[i]; // time 10-15
	PROBUF[16]=0x00;
	PROBUF[17]=0x00;
	CurrtLen=18;
}


// packet length 16, 17
void Length_Init(u8 prolen)
{
	PROBUF[16]=(prolen&0xff00)>>8;
	PROBUF[17]=prolen&0xff;
}


// start with TLV, type length and value
void Tlv_Init(u8 hl,tlv *tlvtable,u8 *prolen)
{
	u8 i;
	PROBUF[hl]=(tlvtable->type&0xff00)>>8;
	PROBUF[hl+1]=tlvtable->type&0xff;
	PROBUF[hl+2]=(tlvtable->length&0xff00)>>8;
	PROBUF[hl+3]=tlvtable->length&0xff;
	for(i=0;i<tlvtable->length;i++)PROBUF[hl+4+i]=tlvtable->value[i];
	*prolen=hl+4+i;
}

// sn number, 01234567
void ProIdInit(void)
{
	u8 i;
	tlv Id;
	Id.type=0x0001;
	Id.length=0x0006;
	for(i=0;i<Id.length;i++)Id.value[i]=Send_packets.packets_Send_snone[i];
	Tlv_Init(CurrtLen,&Id,&CurrtLen);
}

// --
void ProSigStInit(void)
{
	tlv ProSt;
	ProSt.type=0x0005;
	ProSt.length=0x0001;
	ProSt.value[0]=0x02;
	Tlv_Init(CurrtLen,&ProSt,&CurrtLen);
}

// --
void ProSigEndInit(void)
{
	tlv ProSt;
	ProSt.type=0x0006;
	ProSt.length=0x0001;
	ProSt.value[0]=0x03;
	Tlv_Init(CurrtLen,&ProSt,&CurrtLen);
}

// important part
void ProValuesInit(void)
{
	u8 i;
	tlv ProV;

	// sn number
	ProV.type=0xA095;
	ProV.length=0x0006;
	for(i=0;i<ProV.length;i++)ProV.value[i]=Pro.subsn[i];
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);

	// time
	ProV.type= 0x0004;
	ProV.length=0x0006;
	for(i=0;i<ProV.length;i++)ProV.value[i]=Pro.time[i];
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);

	// use either gps or gnss data 0x01 gps avaialble
	// longtitude or latitude should /1000 000
	if(Pro.gpssta==0x01)
		{
			ProV.type=0x5078;
			ProV.length=0x0008;
			for(i=0;i<4;i++)ProV.value[i]=my_core_data.jingdu[i+4];
			for(i=0;i<4;i++)ProV.value[i+4]=my_core_data.weidu[i+4];
			Tlv_Init(CurrtLen,&ProV,&CurrtLen);

			ProV.type=0x507B;
			ProV.length=0x0002;
			for(i=0;i<2;i++)ProV.value[i]=my_core_data.sudu[i+2];
			Tlv_Init(CurrtLen,&ProV,&CurrtLen);
			
			ProV.type=0x507D;
			ProV.length=0x0002;
			for(i=0;i<2;i++)ProV.value[i]=my_core_data.fangxiang[i+2];
			Tlv_Init(CurrtLen,&ProV,&CurrtLen);
		}
	else
		{
			ProV.type=0x5140;
			ProV.length=0x0008;
			for(i=0;i<4;i++)ProV.value[i]=my_core_data.jingdu[i+4];
			for(i=0;i<4;i++)ProV.value[i+4]=my_core_data.weidu[i+4];
			Tlv_Init(CurrtLen,&ProV,&CurrtLen);
		}
	ProV.type=0x4E21;
	ProV.length=0x02;
	ProV.value[0]=Pro.tem>>8;
	ProV.value[1]=Pro.tem&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0x4E22;
	ProV.length=0x02;
	ProV.value[0]=Pro.hum>>8;
	ProV.value[1]=Pro.hum&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0x51A6;
	ProV.length=0x02;
	ProV.value[0]=Pro.ChargIn>>8;
	ProV.value[1]=Pro.ChargIn&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0x51AC;
	ProV.length=0x02;
	ProV.value[0]=Pro.ChargOut>>8;
	ProV.value[1]=Pro.ChargOut&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0xA098;
	ProV.length=0x01;
	ProV.value[0]=Signal;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0x51A9;
	ProV.length=0x02;
	ProV.value[0]=Pro.bvol>>8;
	ProV.value[1]=Pro.bvol&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0x51AB;
	ProV.length=0x02;
	ProV.value[0]=Pro.hvol>>8;
	ProV.value[1]=Pro.hvol&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);

	ProV.type=0xB010;
	ProV.length=0x01;
	ProV.value[0]=Pro.hsta;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);

	ProV.type=0xB012;
	ProV.length=0x04;
	ProV.value[0]=(systemset.SysTime&0xff000000)>>24;
	ProV.value[1]=(systemset.SysTime&0xff0000)>>16;
	ProV.value[2]=(systemset.SysTime&0xff00)>>8;
	ProV.value[3]=systemset.SysTime&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	
	ProV.type=0xB0A2;
	ProV.length=0x01;
	ProV.value[0]=systemset.CallMode;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	
	ProV.type=0xB0A8;
	ProV.length=0x04;
	ProV.value[0]=(systemset.HandInter&0xff000000)>>24;
	ProV.value[1]=(systemset.HandInter&0xff0000)>>16;
	ProV.value[2]=(systemset.HandInter&0xff00)>>8;
	ProV.value[3]=systemset.HandInter&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	
}




void ProCrcInit(void)
{
	u16 crcd;
	tlv ProV;
	ProV.type=0x0007;
	ProV.length=0x02;
	crcd=SDI_CRC16(PROBUF,CurrtLen);
	ProV.value[0]=crcd>>8;
	ProV.value[1]=crcd&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
}

void ProPasswdInit(u8 len,u8 rellen)
{
	u8 mylen,i;
	mylen=Ql_strlen(systemset.Passwd);
	if(mylen>=rellen)mylen=rellen;
	for(i=0;i<mylen;i++)PROBUF[len+i]=PROBUF[len+i]^systemset.Passwd[i];
}


// entrance
void CoreDataInitNew(u8 *len,u16 funcid)
{
	u8 elen;
	u8 plen;
	ProTimeInit(); // get time
	ProDataInit(); // get data 
	Head_Init(funcid); // head
	ProIdInit(); // sn number
	ProSigStInit(); // sig, yet
	plen=CurrtLen;
	ProValuesInit();
	ProMpu6050ValuesInit();

	Tlv_Init(CurrtLen,&CenterIDTlV,&CurrtLen);
	
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}

void CoreDataInitNew2(u8 *len,u16 funcid)
{
	u8 elen;
	u8 plen;
	ProTimeInit();
	ProDataInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	ProValuesInit();
	ProMpu6050ValuesInit();

	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}

// MPU6050

void ProMpu6050ValuesInit(void)
{
	u8 i;
	u32 temp=0;
	tlv ProV;
	
	ProV.type=0x5551;
	ProV.length=0x0009;
	
	temp=(u32)(Acceleration[0]*1000);
	ProV.value[0]=(temp&0xff0000)>>16;
	ProV.value[1]=(temp&0xff00)>>8;
	ProV.value[2]=temp&0xff;

	temp=(u32)(Acceleration[1]*1000);
	ProV.value[3]=(temp&0xff0000)>>16;
	ProV.value[4]=(temp&0xff00)>>8;
	ProV.value[5]=temp&0xff;

	temp=(u32)(Acceleration[2]*1000);
	ProV.value[6]=(temp&0xff0000)>>16;
	ProV.value[7]=(temp&0xff00)>>8;
	ProV.value[8]=temp&0xff;

	Tlv_Init(CurrtLen,&ProV,&CurrtLen);

	ProV.type=0x5552;
	ProV.length=0x0009;
	
	temp=(u32)(AngularVelocity[0]*1000);
	ProV.value[0]=(temp&0xff0000)>>16;
	ProV.value[1]=(temp&0xff00)>>8;
	ProV.value[2]=temp&0xff;

	temp=(u32)(AngularVelocity[1]*1000);
	ProV.value[3]=(temp&0xff0000)>>16;
	ProV.value[4]=(temp&0xff00)>>8;
	ProV.value[5]=temp&0xff;

	temp=(u32)(AngularVelocity[2]*1000);
	ProV.value[6]=(temp&0xff0000)>>16;
	ProV.value[7]=(temp&0xff00)>>8;
	ProV.value[8]=temp&0xff;

	Tlv_Init(CurrtLen,&ProV,&CurrtLen);

	ProV.type=0x5553;
	ProV.length=0x0009;
	
	temp=(u32)(Angle[0]*1000);
	ProV.value[0]=(temp&0xff0000)>>16;
	ProV.value[1]=(temp&0xff00)>>8;
	ProV.value[2]=temp&0xff;

	temp=(u32)(Angle[1]*1000);
	ProV.value[3]=(temp&0xff0000)>>16;
	ProV.value[4]=(temp&0xff00)>>8;
	ProV.value[5]=temp&0xff;

	temp=(u32)(Angle[2]*1000);
	ProV.value[6]=(temp&0xff0000)>>16;
	ProV.value[7]=(temp&0xff00)>>8;
	ProV.value[8]=temp&0xff;

	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	
	
	
}

u8  JsVersion(u8 *buf)
{
	if(buf[2]==((PROVERSION&0xff000000)>>24)&&buf[3]==((PROVERSION&0xff0000)>>16)&&buf[4]==((PROVERSION&0xff00)>>8)&&buf[5]==(PROVERSION&0xff))return 0;
	else return 1;
}

u8 JsLength(u8 *buf,u8 len)
{
	if(buf[16]==((len&0xff00)>>8)&&buf[17]==(len&0xff))return 0;
	else return 1;
}

u8 JsCrcCode(u8 *buf,u8 len)
{
	u16 crcd;
	crcd=SDI_CRC16(buf,len-6);
	if(buf[len-2]==(crcd>>8)&&buf[len-1]==(crcd&0xff))return 0;
	else return 1;
}

u8 JsSysSerNum(u8 *buf)
{
	u8 len,i;
	if(buf[18]==0x00&&buf[19]==0x01)
		{
			len=(buf[20]<<8)|buf[21];
			for(i=0;i<len;i++)
				{
					if(Send_packets.packets_Send_snone[i]!=buf[22+i])return 1;
				}
			return 0;
		}
	return 1;
}


void Tlv_IdAnalys(u8 curlen,u8 *buf, u8 *Nextlen)
{
	u8 len;
	len=(buf[20]<<8)|buf[21];
	*Nextlen=curlen+4+len;
}

void Tlv_StartAnalys(u8 curlen,u8 *buf, u8 *Nextlen,u8 *tatal,u8 *currtnum)
{
	u8 len;
	if(buf[curlen]==0x00&&buf[curlen+1]==0x05)
		{
			len=(buf[curlen+2]<<8)|buf[curlen+3];
			if(len==0x03)
				{
					*tatal=buf[curlen+5];
					*currtnum=buf[curlen+6];
					*Nextlen=curlen+7;
				}
			if(len==0x01)
				{
					*tatal=0xff;
					*currtnum=0xff;
					*Nextlen=curlen+5;
				}
		}
}

void Tlv_PawssAnalys(u8 curlen,u8 *buf,u8 len)
{
	u8 clen;
	u8 mylen,i;
	clen=len-curlen-6-5;
	mylen=Ql_strlen(systemset.Passwd);
	if(mylen>=clen)mylen=clen;
	if(SystemDebug==6)mprintf("\r\n");
	if(SystemDebug==6)mprintf("Tlv_PawssAnalys:\r\n");
	for(i=0;i<mylen;i++)
		{
			buf[curlen+i]=buf[curlen+i]^systemset.Passwd[i];
			if(SystemDebug==5)mprintf("%02X",buf[curlen+i]);
		}
	if(SystemDebug==6)mprintf("\r\n");
}

u8 Tlv_BufAnalys(u8 len,u8 curlen,u8 *buf, u8 *Nextlen,tlv *tlvtable)
{
	u8 i;
	if(curlen>=len)return 1;
	else
		{
			tlvtable->type=buf[curlen]<<8|buf[curlen+1];
			tlvtable->length=buf[curlen+2]<<8|buf[curlen+3];
			if(tlvtable->length>VSLUELEN)return 2;
			for(i=0;i<tlvtable->length;i++)tlvtable->value[i]=buf[curlen+i+4];
			*Nextlen=curlen+4+i;
			return 0;
		}
}

void ProReadTimeValuesInit(void)
{
	u8 i;
	tlv ProV;
	ProV.type= 0x0004;
	ProV.length=0x0006;
	for(i=0;i<ProV.length;i++)ProV.value[i]=Pro.time[i];
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
}

void ProReadIDValuesInit(void)
{
	u8 i;
	tlv Id;
	Id.type=0x0001;
	Id.length=0x0006;
	for(i=0;i<Id.length;i++)Id.value[i]=Send_packets.packets_Send_snone[i];
	Tlv_Init(CurrtLen,&Id,&CurrtLen);
}

void ProReadSendTValuesInit(void)
{
	u8 i;
	tlv ProV;
	ProV.type=T_SENDT;
	ProV.length=0x0002;
	ProV.value[0]=systemset.HandInter>>8;
	ProV.value[1]=systemset.HandInter&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
}



void ReReadTimeDataInitNew(u8 *len,u16 funcid)
{
	u8 plen;
	u8 elen;
	ProTimeInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	ProReadTimeValuesInit();
	Tlv_Init(CurrtLen,&CenterIDTlV,&CurrtLen);
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}

void ReReadIdDataInitNew(u8 *len,u16 funcid)
{
	u8 plen;
	u8 elen;
	ProTimeInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	ProReadIDValuesInit();
	Tlv_Init(CurrtLen,&CenterIDTlV,&CurrtLen);
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}

void ReReadSendTDataInitNew(u8 *len,u16 funcid)
{
	u8 plen;
	u8 elen;
	ProTimeInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	ProReadSendTValuesInit();
	Tlv_Init(CurrtLen,&CenterIDTlV,&CurrtLen);
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}



void BufAnalys(u8 len,u8 *buf,u16 fcunid)
{
	u8 i;
	tlv ProA;
	TlvIsBusy=1;
	while(!Tlv_BufAnalys(len,Center2ModuleLen,buf,&Center2ModuleLen,&ProA))
		{
			Ql_Sleep(10);
			if(SystemDebug==2)
				{	
					mprintf("\r\n");
					mprintf("ProA.type=%04X\r\n",ProA.type);
					mprintf("ProA.length=%04X\r\n",ProA.length);
					mprintf("ProA.value:\r\n");
					for(i=0;i<ProA.length;i++)mprintf("%02X",ProA.value[i]);
					mprintf("\r\n");
				}
			if(ProA.type==T_CENTTER)
				{
					CenterIDTlV.type=ProA.type;
					CenterIDTlV.length=ProA.length;
					for(i=0;i<ProA.length;i++)CenterIDTlV.value[i]=ProA.value[i];
				}
			if(fcunid==WRYHSZ)
				{
					if(ProA.type==T_TIME)
						{
							StRTC_Set(Hex2Dec(ProA.value[0])+2000,Hex2Dec(ProA.value[1]),Hex2Dec(ProA.value[2]),Hex2Dec(ProA.value[3]),Hex2Dec(ProA.value[4]),Hex2Dec(ProA.value[5]));
							StRTC_Get();
							Center2ModuleInfo|=1<<8;                                   
							ReCenterTlV.type=ProA.type;
							ReCenterTlV.length=0x00;
						}
				}
			if(fcunid==RDYHSZ)
				{
					Center2ModuleInfo|=1<<13;						
				}
			if(fcunid==RDJBPZ)
				{
					if(ProA.type==T_MAINID)
						{
							Center2ModuleInfo|=1<<9;				//��ȡID     
						}
					
				}
			if(fcunid==WRJBPZ)
				{
					if(ProA.type==T_MAINID)
						{
							u8 tbuf[3]={0};
							Ql_memset(systemset.SN,0,sizeof(systemset.SN));
							for(i=0;i<ProA.length;i++)
								{
									Send_packets.packets_Send_snone[i]=ProA.value[i];
									Ql_sprintf(tbuf,"%02X",ProA.value[i]);
									Ql_strcat(systemset.SN,tbuf);
									SaveFlashParamsNew(&systemset);
								}
							Center2ModuleInfo|=1<<15;			//������ID��	
							ReCenterTlV.type=ProA.type;
							ReCenterTlV.length=0x00;
						}
				}
			if(fcunid==RDYXCS)
				{
					if(ProA.type==T_SENDT)
						{
							Center2ModuleInfo|=1<<12;				//��ȡ���ͼ���
						}
				}
			if(fcunid==WRYXCS)
				{
					if(ProA.type==T_SENDT)
						{
							systemset.HandInter=ProA.value[0]<<8|ProA.value[1];
							SaveFlashParamsNew(&systemset);
							Center2ModuleInfo|=1<<16;			//���÷���ʱ������
							ReCenterTlV.type=ProA.type;
							ReCenterTlV.length=0x00;
						}
				}
			if(fcunid==CKSSSJ)
				{
					Center2ModuleInfo|=1<<14;						//��ѯ��ǰ����
				}
			if(fcunid==OTACMD)
				{
					Center2ModuleInfo|=1<<10;						//Զ������
				}
			
		}
	TlvIsBusy=0;
}


void DataCore(u8 *databuf,u8 len)
{
	u8 res,cur;
	u16 funcid;
	if(databuf[0]==0x7E&&databuf[1]==0x7E)
		{
			res=JsLength(databuf,len);
			if(res)
				{
					Center2ModuleInfo|=1<<0;	       //���ݳ��ȴ���
					return;
				}
			res=JsCrcCode(databuf,len);
			if(res)
				{
					Center2ModuleInfo|=1<<1;		//crcУ��ʧ��
					return;
				}
			res=JsVersion(databuf);
			if(res)
				{
					Center2ModuleInfo|=1<<2;		//�汾�Ų�һ��
					return;
				}
			res=JsSysSerNum(databuf);
			if(res)
				{
					Center2ModuleInfo|=1<<3;		//��ID����
					return;
				}
			Center2ModuleLen=18;
			Tlv_IdAnalys(Center2ModuleLen,databuf,&Center2ModuleLen);
			Tlv_StartAnalys(Center2ModuleLen,databuf,&Center2ModuleLen,&res,&cur);
			Tlv_PawssAnalys(Center2ModuleLen,databuf,len);
			funcid=databuf[6]<<8|databuf[7];
			if(SystemDebug==6)mprintf("funcid=%04X\r\n",funcid);
			BufAnalys(len,databuf,funcid);
		}
}

void ReSetDataInitNew(u8 *len,u16 funcid,tlv *tlvp)
{
	u8 plen;
	u8 elen;
	ProTimeInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	Tlv_Init(CurrtLen,tlvp,&CurrtLen);
	Tlv_Init(CurrtLen,&CenterIDTlV,&CurrtLen);
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}

void Recive_OutData(void)
{
	u8 res;
	if(!TlvIsBusy)
		{
			if(Center2ModuleInfo&(1<<8))
				{
					Center2ModuleInfo&=~(1<<8);
					ReSetDataInitNew(&res,TYGNM,&ReCenterTlV);
					M203CSendDataNoAck(PROBUF,res,0);
					
				}
			if(Center2ModuleInfo&(1<<9))
				{
					Center2ModuleInfo&=~(1<<9);
					ReReadIdDataInitNew(&res,TYGNM);
					M203CSendDataNoAck(PROBUF,res,0);
					
				}
			if(Center2ModuleInfo&(1<<10))
				{
					Center2ModuleInfo&=~(1<<10);
					
				}
			if(Center2ModuleInfo&(1<<11))
				{
					Center2ModuleInfo&=~(1<<11);
					
				}
			if(Center2ModuleInfo&(1<<12))
				{
					Center2ModuleInfo&=~(1<<12);
					ReReadSendTDataInitNew(&res,TYGNM);
					M203CSendDataNoAck(PROBUF,res,0);
					
				}
			if(Center2ModuleInfo&(1<<13))
				{
					Center2ModuleInfo&=~(1<<13);
					ReReadTimeDataInitNew(&res,TYGNM);
					M203CSendDataNoAck(PROBUF,res,0);
					
				}
			if(Center2ModuleInfo&(1<<14))
				{
					Center2ModuleInfo&=~(1<<14);
					CoreDataInitNew(&res,TYGNM);
					M203CSendDataNoAck(PROBUF,res,0);
				}
			if(Center2ModuleInfo&(1<<15))
				{
					Center2ModuleInfo&=~(1<<15);
					ReSetDataInitNew(&res,TYGNM,&ReCenterTlV);
					M203CSendDataNoAck(PROBUF,res,0);
					
				}
			if(Center2ModuleInfo&(1<<16))
				{
					Center2ModuleInfo&=~(1<<16);
					ReSetDataInitNew(&res,TYGNM,&ReCenterTlV);
					M203CSendDataNoAck(PROBUF,res,0);
					
				}
		}
}




