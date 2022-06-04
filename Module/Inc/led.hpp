#ifndef __led_H
#define __led_H

#include "main.h"
#include "gpio.h"

namespace module
{
	class led
	{
	public:
		led(GPIO_TypeDef*, uint16_t);
		void turnOn(void);
		void turnOff(void);
		void toggle(void);

	private:
		GPIO_TypeDef *_port;
		uint16_t _pin;
	};
}

#endif