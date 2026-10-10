#ifndef __ds18b20
#define __ds18b20


#include "main.h"

void DWT_Init(void);
void delay_us(uint32_t us);
void DS18B20_Init(void);
uint8_t DS18B20_Reset(void);
void DS18B20_WriteBit(uint8_t bit);
uint8_t DS18B20_ReadBit(void);
void DS18B20_WriteByte(uint8_t data);
uint8_t DS18B20_ReadByte(void);
float DS18B20_ReadTemperature(void);

#endif