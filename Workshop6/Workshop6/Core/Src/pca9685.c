#include "pca9685.h"

#include <math.h>
#include <stdlib.h>

#define ADRESS 0x80
#define WAITTIME 100

const uint8_t mode1InitValue = 0x01;

static bool setRegister(I2C_HandleTypeDef hi2c, uint8_t adress, uint8_t value)
{
	uint8_t fullData[2] = {adress, value};
	return HAL_I2C_Master_Transmit(&hi2c, ADRESS, (uint8_t *)&fullData, 2, WAITTIME) == HAL_OK;
}

static bool readRegister(I2C_HandleTypeDef hi2c, uint8_t adress, uint8_t *save)
{
	HAL_I2C_Master_Transmit(&hi2c, ADRESS, (uint8_t *)&adress, 1, WAITTIME);
	uint8_t result = HAL_I2C_Master_Receive(&hi2c, ADRESS, (uint8_t *)save, 1, WAITTIME) == HAL_OK;
	return result;
}

static bool setPin(I2C_HandleTypeDef hi2c, uint8_t adress, uint8_t *data4Elements)
{
	bool result = 1;
	uint8_t fullData[2] = {adress};
	for (int i = 0; i < 4; i++)
	{
		fullData[1] = data4Elements[i];
		result &= HAL_I2C_Master_Transmit(&hi2c, ADRESS, (uint8_t *)&fullData, 2, WAITTIME) == HAL_OK;
		fullData[0] += 1;
	}
	return result;
}

bool pcaInit(I2C_HandleTypeDef hi2c)
{
	bool result = setRegister(hi2c, 0x00, mode1InitValue);
	result &= pcaDisableAllOutputs(hi2c);
	return result;
}

bool pcaEnableAllOutputs(I2C_HandleTypeDef hi2c)
{
	uint8_t data4Elements[4] = {0x00, 0x10, 0x00, 0x00};
	return setPin(hi2c, 0xfa, data4Elements);
}

bool pcaDisableAllOutputs(I2C_HandleTypeDef hi2c)
{
	uint8_t data4Elements[4] = {0x00, 0x00, 0x00, 0x10};
	return setPin(hi2c, 0xfa, data4Elements);
}

bool pcaIsSleeping(I2C_HandleTypeDef hi2c)
{
	uint8_t sleeping;
	if (!readRegister(hi2c, 0x00, &sleeping))
		return 0;
	sleeping &= 0b00010000; // sleeping bit
	return sleeping;
}

bool pcaEnableSleepMode(I2C_HandleTypeDef hi2c)
{
	uint8_t register0;
	if (!readRegister(hi2c, 0x00, &register0))
	{
		return 0;
	}
	register0 |= 0b00010000; // sleep bit
	return setRegister(hi2c, 0x00, register0);
}

bool pcaDisableSleepMode(I2C_HandleTypeDef hi2c)
{
	uint8_t register0;
	if (!readRegister(hi2c, 0x00, &register0))
	{
		return 0;
	}
	register0 &= 0b11101111; // sleep bit
	return setRegister(hi2c, 0x00, register0);
}

bool pcaSetFrequency(I2C_HandleTypeDef hi2c, uint16_t newFrequency)
{
	uint8_t prescale = round(25000000 / (4096 * newFrequency)) - 1;

	if (!pcaIsSleeping(hi2c))
 	{
		pcaEnableSleepMode(hi2c);
		bool result = setRegister(hi2c, 0xfe, prescale);
		pcaDisableSleepMode(hi2c);
		return result;
	}
	return setRegister(hi2c, 0xfe, prescale);
}

bool pcaSetDutyCycle(I2C_HandleTypeDef hi2c, uint8_t channel, uint8_t newDutyCycle)
{
	if (channel >= 1 && channel <= 16)
	{
		if (newDutyCycle <= 100 && newDutyCycle >= 0)
		{
			uint16_t setOn = newDutyCycle == 100 ? 4096 : 0;
			uint16_t setOff = newDutyCycle == 100 ? 0 : round(((float)newDutyCycle / 100) * 4096);

			uint8_t finalChannel = 0x06 + ((channel - 1) * 4);
			uint8_t data4Elements[4] = {setOn, setOn >> 8u, setOff, setOff >> 8u};
			return setPin(hi2c, finalChannel, data4Elements);
		}
	}
	return 0;
}
