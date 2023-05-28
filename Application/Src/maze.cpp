#include "maze.hpp"

namespace application
{
	maze::maze():
		_maze_column{{0}},
		_maze_row{{0}},
		_unknown_column{{0}},
		_unknown_row{{0}}
	{}


	/* ----------------------------------------------------------------------------------
		デバッグ用壁情報入力
	-----------------------------------------------------------------------------------*/
	void maze::setDebugData(void)	// 32x32デバッグ迷路のゴールは(1, 2)
	{
		uint8_t test[MAZE_X][MAZE_Y] = {
		//	  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
			{13, 12, 10, 10,  9, 12, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  8,  8,  8,  8, 11},	// 0
			{ 4,  2, 10, 11,  4,  2, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  8, 11,  5,  7,  7,  4,  9},	// 1
			{ 5, 12,  8,  9,  6,  9, 12,  9, 12, 10,  9, 14,  8, 10,  9, 13, 12,  9, 12, 10, 10, 10, 10, 10,  9,  6,  9,  6, 10, 10,  3,  5},	// 2
			{ 5,  4,  0,  0,  9,  6,  3,  5,  6,  9,  6,  9,  6,  9,  5,  4,  3,  5,  6, 10, 10, 10, 10,  9,  6,  9,  6, 10, 10, 10,  9,  5},	// 3
			{ 5,  6,  2,  3,  6,  9, 14,  2,  9,  4,  9,  6,  8,  3,  6,  3, 13,  6,  9, 12, 10,  9, 12,  2, 11,  5, 12, 10,  9, 12,  3,  5},	// 4
			{ 6,  8,  9, 12,  9,  6,  9, 14,  1,  7,  4, 11,  7, 12,  9, 12,  2,  9,  6,  3, 13,  5,  6,  8,  9,  5,  4, 11,  5,  6,  9,  5},	// 5
			{12,  1,  5,  5,  6,  9,  6,  9,  6, 10,  3, 12, 10,  3,  6,  3, 13,  6,  9, 12,  1,  5, 12,  1,  5,  5,  5, 12,  1, 12,  3,  5},	// 6
			{ 5,  5,  6,  3, 14,  0, 11,  6,  9, 14,  8,  2, 10, 10, 10, 10,  2,  9,  6,  3,  6,  3,  5,  5,  5,  6,  3,  5,  5,  6,  9,  5},	// 7
			{ 5,  5, 14,  8, 10,  3, 12,  9,  6,  9,  7, 12,  9, 12,  8,  9, 13,  6,  9, 12,  8,  9,  5,  5,  6,  9, 12,  1,  5, 12,  3,  5},	// 8
			{ 5,  5, 14,  1, 12, 10,  3,  6,  9,  6,  8,  3,  5,  6,  0,  1,  4,  9,  5,  4,  0,  1,  5,  5, 12,  1,  5,  4,  3,  6,  9,  5},	// 9
			{ 5,  5, 13,  6,  3, 12, 11, 12,  3, 12,  2,  9,  6,  9,  6,  3,  5,  6,  3,  6,  2,  3,  5,  5,  5,  5,  5,  5, 12,  9,  5,  5},	// 10
			{ 5,  5,  6,  8,  8,  3, 12,  3, 14,  2,  9,  6,  9,  6,  9, 14,  1, 14,  8, 10,  8,  9,  5,  5,  4,  3,  4,  3,  5,  5,  5,  5},	// 11
			{ 5,  5, 12,  3,  6,  8,  3, 12,  9, 13,  6,  9,  6,  9,  6,  9,  6,  9,  5, 12,  3,  5,  5,  5,  6,  9,  5, 12,  1,  5,  5,  5},	// 12
			{ 5,  5,  6,  8, 11,  7, 12,  0,  1,  4,  9,  4, 11,  6,  9,  6,  9,  6,  3,  4,  9,  5,  5,  6,  9,  5,  5,  5,  5,  5,  5,  5},	// 13
			{ 5,  5, 13,  6,  9, 13,  6,  2,  3,  5,  6,  2, 11, 13,  6,  9,  6,  8,  9,  5,  4,  3,  6, 10,  2,  1,  5,  5,  5,  4,  3,  5},	// 14
			{ 5,  5,  4, 10,  3,  4, 11, 12,  9,  6, 10, 10,  9,  6,  9,  6,  9,  7,  5,  5,  5, 12, 10,  8, 10,  3,  6,  3,  6,  3, 12,  1},	// 15
			{ 5,  5,  5, 12,  9,  4,  9,  5,  5, 12,  9, 12,  2, 10,  2,  9,  6,  9,  6,  1,  5,  5, 12,  2,  9, 12, 10, 10, 10, 10,  3,  7},	// 16
			{ 5,  5,  6,  3,  5,  5,  5,  5,  5,  5,  5,  5, 12, 10,  9,  4, 11,  6,  9,  7,  5,  5,  4, 10,  1,  6, 10, 10, 10, 10, 10,  9},	// 17
			{ 5,  5, 12,  9,  5,  5,  6,  3,  6,  3,  5,  6,  3, 14,  1,  6, 10,  9,  6,  9,  6,  1,  6,  8,  3, 14, 10,  8, 10, 10, 10,  3},	// 18
			{ 5,  5,  5,  5,  6,  3, 12,  8,  9, 12,  1, 12, 10, 10,  2, 10, 11,  6,  9,  6,  9,  7, 12,  3, 12,  9, 12,  2,  8, 10,  8, 11},	// 19
			{ 5,  5,  5,  5, 12,  9,  4,  0,  1,  5,  7,  6, 10, 10,  9, 12, 10, 10,  2,  9,  6,  8,  3, 12,  0,  1,  4, 10,  0, 10,  0, 11},	// 20
			{ 5,  5,  5,  5,  5,  5,  6,  2,  3,  6, 10, 10, 10, 10,  3,  5, 12, 10, 10,  2, 11,  4,  9,  6,  0,  1,  6,  8,  2,  8,  2,  9},	// 21
			{ 5,  5,  5,  5,  5,  5, 12, 10, 10, 10,  8, 10,  8,  8, 10,  3,  5, 12,  8,  9, 12,  3,  6,  9,  6,  1, 14,  0, 10,  0, 10,  1},	// 22
			{ 5,  5,  5,  5,  5,  5,  6, 10,  9, 12,  2, 10,  3,  6, 10,  9,  4,  1,  5,  4,  3, 12,  9,  6,  9,  7, 12,  2,  8,  2,  8,  3},	// 23
			{ 5,  5,  5,  6,  1,  6, 10,  9,  5,  6, 10, 10, 10, 10,  8,  3,  5,  6,  2,  3, 12,  0,  0,  9,  6,  9,  6, 10,  0, 10,  0, 11},	// 24
			{ 5,  5,  6,  9,  5, 12, 10,  3,  6, 10, 10, 10, 10,  9,  6, 10,  3, 12,  9, 13,  6,  2,  2,  2, 11,  6,  9, 14,  2,  8,  2,  9},	// 25
			{ 5,  6, 10,  1,  5,  6,  8, 11, 12, 10, 10, 10, 10,  2, 10, 10,  9,  5,  5,  5, 12,  8,  9, 13, 12,  9,  6,  9, 14,  0, 10,  1},	// 26
			{ 4, 10,  9,  5,  5, 12,  0, 11,  6, 10,  9, 12, 10, 10, 10, 10,  3,  5,  5,  4,  1,  5,  4,  0,  1,  5, 13,  6,  9,  6,  8,  3},	// 27
			{ 4, 11,  5,  5,  5,  7,  4, 11, 12, 10,  3,  6, 10, 10, 10, 10,  9,  5,  5,  5,  4,  0,  1,  5,  4,  0,  1, 13,  6,  9,  6,  9},	// 28
			{ 4, 11,  5,  5,  6,  9,  4, 11,  6, 10, 10, 10, 10, 10, 10,  8,  3,  5,  5,  5,  5,  5,  4,  0,  1,  5,  4,  0,  9,  6,  9,  5},	// 29
			{ 4,  8,  3,  6, 10,  3,  6, 10, 10, 10, 10, 10, 10, 10, 10,  3, 12,  3,  5,  5,  4,  0,  1,  5,  4,  0,  1,  5,  4,  9,  6,  1},	// 30
			{ 7,  6, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  2, 11,  6,  3,  7,  7,  6,  2,  3,  7,  6,  2,  3,  6, 10,  3},	// 31
		};	// GOAL_X = 1, GOAL_Y = 2

		for(uint8_t j = 0; j < MAZE_Y; j++) {
			for(uint8_t i = 0; i < MAZE_X; i++) {
				_maze_column[j] |= (test[j][i]&0x01) << i;
				_maze_row[j]    |= ((test[j][i]&0x02) >> 1) << i;
			}
			_unknown_column[j] = 0xFFffFFff;
			_unknown_row[j] = 0xFFffFFff;
		}
	}

