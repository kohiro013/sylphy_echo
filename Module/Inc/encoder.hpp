#ifndef __encoder_H
#define __encoder_H

#include "main.h"
#include "dma.h"
#include "spi.h"
#include "baseModule.hpp"
#include "communicate.hpp"

namespace module
{
	class encoder : public BaseModule<encoder>
	{
	public:
		void update(void) override;
		void callback(void);
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

		const int16_t 	_RESOLUTION;
		uint16_t		_address;
		uint16_t 		_value;
		uint16_t		_count_l, _count_r;
		uint16_t 		_count_old_l, _count_old_r;
		float 			_angle_l, _angle_r;

		void write2byte(uint16_t, uint16_t);
	};
}

extern "C"
{
	void Encoder_Initialize(void);
	void Encoder_Callback(void);
}

#endif