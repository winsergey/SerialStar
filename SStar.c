#include "SStar.h"
#include <string.h>

const char * const SStarAT_WR_Str = "WR";
const char * const SStarAT_CN_Str = "CN";
const char * const SStarAT_AC_Str = "AC";
const char * const SStarAT_DC_Str = "DC";
const char * const SStarAT_RE_Str = "RE";
const char * const SStarAT_FR_Str = "FR";
const char * const SStarAT_VR_Str = "VR";
const char * const SStarAT_HV_Str = "HV";
const char * const SStarAT_AP_Str = "AP";
const char * const SStarAT_DM_Str = "DM";
const char * const SStarAT_KY_Str = "KY";
const char * const SStarAT_IO_Str = "IO";
const char * const SStarAT_AI_Str = "AI";
const char * const SStarAT_MY_Str = "MY";
const char * const SStarAT_ID_Str = "ID";
const char * const SStarAT_TX_Str = "TX";
const char * const SStarAT_UR_Str = "UR";
const char * const SStarAT_DR_Str = "DR";
const char * const SStarAT_NM_Str = "NM";
const char * const SStarAT_CH_Str = "CH";
const char * const SStarAT_CF_Str = "CF";
const char * const SStarAT_RB_Str = "RB";
const char * const SStarAT_PL_Str = "PL";
const char * const SStarAT_SM_Str = "SM";
const char * const SStarAT_SP_Str = "SP";
const char * const SStarAT_ST_Str = "ST";
const char * const SStarAT_SO_Str = "SO";
const char * const SStarAT_DS_Str = "DS";

extern void uart_write(void * data, int size);

static uint8_t checkFrame(const SStarAPIFrame * frame);
static inline uint8_t msb(const uint16_t x);
static inline uint16_t lsb(const uint16_t x);
static uint8_t SStarAPI_calcCheckSum(const SStarAPIFrame * frame);
static void SStarAPI_completeFrame(SStarAPIFrame * frame, uint16_t size, SStarFrameType type);



static uint8_t checkFrame(const SStarAPIFrame * frame)
{
	if (frame==0)
		return 0;
	if (frame->startData!=SStarAPIFrameStartByte)
		return 0;
	return 1;
}

static inline uint8_t msb(const uint16_t x)
{
	return x>>8;
}

static inline uint16_t lsb(const uint16_t x)
{
	return x&0xFF;
}

static uint8_t SStarAPI_calcCheckSum(const SStarAPIFrame * frame)
{
	uint16_t size = SStarAPI_frameLen(frame);
	if (size==0) return 0;
	uint8_t cs = frame->type;
	for (int i = 1; i < size; ++i)
	{
		cs += frame->data[i-1];
	}
	cs = 0xFF - cs;
	return cs;
}

static void SStarAPI_completeFrame(SStarAPIFrame * frame, uint16_t size, SStarFrameType type)
{
	frame->startData = SStarAPIFrameStartByte;
	frame->lenLsb = lsb(size);
	frame->lenMsb = msb(size);
	frame->type = type;
	uint8_t cs = SStarAPI_calcCheckSum(frame);
	frame->data[size-1] = cs;
}


