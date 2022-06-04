#include "interrupt.hpp"
#include "led_controller.hpp"

namespace {
	TIM_TypeDef *const HANDLE = TIM5;
	const uint32_t PCLK = 48000000;
	const uint32_t TIMER_COUNT = LL_TIM_GetCounter(HANDLE);
	const uint32_t TIMRE_LOAD = LL_TIM_GetAutoReload(HANDLE) + 1;
	const uint32_t TIMER_PSC = LL_TIM_GetPrescaler(HANDLE) + 1;
}

namespace module
{
	interrupt::interrupt():
		 _global_timer{0},
		_counter{0},
		_dury{0},
		_duty_max{0},
		_boot_time{0.f}
	{}

	void interrupt::preProcess(void) {
		_global_timer++;
		_counter = TIMER_COUNT;
		_boot_time = static_cast<float>(_global_timer) * 0.001f;
	}

	void interrupt::postProcess(void) {
		_dury = static_cast<uint16_t>(std::min(TIMER_COUNT - _counter, 
												TIMER_COUNT - _counter + TIMRE_LOAD)) * 1000 / TIMER_PSC;
		_duty_max = std::max(_duty_max, _dury);
	}

	uint32_t interrupt::getGlobalTimre(void) const {
		return _global_timer;
	}

	int32_t interrupt::getDuty(void) const {
		return _dury;
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
	module::led_controller::getInstance().updateTimer();
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