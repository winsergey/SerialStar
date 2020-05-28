#ifndef __SSTAR_H__
#define __SSTAR_H__

#include <stdint.h>

#define SS_API10_OPTION_DISABLE_ACK	(0x01)
#define SS_API10_OPTION_DISABLE_CCA	(0x10)
#define SS_API10_OPTION_ENCODE		(0x20)
#define SS_API10_OPTION_BUFFERED	(0x40)

#define SS_API17_OPTION_DISABLE_ACK	SS_API10_OPTION_DISABLE_ACK
#define SS_API17_OPTION_QUEUED		(0)		//данные AT комманды будут применены только после соотв команды применения (напр AC)
#define SS_API17_OPTION_REWRITE		(0x02)	//данные будут применены и сохранены
#define SS_API17_OPTION_NOTWRITE	(0x04)	//данные будут применены но не сохранены
#define SS_API17_OPTION_DISABLE_CCA	SS_API10_OPTION_DISABLE_CCA
#define SS_API17_OPTION_ENCODE		SS_API10_OPTION_ENCODE
#define SS_API17_OPTION_BUFFERED	SS_API10_OPTION_BUFFERED

#define SS_API81_OPTION_CONFIRM_DELIVERY	(0x01)
#define SS_API81838F_OPTION_BROADCAST		(0x02)
#define SS_API81_OPTION_DISABLED_CCA		(0x10)
#define SS_API81_OPTION_ENCODED				(0x20)
#define SS_API81_OPTION_BUFFERED			(0x20)

#define SS_AT_DM_OPTION_BROADCAST_DISABLE	(1<<0)							//запрет приема широковещательных пакетов
#define SS_AT_DM_OPTION_VREF_1V5			(1<<1)							//установить опорное напряжение для ADC 1,5в
#define SS_AT_DM_OPTION_RETRANSLATION_EN	(1<<2)							//включает ретрансляцию принятых пакетов
#define SS_AT_DM_OPTION_CCA_DISABLE			SS_API10_OPTION_DISABLE_CCA		//отключение проверки занятости радиоэфира
#define SS_AT_DM_OPTION_ENCODE_EN			SS_API10_OPTION_ENCODE			//включение шифрования
#define SS_AT_DM_OPTIONS_DEFAULT			(0)

#define SS_AT_SM_NOSLEEP		(0)
#define SS_AT_SM_EXTSLEEP		(1)
#define SS_AT_SM_PERIODIC		(4)
#define SS_AT_SM_COMBI			(5)

#define SS_AT_SO_PERIODIC		(1<<1)
#define SS_AT_SO_WAKEUP			(1<<2)


#define SStarAPIFrameStartByte (0x7E)


typedef const uint8_t* SStarAPI_ValPtr;


typedef enum
{
	SStarAPI_AT_Status_Undef = 0xFF,
	SStarAPI_AT_Status_Ok = 0x00,
	SStarAPI_AT_Status_Error = 0x01,
	SStarAPI_AT_Status_InvalidCode = 0x02,
	SStarAPI_AT_Status_InvalidParam = 0x03
} SStarAPI_AT_Status;

typedef enum
{
	SStarAPI_ModemStatus_Undef = 0xFF,
	SStarAPI_ModemStatus_PowerUp = 0x00,	//модем включен
	SStarAPI_ModemStatus_Reset = 0x01		//модем перезагружен
} SStarAPI_ModemStatus;

//статус отправки пакета с типом фрейма 0x01, 0x10, 0x17
typedef enum
{
	SStarAPI_SendingStatus_Undef = 0xFF,
	SStarAPI_SendingStatus_Ok = 0x00,				//Отправлено
	SStarAPI_SendingStatus_Error = 0x01,			//Недостаточно памяти для выполнения команды
	SStarAPI_SendingStatus_InvalidCode = 0x02,		//Недопустимый код AT-команды
	SStarAPI_SendingStatus_InvalidParam = 0x03,		//Недопустимое значение параметра
	SStarAPI_SendingStatus_Failure = 0x04			//не отправлено
} SStarAPI_SendingStatus;

