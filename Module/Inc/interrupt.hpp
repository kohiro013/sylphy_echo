#ifndef __interrupt_H
#define __interrupt_H

#include "main.h"
#include "tim.h"
#include "singleton.hpp"

namespace module
{
	class interrupt : public Singleton<interrupt>
	{
	public:
		void 		preProcess(void);
		void 		postProcess(void);
		uint32_t 	getElapsedUsec(void) const;
		uint32_t    getGlobalTimer(void) const;
		int32_t     getDuty(void) const;
		int32_t     getMaxDuty(void) const;
		void        wait1ms(uint32_t) const;

	private:
		interrupt();
		friend class Singleton<interrupt>;

		volatile uint32_t   _global_timer;
		volatile uint32_t   _counter;
		volatile int32_t    _duty;
		volatile int32_t    _duty_max;
	};
}

extern "C"
{
	void Interrupt_Handler(void);
	void Interrupt_Initialize(void);
}

#endif