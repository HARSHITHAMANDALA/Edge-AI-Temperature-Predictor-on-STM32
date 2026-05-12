#ifndef DHT11_H
#define DHT11_H

#include "stm32f4xx_hal.h"

// Hardware Mapping - PA1 matches your CubeMX setup
#define DHT11_GPIO_PORT   GPIOA
#define DHT11_GPIO_PIN    GPIO_PIN_1

// Timer handle for microsecond delays
extern TIM_HandleTypeDef htim2;

typedef struct {
    float    temperature;   // °C
    float    humidity;      // % RH
    uint8_t  valid;         // 1 = Success, 0 = Fail
} DHT11_Data;

DHT11_Data DHT11_Read(void);

#endif
