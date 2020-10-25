# HX711
HX711 driver for STM32

## Important!

The Clock of the GPIO used by the HX711 shall be initialized **before** calling Init()

The drivers needs that a "delay_us" is implemented. For the STM32F103C8, using the TIM2, this function can be used (TIM2 has to be initialized).

```
void delay_us(uint32_t us){

  uint32_t Tim2Freq = HAL_RCC_GetPCLK1Freq();

  if ( (RCC->CFGR & RCC_CFGR_PPRE1) != 0){
    Tim2Freq *= 2;
  }

  htim2.Instance->CNT = 0;

  uint32_t match = us*Tim2Freq/1000000;
  if(match > ((1<<15)-1))
    return;
  while(htim2.Instance->CNT <= match);

}
```

## Example of use:

```

#include <hx711.h>


HX711 hx;

void main(){

    // Initialize the GPIOs
    hx.Init(GPIOB, GPIO_PIN_14, GPIOB, GPIO_PIN_15);

    // Leave the sensor without load (0 grams)
    // And let it calculate the zero point using 20 samples
    hx.Tare(20);

    // Set the scale, to calculte this value, one should first 
    // find the relationship between the raw values and the actual weight
    hx.setScale(300.0f);

    float weight; 

    while(1){
        //...
        weight = hx.getValue(2);
        //...
    

    }

}

```
