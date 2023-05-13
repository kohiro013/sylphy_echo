#include "irSensor.hpp"
#include <stdio.h>

enum {
	BATTERY_0	= 0,
	BATTERY_1	= 1,
	LED_FL_OFF 	= 4,
	LED_FL_ON 	= 5,
	LED_SL_OFF 	= 8,
	LED_SL_ON 	= 9,
	LED_SR_OFF 	= 6,
	LED_SR_ON 	= 7,
	LED_FR_OFF 	= 2,
	LED_FR_ON 	= 3,
	BATTERY_2	= 10,
	BATTERY_3	= 11,
	NUM_ADC,
};

void IRSensor_Initialize(void) {
	module::irSensor::getInstance().initialize();
}

namespace module
{
	irSensor::irSensor():
		_led_on_pattern{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		_led_off_pattern{0x00F00000},
		_adc_value{}
	{}

	void irSensor::turnOff(void) {
		for(uint8_t i = 0; i < NUM_ADC; i++) {
			_led_on_pattern[i] = 0;
		}
	}

	void irSensor::turnOn(void) {
		irSensor::turnOff();
		_led_on_pattern[LED_FL_OFF] = LED_FL_Pin;
		_led_on_pattern[LED_SL_OFF] = LED_SL_Pin;
		_led_on_pattern[LED_SR_OFF] = LED_SR_Pin;
		_led_on_pattern[LED_FR_OFF] = LED_FR_Pin;
	}

	void irSensor::setFrequency(float hz) {
		LL_TIM_SetAutoReload(TIM1, (LL_TIM_GetAutoReload(TIM1) + 1) / 10.f * hz - 1);
	}

	void irSensor::initialize(void) {
		// TIMによるDMAリクエストon
		LL_TIM_EnableDMAReq_CC1(TIM1);
		LL_TIM_EnableDMAReq_CC2(TIM1);
		LL_TIM_EnableAllOutputs(TIM1);

		// AD変換用DMAの設定
		LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_4);
		LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_4, LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
			(uint32_t)_adc_value, LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_4));
		LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_4, sizeof(_adc_value)/sizeof(_adc_value[0]));
		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_4);

		// 点灯用DMAの設定
		LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);
		LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_1, (uint32_t)_led_on_pattern,
			(uint32_t)(&(GPIOA->BSRR)), LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_1));
		LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_1, sizeof(_led_on_pattern)/sizeof(_led_on_pattern[0]));
		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);

		// 消灯用DMAの設定
		LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
		LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_2, (uint32_t)(&_led_off_pattern),
			(uint32_t)(&(GPIOA->BSRR)), LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_2));
		LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, 1);
		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);

		// AD変換の開始
		LL_ADC_Enable(ADC1);
		LL_TIM_EnableCounter(TIM1);
		LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
		LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
		LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);
	}

	uint16_t irSensor::getBatteryValue(void) {
		return (_adc_value[BATTERY_0] + _adc_value[BATTERY_1] + _adc_value[BATTERY_2] + _adc_value[BATTERY_3]) / 4;
	}

	int16_t irSensor::getValue(uint8_t dir) {
		switch (dir) {
			case FRONT_LEFT:	return _adc_value[LED_FL_ON] - _adc_value[LED_FL_OFF];	break;
			case SIDE_LEFT:		return _adc_value[LED_SL_ON] - _adc_value[LED_SL_OFF];	break;
			case SIDE_RIGHT:	return _adc_value[LED_SR_ON] - _adc_value[LED_SR_OFF];	break;
			case FRONT_RIGHT:	return _adc_value[LED_FR_ON] - _adc_value[LED_FR_OFF];	break;
			default: 			return -1;												break;
		}
	}

	void irSensor::monitorDebug(void) {
		irSensor::turnOn();
		while(Communicate_ReceiceDMA() != 0x1b) {
			printf( "%5d (%4d - %4d), %5d (%4d - %4d), %5d (%4d - %4d), %5d (%4d - %4d)\r\n",
					_adc_value[LED_FL_ON] - _adc_value[LED_FL_OFF], _adc_value[LED_FL_ON], _adc_value[LED_FL_OFF],
					_adc_value[LED_SL_ON] - _adc_value[LED_SL_OFF], _adc_value[LED_SL_ON], _adc_value[LED_SL_OFF],
					_adc_value[LED_SR_ON] - _adc_value[LED_SR_OFF], _adc_value[LED_SR_ON], _adc_value[LED_SR_OFF],
					_adc_value[LED_FR_ON] - _adc_value[LED_FR_OFF], _adc_value[LED_FR_ON], _adc_value[LED_FR_OFF] );
/*
			printf("%5d", _adc_value[0]);
			for(uint8_t i = 1; i < sizeof(_adc_value) / sizeof(_adc_value[0]); i++) {
				printf(" ,%5d", _adc_value[i]);
			}
			printf("\r\n");
*/			LL_mDelay(100);
		}
		irSensor::turnOff();
	}
}