typedef enum
{
	SStarChannelMode_Disabled = 0,
	SStarChannelMode_NotAvailable = 1,
	SStarChannelMode_ADC = 2,
	SStarChannelMode_DigitalInput = 3,
	SStarChannelMode_DigitalOutput_Low = 4,
	SStarChannelMode_DigitalOutput_High = 5,
	SStarChannelMode_UART_TX = 6,
	SStarChannelMode_UART_RX = 7,
	SStarChannelMode_UART_CTS = 8,
	SStarChannelMode_UART_RTS = 9,
	SStarChannelMode_SystemLed = 10,
	SStarChannelMode_SleepReq = 11,
	SStarChannelMode_SleepStatus = 12,
	SStarChannelMode_Couner1 = 13,
	SStarChannelMode_Couner2 = 14,
	SStarChannelMode_WU_Fall = 15,
	SStarChannelMode_WU_Rising = 16,
	SStarChannelMode_PWM1 = 17,
	SStarChannelMode_PWM2 = 18,
	SStarChannelMode_PWM3 = 19,
	SStarChannelMode_PWM4 = 20,
	SStarChannelMode_RS485Dir = 21,
	SStarChannelMode_DigitalInputLow =	 SStarChannelMode_DigitalInput,
	SStarChannelMode_DigitalInputHigh = (SStarChannelMode_DigitalInput | 0x80)
} SStarChannelModes;

const char * const SStarAT_WR_Str;
const char * const SStarAT_CN_Str;
const char * const SStarAT_AC_Str;
const char * const SStarAT_DC_Str;
const char * const SStarAT_RE_Str;
const char * const SStarAT_FR_Str;
const char * const SStarAT_VR_Str;
const char * const SStarAT_HV_Str;
const char * const SStarAT_AP_Str;
const char * const SStarAT_DM_Str;
const char * const SStarAT_KY_Str;
const char * const SStarAT_IO_Str;
const char * const SStarAT_AI_Str;
const char * const SStarAT_MY_Str;
const char * const SStarAT_ID_Str;
const char * const SStarAT_TX_Str;
const char * const SStarAT_UR_Str;
const char * const SStarAT_DR_Str;
const char * const SStarAT_NM_Str;
const char * const SStarAT_CH_Str;
const char * const SStarAT_CF_Str;
const char * const SStarAT_RB_Str;
const char * const SStarAT_PL_Str;
const char * const SStarAT_SM_Str;
const char * const SStarAT_SP_Str;
const char * const SStarAT_ST_Str;
const char * const SStarAT_SO_Str;
const char * const SStarAT_DS_Str;

