#include "potential.hpp"

namespace application
{
	potential::potential():
		_potential{{0xffff}}
	{}

	/* ----------------------------------------------------------------------------------
		周囲のポテンシャルを取得
	-----------------------------------------------------------------------------------*/
	uint16_t potential::getAroundSection(t_position* pos)
	{
		int8_t x 	= pos->x;
		int8_t y 	= pos->y;
		
		return _potential[x][y];
	}

	uint16_t potential::getAroundSection(int8_t x, int8_t y)
	{
		return _potential[x][y];
	}

	uint16_t potential::getAroundSection(t_position* pos, int8_t ldir)
	{
		t_position temp = application::position::getInstance().move(pos, ldir);

		if((temp.x < 0) || (temp.y < 0)) {
			return 0xffff;
		} else if((temp.x > MAZE_X-1) || (temp.y > MAZE_Y-1)) {
			return 0xffff;
		} else {
			return _potential[temp.x][temp.y];
		}
	}

	/* ----------------------------------------------------------------------------------
		高速ポテンシャルマップ生成
	-----------------------------------------------------------------------------------*/
	void potential::makeMap(int8_t gx, int8_t gy)
	{
		uint16_t 	queue[1024];
		uint16_t	step;
		int16_t		head, tail;

		for(int8_t y = 0; y < MAZE_Y; y++) {
			for(int8_t x = 0; x < MAZE_X; x++) {
				_potential[x][y] = 0xffff;
			}
		}

		_potential[gx][gy] = 0;				// 目標地点に距離０を書き込む
		queue[0] = (gy << 8) + gx;			// 目標地点の座標を記憶
		head = 0;							// 先頭位置を初期化
		tail = 1;							// 末尾位置は、最後の情報位置 + 1

		while(head != tail) {				// 配列の中身が空ならループを抜ける
			int8_t x = queue[head] & 255;	// 配列から区画の座標を取り出す
			int8_t y = queue[head] >> 8;
			head++;							// 情報を取り出したので先頭位置をずらす
			step = _potential[x][y] + 1;	// 新しいステップ数

			if((maze::getInstance().getGlobalData(x, y, EAST) == false) && (x != MAZE_X - 1)) {
				if(_potential[x+1][y] == 0xffff) {
					_potential[x+1][y] = step;
					queue[tail] = (y << 8) + x + 1;	// 次の区間の座標を記憶
					tail++;							// 情報を入れたので末尾位置をすらす
				} else;
			} else;

			if((maze::getInstance().getGlobalData(x, y, NORTH) == false) && (y != MAZE_Y - 1)) {
				if(_potential[x][y+1] == 0xffff) {
					_potential[x][y+1] = step;
					queue[tail] = ((y + 1) << 8) + x;
					tail++;
				} else;
			} else;

			if((maze::getInstance().getGlobalData(x, y, WEST) == false) && (x != 0)) {
				if(_potential[x-1][y] == 0xffff) {
					_potential[x-1][y] = step;
					queue[tail] = (y << 8) + x - 1;
					tail++;
				} else;
			} else;

			if((maze::getInstance().getGlobalData(x, y, SOUTH) == false) && (y != 0)) {
				if(_potential[x][y-1] == 0xffff ) {
					_potential[x][y-1] = step;
					queue[tail] = ((y - 1) << 8) + x;
					tail++;
				} else;
			} else;
		}
	}

	void potential::makeUnknownMap(int8_t gx, int8_t gy)
	{
		uint16_t 	queue[1025];
		uint16_t 	step;
		int16_t 	head, tail;		// 先頭位置, 末尾位置
		int16_t		flag = 0;

		head = 0;						// 先頭位置を初期化
		tail = 1;						// 末尾位置は、最後の情報位置＋１

		for(int8_t y = 0; y < MAZE_Y; y++) {
			for(int8_t x = 0; x < MAZE_X; x++) {
				if(_potential[x][y] != 0xffff && (maze::getInstance().getIsUnknown(x, y) == false)) {
					_potential[x][y] = 0x0000;		// 目標地点に距離０を書き込む
					queue[head] = (y << 8) + x;		// 目標地点の座標を記憶
					head++;
					tail++;
				} else {
					_potential[x][y] = 0xffff;
				}
			}
		}
		flag = tail;

		head = 0;
		while(head != tail) {				// 配列の中身が空ならループを抜ける
			int8_t x = queue[head] & 255;	// 配列から区画の座標を取り出す
			int8_t y = queue[head] >> 8;
			head++;							// 情報を取り出したので先頭位置をずらす
			step = _potential[x][y] + 1;	// 新しいステップ数

			// 何故かないと動かない
			if(step == 0) {
				continue;
			} else;

			if((maze::getInstance().getGlobalData(x, y, EAST) == false) && (x != MAZE_X-1)) {
				if(_potential[x+1][y] == 0xffff) {
					_potential[x+1][y] = step;
					queue[tail] = (y << 8) + x + 1;	// 次の区間の座標を記憶
					tail++;							// 情報を入れたので末尾位置をすらす
				} else;
			} else;

			if((maze::getInstance().getGlobalData(x, y, NORTH) == false) && (y != MAZE_Y-1)) {
				if(_potential[x][y+1] == 0xffff) {
					_potential[x][y+1] = step;
					queue[tail] = ((y + 1) << 8) + x;
					tail++;
				} else;
			} else;

			if((maze::getInstance().getGlobalData(x, y, WEST) == false) && (x != 0)) {
				if(_potential[x-1][y] == 0xffff) {
					_potential[x-1][y] = step;
					queue[tail] = (y << 8) + x - 1;
					tail++;
				} else;
			} else;

			if((maze::getInstance().getGlobalData(x, y, SOUTH) == false) && (y != 0)) {
				if(_potential[x][y-1] == 0xffff) {
					_potential[x][y-1] = step;
					queue[tail] = ((y - 1) << 8) + x;
					tail++;
				} else;
			} else;
		}

		if(flag == tail) {
			potential::makeMap(gx, gy);
		}
	}
}