#ifndef __imu_H
#define __imu_H

#include "main.h"
#include "dma.h"
#include "spi.h"
#include "baseModule.hpp"
#include "communicate.hpp"

namespace module
{
	class imu : public BaseModule<imu>
	{
	public:
		void update(void) override;
		void callback(void);
		uint8_t checkWHOAMI(void);
		void initialize(void);
		void resetReference(void);
		float getAccel_X(void);
		float getGyro_Z(void);
		float getGyroAngle_Z(void);
		void setGyroAngle_Z(float);
		void resetGyroAngle_Z(void);
		void monitorDebug(void);

	private:
		imu();
		friend class BaseModule<imu>;

		const uint8_t		_ADDRESS;
		volatile uint8_t 	_value[13];

		volatile uint16_t	_accel_x_value;
		volatile int32_t 	_accel_x_reference;
		volatile float 		_accel_x;

		volatile uint16_t	_accel_y_value;
		volatile int32_t 	_accel_y_reference;
		volatile float 		_accel_y;

		volatile uint16_t	_accel_z_value;
		volatile int32_t 	_accel_z_reference;
		volatile float 		_accel_z;

		volatile uint16_t	_gyro_z_value;
		volatile int32_t 	_gyro_z_reference;
		volatile float 		_gyro_z;
		volatile float 		_angle_z;

		void 	communicate(uint8_t*, uint8_t*, uint8_t);
		void 	write1byte(uint8_t, uint8_t);
		uint8_t read1byte(uint8_t);
		void 	startDMA(void);
	};
}

extern "C"
{
	void IMU_Initialize(void);
	void IMU_Callback(void);
}

#endif