typedef enum
{
	SStarAT_Undef = -1,
	SStarAT_WR,		//запись параметров в память. Применение параметров только после перезагрузки
	SStarAT_CN,		//запись параметров в память. Выход из командного режима. Перезапуск
	SStarAT_AC,		//запись параметров в память. Применение параметров немедленно
	SStarAT_DC,		//отмена изменений до применения комманд CN и AC
	SStarAT_RE,		//восстановление заводских настроек
	SStarAT_FR,		//Перезапуск
	SStarAT_VR,		//чтение версии ПО модуля
	SStarAT_HV,		//чтение настроек
	SStarAT_AP,		//режим работы модема (Прозрачный/Пакетный/...)
	SStarAT_DM,		//[0] - запрет приема широковещ., [1] - опорное для АЦП, [2] - ретрансляция, [4] - CCA, [5] - шифрование
	SStarAT_KY,		//ключ шифрования (16 байт)
	SStarAT_IO,		//запрос состояния входох/выходов
	SStarAT_AI,		//Применение параметров немедленно
//сетевые настройки
	SStarAT_MY,		//адрес модуля MyID (2 байта)
	SStarAT_ID,		//сетевой идентификатор (3 байта)
	SStarAT_TX,		//ID получателя (для прозрачного режима)
	SStarAT_UR,		//ID следующего в маршруте (к Главному)
	SStarAT_DR,		//ID предыдущего в маршруте (от Главного)
	SStarAT_NM,		//маска сети
//настройки радио
	SStarAT_CH,		//частотный канал
	SStarAT_CF,		//точная настройка частоты
	SStarAT_RB,		//эфирная битовая скорость
	SStarAT_PL,		//мощность передатчика
//настройка режимов сна
	SStarAT_SM,		//режим сна
	SStarAT_SP,		//инстервал пробуждения
	SStarAT_ST,		//длительность активного режима
	SStarAT_SO,		//отправка пакета с данными GPIO при пробуждении по интервалу (бит 1), по внешнему сигналу WU (бит 2)
	SStarAT_DS,		//задержка запуска ADC
//команды управления GPIO
	SStarAT_L0,
	SStarAT_L1,
	SStarAT_L2,
	SStarAT_L3,
	SStarAT_L4,
	SStarAT_L5,
	SStarAT_L6,
	SStarAT_L7,
	SStarAT_L8,
	SStarAT_B0,
	SStarAT_B1,
	SStarAT_B2,
	SStarAT_B3,
	SStarAT_B4,
	SStarAT_B5,
	SStarAT_R0,
	SStarAT_R1,
	SStarAT_R2,
	SStarAT_R3,
	SStarAT_R4,
	SStarAT_R5,
	SStarAT_R6,
	SStarAT_R7,
	SStarAT_R8,
	SStarAT_R9,
//управление ШИМ каналами
	SStarAT_M1,
	SStarAT_M2,
	SStarAT_M3,
	SStarAT_M4
} SStarAT_commands;

typedef enum
{
	SStarInput_L0 = 2,
	SStarInput_L1 = 3,
	SStarInput_L2 = 4,
	SStarInput_L3 = 6,
	SStarInput_L4 = 7,
	SStarInput_L5 = 9,
	SStarInput_L6 = 11,
	SStarInput_L7 = 12,
	SStarInput_L8 = 13,

	SStarInput_B0 = 14,
	SStarInput_B1 = 15,
	SStarInput_B2 = 16,
	SStarInput_B3 = 17,
	SStarInput_B4 = 18,
	SStarInput_B5 = 19,

	SStarInput_R0 = 35,
	SStarInput_R1 = 34,
	SStarInput_R2 = 33,
	SStarInput_R3 = 32,
	SStarInput_R4 = 31,
	SStarInput_R5 = 30,
	SStarInput_R6 = 29,
	SStarInput_R7 = 28,
	SStarInput_R8 = 27,
	SStarInput_R9 = 24
}SStarInputs;

typedef enum
{
	SStarFrameType_Undef = 0,
	SStarFrameType_API10 = 0x10,
	SStarFrameType_API07 = 0x07,
	SStarFrameType_API08 = 0x08,
	SStarFrameType_API09 = 0x09,
	SStarFrameType_API0F = 0x0F,
	SStarFrameType_API17 = 0x17,
	SStarFrameType_API81 = 0x81,	//принятые данные от удаленного модема
	SStarFrameType_API83 = 0x83,	//состояние i/o линий удаленного модема
	SStarFrameType_API87 = 0x87,	//результат выполнения локальной AT-команды фрейм 0x07
	SStarFrameType_API88 = 0x88,	//результат выполнения локальной AT-команды фрейм 0x08
	SStarFrameType_API89 = 0x89,	//результат выполнения локальной AT-команды фрейм 0x09
	SStarFrameType_API8A = 0x8A,	//статус модема
	SStarFrameType_API8B = 0x8B,	//статус отправки пакета
	SStarFrameType_API8C = 0x8C,	//подтвержение доставки пакета
	SStarFrameType_API8F = 0x8F,
	SStarFrameType_API97 = 0x97		//результат от удаленного модема на AT-комманду фрейм 0x17
} SStarFrameType;


