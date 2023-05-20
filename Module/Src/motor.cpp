#include "motor.hpp"
#include <stdio.h>
#include "arm_math.h"

#define TIM_HANDLE 	(TIM4)
// モータの向き設定
#define MOT_SET_COMPARE_L_FORWARD(x)	LL_TIM_OC_SetCompareCH2(TIM_HANDLE, x)
#define MOT_SET_COMPARE_L_REVERSE(x)	LL_TIM_OC_SetCompareCH1(TIM_HANDLE, x)
#define MOT_SET_COMPARE_R_FORWARD(x)	LL_TIM_OC_SetCompareCH4(TIM_HANDLE, x)
#define MOT_SET_COMPARE_R_REVERSE(x)	LL_TIM_OC_SetCompareCH3(TIM_HANDLE, x)

void Motor_Initialize(void) {
	module::motor::getInstance().initialize();
}

namespace module
{
	motor::motor():
		_AUTORELOAD{LL_TIM_GetAutoReload(TIM_HANDLE) + 1},
		_DUTY_MIN{10},
		_DUTY_MAX{950}
	 {}

	/* ---------------------------------------------------------------
		モータ用のタイマーを開始する関数
	--------------------------------------------------------------- */
	void motor::initialize(void)
	{
		LL_TIM_CC_EnableChannel(TIM_HANDLE, LL_TIM_CHANNEL_CH1);
		LL_TIM_CC_EnableChannel(TIM_HANDLE, LL_TIM_CHANNEL_CH2);
		LL_TIM_CC_EnableChannel(TIM_HANDLE, LL_TIM_CHANNEL_CH3);
		LL_TIM_CC_EnableChannel(TIM_HANDLE, LL_TIM_CHANNEL_CH4);
		LL_TIM_EnableCounter(TIM_HANDLE);
	}

	/* ---------------------------------------------------------------
		モータの動作周波数を設定する関数(DRV8835は100kHzまでなので使用不可)
	--------------------------------------------------------------- */
/*	void motor::setFrequency(uint16_t khz)
	{
		motor::stop();
		LL_TIM_SetAutoReload(TIM4, (PCLK / (khz * 1000)) - 1);
	}
*/
	/* ---------------------------------------------------------------
		モータのの回転を止める関数
	--------------------------------------------------------------- */
	void motor::stop(void)
	{
		MOT_SET_COMPARE_R_FORWARD(0xffff);
		MOT_SET_COMPARE_R_REVERSE(0xffff);
		MOT_SET_COMPARE_L_FORWARD(0xffff);
		MOT_SET_COMPARE_L_REVERSE(0xffff);
	}

	/* ---------------------------------------------------------------
		右モータを指定のDuty(0~1)で回転させる関数
	--------------------------------------------------------------- */
	void motor::setDuty_Right(int16_t duty)
	{
		uint32_t	pulse = 0;

		if(std::abs(duty) > _DUTY_MAX) {
			pulse = static_cast<uint32_t>(_AUTORELOAD * _DUTY_MAX / 1000) - 1;
		} else if(std::abs(duty) < _DUTY_MIN) {
			pulse = static_cast<uint32_t>(_AUTORELOAD * _DUTY_MIN / 1000) - 1;
		} else {
			pulse = static_cast<uint32_t>(_AUTORELOAD * std::abs(duty) / 1000) - 1;
		}

		if(duty > 0) {
			MOT_SET_COMPARE_R_FORWARD(pulse);
			MOT_SET_COMPARE_R_REVERSE(0);
		} else if(duty < 0) {
			MOT_SET_COMPARE_R_FORWARD(0);
			MOT_SET_COMPARE_R_REVERSE(pulse);
		} else {
			MOT_SET_COMPARE_R_FORWARD(0);
			MOT_SET_COMPARE_R_REVERSE(0);
		}
	}

	/* ---------------------------------------------------------------
		左モータを指定のDuty(0~1)で回転させる関数
	--------------------------------------------------------------- */
	void motor::setDuty_Left(int16_t duty)
	{
		uint32_t	pulse = 0;

		if(std::abs(duty) > _DUTY_MAX) {
			pulse = static_cast<uint32_t>(_AUTORELOAD * _DUTY_MAX / 1000) - 1;
		} else if(std::abs(duty) < _DUTY_MIN) {
			pulse = static_cast<uint32_t>(_AUTORELOAD * _DUTY_MIN / 1000) - 1;
		} else {
			pulse = static_cast<uint32_t>(_AUTORELOAD * std::abs(duty) / 1000) - 1;
		}

		if(duty > 0.f) {
			MOT_SET_COMPARE_L_FORWARD(pulse);
			MOT_SET_COMPARE_L_REVERSE(0);
		} else if(duty < 0.f) {
			MOT_SET_COMPARE_L_FORWARD(0);
			MOT_SET_COMPARE_L_REVERSE(pulse);
		} else {
			MOT_SET_COMPARE_L_FORWARD(0);
			MOT_SET_COMPARE_L_REVERSE(0);
		}
	}
}