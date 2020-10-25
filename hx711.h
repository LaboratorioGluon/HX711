#ifndef __HX711_H__
#define __HX711_H__

#include <stdint.h>

#include <stm32f1xx_hal.h>

class HX711{
public:
    HX711();
    ~HX711();

	/**
	 * @brief Initialize HW and variables for HX711. This initialize:
	 * 	- GPIOx/y RCC Clock
	 * 	- GPIOx/y for CLK/DATA pins
	 * 
	 * @params:
	 * 	- pDataGpio [in] GPIO typedef for DATA pin. For example: GPIOA
	 *  - pDataPin	[in] DATA pin, for example: GPIO_PIN_11
	 *  - pClkGpio 	[in] GPIO typedef for CLK pin. For example: GPIOA
	 * 	- pClkPin	[in] DATA pin, for example: GPIO_PIN_12
	 * 
	 **/
    void Init(GPIO_TypeDef* pDataGpio, uint16_t pDataPin, GPIO_TypeDef* pClkGpio, uint16_t pClkPin, uint16_t gain = 27);

	/**
	 * @brief Calibrate the zero point value, the calculated value, is used in the getValue function
	 * 
	 * @params:
	 * 	- pTimes [in] Number of readings to calculate the mean (Default: 10)
	 * 
	 **/
    void Tare(int pTimes=10);

	/**
	 * @brief Return the raw value for the zero-point.
	 * 
	 * @returns The zero point raw value
	 * 
	 **/
	uint16_t getZeroPoint(){ return mOffset; }


	/**
	 * @brief Set Scale for the readings
	 * 
	 * 
	 **/ 
	void setScale(float scale){mScale = scale;}

	
	/**
	 * @brief Get the raw value read from the HX711 sensor.
	 * 
	 * @returns Read raw value.
	 *
	 **/
    uint32_t getRawValue();

	/**
	 * @brief Get the calculated value for weight.
	 * 
	 * @params:
	 * 	- pTimes [in] Number of reads to do the mean
	 * 
	 * @returns Get the calculated weight.
	 *
	 **/
    float getValue(int pTimes = 1);

    
private:

	uint8_t mIsInitialized;

    GPIO_TypeDef *mDataGpio, *mClkGpio;
	uint16_t mDataPin, mClkPin;

	// Offset for zero point calibration
	uint32_t mOffset;

	// Scale value
	float mScale;

	// Gain
	uint16_t mGain;

	float getMeanAfterNReads(uint16_t pTimes);

};

#endif //__HX711_H__