#include "led.hpp"

namespace module
{
	led::led(GPIO_TypeDef *port, uint16_t pin) : _port(port), _pin(pin)
	{
		LL_GPIO_ResetOutputPin(_port, _pin);
	}

	void led::turnOn(void) {
		LL_GPIO_SetOutputPin(_port, _pin);
	}

	void led::turnOff(void) {
		LL_GPIO_ResetOutputPin(_port, _pin);
	}

	void led::toggle(void) {
		LL_GPIO_TogglePin(_port, _pin);
	}
}