	/* ----------------------------------------------------------------------------------
		ビットローテーション
	-----------------------------------------------------------------------------------*/
	uint8_t maze::rotateBit(uint8_t byte, int8_t n, int8_t nbit)
	{
		if (n == 0) {
		} else if (n > 0) {
			byte = ((byte << n) | (byte >> (nbit - n)));
		} else {
			byte = ((byte >> -n) | (byte << (nbit + n)));
		}
		return byte;
	}

	/* ----------------------------------------------------------------------------------
		相対座標系と絶対座標系の相互変換
	-----------------------------------------------------------------------------------*/
	t_maze maze::convertGlobalAndLocal(t_maze maze, int8_t dir)
	{
		t_maze temp;

		temp.byte = ((maze.byte & 0xf0) >> 4);
		temp.byte = maze::rotateBit(temp.byte, dir, 4);

		maze.byte = (maze.byte & 0x0f);
		maze.byte = maze::rotateBit(maze.byte, dir, 4);

		maze.byte = (maze.byte & 0x0f);
		maze.byte |= ((temp.byte << 4) & 0xf0);

		return maze;
	}

	/* ----------------------------------------------------------------------------------
		絶対座標系の壁情報
	-----------------------------------------------------------------------------------*/
	t_maze maze::getGlobalData(int8_t x, int8_t y)
	{
		t_maze temp;

		temp.bit.east = (_maze_column[y]&(1 << x)) >> x;
		temp.bit.north = (_maze_row[y]&(1 << x)) >> x;
		temp.bit.west = (x == 0) ? 1 : (_maze_column[y]&(1 << (x - 1))) >> (x - 1);
		temp.bit.south = (y == 0) ? 1 : (_maze_row[y - 1]&(1 << x)) >> x;

		temp.bit.unknown_east = (_unknown_column[y]&(1 << x)) >> x;
		temp.bit.unknown_north = (_unknown_row[y]&(1 << x)) >> x;
		temp.bit.unknown_west = (x == 0) ? 1 : (_unknown_column[y]&(1 << (x - 1))) >> (x - 1);
		temp.bit.unknown_south = (y == 0) ? 1 : (_unknown_row[y - 1]&(1 << x)) >> x;

		return temp;
	}

