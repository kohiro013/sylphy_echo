#include "led.hpp"

led::led(GPIO_TypeDef *_port, uint16_t _pin) : port(_port), pin(_pin)
{
}

void led::turnOn(void)
{
    LL_GPIO_SetOutputPin(this->port, this->pin);
}

void led::turnOff(void)
{
    LL_GPIO_ResetOutputPin(this->port, this->pin);
}

void led::toggle(void)
{
    LL_GPIO_TogglePin(this->port, this->pin);
}

void led_c_wrapper_example(void)
{
    led led0(LED0_GPIO_Port, LED0_Pin);
    led led1(LED1_GPIO_Port, LED1_Pin);
    led led2(LED2_GPIO_Port, LED2_Pin);
    led led3(LED3_GPIO_Port, LED3_Pin);

    for (;;)
    {
        led0.toggle();
        led1.toggle();
        led2.toggle();
        led3.toggle();
        LL_mDelay(500);
    }
}