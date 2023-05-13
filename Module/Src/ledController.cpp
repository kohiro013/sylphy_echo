#include "ledController.hpp"

// LEDの点灯モード列挙
enum {
	OFF			= 0,
	ON 			= 1,
	TIMER 		= 2,
};

namespace module
{
	ledController::ledController():
		_mode_right{OFF},
		_mode_left{OFF},
		_interval_right{0},
		_interval_left{0}
	{
		setModuleName("LedController");
		_led_right = std::make_unique<module::led>(LED1_GPIO_Port, LED1_Pin);
		_led_left  = std::make_unique<module::led>(LED0_GPIO_Port, LED0_Pin);
	}

	void ledController::update(void) {
		static uint16_t timer_right = 0;
		static uint16_t timer_left = 0;

		// 右LED
		if(_mode_right == TIMER) {
			timer_right ++;
			if(timer_right > _interval_right) {
				timer_right = 0;
				_led_right->toggle();
			} else;
		} else if(_mode_right == ON) {
			timer_right = 0;
			_led_right->turnOn();
		} else {
			timer_right = 0;
			_led_right->turnOff();
		}

		// 左LED
		if(_mode_left == TIMER) {
			timer_left ++;
			if(timer_left > _interval_left) {
				timer_left = 0;
				_led_left->toggle();
			} else;
		} else if(_mode_left == ON) {
			timer_left = 0;
			_led_left->turnOn();
		} else {
			timer_left = 0;
			_led_left->turnOff();
		}
	}

	void ledController::turnOn_Right(void) {
		_mode_right = ON;
		_interval_right = 0;
	}

	void ledController::turnOff_Right(void) {
		_mode_right = OFF;
		_interval_right = 0;
	}

	void ledController::toggle_Right(void) {
		if(_mode_right == OFF) {
			_mode_right = ON;
		} else {
			_mode_right = OFF;
		}
		_interval_right = 0;
	}

	void ledController::flash_Right(uint16_t ms) {
		_mode_right = TIMER;
		_interval_right = ms;
	}

	void ledController::turnOn_Left(void) {
		_mode_left = ON;
		_interval_left = 0;
	}

	void ledController::turnOff_Left(void) {
		_mode_left = OFF;
		_interval_left = 0;
	}

	void ledController::toggle_Left(void) {
		if(_mode_left == OFF) {
			_mode_left = ON;
		} else {
			_mode_left = OFF;
		}
		_interval_left = 0;
	}

	void ledController::flash_Left(uint16_t ms) {
		_mode_left = TIMER;
		_interval_left = ms;
	}

	void ledController::turnOn_All(void) {
		ledController::turnOn_Right();
		ledController::turnOn_Left();
	}

	void ledController::turnOff_All(void)  {
		ledController::turnOff_Right();
		ledController::turnOff_Left();
	}

	void ledController::toggle_All(void) {
		ledController::toggle_Right();
		ledController::toggle_Left();
	}

	void ledController::flash_All(uint16_t ms) {
		ledController::flash_Right(ms);
		ledController::flash_Left(ms);
	}
}