	bool maze::getGlobalData(int8_t x, int8_t y, int8_t dir)
	{
		switch (dir) {
			case EAST: 	return (_maze_column[y]&(1 << x)) == static_cast<uint32_t>(1 << x);
			case NORTH:	return (_maze_row[y]&(1 << x)) == static_cast<uint32_t>(1 << x);
			case WEST:	return (x == 0) ? true : (_maze_column[y]&(1 << (x - 1))) == static_cast<uint32_t>(1 << (x - 1));
			case SOUTH:	return (y == 0) ? true : (_maze_row[y - 1]&(1 << x)) == static_cast<uint32_t>(1 << x);
		}
		return false;
	}

	/* ----------------------------------------------------------------------------------
		相対座標系の壁情報
	-----------------------------------------------------------------------------------*/
	t_maze maze::getLocalData(t_position* pos)
	{
		int8_t x 	= pos->x;
		int8_t y 	= pos->y;
		int8_t dir 	= pos->dir;

		return convertGlobalAndLocal(getGlobalData(x, y), 1 - dir);
	}

	bool maze::getLocalData(t_position* pos, int8_t ldir)
	{
		int8_t x 	= pos->x;
		int8_t y 	= pos->y;
		int8_t dir 	= pos->dir;

		t_maze temp = convertGlobalAndLocal(getGlobalData(x, y), 1 - dir);
		switch (ldir) {
			case RIGHT: return temp.bit.east;
			case FRONT: return temp.bit.north;
			case LEFT: 	return temp.bit.west;
			case REAR: 	return temp.bit.south;
		}
		return false;
	}

