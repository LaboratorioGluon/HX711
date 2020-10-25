#include "hx711.h"

#include <stm32f1xx_hal.h>

extern void delay_us(uint32_t);

HX711::HX711(){
    mOffset = 0;
    mScale = 1.0f;
    mIsInitialized = 0;
}

void HX711::Init(GPIO_TypeDef* pDataGpio, uint16_t pDataPin, GPIO_TypeDef* pClkGpio, uint16_t pClkPin, uint16_t gain){
    
    mDataGpio = pDataGpio;
    mDataPin = pDataPin;
    mClkGpio = pClkGpio;
    mClkPin = pClkPin;
    mGain = gain;

    GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = pClkPin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pClkGpio, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = pDataPin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pDataGpio, &GPIO_InitStruct);


    // Reset the HX711
	HAL_GPIO_WritePin(pClkGpio, pClkPin, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(pClkGpio, pClkPin, GPIO_PIN_RESET);

    mIsInitialized = 1;

}


void HX711::Tare(int pTimes){
    mOffset = getMeanAfterNReads(pTimes);
}


uint32_t HX711::getRawValue(){

    if (!mIsInitialized)
        return -1;

    uint32_t buffer;
    buffer = 0;

    while (HAL_GPIO_ReadPin(mDataGpio, mDataPin)==1);

    for (uint8_t i = 0; i < 24; i++)
    {
    	HAL_GPIO_WritePin(mClkGpio, mClkPin, GPIO_PIN_SET);

        buffer = buffer << 1 ;
        delay_us(1);
        if (HAL_GPIO_ReadPin(mDataGpio, mDataPin))
        {
            buffer ++;
        }
        delay_us(1);
        HAL_GPIO_WritePin(mClkGpio, mClkPin, GPIO_PIN_RESET);
    }

    for (int i = 0; i < mGain; i++)
    {
    	HAL_GPIO_WritePin(mClkGpio, mClkPin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(mClkGpio, mClkPin, GPIO_PIN_RESET);
    }

    buffer = buffer ^ 0x800000;

    return buffer;
}


float HX711::getValue(int pTimes){
    float value = (float) getMeanAfterNReads(pTimes);
    return (value - mOffset)/mScale;
}

float HX711::getMeanAfterNReads(uint16_t pTimes){
    uint32_t lAcum = 0;
    for(uint16_t i =0; i < pTimes ; i++){
        lAcum += getRawValue();
    }

    return lAcum/(1.0f*pTimes);
}