SStarAT_commands SStarATcmd(const char * ATcmdStr)
{
	if (ATcmdStr[0]=='L')
	{
		if (ATcmdStr[1]>='0' && ATcmdStr[1]<='9')
			return SStarAT_L0+(ATcmdStr[1]-'0');
	}
	if (ATcmdStr[0]=='R')
	{
		if (ATcmdStr[1]>='0' && ATcmdStr[1]<='9')
			return SStarAT_R0+(ATcmdStr[1]-'0');
	}
	if (ATcmdStr[0]=='B')
	{
		if (ATcmdStr[1]>='0' && ATcmdStr[1]<='9')
			return SStarAT_B0+(ATcmdStr[1]-'0');
	}
	if (ATcmdStr[0]=='W' && ATcmdStr[1]=='R')
		return SStarAT_WR;
	if (ATcmdStr[0]=='C')
	{
		if (ATcmdStr[1]=='N')
			return SStarAT_CN;
		else if(ATcmdStr[1]=='H')
			return SStarAT_CH;
		else if(ATcmdStr[1]=='F')
			return SStarAT_CF;
	}
	if (ATcmdStr[0]=='A')
	{
		if (ATcmdStr[1]=='C')
			return SStarAT_AC;
		else if(ATcmdStr[1]=='P')
			return SStarAT_AP;
		else if(ATcmdStr[1]=='I')
			return SStarAT_AI;
	}
	if (ATcmdStr[0]=='D')
	{
		if (ATcmdStr[1]=='C')
			return SStarAT_DC;
		else if(ATcmdStr[1]=='M')
			return SStarAT_DM;
		else if(ATcmdStr[1]=='R')
			return SStarAT_DR;
		else if(ATcmdStr[1]=='S')
			return SStarAT_DS;
	}
	if (ATcmdStr[0]=='R')
	{
		if (ATcmdStr[1]=='B')
			return SStarAT_RB;
		else if(ATcmdStr[1]=='E')
			return SStarAT_RE;
	}
	if (ATcmdStr[0]=='F' && ATcmdStr[1]=='R')
		return SStarAT_FR;
	if (ATcmdStr[0]=='V' && ATcmdStr[1]=='R')
		return SStarAT_VR;
	if (ATcmdStr[0]=='H' && ATcmdStr[1]=='V')
		return SStarAT_HV;
	if (ATcmdStr[0]=='K' && ATcmdStr[1]=='Y')
		return SStarAT_KY;
	if (ATcmdStr[0]=='I')
	{
		if (ATcmdStr[1]=='O')
			return SStarAT_IO;
		else if(ATcmdStr[1]=='D')
			return SStarAT_ID;
	}
	if (ATcmdStr[0]=='M')
	{
		if (ATcmdStr[1]>='1' && ATcmdStr[1]<='4')
			return SStarAT_M1+(ATcmdStr[1]-'1');
		if (ATcmdStr[1]=='Y')
			return SStarAT_MY;
	}
	if (ATcmdStr[0]=='T' && ATcmdStr[1]=='X')
		return SStarAT_TX;
	if (ATcmdStr[0]=='U' && ATcmdStr[1]=='R')
		return SStarAT_UR;
	if (ATcmdStr[0]=='N' && ATcmdStr[1]=='M')
		return SStarAT_NM;
	if (ATcmdStr[0]=='P' && ATcmdStr[1]=='L')
		return SStarAT_PL;
	if (ATcmdStr[0]=='S')
	{
		if (ATcmdStr[1]=='M')
			return SStarAT_SM;
		else if(ATcmdStr[1]=='P')
			return SStarAT_SP;
		else if(ATcmdStr[1]=='T')
			return SStarAT_ST;
		else if(ATcmdStr[1]=='O')
			return SStarAT_SO;
	}
	return SStarAT_Undef;
}

SStarFrameType SStarAPI_frameType(const SStarAPIFrame * frame)
{
	if (checkFrame(frame)==0) return SStarFrameType_Undef;
	return frame->type;
}

SStarAPI10 * SStarAPI_toAPI10(SStarAPIFrame * frame)
{
	return (SStarAPI10*) frame->data;
}

SStarAPI07_09 * SStarAPI_toAPI07_09(SStarAPIFrame * frame)
{
	return (SStarAPI07_09*) frame->data;
}

SStarAPI0F * SStarAPI_toAPI0F(SStarAPIFrame * frame)
{
	return (SStarAPI0F*) frame->data;
}

SStarAPI17 * SStarAPI_toAPI17(SStarAPIFrame * frame)
{
	return (SStarAPI17*) frame->data;
}

const SStarAPI81838F * SStarAPI_toAPI81838F(const SStarAPIFrame * frame)
{
	if (checkFrame(frame) == 0) return 0;
	if (frame->type==SStarFrameType_API81 || frame->type==SStarFrameType_API83 || frame->type==SStarFrameType_API8F)
		return (SStarAPI81838F*) frame->data;
	return 0;
}

