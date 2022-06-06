#include "interrupt.hpp"
#include "ledController.hpp"

#define HANDLE 		(TIM5)
#define TIMER_COUNT (LL_TIM_GetCounter(HANDLE))
#define TIMER_LOAD	(LL_TIM_GetAutoReload(HANDLE) + 1)
#define TIMER_PSC 	(LL_TIM_GetPrescaler(HANDLE) + 1)

namespace module
{
	interrupt::interrupt():
		 _global_timer{0},
		_counter{0},
		_duty{0},
		_duty_max{0},
		_boot_time{0.f}
	{}

	void interrupt::preProcess(void) {
		_global_timer++;
		_counter = TIMER_COUNT;
		_boot_time = static_cast<float>(_global_timer) * 0.001f;
	}

	void interrupt::postProcess(void) {
		_duty = static_cast<uint16_t>(std::min(TIMER_COUNT - _counter, 
											   TIMER_COUNT - _counter + TIMER_LOAD)) * 1000 / TIMER_LOAD;
		_duty_max = std::max(_duty_max, _duty);
	}

	uint32_t interrupt::getElapsedUsec(void) const {
		return  _global_timer*1000 + static_cast<float>(TIMER_COUNT) / TIMER_PSC / TIMER_LOAD * 1000000;
	}

	uint32_t interrupt::getGlobalTimer(void) const {
		return _global_timer;
	}

	int32_t interrupt::getDuty(void) const {
		return _duty;
	}

	int32_t interrupt::getMaxDuty(void) const {
		return _duty_max;
	}
	
	float interrupt::getBootTime(void) const {
		return _boot_time;
	}

	void interrupt::wait1ms(uint32_t ms) const {
		volatile uint32_t tmp_timer = _global_timer;
		while(_global_timer - tmp_timer < ms);
	}
}

void Interrupt_Main(void) {
	module::ledController::getInstance().cycle();
}

void Interrupt_Initialize(void) {
	LL_TIM_EnableIT_UPDATE(HANDLE);
	LL_TIM_EnableCounter(HANDLE);
}

void Interrupt_PreProcess(void) {
	module::interrupt::getInstance().preProcess();
}

void Interrupt_PostProcess(void) {
	module::interrupt::getInstance().postProcess();
}