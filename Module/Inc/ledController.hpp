#ifndef __led_controller_H
#define __led_controller_H

#include "baseModule.hpp"
#include "led.hpp"
#include <memory>

namespace module
{
	class ledController : public BaseModule<ledController>
	{
	public:
		void update(void) override;
		void lightBinary(uint8_t);
		void toggleBinary(uint8_t);
		void flashBinary(uint8_t, uint16_t);

	private:
		ledController();
		friend class BaseModule<ledController>;

		uint8_t  _num;
		uint8_t  _mode;
		uint16_t _interval;
		
		std::unique_ptr<module::led> _led0;
		std::unique_ptr<module::led> _led1;
		std::unique_ptr<module::led> _led2;
		std::unique_ptr<module::led> _led3;
	};
}

extern "C" void LED_HardwareTest(void);

#endif