#ifndef __maze_H
#define __maze_H

#include <iostream>
#include "stdio.h"
#include "stdint.h"
#include "singleton.hpp"

constexpr uint8_t MAZE_X = 32;
constexpr uint8_t MAZE_Y = 32;

namespace application
{
	class maze : public Singleton<maze>
	{
	public:
		void setDebugData(void);
		//void getGlobalData(void);
		//void getGlobalData(int8_t, int8_t);
		void display(void);

	private:
		maze();
		friend class Singleton<maze>;

		uint32_t _column[MAZE_Y];
		uint32_t _row[MAZE_Y];
	};
}

#endif