typedef enum
{
	SStarAPI07_09_option_NotWrite = SStarFrameType_API07,
	SStarAPI07_09_option_Rewrite = SStarFrameType_API08,
	SStarAPI07_09_option_Queued = SStarFrameType_API09
} SStarAPI07_09_options;


typedef struct
{
	uint8_t startData;
	uint8_t lenMsb;
	uint8_t lenLsb;
	uint8_t type;		//SStarFrameType
	uint8_t data[60];
} SStarAPIFrame;

typedef struct
{
	uint8_t frameID;
	uint8_t destMsb;
	uint8_t destLsb;
	uint8_t options;
	uint8_t data[];
} SStarAPI10;

typedef struct
{
	uint8_t frameID;
	uint8_t destMsb;
	uint8_t destLsb;
	uint8_t data[];
} SStarAPI0F;

typedef struct
{
	uint8_t frameID;
	char ATcmd[2];
	uint8_t data[];
} SStarAPI07_09;

typedef SStarAPI07_09 SStarAPI07;
typedef SStarAPI07_09 SStarAPI08;
typedef SStarAPI07_09 SStarAPI09;

typedef struct
{
	uint8_t frameID;
	uint8_t destMsb;
	uint8_t destLsb;
	uint8_t options;
	char ATcmd[2];
	uint8_t data[];
} SStarAPI17;

typedef const struct
{
	uint8_t sourceMsb;
	uint8_t sourceLsb;
	uint8_t RSSI;
	uint8_t options;
	uint8_t data[];
} SStarAPI81838F;

typedef SStarAPI81838F SStarAPI81;
typedef SStarAPI81838F SStarAPI8F;

typedef const struct
{
	uint8_t sourceMsb;
	uint8_t sourceLsb;
	uint8_t RSSI;
	uint8_t options;
	uint8_t temperature;
	uint8_t vBatt;
	uint8_t data[];
} SStarAPI83;

typedef const struct
{
	uint8_t sourceMsb;
	uint8_t sourceLsb;
	uint8_t RSSI;
	uint8_t options;
	uint8_t frameID;
} SStarAPI8C;

typedef const struct
{
	uint8_t frameID;
	char ATcmd[2];
	uint8_t status;
	uint8_t ATparam[];
} SStarAPI87_89;

typedef SStarAPI87_89 SStarAPI87;
typedef SStarAPI87_89 SStarAPI88;
typedef SStarAPI87_89 SStarAPI89;

typedef const struct
{
	uint8_t frameID;
	uint8_t destMsb;
	uint8_t destLsb;
	uint8_t TX_retry;
	SStarAPI_SendingStatus status;
	uint8_t __reserve;
} SStarAPI8B;

typedef const struct
{
	uint8_t sourceMsb;
	uint8_t sourceLsb;
	uint8_t RSSI;
	uint8_t __options;
	char ATcmd[2];
	uint8_t __status;
	uint8_t ATparam[];
} SStarAPI97;


int SStarAPI_calcFrameSize(const uint8_t * data);
const SStarAPIFrame * SStarAPI_readData(const uint8_t * data);
SStarFrameType SStarAPI_frameType(const SStarAPIFrame * frame);
uint16_t SStarAPI_frameLen(const SStarAPIFrame * frame);

SStarAT_commands SStarATcmd(const char * ATcmdStr);

SStarAPI10 * SStarAPI_toAPI10(SStarAPIFrame * frame);
SStarAPI07_09 * SStarAPI_toAPI07_09(SStarAPIFrame * frame);
SStarAPI0F * SStarAPI_toAPI0F(SStarAPIFrame * frame);
SStarAPI17 * SStarAPI_toAPI17(SStarAPIFrame * frame);

const SStarAPI81838F * SStarAPI_toAPI81838F(const SStarAPIFrame * frame);
const SStarAPI83 * SStarAPI_toAPI83(const SStarAPIFrame * frame);
const SStarAPI87_89 * SStarAPI_toAPI87_89(const SStarAPIFrame * frame);
const SStarAPI8B * SStarAPI_toAPI8B(const SStarAPIFrame * frame);
const SStarAPI8C * SStarAPI_toAPI8C(const SStarAPIFrame * frame);
const SStarAPI97 * SStarAPI_toAPI97(const SStarAPIFrame * frame);


