#ifndef __encoder_H
#define __emcoder_H

#include "main.h"
#include "dma.h"
#include "spi.h"
#include "baseModule.hpp"

namespace module
{
	class encoder : public BaseModule<encoder>
	{
	public:
		void update(void) override;
		void callback(void);
		void write2byte(uint16_t, uint16_t);
		uint16_t read2byte(uint16_t, int8_t);
		void initialize(void);
		void resetCountLeft(void);
		void resetCountRight(void);
		float getAngleLeft(void);
		float getAngleRight(void);
		float getAnglerVelocityLeft(void);
		float getAnglerVelocityRight(void);
		void monitorDebug(void);

	private:
		encoder();
		friend class BaseModule<encoder>;

		const int16_t 		_RESOLUTION;
		volatile uint16_t	_address;
		volatile uint16_t 	_value;
		volatile uint16_t	_count_l, _count_r;
		volatile uint16_t 	_count_old_l, _count_old_r;
		volatile float 		_angle_l, _angle_r;
	};
}

extern "C"
{
	void Encoder_Initialize(void);
	void Encoder_Handler(void);
}

#endif