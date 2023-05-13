#include "ws2812c.hpp"
#include <stdio.h>

namespace {
	const uint8_t NUM_LED 	 		= 8;	// LEDの数
	const uint8_t NUM_COLOR_BIT 	= 24;	// RGB3色 x 8bit
	const uint8_t COMPARE_LED_OFF	= 16;	// オフ時のカウント値
	const uint8_t COMPARE_LED_ON	= 33;	// オン時のカウント値
	const uint8_t NUM_OFFSET 		= 4;	// LEDの原点位置
	const uint8_t BRIGHTNESS 		= 0x0f; // 明るさ(0~0xff)

	const uint8_t COLOR_MAP[NUM_COLOR][3] = {
		{0x00, 		 0x00, 		 0x00		},	// NONE
		{BRIGHTNESS, 0x00, 		 0x00		},	// RED
		{0x00, 		 BRIGHTNESS, 0x00		},	// GREEN
		{0x00, 		 0x00, 		 BRIGHTNESS	},	// BLUE
		{BRIGHTNESS, BRIGHTNESS, 0x00		},	// YELLOW
		{0x00, 		 BRIGHTNESS, BRIGHTNESS	},	// SKY
		{BRIGHTNESS, 0x00, 		 BRIGHTNESS	},	// PURPLE
		{BRIGHTNESS, BRIGHTNESS, BRIGHTNESS	},	// WHITE
	};

	uint16_t _pattern[NUM_LED * NUM_COLOR_BIT * 2] = {0};	// 後半はリセット時間確保のため0固定
}

void WS2812C_Initialize(void) {
	module::ws2812c::getInstance().initialize();
	module::ws2812c::getInstance().startBootSequence();
}

namespace module
{
	ws2812c::ws2812c():
		_num{0x00},
		_color{0},
		_interval{0}
	{}

