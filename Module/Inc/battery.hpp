#ifndef __battery_H
#define __battery_H

#include "main.h"
#include "singleton.hpp"

namespace module
{
	class battery : public Singleton<battery>
	{
	public:
		float 	getVoltage(void);
		void 	limiterVoltage(void);

	private:
		battery();
		friend class Singleton<battery>;
	};
}

extern "C"
{
	void Battery_LimiterVoltage(void);
}

#endif