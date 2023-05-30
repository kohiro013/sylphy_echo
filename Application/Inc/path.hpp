#ifndef __path_H
#define __path_H

#include "stdio.h"
#include "stdint.h"
#include <vector>
#include "singleton.hpp"
#include "position.hpp"

enum t_path_type {	// 走行パス列挙
	TURN_0		= 0,
	TURN_90		= 1,
	TURN_90L	= 2,
	TURN_180	= 3,
	TURN_45IN	= 4,
	TURN_135IN	= 5,
	TURN_90V	= 6,
	TURN_KOJIMA	= 7,
	TURN_45OUT	= 8,
	TURN_135OUT	= 9,
	GOAL		= 10,
};

struct  t_path {	// 走行パス用構造体
	int8_t straight;
	int8_t direction;
	int8_t type;
};

namespace application
{
	class path : public Singleton<path>
	{
	public:
		void 		reset(void);
		void 		setStraightSection(int8_t);
		void 		setTurnSection(int8_t, int8_t);
		t_path 		getSequence(uint8_t);
		t_path 		getReturnSequence(uint8_t);
		uint8_t 	getSequenceNumber(void);
		void 		convert(void);
		void 		display(t_path);
		void 		displayAll(void);

	private:
		path();
		friend class Singleton<path>;

		uint16_t 			_num;
		std::vector<t_path> _path;

		void 		compress(uint16_t);
		void 		convertTurnLarge(void);
		void 		convertTurn180(void);
		bool 		isTurn45in(uint16_t);
		bool 		isTurn45out(uint16_t);
		bool 		isTurn90v(uint16_t);
		bool 		isTurn135in(uint16_t);
		bool 		isTurn135out(uint16_t);
		void 		convertTurn45in(uint16_t);
		void 		convertTurn45out(uint16_t);
		void 		convertTurn90v(uint16_t);
		void 		convertTurn135in(uint16_t);
		void 		convertTurn135out(uint16_t);
		void 		convertDiagonal(void);
		void 		convertAdvancedTurn(void);
	};
}

#endif