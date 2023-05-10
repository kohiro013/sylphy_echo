#include "imu.hpp"
#include "lsm6dsrx_reg.h"
#include <stdio.h>
#include "arm_math.h"

#define SPI_HANDLE 	(SPI1)
#define DMA_HANDLE	(DMA2)
#define RX_STREAM	(LL_DMA_STREAM_0)
#define TX_STREAM	(LL_DMA_STREAM_3)
#define CS_PORT		(SPI1_CS0_GPIO_Port)
#define CS_PIN		(SPI1_CS0_Pin)

namespace {
	const uint16_t 	NUM_REFERENCE = 1000;

	const float 	SIGN_ACCEL_X = -1.f;
	const float 	SIGN_ACCEL_Y = -1.f;
	const float 	SIGN_ACCEL_Z = 1.f;
	const float 	SENSITIVITY_ACEEL = 9.80665f * 0.244f/1000.f;

	const float 	SIGN_GYRO_Z = 1.f;
	const float 	SENSITIVITY_GYRO = 0.1383f;
}

namespace module
{
	imu::imu():
		_ADDRESS{LSM6DSRX_OUTX_L_G | 0x80},
		_value{0x00, 0x00, 0x00, 0x00, 0x00, 
			   0x00, 0x00, 0x00, 0x00, 0x00, 
			   0x00, 0x00, 0x00},

		_accel_x_value{0},
		_accel_x_reference{0},
		_accel_x{0.f},

		_accel_y_value{0},
		_accel_y_reference{0},
		_accel_y{0.f},

		_accel_z_value{0},
		_accel_z_reference{0},
		_accel_z{0.f},

		_gyro_z_value{0},
		_gyro_z_reference{0},
		_gyro_z{0.f},
		_angle_z{0.f}
	{}

	void imu::communicate(uint8_t *tx_data, uint8_t *rx_data, uint8_t length) {
		uint8_t count = 0;

		if(LL_SPI_IsEnabled(SPI_HANDLE) == RESET) {
			LL_SPI_Enable(SPI_HANDLE);
		} else;

		while(LL_SPI_IsActiveFlag_BSY(SPI_HANDLE) == SET);
		LL_GPIO_ResetOutputPin(CS_PORT, CS_PIN);
		while(length > count) {
			while(LL_SPI_IsActiveFlag_TXE(SPI_HANDLE) == RESET);
			LL_SPI_TransmitData8(SPI_HANDLE, *(tx_data + count));
			while(LL_SPI_IsActiveFlag_RXNE(SPI_HANDLE) == RESET);
			rx_data[count] = LL_SPI_ReceiveData8(SPI_HANDLE);
			count++;
		}
		while(LL_SPI_IsActiveFlag_BSY(SPI_HANDLE) == SET);
		LL_GPIO_SetOutputPin(CS_PORT, CS_PIN);
		
		LL_SPI_Disable(SPI_HANDLE);
	}

	void imu::write1byte(uint8_t addr, uint8_t value) {
		uint8_t tx_data[2] = {(uint8_t)(addr&0x7f), value};
		uint8_t rx_data[2];

		imu::communicate(tx_data, rx_data, sizeof(tx_data)/sizeof(tx_data[0]));
	}

	uint8_t imu::read1byte(uint8_t addr) {
		uint8_t tx_data[2] = {(uint8_t)(addr|0x80), 0x00};
		uint8_t rx_data[2];
		
		imu::communicate(tx_data, rx_data, sizeof(tx_data)/sizeof(tx_data[0]));
		return rx_data[1];
	}

	uint8_t imu::checkWHOAMI(void) {
		return imu::read1byte(LSM6DSRX_WHO_AM_I);
	}

	void imu::startDMA(void) {
		LL_GPIO_ResetOutputPin(CS_PORT, CS_PIN);
		LL_DMA_EnableStream(DMA_HANDLE, RX_STREAM);
		LL_DMA_EnableStream(DMA_HANDLE, TX_STREAM);
	}