	/* ---------------------------------------------------------------
		WS2812Cに点灯指令を送る関数
	--------------------------------------------------------------- */
	void ws2812c::startPWM()
	{
		LL_DMA_ClearFlag_TC2(DMA1);
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);
	}

	/* ---------------------------------------------------------------
		WS2812Cの初期設定関数
	--------------------------------------------------------------- */
	void ws2812c::initialize(void)
	{
		// TIMによるDMAリクエストon
		LL_TIM_EnableDMAReq_CC4(TIM3);
		LL_TIM_EnableAllOutputs(TIM3);

		// 点灯用DMAの設定
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_2);
		LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_2, (uint32_t)_pattern,
			(uint32_t)(&(TIM3->CCR4)), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_2));
		LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, sizeof(_pattern)/sizeof(_pattern[0]));

		// TIMの開始
		LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
		LL_TIM_EnableCounter(TIM3);

		// LEDの消灯
		for(uint8_t i = 0; i < NUM_LED * NUM_COLOR_BIT; i++) {
			_pattern[i] = COMPARE_LED_OFF;
		}
		ws2812c::startPWM();
	}

	/* ---------------------------------------------------------------
		WS2812Cのリセット関数
	--------------------------------------------------------------- */
	void ws2812c::reset( void )
	{
		for(uint8_t i = 0; i < sizeof(_pattern)/sizeof(_pattern[0]); i++) {
			_pattern[i] = 0;
		}
		ws2812c::startPWM();
	}

	/* ---------------------------------------------------------------
		指定のLEDにRGB値を入力する関数
	--------------------------------------------------------------- */
	void ws2812c::setRGB( uint8_t num, uint8_t byte_r, uint8_t byte_g, uint8_t byte_b )
	{
		for(int8_t i = 0; i < 8; i++) {
			_pattern[NUM_COLOR_BIT * num + 7 - i]  = ((byte_g&(0x01 << i)) == (0x01 << i)) ? COMPARE_LED_ON : COMPARE_LED_OFF;
			_pattern[NUM_COLOR_BIT * num + 15 - i] = ((byte_r&(0x01 << i)) == (0x01 << i)) ? COMPARE_LED_ON : COMPARE_LED_OFF;
			_pattern[NUM_COLOR_BIT * num + 23 - i] = ((byte_b&(0x01 << i)) == (0x01 << i)) ? COMPARE_LED_ON : COMPARE_LED_OFF;
		}
	}

	/* ---------------------------------------------------------------
		割り込み内でLEDの点灯時間をカウントする関数
	--------------------------------------------------------------- */
	void ws2812c::update(void)
	{
		static uint16_t 	status_hash = 0;
		uint16_t 			change_hash = 0;
		static uint16_t		timer = 0;

		if(_interval > 0) {
			timer++;
			if(timer > _interval) {
				timer = 0;
				if(status_hash == 0x0000) {
					change_hash = ((uint16_t)_num << 8) + _color;
				} else {
					change_hash = 0x0000;
				}
			} else {
				change_hash = status_hash;
			}
		} else {
			timer = 0;
			change_hash = ((uint16_t)_num << 8) + _color;
		}

		if(status_hash != change_hash) {
			status_hash = change_hash;
			uint8_t num = (status_hash >> 8) &0xff;
			uint8_t color = status_hash &0xff;
			for(uint8_t i = 0; i < NUM_LED; i++) {
				if((num&(1<<i)) == (1<<i)) {
					ws2812c::setRGB((i + NUM_OFFSET) % NUM_LED, COLOR_MAP[color][0], COLOR_MAP[color][1], COLOR_MAP[color][2]);
				} else {
					ws2812c::setRGB((i + NUM_OFFSET) % NUM_LED, COLOR_MAP[BLACK][0], COLOR_MAP[BLACK][1], COLOR_MAP[BLACK][2]);
				}
			}
			ws2812c::startPWM();
		} else;
	}

	/* ---------------------------------------------------------------
		バイナリ指定でLEDを点灯させる関数
	--------------------------------------------------------------- */
	void ws2812c::lightBinary(uint8_t num, uint8_t color)
	{
		_num = num % (1 << NUM_LED);
		_color = color % NUM_COLOR;
		_interval = 0;
	}

	/* ---------------------------------------------------------------
		バイナリ指定でLEDを指定時間点滅させる関数
	--------------------------------------------------------------- */
	void ws2812c::lightBinary( uint8_t num, uint8_t color, uint16_t ms )
	{
		_num = num % (1 << NUM_LED);
		_color = color % NUM_COLOR;
		_interval = ms;
	}

	/* ---------------------------------------------------------------
		パターン点灯関数関数
	--------------------------------------------------------------- */
	void ws2812c::startGamingSequence( void )
	{
		static uint8_t counter = 0;

		while(button::getInstance().getIsPush() == false) {
			ws2812c::setRGB((counter + 0) % NUM_LED, BRIGHTNESS, 0x00, 		0x00	  );
			ws2812c::setRGB((counter + 1) % NUM_LED, 0x00, 		BRIGHTNESS, 0x00	  );
			ws2812c::setRGB((counter + 2) % NUM_LED, 0x00, 		0x00, 		BRIGHTNESS);
			ws2812c::setRGB((counter + 3) % NUM_LED, 0x0f, 		BRIGHTNESS, 0x00	  );
			ws2812c::setRGB((counter + 4) % NUM_LED, 0x00, 		BRIGHTNESS, BRIGHTNESS);
			ws2812c::setRGB((counter + 5) % NUM_LED, BRIGHTNESS, 0x00, 		BRIGHTNESS);
			ws2812c::setRGB((counter + 6) % NUM_LED, 0x00, 		0x00, 		0x00	  );
			ws2812c::setRGB((counter + 7) % NUM_LED, BRIGHTNESS, BRIGHTNESS, BRIGHTNESS);

			ws2812c::startPWM();
			LL_mDelay(100);
			counter = (counter + 1) % NUM_LED;
		}
		ws2812c::lightBinary(0xff, BLACK);
	}

	void ws2812c::startBootSequence( void )
	{
		for(uint8_t i = 1; i < NUM_LED; i++) {
			ws2812c::lightBinary(1<<i, YELLOW);
			LL_mDelay(50);
		}
		for(uint8_t i = 1; i < NUM_LED; i++) {
			ws2812c::lightBinary(1<<(NUM_LED-i), YELLOW);
			LL_mDelay(50);
		}
		ws2812c::lightBinary(0xff, BLACK);
	}

	void ws2812c::startGoalSequence( void )
	{
		for(uint8_t i = 1; i < NUM_COLOR; i++) {
			ws2812c::lightBinary(0xff, i);		LL_mDelay(100);
			ws2812c::lightBinary(0xff, BLACK);	LL_mDelay(100);
		}
		ws2812c::lightBinary(0xff, BLACK);
	}

	void ws2812c::startErrorSequence( void )
	{
		while(1) {
			for(uint8_t i = 0; i < NUM_LED; i++) {
				ws2812c::lightBinary(1<<i, PURPLE);
				uint32_t tick = interrupt::getInstance().getGlobalTimer();
				while(interrupt::getInstance().getGlobalTimer() - tick < 100) {
					if(button::getInstance().getIsPush() != false) {
						goto END;
					} else;
				}
			}
		}
		END:
			ws2812c::lightBinary(0xff, BLACK);
	}

	void ws2812c::startDeadSequence( void )
	{
		while(button::getInstance().getIsPush() == false) {
			ws2812c::lightBinary(0x55, RED);	LL_mDelay(100);
			ws2812c::lightBinary(0xaa, RED);	LL_mDelay(100);
		}
		ws2812c::lightBinary(0xff, BLACK);
	}

}