#ifndef __led_controller_H
#define __led_controller_H

#include "led.hpp"
#include <memory>

namespace module
{
	class led_controller
	{
	public:
		static led_controller& getInstance(void) {
			static led_controller _instance;
			return _instance;
		}
		void updateTimer(void);
		void lightBinary(uint8_t);
		void toggleBinary(uint8_t);
		void flashBinary(uint8_t, uint16_t);

	private:
		led_controller();
		led_controller(const led_controller&) = delete;
		led_controller& operator=(const led_controller&) = delete;
		led_controller(led_controller&&) = delete;
		led_controller& operator=(led_controller&&) = delete;

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