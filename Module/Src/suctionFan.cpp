#include "suctionFan.hpp"
#include <stdio.h>

namespace {
	const uint32_t AUTORELOAD = LL_TIM_GetAutoReload(TIM2) + 1;
}

void SuctionFan_Initialize(void) {
	module::suctionFan::getInstance().initialize();
}

namespace module
{
	suctionFan::suctionFan() {}

	/* ---------------------------------------------------------------
		吸引ファンの初期設定関数
	--------------------------------------------------------------- */
	void suctionFan::initialize(void)
	{
		LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH3);
		LL_TIM_EnableCounter(TIM2);
	}

	/* ---------------------------------------------------------------
		吸引ファンを回転させる関数
	--------------------------------------------------------------- */
	void suctionFan::start(float duty)
	{
		if(0.f < duty && duty < 1.f) {
			LL_TIM_OC_SetCompareCH3(TIM2, (uint32_t)(AUTORELOAD * duty) - 1);
		} else {
			suctionFan::stop();
		}
	}

	/* ---------------------------------------------------------------
		吸引ファンの回転を停止する関数
	--------------------------------------------------------------- */
	void suctionFan::stop(void)
	{
		LL_TIM_OC_SetCompareCH3(TIM2, 0);
	}

}