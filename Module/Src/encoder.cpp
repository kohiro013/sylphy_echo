#include "encoder.hpp"
#include <stdio.h>
#include <algorithm>
#include "arm_math.h"

#define SPI_HANDLE 	(SPI2)
#define DMA_HANDLE	(DMA1)
#define RX_STREAM	(LL_DMA_STREAM_3)
#define TX_STREAM	(LL_DMA_STREAM_4)
#define CS_L_PORT	(SPI2_CS0_GPIO_Port)
#define CS_R_PORT	(SPI2_CS1_GPIO_Port)
#define CS_L_PIN	(SPI2_CS0_Pin)
#define CS_R_PIN	(SPI2_CS1_Pin)

void Encoder_Initialize(void) {
	module::encoder::getInstance().initialize();
}

void Encoder_Callback(void) {
	module::encoder::getInstance().callback();
}

namespace module
{
	encoder::encoder():
		_RESOLUTION{4096 - 1},
		_address{(0x3fff<<1) | 0x8000},
		_value{0},
		_count_l{0}, _count_r{0},
		_count_old_l{0}, _count_old_r{0},
		_angle_l{0.f}, _angle_r{0.f}
	{
		_address |=  __builtin_parity(_address);
	}

	void encoder::write2byte(uint16_t addr, uint16_t data) {
		uint16_t address = (addr<<1) | 0x7fff;
		address |= __builtin_parity(address);
		data    |= __builtin_parity(data);

		// SPIが無効化されている場合有効化する
		if(LL_SPI_IsEnabled(SPI_HANDLE) == RESET) {
			LL_SPI_Enable(SPI_HANDLE);
		} else;

		while(LL_SPI_IsActiveFlag_BSY(SPI_HANDLE) == SET);
		LL_GPIO_ResetOutputPin(CS_L_PORT, CS_L_PIN);
		while(LL_SPI_IsActiveFlag_TXE(SPI_HANDLE) == RESET);
		LL_SPI_TransmitData16(SPI_HANDLE, address);
		LL_SPI_TransmitData16(SPI_HANDLE, data);
		while(LL_SPI_IsActiveFlag_RXNE(SPI_HANDLE) == RESET);
		while(LL_SPI_IsActiveFlag_BSY(SPI_HANDLE) == SET);
		LL_GPIO_SetOutputPin(CS_L_PORT, CS_L_PIN);

		while(LL_SPI_IsActiveFlag_BSY(SPI_HANDLE) == SET);
		LL_GPIO_ResetOutputPin(CS_R_PORT, CS_R_PIN);
		while(LL_SPI_IsActiveFlag_TXE(SPI_HANDLE) == RESET);
		LL_SPI_TransmitData16(SPI_HANDLE, address);
		LL_SPI_TransmitData16(SPI_HANDLE, data);
		while(LL_SPI_IsActiveFlag_RXNE(SPI_HANDLE) == RESET);
		while(LL_SPI_IsActiveFlag_BSY(SPI_HANDLE) == SET);
		LL_GPIO_SetOutputPin(CS_R_PORT, CS_R_PIN);

		// SPIの無効化
		LL_SPI_Disable(SPI_HANDLE);
	}

	void encoder::initialize(void) {
		module::encoder::getInstance().write2byte(0x33A5, 0x0000);
		LL_mDelay(100);

		// SPIによるDMAリクエストon
		LL_SPI_EnableDMAReq_TX(SPI_HANDLE);
		LL_SPI_EnableDMAReq_RX(SPI_HANDLE);
		LL_DMA_EnableIT_TC(DMA_HANDLE, RX_STREAM);

		// 受信DMA動作設定
		LL_DMA_DisableStream(DMA_HANDLE, RX_STREAM);
		LL_DMA_ConfigAddresses(DMA_HANDLE, RX_STREAM, LL_SPI_DMA_GetRegAddr(SPI_HANDLE),
			(uint32_t)(&_value), LL_DMA_GetDataTransferDirection(DMA_HANDLE, RX_STREAM));
		LL_DMA_SetDataLength(DMA_HANDLE, RX_STREAM, 1);

		// 送信DMA動作設定
		LL_DMA_DisableStream(DMA_HANDLE, TX_STREAM);
		LL_DMA_ConfigAddresses(DMA_HANDLE, TX_STREAM, (uint32_t)(&_address),
			LL_SPI_DMA_GetRegAddr(SPI_HANDLE), LL_DMA_GetDataTransferDirection(DMA_HANDLE, TX_STREAM));
		LL_DMA_SetDataLength(DMA_HANDLE, TX_STREAM, 1);

		// DMAの開始
		LL_SPI_Enable(SPI_HANDLE);
		LL_GPIO_ResetOutputPin(CS_L_PORT, CS_L_PIN);
		LL_DMA_EnableStream(DMA_HANDLE, RX_STREAM);
		LL_DMA_EnableStream(DMA_HANDLE, TX_STREAM);
	}

