#include "dht11.h"

static void delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);
}

static void set_pin_output(void) {
    GPIO_InitTypeDef cfg = {0, .Pin = DHT11_GPIO_PIN, .Mode = GPIO_MODE_OUTPUT_PP, .Speed = GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(DHT11_GPIO_PORT, &cfg);
}

static void set_pin_input(void) {
    GPIO_InitTypeDef cfg = {0, .Pin = DHT11_GPIO_PIN, .Mode = GPIO_MODE_INPUT, .Pull = GPIO_NOPULL};
    HAL_GPIO_Init(DHT11_GPIO_PORT, &cfg);
}

static uint8_t read_byte(void) {
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET);
        delay_us(40);
        if (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET) byte |= (1 << (7 - i));
        while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET);
    }
    return byte;
}

DHT11_Data DHT11_Read(void) {
    DHT11_Data result = {0};
    uint8_t raw[5] = {0};

    set_pin_output();
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET);
    HAL_Delay(18); // Start pulse
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);
    delay_us(30);
    set_pin_input();

    // Wait for sensor response
    uint16_t timeout = 0;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET) if (++timeout > 100) return result;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET);
    while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET);

    for (int i = 0; i < 5; i++) raw[i] = read_byte();
    if ((uint8_t)(raw[0] + raw[1] + raw[2] + raw[3]) == raw[4]) {
        result.humidity = (float)raw[0] + (float)raw[1] * 0.1f;
        result.temperature = (float)raw[2] + (float)raw[3] * 0.1f;
        result.valid = 1;
    }
    return result;
}
