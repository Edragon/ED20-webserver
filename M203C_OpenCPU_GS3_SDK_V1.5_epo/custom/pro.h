#ifndef _PRO_H_
#define _PRO_H_


#define PORALEN	200

#define VSLUELEN	20

#define PROVERSION	0x23235554



#define T_TIME		0x0004
#define UTC_TIME	0x0014
#define T_CENTTER	0x0008
#define T_MAINID	0xA264
#define T_MAINIP	0xA265
#define T_MAINPOT	0xA266
#define T_SENDT	0xA268

#define RDJBPZ	0x4E0A
#define WRJBPZ	0x4E09
#define RDYXCS	0x4E0C
#define WRYXCS	0x4E0B
#define WRYHSZ	0x4E11
#define RDYHSZ	0x4E13
#define CKSSSJ	0x4E06
#define TYGNM		0x4E15
#define OTACMD	0x4E14

typedef struct 
{
	u16 type;
	u16 length;
	u8 value[VSLUELEN];
}tlv;


typedef struct 
{
	u8 sn[6];
	u8 subsn[6];
	u8 time[6];
	u8 paswd[2];
	u16 tem;
	u16 hum;
	u16 ChargIn;
	u16 ChargOut;
	u16 hvol;
	u16 bvol;
	u8 hsta;
	u8 gpssta;
	u8 csta;
	u8 bleve;
}_pro_setings;

extern _pro_setings Pro;

extern u8 PROBUF[PORALEN];


extern tlv CenterIDTlV;
extern tlv ReCenterTlV;


void ReSetDataInitNew(u8 *len,u16 funcid,tlv *tlvp);
void CoreDataInitNew2(u8 *len,u16 funcid);

void ProMpu6050ValuesInit(void);


#endif

