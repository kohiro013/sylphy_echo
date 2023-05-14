#include "battery.hpp"
#include "irSensor.hpp"
#include "ws2812c.hpp"

namespace {
	const float VOLTAGE_R1 		= 20.0f;	// バッテリ電源分圧抵抗値(上段)
	const float VOLTAGE_R2 		= 10.0f;	// バッテリ電源分圧抵抗値(下段)
	const float ADC_REFERENCE 	= 3.0f;		// AD変換の基準電圧
	const float ADC_RESOLUTION	= 4096.f;	// AD変換の分解能
	const float LIMIT_VOLTAGE 	= 6.8f; 	// 制限電圧値
}

void Battery_LimiterVoltage(void) {
	module::battery::getInstance().limiterVoltage();
}

namespace module
{
	battery::battery() {}

	/* ---------------------------------------------------------------
		バッテリの電圧を取得する関数
	--------------------------------------------------------------- */
	float battery::getVoltage(void)
	{
		return ADC_REFERENCE * ((VOLTAGE_R1 + VOLTAGE_R2) / VOLTAGE_R2) * static_cast<float>(irSensor::getInstance().getBatteryValue()) / ADC_RESOLUTION;
	}


	/* ---------------------------------------------------------------
		バッテリの電圧制限関数
	--------------------------------------------------------------- */
	void battery::limiterVoltage( void )
	{
		float voltage_average = 0.f;

		for(uint8_t i = 0; i < 10; i++) {
			LL_mDelay(10);
			voltage_average += battery::getVoltage();
		}
		voltage_average /= 10;

		if(voltage_average < LIMIT_VOLTAGE) {
			ws2812c::getInstance().startDeadSequence();
		} else;
	}
}