const SStarAPI83 * SStarAPI_toAPI83(const SStarAPIFrame * frame)
{
	if (checkFrame(frame) == 0) return 0;
	if (frame->type==SStarFrameType_API83)
		return (SStarAPI83*) frame->data;
	return 0;
}

const SStarAPI87_89 * SStarAPI_toAPI87_89(const SStarAPIFrame * frame)
{
	if (checkFrame(frame) == 0) return 0;
	if (frame->type==SStarFrameType_API87 || frame->type==SStarFrameType_API88 || frame->type==SStarFrameType_API89)
		return (SStarAPI87_89*) frame->data;
	return 0;
}

const SStarAPI8B * SStarAPI_toAPI8B(const SStarAPIFrame * frame)
{
	if (checkFrame(frame) == 0) return 0;
	if (frame->type==SStarFrameType_API8B)
		return (SStarAPI8B*) frame->data;
	return 0;
}

const SStarAPI8C * SStarAPI_toAPI8C(const SStarAPIFrame * frame)
{
	if (checkFrame(frame) == 0) return 0;
	if (frame->type==SStarFrameType_API8C)
		return (SStarAPI8C*) frame->data;
	return 0;
}

const SStarAPI97 * SStarAPI_toAPI97(const SStarAPIFrame * frame)
{
	if (checkFrame(frame) == 0) return 0;
	if (frame->type==SStarFrameType_API97)
		return (SStarAPI97*) frame->data;
	return 0;
}

//

//размер полезной нагрузки фрейма
inline uint16_t SStarAPI_frameLen(const SStarAPIFrame * frame)
{
	if (checkFrame(frame)==0) return 0;
	return frame->lenLsb | (frame->lenMsb<<8);
}


// Проверяет данные на соответствие фрейму. Возвращает преобразованный
// указатель если успешно или 0 если нет
const SStarAPIFrame * SStarAPI_readData(const uint8_t * data)
{
	if (data==0) return 0;
	int sz = SStarAPI_calcFrameSize(data);
	if (sz==0) return 0;
	const SStarAPIFrame * frame = (const SStarAPIFrame *)data;
	if (data[sz-1] == SStarAPI_calcCheckSum(frame))
		return frame;
	return 0;
}

// Возвращает количество байт, которое должно содержаться в этом фрагменте (на основе frame->lenLsb и lenMsb)
int SStarAPI_calcFrameSize(const uint8_t * data)
{
	const SStarAPIFrame * frame = (const SStarAPIFrame *)data;
	if (checkFrame(frame)==0) return 0;
	return SStarAPI_frameLen(frame)+4;
}


// API-фрейм для передачи данных на UART удаленного модема с
// управлением режимом передачи/подтверждения
void SStarAPI_sendAPI10(SStarAPIFrame * frame, uint8_t frameID, uint16_t destination, uint8_t API10_options, const uint8_t * data, uint8_t size)
{
	SStarAPI10 * api = SStarAPI_toAPI10(frame);
	memcpy(api->data,data,size);
	api->frameID = frameID;
	api->options = API10_options;
	api->destLsb = lsb(destination);
	api->destMsb = msb(destination);
	SStarAPI_completeFrame(frame, size+5, SStarFrameType_API10);
	uart_write(frame,size+5+4);

	{
		int dely = 60000;
		while (dely--);
	}
}

// API-фрейм для передачи данных на UART удаленного модема с
// управлением режимом передачи/подтверждения
void SStarAPI_sendAPI0F(SStarAPIFrame * frame, uint8_t frameID, uint16_t destination, const uint8_t * data, uint8_t size)
{
	SStarAPI0F * api = SStarAPI_toAPI0F(frame);
	memcpy(api->data,data,size);
	api->frameID = frameID;
	api->destLsb = lsb(destination);
	api->destMsb = msb(destination);
	SStarAPI_completeFrame(frame, size+4, SStarFrameType_API0F);
	uart_write(frame,size+4+4);

	{
		int dely = 60000;
		while (dely--);
	}
}

