#ifndef __ir_sensor_H
#define __ir_sensor_H

#include "main.h"
#include "dma.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include"singleton.hpp"
#include "communicate.hpp"

namespace module
{
	class irSensor : public Singleton<irSensor>
	{
	public:
		void initialize(void);
		void turnOff(void);
		void turnOn(void);
		void setFrequency(float);
		uint16_t getBatteryValue(void);
		int16_t getValue(uint8_t);
		void monitorDebug(void);

	private:
		irSensor();
		friend class Singleton<irSensor>;

		volatile uint32_t _led_on_pattern[12];
		volatile uint32_t _led_off_pattern;
		volatile uint16_t _adc_value[12];
	};
}

extern "C"
{
	void IRSensor_Initialize(void);
}

#endif