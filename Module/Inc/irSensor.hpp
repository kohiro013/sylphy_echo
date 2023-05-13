#ifndef __ir_sensor_H
#define __ir_sensor_H

#include "main.h"
#include "dma.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "communicate.hpp"

namespace module
{
	class irSensor
	{
	public:
		static irSensor& getInstance(void) {
			static irSensor _instance;
			return _instance;
		}
		void initialize(void);
		void turnOff(void);
		void turnOn(void);
		void setFrequency(float);
		uint16_t getBatteryValue(void);
		int16_t getValue(uint8_t);
		void monitorDebug(void);

	private:
		irSensor();
		irSensor(const irSensor&) = delete;
		irSensor& operator=(const irSensor&) = delete;
		irSensor(irSensor&&) = delete;
		irSensor& operator=(irSensor&&) = delete;

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