void SStarAPI_sendAPI07_09(SStarAPIFrame * frame, uint8_t frameID, const char * at, const uint8_t * atParam, uint8_t atParamSize, SStarAPI07_09_options option)
{
	SStarAPI07_09 * api = SStarAPI_toAPI07_09(frame);
//	int size = strlen(at);
	memcpy(api->ATcmd,at,2);
	api->frameID = frameID;
	if (atParamSize)
	{
		memcpy(api->data,atParam,atParamSize);
	}
	SStarAPI_completeFrame(frame, atParamSize+4, option);
	uart_write(frame,atParamSize+4+4);

	{
		int dely = 60000;
		while (dely--);
	}
}

// API-фрейм с локальной AT-командой и немедленным применением
// изменений без сохранения их в энергонезависимой памяти
void SStarAPI_sendAPI07(SStarAPIFrame * frame, uint8_t frameID, const char * at, const uint8_t * atParam, uint8_t atParamSize)
{
	SStarAPI_sendAPI07_09(frame, frameID, at, atParam, atParamSize, SStarAPI07_09_option_NotWrite);
}

// API-фрейм с локальной AT-командой и немедленным применением
// изменений с сохранением их в энергонезависимой памяти
void SStarAPI_sendAPI08(SStarAPIFrame * frame, uint8_t frameID, const char * at, const uint8_t * atParam, uint8_t atParamSize)
{
	SStarAPI_sendAPI07_09(frame, frameID, at, atParam, atParamSize, SStarAPI07_09_option_Rewrite);
}

// API-фрейм с локальной AT-командой с помещением
// измененного значения в очередь
void SStarAPI_sendAPI09(SStarAPIFrame * frame, uint8_t frameID, const char * at, const uint8_t * atParam, uint8_t atParamSize)
{
	SStarAPI_sendAPI07_09(frame, frameID, at, atParam, atParamSize, SStarAPI07_09_option_Queued);
}

// API-фрейм с AT-командой удаленному модему
void SStarAPI_sendAPI17(SStarAPIFrame * frame, uint8_t frameID, uint16_t destination, uint8_t API17_options, const char * at, const uint8_t * atParam, uint8_t atParamSize)
{
	SStarAPI17 * api = SStarAPI_toAPI17(frame);
//	int size = strlen(at);
	memcpy(api->ATcmd,at,2);
	api->frameID = frameID;
	if (atParamSize)
	{
		memcpy(api->data,atParam,atParamSize);
	}
	api->options = API17_options;
	api->destLsb = lsb(destination);
	api->destMsb = msb(destination);
	SStarAPI_completeFrame(frame, atParamSize+7, SStarFrameType_API17);
	uart_write(frame,atParamSize+7+4);

	{
		int dely = 60000;
		while (dely--);
	}
}

//======================================
//==== далее разбор принятых данных ====
//======================================

// длина данных (полезной нагрузки) в пакете. Для получения данных
// используйте SStarAPI_toAPI81(frame)->data[]
uint16_t SStarAPI_readAPI81838F_dataLen(const SStarAPIFrame * frame)
{
	uint16_t r = SStarAPI_frameLen(frame);
	return r==0 ? 0 : r-5;
}

const uint8_t * SStarAPI_readAPI81838F_data(const SStarAPIFrame * frame)
{
	const SStarAPI81838F * api = SStarAPI_toAPI81838F(frame);
	if (api==0) return 0;
	return api->data;
}

inline uint16_t SStarAPI_readAPI81838F_source(const SStarAPIFrame * frame)
{
	const SStarAPI81838F * api = SStarAPI_toAPI81838F(frame);
	if (api==0) return 0;
	return api->sourceLsb | (api->sourceMsb<<8);
}


uint8_t SStarAPI_readAPI81838F_isBroadcast(const SStarAPIFrame * frame)
{
	const SStarAPI81838F * api = SStarAPI_toAPI81838F(frame);
	if (api==0) return 0;
	return (api->options & SS_API81838F_OPTION_BROADCAST) ? 1 : 0;
}

uint16_t SStarAPI_readAPI83_dataLen(const SStarAPIFrame * frame)
{
	uint16_t r = SStarAPI_frameLen(frame);
	return r==0 ? 0 : r-7;
}

uint8_t SStarAPI_readAPI83_temp(const SStarAPIFrame * frame)
{
	if (frame->type!=SStarFrameType_API83)
		return 0;
	return SStarAPI_toAPI81838F(frame)->data[0];
}

