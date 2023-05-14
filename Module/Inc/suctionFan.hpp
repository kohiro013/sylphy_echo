#ifndef __suction_fan_H
#define __suction_fan_H

#include "main.h"
#include "tim.h"
#include "singleton.hpp"

namespace module
{
	class suctionFan : public Singleton<suctionFan>
	{
	public:
		void initialize(void);
		void start(float);
		void stop(void);

	private:
		suctionFan();
		friend class Singleton<suctionFan>;
	};
}

extern "C"
{
	void SuctionFan_Initialize(void);
}


#endif