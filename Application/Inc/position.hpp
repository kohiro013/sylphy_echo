#ifndef __position_H
#define __position_H

#include "stdio.h"
#include "stdint.h"
#include "singleton.hpp"

#ifdef linux
#include <math.h>
#else
#include "arm_math.h"
#endif

constexpr uint8_t GOAL_X 	= 1;
constexpr uint8_t GOAL_Y 	= 2;
constexpr uint8_t GOAL_SIZE = 3;

enum t_globaldir {	// グローバル方向列挙
	EAST	= 0,
	NORTH	= 1,
	WEST	= 2,
	SOUTH	= 3,
};

enum t_localdir {	// ローカル方向列挙
	RIGHT	= 0,
	FRONT	= 1,
	LEFT	= 2,
	REAR	= 3,
};

struct t_position {
	int8_t x;
	int8_t y;
	int8_t dir;
};

namespace application
{
	class position : public Singleton<position>
	{
	public:
		t_position 	getMyPlace(void);
		void 		setMyPlace(t_position);
		void 		reset(void);
		t_position 	rotate(t_position*, int8_t);
		t_position	rotateMyDirection(int8_t);
		t_position 	move(t_position*, int8_t);
		t_position	moveMyPlace(int8_t);
		bool 		getIsGoal(int8_t, int8_t);

	private:
		position();
		friend class Singleton<position>;

		t_position _ego;
	};
}

#endif