#pragma once

#include "stm32f4xx_it.h"

#ifndef __STM32F4xx_HAL_H
#define __STM32F4xx_HAL_H
#include "stm32f4xx_hal_conf.h"
#endif

#include "stdbool.h"

bool pcaInit(I2C_HandleTypeDef hi2c);

bool pcaEnableAllOutputs(I2C_HandleTypeDef hi2c);

bool pcaDisableAllOutputs(I2C_HandleTypeDef hi2c);

bool pcaIsSleeping(I2C_HandleTypeDef hi2c);

bool pcaEnableSleepMode(I2C_HandleTypeDef hi2c);

bool pcaDisableSleepMode(I2C_HandleTypeDef hi2c);

bool pcaSetFrequency(I2C_HandleTypeDef hi2c, uint16_t newFrequency);

bool pcaSetDutyCycle(I2C_HandleTypeDef hi2c, uint8_t channel, uint8_t newDutyCycle);
