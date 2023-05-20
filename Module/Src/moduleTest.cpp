#include "moduleTest.hpp"
#include "arm_math.h"
#include <algorithm>

namespace module
{
	moduleTest::moduleTest() {}

	/* ---------------------------------------------------------------
		各機能の動作確認用関数
	--------------------------------------------------------------- */
	void moduleTest::run(void)
	{
		uint16_t	line 	  = 0;
		uint8_t		key;
		int16_t		duty_l	  = 0;
		int16_t		duty_r	  = 0;

		// IMuのリファレンスをリセット
		LL_mDelay(1000);
		imu::getInstance().resetReference();

		// エンコーダのカウントをリセット
		encoder::getInstance().resetCountRight();
		encoder::getInstance().resetCountLeft();

		// 壁センサ用LEDの点灯開始
		irSensor::getInstance().turnOn();

		// UARTの受信バッファをリセット
		Communicate_ClearReceiveBuffer();

		while( 1 ) {
			// 割り込み処理率を表示
			printf("<Boot Time> %8.3f[s]\r\n", interrupt::getInstance().getElapsedUsec() / 1000000.f); line++;
			printf("<Interrupt> %3.1f[%%] (MAX : %3.1f[%%])\r\n",
					static_cast<float>(interrupt::getInstance().getDuty())/10.f, 
					static_cast<float>(interrupt::getInstance().getMaxDuty())/10.f); line++;

			// モータを指定のDutyを表示
			printf("<PWM Duty> L: %4.1f[%%],  R: %4.1f[%%]\r\n",
					static_cast<float>(duty_l)/10.f, static_cast<float>(duty_r)/10.f); line++;

			// エンコーダの角度表示
			printf("<Encoder> L: %5.1f[deg],  R: %5.1f[deg]\r\n",
					encoder::getInstance().getAngleLeft() * 180.f/PI, encoder::getInstance().getAngleRight() * 180.f/PI); line++;

			// バッテリー電圧の表示
			printf("<Battery> Battery : %3.2f[V]\r\n",
					battery::getInstance().getVoltage()); line++;

			// 壁センサのAD値表示
			printf("<IR Sensor> FL: %4d, SL: %4d SR: %4d, FR: %4d\r\n",
					irSensor::getInstance().getValue(FRONT_LEFT), 
					irSensor::getInstance().getValue(SIDE_LEFT),
					irSensor::getInstance().getValue(SIDE_RIGHT), 
					irSensor::getInstance().getValue(FRONT_RIGHT)); line++;

			// IMU（加速度計とジャイロ）の計測値表示
			printf("<IMU> Accel_X: %5.3f[m/s^2], Gyro_Z: %6.3f[rad/s]\r\n",
					imu::getInstance().getAccel_X() / 1000.f, imu::getInstance().getGyro_Z()); line++;

			// モータのDuty入力
			key = Communicate_ReceiceDMA();
			switch( key ) {
				case '1': duty_l += 1;		break;
				case 'q': duty_l -= 1;		break;
				case '2': duty_l += 10;		break;
				case 'w': duty_l -= 10;		break;
				case '3': duty_l += 100;	break;
				case 'e': duty_l -= 100;	break;
				case '7': duty_r += 1;		break;
				case 'u': duty_r -= 1;		break;
				case '8': duty_r += 10;		break;
				case 'i': duty_r -= 10;		break;
				case '9': duty_r += 100;	break;
				case 'o': duty_r -= 100;	break;
				case 'r': // reset
					duty_l = duty_r = 0;
					break;
				case 0x1b: goto END; // [esc] exit
			}
			duty_l = std::clamp((int)duty_l, -950, 950);
			duty_r = std::clamp((int)duty_r, -950, 950);

			// モータを回転
			motor::getInstance().setDuty_Right(duty_r);
			motor::getInstance().setDuty_Left(duty_l);

			fflush(stdout);
			LL_mDelay(100);
			// 画面のクリア
			printf("%c[0J", 0x1b);
			printf("%c[%dA", 0x1b, line);
		}
		END:;
		printf("\r\n");

		// モータの停止
		motor::getInstance().stop();

		// 壁センサ用LEDの点灯開始
		irSensor::getInstance().turnOff();
	}
}