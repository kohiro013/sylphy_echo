#ifndef __motor_H
#define __motor_H

#include "main.h"
#include "tim.h"
#include "singleton.hpp"

namespace module
{
	class motor : public Singleton<motor>
	{
	public:
		void initialize(void);
		void stop(void);
		void setDuty_Right(int16_t);
		void setDuty_Left(int16_t);

	private:
		motor();
		friend class Singleton<motor>;

		const uint32_t 		_AUTORELOAD;
		const uint16_t		_DUTY_MIN;
		const uint16_t 		_DUTY_MAX;
	};
}

extern "C"
{
	void Motor_Initialize(void);
}

#endif