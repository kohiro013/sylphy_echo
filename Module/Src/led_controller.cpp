#include "led_controller.hpp"
#include "interrupt.hpp"

// LEDの点灯モード列挙
enum {
	NORMAL 			= 0,
	TIMER 			= 1,
	KNIGHT_RIDER	= 2,
};

namespace module
{
	led_controller::led_controller():
		_num{0x00},
		_mode{NORMAL},
		_interval{0}
	{
		_led0 = std::make_unique<module::led>(LED0_GPIO_Port, LED0_Pin);
		_led1 = std::make_unique<module::led>(LED1_GPIO_Port, LED1_Pin);
		_led2 = std::make_unique<module::led>(LED2_GPIO_Port, LED2_Pin);
		_led3 = std::make_unique<module::led>(LED3_GPIO_Port, LED3_Pin);
	}

	void led_controller::updateTimer(void) {
		static uint16_t timer = 0;

		if( _mode == TIMER ) {
			timer ++;
			if( timer > _interval ) {
				timer = 0;
				if((_num &0x01) == 0x01)	_led0->toggle();
				else						_led0->turnOff();
				if((_num &0x02) == 0x02)	_led1->toggle();
				else						_led1->turnOff();
				if((_num &0x04) == 0x04)	_led2->toggle();
				else						_led2->turnOff();
				if((_num &0x08) == 0x08)	_led3->toggle();
				else						_led3->turnOff();
			} else;
		} else {
			if((_num &0x01) == 0x01)	_led0->turnOn();
			else						_led0->turnOff();
			if((_num &0x02) == 0x02)	_led1->turnOn();
			else						_led1->turnOff();
			if((_num &0x04) == 0x04)	_led2->turnOn();
			else						_led2->turnOff();
			if((_num &0x08) == 0x08)	_led3->turnOn();
			else						_led3->turnOff();
		}
	}

	void led_controller::lightBinary(uint8_t num) {
		_mode      = NORMAL;
		_interval  = 0;
		_num       = num;
	}

	void led_controller::toggleBinary(uint8_t num) {
		_mode      = NORMAL;
		_interval  = 0;
		_num      ^= num;
	}

	void led_controller::flashBinary(uint8_t num, uint16_t ms) {
		_mode      = TIMER;
		_interval  = ms;
		_num       = num;
	}
}

void LED_HardwareTest(void) {
	module::led_controller::getInstance().toggleBinary(0x0f);
	module::interrupt::getInstance().wait1ms(500);
}