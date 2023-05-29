#include "adachi.hpp"

namespace application
{
	adachi::adachi()
	{}

	/* ----------------------------------------------------------------------------------
		進む方向の決定（足立法）
	-----------------------------------------------------------------------------------*/
	int8_t adachi::getNextDirection(t_position* pos)
	{
		static int8_t 	old_direction 	= -1;
		int8_t 			next_direction;
		uint16_t 		min_potential 	= potential::getInstance().getAroundSection(pos);

		if(maze::getInstance().getLocalData(pos, FRONT) == false) {
			next_direction = FRONT;
		} else if(maze::getInstance().getLocalData(pos, LEFT) == false) {
			next_direction = LEFT;
		} else if(maze::getInstance().getLocalData(pos, RIGHT) == false) {
			next_direction = RIGHT;
		} else {
			next_direction = REAR;
		}

		// 斜め優先（前回の方向が左だった場合右を優先）
		if((old_direction == LEFT) && (maze::getInstance().getLocalData(pos, RIGHT) == false)) {
			if(min_potential > potential::getInstance().getAroundSection(pos, RIGHT)) {
				min_potential = potential::getInstance().getAroundSection(pos, RIGHT);
				next_direction = RIGHT;
			} else;
		} else;

		// 斜め優先（前回の方向が右だった場合左を優先）
		if((old_direction == RIGHT) && (maze::getInstance().getLocalData(pos, LEFT) == false)) {
			if(min_potential > potential::getInstance().getAroundSection(pos, LEFT)) {
				min_potential = potential::getInstance().getAroundSection(pos, LEFT);
				next_direction = LEFT;
			} else;
		} else;

		if(maze::getInstance().getLocalData(pos, FRONT) == false) {
			if(min_potential > potential::getInstance().getAroundSection(pos, FRONT)) {
				min_potential = potential::getInstance().getAroundSection(pos, FRONT);
				next_direction = FRONT;
			} else;
		} else;

		if(maze::getInstance().getLocalData(pos, RIGHT) == false) {
			if(min_potential > potential::getInstance().getAroundSection(pos, RIGHT)) {
				min_potential = potential::getInstance().getAroundSection(pos, RIGHT);
				next_direction = RIGHT;
			} else;
		} else;

		if(maze::getInstance().getLocalData(pos, LEFT) == false) {
			if(min_potential > potential::getInstance().getAroundSection(pos, LEFT)) {
				min_potential = potential::getInstance().getAroundSection(pos, LEFT);
				next_direction = LEFT;
			} else;
		} else;

		if(min_potential > potential::getInstance().getAroundSection(pos, REAR)) {
			min_potential = potential::getInstance().getAroundSection(pos, REAR);
			next_direction = REAR;
		} else;

		old_direction = next_direction;
		return next_direction;
	}
	
	/* ----------------------------------------------------------------------------------
		進む方向の決定（拡張足立法）
	-----------------------------------------------------------------------------------*/
	int8_t adachi::getUnknownDirection(t_position* pos)
	{
		static int8_t 	old_direction 	= -1;
		int8_t 			next_direction 	= getNextDirection(pos);


		if((maze::getInstance().getLocalData(pos, LEFT) == false) && (maze::getInstance().getIsUnknown(pos, LEFT) == false)) {
			next_direction = LEFT;
		} else;

		if((maze::getInstance().getLocalData(pos, RIGHT) == false) && (maze::getInstance().getIsUnknown(pos, RIGHT) == false)) {
			next_direction = RIGHT;
		} else;

		// 斜め優先（前回の方向が左だった場合右を優先）
		if((old_direction == LEFT) && (maze::getInstance().getLocalData(pos, RIGHT) == false) && (maze::getInstance().getIsUnknown(pos, RIGHT) == false)) {
			next_direction = RIGHT;
		} else;

		// 斜め優先（前回の方向が右だった場合左を優先）
		if((old_direction == RIGHT) && (maze::getInstance().getLocalData(pos, LEFT) == false) && (maze::getInstance().getIsUnknown(pos, LEFT) == false)) {
			next_direction = LEFT;
		} else;

		// 直進優先
		if((maze::getInstance().getLocalData(pos, FRONT) == false) && (maze::getInstance().getIsUnknown(pos, FRONT) == false)) {
			next_direction = FRONT;
		} else;

		old_direction = next_direction;
		return next_direction;
	}

}