uint8_t SStarAPI_readAPI83_vBat(const SStarAPIFrame * frame)
{
	if (frame->type!=SStarFrameType_API83)
		return 0;
	return SStarAPI_toAPI81838F(frame)->data[1];
}

uint16_t SStarAPI_readAPI83_vBat_mV(const SStarAPIFrame * frame)
{
	return (SStarAPI_readAPI83_vBat(frame)*1000)/51;
}

uint8_t SStarAPI_readAPI83_valCount(const SStarAPIFrame * frame)
{
	if (frame->type!=SStarFrameType_API83)
		return 0;
	uint8_t dataLen = SStarAPI_readAPI81838F_dataLen(frame)-2;
	const uint8_t * data = &SStarAPI_toAPI81838F(frame)->data[2];
	uint8_t valCnt = 0;
	for(int i=0; i<dataLen; )
	{
//		uint8_t chanel = data[i];
		uint8_t type = data[i+1];
		switch (type)
		{
		case SStarChannelMode_DigitalInput:
		case (SStarChannelMode_DigitalInput|0x80):
		case SStarChannelMode_DigitalOutput_Low:	//digital output low
		case SStarChannelMode_DigitalOutput_High:	//maybe digital output high
		case (SStarChannelMode_DigitalOutput_Low|0x80):	//digital output high
		case (SStarChannelMode_DigitalOutput_High|0x80):	//maybe digital output high
			i+=2;
			valCnt++;
			break;
		case SStarChannelMode_Couner1:	//counter1
		case SStarChannelMode_Couner2:	//counter2
			i+=6;
			valCnt++;
			break;
		case SStarChannelMode_ADC:	//ADC
			i+=4;
			valCnt++;
			break;
		default:
			i+=2;
			break;
		}
	}
	return valCnt;
}


// Получение специфического указателя для последующего преобразования
// его в данные. Параметр i указывает какой из указателей необходмо получить
// в пределах от 0 до (но не включая) SStarAPI_readAPI83_valCount()
SStarAPI_ValPtr SStarAPI_readAPI83_getValPtr(const SStarAPIFrame * frame, uint8_t i)
{
	if (frame->type!=SStarFrameType_API83)
		return 0;
	uint8_t dataLen = SStarAPI_readAPI83_dataLen(frame);//SStarAPI_readAPI81838F_dataLen(frame)-2;
	const uint8_t * data = SStarAPI_toAPI83(frame)->data;//&SStarAPI_toAPI81838F(frame)->data[2];
	uint8_t valCnt = 0;
	for(int j=0; j<dataLen; )
	{
		int curJ = j;
		uint8_t chanel = data[j];	//SStarAPI_ValPtr_input(data);
		uint8_t type = data[j+1];	//SStarAPI_ValPtr_mode(data);
		switch (type)
		{
		case SStarChannelMode_DigitalInput:
		case (SStarChannelMode_DigitalInput|0x80):
		case SStarChannelMode_DigitalOutput_Low:	//digital output low
		case SStarChannelMode_DigitalOutput_High:
		case SStarChannelMode_DigitalOutput_High|0x80:	//maybe digital output high
//		case (SStarChannelMode_DigitalOutput_Low):	//digital output high
			j+=2;
			valCnt++;
			break;
		case SStarChannelMode_Couner1:	//counter1
		case SStarChannelMode_Couner2:	//counter2
			j+=6;
			valCnt++;
			break;
		case SStarChannelMode_ADC:	//ADC
			j+=4;
			valCnt++;
			break;
		default:
			j+=2;
			break;
		}
		if (valCnt!=0)
			if (valCnt-1==i)
				return (SStarAPI_ValPtr) &data[curJ];
	}
	return 0;
}

