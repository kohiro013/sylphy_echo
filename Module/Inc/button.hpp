#ifndef __button_H
#define __button_H

#include "main.h"
#include "baseModule.hpp"

namespace module
{
	class button : public BaseModule<button>
	{
	public:
		void update(void) override;
		int8_t getIsPush(void);
		int8_t getIsFrontSensor(void);
		int8_t waitFrontSensor(void);

	private:
		button();
		friend class BaseModule<button>;

		volatile uint32_t _timer_switch;
		volatile uint8_t _is_switch;

		volatile uint32_t _timer_sensor;
		volatile uint8_t _is_sensor;
	};
}

#endif