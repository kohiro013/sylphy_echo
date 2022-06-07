#ifndef __interrupt_H
#define __interrupt_H

#include "main.h"
#include "tim.h"

namespace module
{
	class interrupt
	{
	public:
		static interrupt& getInstance(void) {
			static interrupt _instance;
			return _instance;
		}
		void 		preProcess(void);
		void 		postProcess(void);
		uint32_t 	getElapsedUsec(void) const;
		uint32_t    getGlobalTimer(void) const;
		int32_t     getDuty(void) const;
		int32_t     getMaxDuty(void) const;
		void        wait1ms(uint32_t) const;

	private:
		interrupt();
		interrupt(const interrupt&) = delete;
		interrupt& operator=(const interrupt&) = delete;
		interrupt(interrupt&&) = delete;
		interrupt& operator=(interrupt&&) = delete;

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