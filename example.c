#include "SStar.h"

//в проекте используется буфер для получения данных с линии UART
//для работы с буфером используются следующие функции
extern int uart_bufferDataCount();							//кол-во байт, принятых по uart
extern const uint8_t * uart_bufferData();					//указатель на данные
extern void uart_bufferClear();								//очистить буфер

//для удобства работы с буфером дополнительно введены функции поиска и обрезки буфера
extern int uart_bufferFind(uint8_t data, int startFrom);	//поиск байта в буфере
extern void uart_bufferMid(int pos, int len);				//обрезать буфер с позиции pos на длину len и поместить в начало

void example()
{
	int frSz=0;
	const SStarAPIFrame * frame = 0;

	if (uart_bufferData()[0]!=SStarAPIFrameStartByte)		//если заголовок не соотвестствует
		goto BREAK_FRAME_EXIT;								//пропускаем данные

	frSz = SStarAPI_calcFrameSize(uart_bufferData());		//считаем размер пакета

	if (uart_bufferDataCount() < frSz)						//если принято недостаточно
		return;												//ждем еще данные

	frame = SStarAPI_readData(uart_bufferData());			//пытаемся прочитать пакет
	if (frame == 0)											//ошибка
	{
		frSz = 0;
		goto BREAK_FRAME_EXIT;								//пропускаем данные
	}

	SStarFrameType frTp = SStarAPI_frameType(frame);		//тип полученного фрейма

	//пакет с данными от удаленного устройства
	//данный пакет приходит, когда удаленное устройство отправляет пакет 0x10
	if (frTp == SStarFrameType_API81)
	{
		uint16_t sourceID = SStarAPI_readAPI81838F_source(frame);			//от кого пришел пакет
		uint8_t isBroadcast = SStarAPI_readAPI81838F_isBroadcast(frame);	//принятый пакет - широковещательный?
		uint16_t dataLen = SStarAPI_readAPI81838F_dataLen(frame);			//размер данных, принятых от удаленного устройства
		uint8_t * data = SStarAPI_readAPI81838F_data(frame);				//данные от удаленного устройства
	}

	//пакет с данными от удаленнго устройства
	//обычно такой пакет отпралвет устройство, которое находится в режиме пониженного потребления,
	//при выходе из которого отправляет пакет с состоянием пинов, счетчиков и т.п
	if (frTp == SStarFrameType_API83)
	{
		uint16_t sourceID = SStarAPI_readAPI81838F_source(frame);			//от кого пришел пакет
		uint8_t temp = SStarAPI_readAPI83_temp(frame);						//температура
		uint16_t vBat = SStarAPI_readAPI83_vBat_mV(frame);					//напряжение vin
		uint8_t valCnt = SStarAPI_readAPI83_valCount(frame);				//кол-во данных, принятых в пакете (каждый пин, настроенный на вход,
																			//выход, АЦП или счетчик)
		for (int i=0; i<valCnt; ++i)
		{
			SStarAPI_ValPtr valPtr = SStarAPI_readAPI83_getValPtr(frame, i);	//контейнер, в котором хранятся данные о состоянии пина/счетчика
			SStarInputs input = SStarAPI_ValPtr_input(valPtr);					//извлекаем из контейнера Пин
			SStarChannelModes inputMode = SStarAPI_ValPtr_mode(valPtr);			//режим, на который настроен Пин
			uint32_t val = SStarAPI_ValPtr_value(valPtr);						//значение пина (или счетчика, или АЦП)
		}
	}

	//пакет типа 0x01, 0x10 или 0x17 был успешно (или нет) отправлен
	if (frTp == SStarFrameType_API8B)
	{
		uint16_t dest = SStarAPI_readAPI8B_destination(frame);			//адрес модема, которому был предназначен пакет
		uint8_t frameID = SStarAPI_readAPI8B_frameID(frame);			//id фрейма
		SStarAPI_SendingStatus st = SStarAPI_readAPI8B_status(frame);	//статус отправки
		if (st!=SStarAPI_SendingStatus_Ok)
		{
			//error
		}
	}

	//пакет типа 0x01, 0x10 был успешно доставлен
	if (frTp == SStarFrameType_API8C)
	{
		uint16_t source = SStarAPI_readAPI8C_source(frame);				//адрес модема, которому был предназначен пакет
		uint8_t frameID = SStarAPI_readAPI8C_frameID(frame);			//id фрейма
	}

	//отправка данных удаленному устройству БЕЗ подтверждения доставки
	const char * const str = "Hello!";
	//1 - id фрейма; 2 - адрес устройства; 3 - указатель на данные; 4 - размер
	SStarAPI_sendAPI0F(1, 0x0001, str, strlen(str));

	//отправка данных удаленному устройству с возможностью подтвержения доставки
	//Внимание! Может быть ситуация, когда сообщение будет доставлено удаленному модему, а подтверждение доставки до вас не дойдет!
	//1 - id фрейма; 2 - адрес устройства; 3 - опции (BUFFERED - отправка только в ответ на полученное сообщение, ENCODE - шифорование);
	//  4 - указатель на данные; 5 - размер
	//в ответ от локального модема придет пакет 0x8B
	//в случае успешной доставки от удаленного модема придет пакет 0x8C
	SStarAPI_sendAPI10(2, 0x0001, SS_API10_OPTION_BUFFERED|SS_API10_OPTION_ENCODE, str, strlen(str));

	//1 - id фрейма; 2 - AT-команда; 3 - параметр для AT; 4 - размер передаваемого параметра; 5 - опция сохранения
	uint16_t myAddr = 0x0002;
	//пример передачи локальному модему нового собственного адреса (AT команда "MY"). Сохранение и применение настроек моментально
	SStarAPI_sendAPI07_09(3, SStarAT_MY_Str, &myAddr, sizeof(myAddr), SStarAPI07_09_option_Rewrite);

	//пример передачи локальному модему новой конфигурации для пина B3 (AT команда "B3"). Применение настроек моментально, без сохранения
	uint8_t pinMode = SStarChannelMode_DigitalOutput_High;
	SStarAPI_sendAPI07_09(4, "B3", &pinMode, sizeof(pinMode), SStarAPI07_09_option_NotWrite);


BREAK_FRAME_EXIT:
	//отбрасываем данные которые не подходят по какой-либо причине
	//ищем в буфере следующий символ начала фрейма (SStarAPIFrameStartByte) и смещаем эти данные в начало буфера
	//пробежимся по всему буферу поищем символ начала пакета
	{
		int pos = uart_bufferFind(SStarAPIFrameStartByte, (frSz>0 ? frSz : 1));
		if (pos>0)
			uart_bufferMid(pos, -1);
		else	//если не нашли подходящий символ - обнуляем буфер
			uart_bufferClear();
	}

}