	t_maze maze::getLocalData(int8_t x, int8_t y, int8_t dir)
	{
		return convertGlobalAndLocal(getGlobalData(x, y), 1 - dir);
	}

	bool maze::getLocalData(int8_t x, int8_t y, int8_t gdir, int8_t ldir)
	{
		t_maze temp = convertGlobalAndLocal(getGlobalData(x, y), 1 - gdir);
		switch (ldir) {
			case RIGHT: return temp.bit.east;
			case FRONT: return temp.bit.north;
			case LEFT: 	return temp.bit.west;
			case REAR: 	return temp.bit.south;
		}
		return false;
	}

	/* ----------------------------------------------------------------------------------
		探索済みの壁かどうかの判定
	-----------------------------------------------------------------------------------*/
	bool maze::getIsUnknown(t_position* pos)
	{
		int8_t x 	= pos->x;
		int8_t y 	= pos->y;
		t_maze temp = getGlobalData(x, y);

		return (temp.byte & 0xf0) == 0xf0;
	}

	bool maze::getIsUnknown(int8_t x, int8_t y)
	{
		t_maze temp = getGlobalData(x, y);
		return (temp.byte & 0xf0) == 0xf0;
	}

	bool maze::getIsUnknown(t_position* pos, int8_t ldir)
	{
		int8_t x 	= pos->x;
		int8_t y 	= pos->y;
		int8_t gdir = pos->dir;
		
		gdir += (ldir - 1);
		if(gdir < EAST) {
			gdir = SOUTH;
		} else if(gdir > SOUTH) {
			gdir -= (SOUTH + 1);
		} else;

#ifdef linux
		x = round(cos(M_PI/180.f) * (90.f * gdir));
		y = round(sin(M_PI/180.f) * (90.f * gdir));
#else
		x = roundf(arm_cos_f32(PI/180.f * (90.f * gdir)));
		y = roundf(arm_sin_f32(PI/180.f * (90.f * gdir)));
#endif

		if((x < 0) || (y < 0)) {
			return false;
		} else if((x > MAZE_X-1) || (y > MAZE_Y-1)) {
			return false;
		} else {
			t_maze temp = getGlobalData(x, y);
			return (temp.byte & 0xf0) == 0xf0;
		}
	}

	/* ----------------------------------------------------------------------------------
		壁情報の表示
	-----------------------------------------------------------------------------------*/
	void maze::display(void)
	{
		int8_t i, j;

		printf("\n\r");
		for(j = MAZE_Y-1; j >= 0; j--) {
			printf("   ");
			for(i = 0; i < MAZE_X; i++) {
				printf("+");
				if((_maze_row[j]&(1 << i)) == static_cast<uint32_t>(1 << i)) {
					printf("----");
				} else {
					printf("    ");
				}
			}
			printf("+\n\r%02d |    ", j);
			for(i = 0; i < MAZE_X; i++) {
				if((_maze_column[j]&(1 << i)) == static_cast<uint32_t>(1 << i)) {
					printf("|");
				} else {
					printf(" ");
				}
				printf("    " );
			}
			printf("\n\r");
		}
		printf("   ");
		for(i = 0; i < MAZE_X; i++) {
			printf("+");
			printf("----");
		}
		printf("+\r\n   ");
		for(i = 0; i < MAZE_X; i++) {
			printf("  %02d ", i);
		}
		printf("\r\n");
	}
}