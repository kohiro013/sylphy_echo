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
		
		void turnOn_Right(void);
		void turnOff_Right(void);
		void toggle_Right(void);
		void flash_Right(uint16_t);

		void turnOn_Left(void);
		void turnOff_Left(void);
		void toggle_Left(void);
		void flash_Left(uint16_t);

		void turnOn_All(void);
		void turnOff_All(void);
		void toggle_All(void);
		void flash_All(uint16_t);

	private:
		ledController();
		friend class BaseModule<ledController>;

		uint8_t  _mode_right;
		uint8_t  _mode_left;
		uint16_t _interval_right;
		uint16_t _interval_left;
		
		std::unique_ptr<module::led> _led_right;
		std::unique_ptr<module::led> _led_left;
	};
}

#endif