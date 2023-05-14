#ifndef __ws2812c_H
#define __ws2812c_H

#include "main.h"
#include "dma.h"
#include "tim.h"
#include "baseModule.hpp"
#include "button.hpp"

enum {	// LEDの色列挙
	BLACK	= 0,
	RED		= 1,
	GREEN	= 2,
	BLUE	= 3,
	YELLOW	= 4,
	SKY 	= 5,
	PURPLE	= 6,
	WHITE 	= 7,
	NUM_COLOR,
};

namespace module
{
	class ws2812c : public BaseModule<ws2812c>
	{
	public:
		void 	update(void) override;
		void 	initialize(void);
		void 	lightBinary(uint8_t, uint8_t);
		void 	lightBinary(uint8_t, uint8_t, uint16_t);

		void 	startGamingSequence(void);
		void 	startBootSequence(void);
		void 	startGoalSequence(void);
		void 	startErrorSequence(void);
		void 	startDeadSequence(void);

	private:
		ws2812c();
		friend class BaseModule<ws2812c>;

		uint8_t 	_num;
		uint8_t 	_color;
		uint16_t 	_interval;

		void 	startPWM(void);
		void 	reset(void);
		void 	setRGB(uint8_t, uint8_t, uint8_t, uint8_t);
	};
}

extern "C"
{
	void WS2812C_Initialize(void);
	void WS2812C_StartBootSequence(void);
}


#endif