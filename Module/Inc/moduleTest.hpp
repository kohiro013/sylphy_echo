#ifndef __module_test_H
#define __module_test_H

#include "singleton.hpp"
#include "communicate.hpp"
#include "interrupt.hpp"
#include "imu.hpp"
#include "encoder.hpp"
#include "irSensor.hpp"
#include "battery.hpp"
#include "motor.hpp"

namespace module
{
	class moduleTest : public Singleton<moduleTest>
	{
	public:
		void run(void);

	private:
		moduleTest();
		friend class Singleton<moduleTest>;
	};
}

#endif