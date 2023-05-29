#ifndef __adachi_H
#define __adachi_H

#include "stdio.h"
#include "stdint.h"
#include "singleton.hpp"
#include "position.hpp"
#include "maze.hpp"
#include "potential.hpp"

namespace application
{
	class adachi : public Singleton<adachi>
	{
	public:
		int8_t 		getNextDirection(t_position*);
		int8_t		getUnknownDirection(t_position*);

	private:
		adachi();
		friend class Singleton<adachi>;
	};
}

#endif