#include "button.hpp"
#include "irSensor.hpp"

#define BUTTON_IsPush()		LL_GPIO_IsInputPinSet(SWITCH_GPIO_Port, SWITCH_Pin)

namespace {
	// プッシュスイッチの設定
	const uint16_t SW_SHORT_PUSH 	= 10;	// 押下判定の閾値
	const uint16_t SW_LONG_PUSH 	= 500; 	// 長押し判定の閾値

	// センサースイッチの設定
	const uint16_t SW_THRESHOLD 	= 800;	// スイッチ判定の閾値
	const uint16_t SW_TIME 			= 300;	// スイッチ判定の時間閾値
}

namespace module
{
	button::button():
		_timer_switch{0},
		_is_switch{0},

		_timer_sensor{0},
		_is_sensor{0}
	{}

	/* ---------------------------------------------------------------
		割り込み内でスイッチが反応している時間をカウントする関数
	--------------------------------------------------------------- */
	void button::update(void) {
		// プッシュスイッチが押下されている時間をカウントする
		if(BUTTON_IsPush() == true) {
			_timer_switch++;
		} else {
			_timer_switch = 0;
		}

		// 両方の前センサが反応している時間をカウントする
		if(irSensor::getInstance().getValue(FRONT_RIGHT) > SW_THRESHOLD || irSensor::getInstance().getValue(FRONT_RIGHT) > SW_THRESHOLD) {
			if(_is_sensor == -1) {
				_timer_sensor ++;
			} else {
				_timer_sensor = 0;
			}
		} else {
			_timer_sensor = 0;
			_is_sensor = -1;
		}
	}

	/* ---------------------------------------------------------------
		スイッチが押された時間が指定した時間を超えるとtrueを返す関数
	--------------------------------------------------------------- */
	int8_t button::getIsPush(void)
	{
		// 長押し判定
		if(BUTTON_IsPush() == true) {
			if(_timer_switch > SW_LONG_PUSH) {
				return 2;
			} else {
				_is_switch = 0;
			}
		// 押下判定
		} else if(_timer_switch > SW_SHORT_PUSH) {
			_is_switch = 1;
			// チャタリング防止
			while(BUTTON_IsPush() == true) {}
			LL_mDelay(SW_SHORT_PUSH);
		} else {
			_is_switch = 0;
		}
		
		return _is_switch;
	}

	/* ---------------------------------------------------------------
		両方の前センサが反応するまで待機
	--------------------------------------------------------------- */
	int8_t button::getIsFrontSensor(void) {
		if( _timer_sensor > SW_TIME ) {
			if(irSensor::getInstance().getValue(FRONT_RIGHT) > SW_THRESHOLD && irSensor::getInstance().getValue(FRONT_LEFT) > SW_THRESHOLD) {
				_is_sensor = 1;
			} else if(irSensor::getInstance().getValue(FRONT_RIGHT) > SW_THRESHOLD) {
				_is_sensor = 0;
			} else if(irSensor::getInstance().getValue(FRONT_LEFT) > SW_THRESHOLD) {
				_is_sensor = 2;
			} else;
		} else {
			_is_sensor = -1;
		}
		return _is_sensor;
	}

	/* ---------------------------------------------------------------
		両方の前センサが反応するまで待機
	--------------------------------------------------------------- */
	int8_t button::waitFrontSensor(void) {
		irSensor::getInstance().turnOn();
		while(button::getIsFrontSensor() != -1 );
		while(1) {
			if(button::getIsFrontSensor() != -1) {
				irSensor::getInstance().turnOff();
				return _is_sensor;
			} else;
		}
	}
}