	void encoder::update(void) {
		int32_t delta_l = static_cast<int32_t>(_count_l) - static_cast<int32_t>(_count_old_l);
		if(std::abs(delta_l) < std::min(std::abs(delta_l - _RESOLUTION), std::abs(delta_l + _RESOLUTION))) {
			_angle_l = 2*PI * static_cast<float>(delta_l) / static_cast<float>(_RESOLUTION);
		} else {
			if(std::abs(delta_l - _RESOLUTION) < std::abs(delta_l + _RESOLUTION)) {
				_angle_l = 2*PI * static_cast<float>(delta_l - _RESOLUTION) / static_cast<float>(_RESOLUTION);
			} else {
				_angle_l = 2*PI * static_cast<float>(delta_l + _RESOLUTION) / static_cast<float>(_RESOLUTION);
			}
		}

		int32_t delta_r = static_cast<int32_t>(_count_r) - static_cast<int32_t>(_count_old_r);
		if(std::abs(delta_r) < std::min(std::abs(delta_r - _RESOLUTION), std::abs(delta_r + _RESOLUTION))) {
			_angle_r = 2*PI * static_cast<float>(delta_r) / static_cast<float>(_RESOLUTION);
		} else {
			if(std::abs(delta_r - _RESOLUTION) < std::abs(delta_r + _RESOLUTION)) {
				_angle_r = 2*PI * static_cast<float>(delta_r - _RESOLUTION) / static_cast<float>(_RESOLUTION);
			} else {
				_angle_r = 2*PI * static_cast<float>(delta_r + _RESOLUTION) / static_cast<float>(_RESOLUTION);
			}
		}
	}

	void encoder::callback(void) {
		volatile static int8_t switching = 0;

		if( switching == 0 ) {
			LL_GPIO_SetOutputPin(CS_L_PORT, CS_L_PIN);
			_count_l = (_value>>2)&0x0fff;
			LL_GPIO_ResetOutputPin(CS_R_PORT, CS_R_PIN);
		} else {
			LL_GPIO_SetOutputPin(CS_R_PORT, CS_R_PIN);
			_count_r = _RESOLUTION - ((_value>>2)&0x0fff);
			LL_GPIO_ResetOutputPin(CS_L_PORT, CS_L_PIN);
		}
		switching ^= 1;	// 左右の切り替え

		// 通信再開
		LL_DMA_EnableStream(DMA_HANDLE, RX_STREAM);
		LL_DMA_EnableStream(DMA_HANDLE, TX_STREAM);
	}

	void encoder::resetCountLeft(void) {
		_count_old_l = _count_l;
	}

	void encoder::resetCountRight(void) {
		_count_old_r = _count_r;
	}

	float encoder::getAngleLeft(void) {
		return _angle_l;
	}

	float encoder::getAngleRight(void) {
		return _angle_r;
	}

	float encoder::getAnglerVelocityLeft(void) {
		return _angle_l / _delta_t;
	}

	float encoder::getAnglerVelocityRight(void) {
		return _angle_r / _delta_t;
	}

	void encoder::monitorDebug(void) {
		while(Communicate_ReceiceDMA() != 0x1b) {
			printf("%04x | %5d, %5d, %8.3f | %5d, %5d, %8.3f\r\n", 
				_value,	_count_l, _count_old_l, _angle_l * 180.f/PI, _count_r, _count_old_r, _angle_r * 180.f/PI);
			LL_mDelay(100);
		}
	}
}
