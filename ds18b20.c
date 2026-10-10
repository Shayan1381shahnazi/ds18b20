
#include "ds18b20.h"
#include "main.h"


void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CYCCNT = 0;

    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000U);

    while ((uint32_t)(DWT->CYCCNT - start) < ticks)
    {
    }
}

/// inte ds18bb20

void DS18B20_Init(void)
{
    DWT_Init();
		HAL_GPIO_WritePin(Ds18b20_GPIO_Port,Ds18b20_Pin,1);
}

uint8_t DS18B20_Reset(void)
{
    uint8_t presence;
	
		HAL_GPIO_WritePin(Ds18b20_GPIO_Port,Ds18b20_Pin,0);
    delay_us(480);
		HAL_GPIO_WritePin(Ds18b20_GPIO_Port,Ds18b20_Pin,1);
    delay_us(70);

    if (HAL_GPIO_ReadPin(Ds18b20_GPIO_Port,Ds18b20_Pin) == GPIO_PIN_RESET)
    {
        presence = 1;
    }
    else
    {
        presence = 0;
    }

    delay_us(410);


    return presence;
}

void DS18B20_WriteBit(uint8_t bit)
{
    if (bit)
    {
        HAL_GPIO_WritePin(Ds18b20_GPIO_Port,Ds18b20_Pin,0);
        delay_us(5);
				HAL_GPIO_WritePin(Ds18b20_GPIO_Port,Ds18b20_Pin,1);
        delay_us(55);
			}
			else
			{
			
				HAL_GPIO_WritePin(Ds18b20_GPIO_Port,Ds18b20_Pin,0);
				delay_us(60);
				HAL_GPIO_WritePin(Ds18b20_GPIO_Port,Ds18b20_Pin,1);
				delay_us(5);
			}
}

uint8_t DS18B20_ReadBit(void)
{
	
	
  uint8_t bit;
	HAL_GPIO_WritePin(Ds18b20_GPIO_Port,Ds18b20_Pin,0);
  delay_us(2);
	HAL_GPIO_WritePin(Ds18b20_GPIO_Port,Ds18b20_Pin,1);
  delay_us(10);
  if (HAL_GPIO_ReadPin(Ds18b20_GPIO_Port,Ds18b20_Pin) == GPIO_PIN_SET)
		
    {
        bit = 1;
    }
    else
    {
        bit = 0;
    }
    delay_us(50);
    return bit;
}

void DS18B20_WriteByte(uint8_t data)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		DS18B20_WriteBit(data & 0x01);
		data >>=1;
	}
	
}

uint8_t DS18B20_ReadByte(void)
{
    uint8_t i;
    uint8_t data = 0;
    for (i = 0; i < 8; i++)
    {
      if (DS18B20_ReadBit())
				
       {
         data |= (1 << i);
       }
    }
    return data;
}

float DS18B20_ReadTemperature(void)
{
    uint8_t temp_lsb;
    uint8_t temp_msb;

    int16_t temperature_raw;


    if (!DS18B20_Reset())
    {
        return -1000.0f;
    }

    DS18B20_WriteByte(0xCC);

    DS18B20_WriteByte(0x44);

    HAL_Delay(750);

    if (!DS18B20_Reset())
    {
        return -1000.0f;
    }

    DS18B20_WriteByte(0xCC);

    DS18B20_WriteByte(0xBE);

    temp_lsb = DS18B20_ReadByte();

    temp_msb = DS18B20_ReadByte();

    temperature_raw =
        ((int16_t)temp_msb << 8) |
        temp_lsb;

    return ((float)temperature_raw / 16.0f);
}