uint32_t SStarAPI_ValPtr_value(SStarAPI_ValPtr valPtr)
{
	if (valPtr==0) return 0;
	uint32_t val = 0;
	const uint8_t * data = (const uint8_t*)valPtr;
	uint8_t type = data[1];
	switch (type)
	{
	case SStarChannelMode_DigitalInput:
	case SStarChannelMode_DigitalOutput_Low:	//digital_output low
		val = 0;
		break;
	case SStarChannelMode_DigitalOutput_High:
	case (SStarChannelMode_DigitalInput|0x80):	//digital_input_pullup high
	case (SStarChannelMode_DigitalOutput_Low|0x80):	//digital_output high
		val = 1;
		break;
	case SStarChannelMode_Couner1:	//counter1
	case SStarChannelMode_Couner2:	//counter2
		val = (data[2]<<24 | data[3]<<16 | data[4]<<8 | data[5]);
		break;
	case SStarChannelMode_ADC:	//ADC
		val = (data[2]<<8 | data[3]);
		break;
	default:
		return 0;
	}
	return val;
}

SStarChannelModes SStarAPI_ValPtr_mode(SStarAPI_ValPtr valPtr)
{
	if (valPtr==0) return 0;
	const uint8_t * data = (const uint8_t*)valPtr;
	return data[1];
}

SStarInputs SStarAPI_ValPtr_input(SStarAPI_ValPtr valPtr)
{
	if (valPtr==0) return 0;
	const uint8_t * data = (const uint8_t*)valPtr;
	return data[0];
}

SStarAT_commands SStarAPI_readAPI87_89_ATcommand(const SStarAPIFrame * frame)
{
	const SStarAPI87_89 * api = SStarAPI_toAPI87_89(frame);
	if (api == 0) return SStarAT_Undef;
	return SStarATcmd(api->ATcmd);
}

SStarAPI_AT_Status SStarAPI_readAPI87_89_status(const SStarAPIFrame * frame)
{
	const SStarAPI87_89 * api = SStarAPI_toAPI87_89(frame);
	if (api == 0) return SStarAPI_AT_Status_Undef;
	return SStarAPI_toAPI87_89(frame)->status;
}

uint8_t SStarAPI_readAPI87_89_ATparamLen(const SStarAPIFrame * frame)
{
	uint16_t sz = SStarAPI_frameLen(frame);
	if (sz<5) return 0;
	return sz-5;
}

//статус модема. Отправляется при включении питания или при перезагрузке
SStarAPI_ModemStatus SStarAPI_readAPI8A(const SStarAPIFrame * frame)
{
	if (checkFrame(frame)==0) return SStarAPI_ModemStatus_Undef;
	return frame->data[0];
}


uint16_t SStarAPI_readAPI8B_destination(const SStarAPIFrame * frame)
{
	const SStarAPI8B * api = SStarAPI_toAPI8B(frame);
	if (api==0) return 0;
	return api->destLsb | (api->destMsb<<8);
}

SStarAPI_SendingStatus SStarAPI_readAPI8B_status(const SStarAPIFrame * frame)
{
	const SStarAPI8B * api = SStarAPI_toAPI8B(frame);
	if (api==0) return 0;
	return api->status;
}

uint16_t SStarAPI_readAPI8C_source(const SStarAPIFrame * frame)
{
	const SStarAPI8C * api = SStarAPI_toAPI8C(frame);
	if (api==0) return 0;
	return api->sourceLsb | (api->sourceMsb<<8);
}

uint16_t SStarAPI_readAPI97_source(const SStarAPIFrame * frame)
{
	const SStarAPI97 * api = SStarAPI_toAPI97(frame);
	if (api==0) return 0;
	return api->sourceLsb | (api->sourceMsb<<8);
}

SStarAT_commands SStarAPI_readAPI97_ATcommand(const SStarAPIFrame * frame)
{
	SStarAPI97 * api = SStarAPI_toAPI97(frame);
	if (api==0) return SStarAT_Undef;
	return SStarATcmd(api->ATcmd);
}

// SStarAPI_AT_Status SStarAPI_readAPI97_status(SStarAPIFrame * frame)
// {
// 	return SStarAPI_toAPI97(frame)->status;
// }

uint8_t SStarAPI_readAPI97_ATparamLen(const SStarAPIFrame * frame)
{
	uint16_t sz = SStarAPI_frameLen(frame);
	if (sz<8) return 0;
	return sz-8;
}