void SStarAPI_sendAPI10(SStarAPIFrame * frame, uint8_t frameID, uint16_t destination, uint8_t API10_options, const uint8_t * data, uint8_t size);
void SStarAPI_sendAPI0F(SStarAPIFrame * frame, uint8_t frameID, uint16_t destination, const uint8_t * data, uint8_t size);
void SStarAPI_sendAPI07_09(SStarAPIFrame * frame, uint8_t frameID, const char * at, const uint8_t * atParam, uint8_t atParamSize, SStarAPI07_09_options option);
void SStarAPI_sendAPI07(SStarAPIFrame * frame, uint8_t frameID, const char * at, const uint8_t * atParam, uint8_t atParamSize);
void SStarAPI_sendAPI08(SStarAPIFrame * frame, uint8_t frameID, const char * at, const uint8_t * atParam, uint8_t atParamSize);
void SStarAPI_sendAPI09(SStarAPIFrame * frame, uint8_t frameID, const char * at, const uint8_t * atParam, uint8_t atParamSize);
void SStarAPI_sendAPI17(SStarAPIFrame * frame, uint8_t frameID, uint16_t destination, uint8_t API17_options, const char * at, const uint8_t * atParam, uint8_t atParamSize);

uint16_t SStarAPI_readAPI81838F_dataLen(const SStarAPIFrame * frame);
const uint8_t * SStarAPI_readAPI81838F_data(const SStarAPIFrame * frame);
uint16_t SStarAPI_readAPI81838F_source(const SStarAPIFrame * frame);
uint8_t SStarAPI_readAPI81838F_isBroadcast(const SStarAPIFrame * frame);
uint16_t SStarAPI_readAPI83_dataLen(const SStarAPIFrame * frame);
//#define SStarAPI_readAPI83_dataLen SStarAPI_readAPI81838F_dataLen
#define SStarAPI_readAPI83_source SStarAPI_readAPI81838F_source
uint8_t SStarAPI_readAPI83_temp(const SStarAPIFrame * frame);
uint8_t SStarAPI_readAPI83_vBat(const SStarAPIFrame * frame);
uint16_t SStarAPI_readAPI83_vBat_mV(const SStarAPIFrame * frame);
uint8_t SStarAPI_readAPI83_valCount(const SStarAPIFrame * frame);
SStarAPI_ValPtr SStarAPI_readAPI83_getValPtr(const SStarAPIFrame * frame, uint8_t i);
SStarAT_commands SStarAPI_readAPI87_89_ATcommand(const SStarAPIFrame * frame);
SStarAPI_AT_Status SStarAPI_readAPI87_89_status(const SStarAPIFrame * frame);
uint8_t SStarAPI_readAPI87_89_ATparamLen(const SStarAPIFrame * frame);
SStarAPI_ModemStatus SStarAPI_readAPI8A(const SStarAPIFrame * frame);
uint16_t SStarAPI_readAPI8B_destination(const SStarAPIFrame * frame);
SStarAPI_SendingStatus SStarAPI_readAPI8B_status(const SStarAPIFrame * frame);
uint16_t SStarAPI_readAPI8C_source(const SStarAPIFrame * frame);
uint16_t SStarAPI_readAPI97_source(const SStarAPIFrame * frame);
SStarAT_commands SStarAPI_readAPI97_ATcommand(const SStarAPIFrame * frame);
uint8_t SStarAPI_readAPI97_ATparamLen(const SStarAPIFrame * frame);

uint32_t SStarAPI_ValPtr_value(SStarAPI_ValPtr valPtr);
SStarChannelModes SStarAPI_ValPtr_mode(SStarAPI_ValPtr valPtr);
SStarInputs SStarAPI_ValPtr_input(SStarAPI_ValPtr valPtr);


#endif