	void imu::initialize(void) {
		imu::write1byte(LSM6DSRX_CTRL3_C, 0x01);	// LSM6DSRXをリセット
		LL_mDelay(100);
		while((imu::read1byte(LSM6DSRX_CTRL3_C)&0x01) == 0x01);

		imu::write1byte(LSM6DSRX_CTRL9_XL, 0xE2);	// I3CモードをDisableに設定
		LL_mDelay(1);
		imu::write1byte(LSM6DSRX_CTRL4_C, 0x06);	// I2CモードをDisableに設定
		LL_mDelay(1);

		// 加速度計の設定
		imu::write1byte(LSM6DSRX_CTRL1_XL, 0xae);	// 加速度計のスケールを±8gに設定
		LL_mDelay(1);								// 加速度計の出力データレートを416Hzに設定
		imu::write1byte(LSM6DSRX_CTRL8_XL, 0xc0);	// 加速度計のLPFを6.66kHz/400に設定
		LL_mDelay(1);

		// ジャイロの設定
		imu::write1byte(LSM6DSRX_CTRL2_G, 0xad);	// ジャイロのスケールを±4000deg/sに設定
													// ジャイロの出力データレートを6.66Hzに設定
		LL_mDelay(100);

		// USARTによるDMAリクエストon
		LL_SPI_EnableDMAReq_TX(SPI_HANDLE);
		LL_SPI_EnableDMAReq_RX(SPI_HANDLE);
		LL_DMA_EnableIT_TC(DMA_HANDLE, RX_STREAM);

		// 受信DMA動作設定
		LL_DMA_DisableStream(DMA_HANDLE, RX_STREAM);
		LL_DMA_ConfigAddresses(DMA_HANDLE, RX_STREAM, LL_SPI_DMA_GetRegAddr(SPI_HANDLE),
			(uint32_t)_value, LL_DMA_GetDataTransferDirection(DMA_HANDLE, RX_STREAM));
		LL_DMA_SetDataLength(DMA_HANDLE, RX_STREAM, sizeof(_value)/sizeof(_value[0]));

		// 送信DMA動作設定
		LL_DMA_DisableStream(DMA_HANDLE, TX_STREAM);
		LL_DMA_ConfigAddresses(DMA_HANDLE, TX_STREAM, (uint32_t)(&_ADDRESS),
			LL_SPI_DMA_GetRegAddr(SPI_HANDLE), LL_DMA_GetDataTransferDirection(DMA_HANDLE, TX_STREAM));
		LL_DMA_SetDataLength(DMA_HANDLE, TX_STREAM, sizeof(_value)/sizeof(_value[0]));

		// DMAの開始
		LL_SPI_Enable(SPI_HANDLE);
		imu::startDMA();
	}

	void imu::callback(void) {
		LL_GPIO_SetOutputPin(CS_PORT, CS_PIN);
		_accel_x_value = (((uint16_t)_value[10]<<8) | ((uint16_t)_value[ 9]&0x00ff));
		_accel_y_value = (((uint16_t)_value[ 8]<<8) | ((uint16_t)_value[ 7]&0x00ff));
		_accel_z_value = (((uint16_t)_value[12]<<8) | ((uint16_t)_value[11]&0x00ff));
		_gyro_z_value =  (((uint16_t)_value[ 6]<<8) | ((uint16_t)_value[ 5]&0x00ff));

		// 通信再開
		imu::startDMA();
	}

	void imu::update(void) {
		_accel_x = SIGN_ACCEL_X * ((int32_t)_accel_x_value - _accel_x_reference) * SENSITIVITY_ACEEL;
		_accel_y = SIGN_ACCEL_Y * ((int32_t)_accel_y_value - _accel_y_reference) * SENSITIVITY_ACEEL;
		_accel_z = SIGN_ACCEL_Z * ((int32_t)_accel_z_value - _accel_z_reference) * SENSITIVITY_ACEEL;
		_gyro_z	= SIGN_GYRO_Z * PI/180.f * (((int32_t)_gyro_z_value - _gyro_z_reference) * SENSITIVITY_GYRO);
		_angle_z += _gyro_z * _delta_t;
	}

	void imu::resetReference(void) {
		for(int16_t i = 0; i < NUM_REFERENCE; i++) {
			LL_mDelay(1);
			_accel_x_reference += (int32_t)_accel_x_value;
			_gyro_z_reference += (int32_t)_gyro_z_value;
		}
		_accel_x_reference /= NUM_REFERENCE;
		_gyro_z_reference /= NUM_REFERENCE;
	}

	float imu::getAccel_X(void) {
		return _accel_x;
	}

	float imu::getGyro_Z(void) {
		return _gyro_z;
	}

	float imu::getGyroAngle_Z(void) {
		return _angle_z;
	}

	void imu::setGyroAngle_Z(float rad) {
		_angle_z = rad;
	}

	void imu::resetGyroAngle_Z(void) {
		_angle_z = 0.f;
	}

	void imu::monitorDebug(void) {
		while(1) {
			printf("%04x, %6d, %6ld, %7.3f\t| %04x, %6d, %6ld, %7.3f\t| %04x, %6d, %6ld, %7.3f\t| %04x, %6d, %6ld, %7.3f\r\n",
					_accel_x_value, (int16_t)_accel_x_value, _accel_x_reference, _accel_x,
					_accel_y_value, (int16_t)_accel_y_value, _accel_y_reference, _accel_y,
					_accel_z_value, (int16_t)_accel_z_value, _accel_z_reference, _accel_z,
					_gyro_z_value,  (int16_t)_gyro_z_value,  _gyro_z_reference,  _gyro_z);
			LL_mDelay(100);
		}
	}
}

void IMU_Initialize(void) {
	module::imu::getInstance().initialize();
}

void IMU_Handler(void) {
	module::imu::getInstance().callback();
}