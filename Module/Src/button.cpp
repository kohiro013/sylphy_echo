#include "button.hpp"

namespace {
	// プッシュスイッチの設定
	const uint16_t SW_SHORT_PUSH = 10;
	const uint16_t SW_LONG_PUSH = 500;

	// センサースイッチの設定
	const uint16_t SW_THRESHOLD = 800;	// スイッチ判定の閾値
	const uint16_t SW_TIME = 300;		// スイッチ判定の時間閾値
}

namespace module
{
	button::button():
		_timer_switch{0},
		_is_switch{0},

		_timer_sensor{0},
		_is_sensor{0}
	{}

	void button::update(void) {
		// プッシュスイッチが押下されている時間をカウントする
		if( LL_GPIO_IsInputPinSet(SWITCH_GPIO_Port, SWITCH_Pin) == true ) {
			_timer_switch++;
		} else {
			_timer_switch = 0;
		}

		// 両方の前センサが反応している時間をカウントする
/*		if( ( Sensor_GetValue(FRONT + RIGHT) > SW_THRESHOLD ) || ( Sensor_GetValue(FRONT + LEFT) > SW_THRESHOLD ) ) {
			if( is_sensor_switch == -1 ) {
				sensor_switch_timer ++;
			} else {
				sensor_switch_timer = 0;
			}
		} else {
			sensor_switch_timer = 0;
			is_sensor_switch = -1;
		}
*/	}

	int8_t button::getIsPush(void) {
		if( LL_GPIO_IsInputPinSet(SWITCH_GPIO_Port, SWITCH_Pin) == true ) {
			if( _timer_switch > SW_LONG_PUSH ) {
				return 2;
			} else {
				_is_switch = 0;
			}
		} else if( _timer_switch > SW_SHORT_PUSH ) {
			_is_switch = 1;
		} else {
			_is_switch = 0;
		}
		return _is_switch;
	}

	int8_t button::getIsFrontSensor(void) {
/*		if( sensor_switch_timer > SW_TIME ) {
			if( ( Sensor_GetValue(FRONT + RIGHT) > SW_THRESHOLD ) && ( Sensor_GetValue(FRONT + LEFT) > SW_THRESHOLD ) ) {
				is_sensor_switch = FRONT;
			} else if( Sensor_GetValue(FRONT + RIGHT) > SW_THRESHOLD ) {
				is_sensor_switch = RIGHT;
			} else if( Sensor_GetValue(FRONT + LEFT) > SW_THRESHOLD ) {
				is_sensor_switch = LEFT;
			} else;
		} else {
			is_sensor_switch = -1;
		}
		return is_sensor_switch;
*/	}

	int8_t button::waitFrontSensor(void) {
/*		Sensor_StartLED();
		while( Switch_GetIsFrontSensor() != -1 );
		while( 1 ) {
			if( Switch_GetIsFrontSensor() != -1 ) {
				return is_sensor_switch;
			} else;
		}
		Sensor_StopLED();
*/	}
}

