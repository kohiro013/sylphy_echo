#ifndef __potential_H
#define __potential_H

#include "stdio.h"
#include "stdint.h"
#include <vector>
#include "singleton.hpp"
#include "position.hpp"
#include "maze.hpp"

namespace application
{
	class potential : public Singleton<potential>
	{
	public:
		uint16_t 		getAroundSection(t_position*);
		uint16_t 		getAroundSection(int8_t, int8_t);
		uint16_t 		getAroundSection(t_position*, int8_t);
		void 			makeMap(int8_t, int8_t);
		void 			makeUnknownMap(int8_t, int8_t);

	private:
		potential();
		friend class Singleton<potential>;

		uint32_t 	_potential[MAZE_X][MAZE_Y];
	};
}

#endif