#ifndef __maze_H
#define __maze_H

#include "stdio.h"
#include "stdint.h"
#include "singleton.hpp"
#include "position.hpp"

constexpr uint8_t MAZE_X = 32;
constexpr uint8_t MAZE_Y = 32;

union t_maze {		// 壁情報用構造体
	uint8_t byte;
	struct {
		uint8_t east			:1;
		uint8_t north			:1;
		uint8_t west			:1;
		uint8_t south			:1;
		uint8_t unknown_east	:1;
		uint8_t unknown_north	:1;
		uint8_t unknown_west	:1;
		uint8_t unknown_south	:1;
	} bit;
};

namespace application
{
	class maze : public Singleton<maze>
	{
	public:
		void 		setDebugData(void);
		t_maze 		getGlobalData(int8_t, int8_t);
		bool 		getGlobalData(int8_t, int8_t, int8_t);
		t_maze 		getLocalData(t_position*);
		bool 		getLocalData(t_position*, int8_t);
		t_maze		getLocalData(int8_t, int8_t, int8_t);
		bool 		getLocalData(int8_t, int8_t, int8_t, int8_t);
		bool		getIsUnknown(t_position*);
		bool		getIsUnknown(int8_t, int8_t);
		bool		getIsUnknown(t_position*, int8_t);
		void 		display(void);

	private:
		maze();
		friend class Singleton<maze>;

		uint32_t 	_maze_column[MAZE_Y];
		uint32_t 	_maze_row[MAZE_Y];
		uint32_t 	_unknown_column[MAZE_Y];
		uint32_t 	_unknown_row[MAZE_Y];

		uint8_t 	rotateBit(uint8_t, int8_t, int8_t);
		t_maze		convertGlobalAndLocal(t_maze, int8_t);
	};
}

#endif