#include "position.hpp"

namespace application
{
	position::position():
		_ego{0, 0, NORTH}
	{}

	/* ----------------------------------------------------------------------------------
		自己位置を取得
	-----------------------------------------------------------------------------------*/
	t_position position::getMyPlace(void)
	{
		return _ego;
	}

	/* ----------------------------------------------------------------------------------
		自己位置を入力
	-----------------------------------------------------------------------------------*/
	void position::setMyPlace(t_position pos)
	{
		_ego = pos;
	}

	/* ----------------------------------------------------------------------------------
		自己位置をリセット
	-----------------------------------------------------------------------------------*/
	void position::reset(void)
	{
		_ego.x = _ego.y = 0;
		_ego.dir = NORTH;
	}

	/* ----------------------------------------------------------------------------------
		自己位置を回転
	-----------------------------------------------------------------------------------*/
	t_position position::rotate(t_position* pos, int8_t dir)
	{
		t_position temp = *pos;

		temp.dir += (dir - 1);
		if(temp.dir < EAST) {
			temp.dir = SOUTH;
		} else if(temp.dir > SOUTH) {
			temp.dir -= (SOUTH + 1);
		} else;
		return temp;
	}

	t_position position::rotateMyDirection(int8_t dir)
	{
		_ego = rotate(&_ego, dir);
		return _ego;
	}

	/* ----------------------------------------------------------------------------------
		自己位置を移動
	-----------------------------------------------------------------------------------*/
	t_position position::move(t_position* pos, int8_t dir)
	{
		t_position temp = rotate(pos, dir);
#ifdef linux
		temp.x += round(cos(M_PI/180.f * (90.f * temp.dir)));
		temp.y += round(sin(M_PI/180.f * (90.f * temp.dir)));
#else
		temp.x += round(arm_cos_f32(PI/180.f * (90.f * temp.dir)));
		temp.y += round(arm_sin_f32(PI/180.f * (90.f * temp.dir)));
#endif
		return temp;
	}

	t_position position::moveMyPlace(int8_t dir)
	{
		_ego = move(&_ego, dir);
		return _ego;
	}

	/* ----------------------------------------------------------------------------------
		自己位置がゴール座標と一致したか
	-----------------------------------------------------------------------------------*/
	bool position::getIsGoal(int8_t gx, int8_t gy)
	{
		if(gx == 0 && gy == 0) {
			if(_ego.x == 0 && _ego.y == 0) {
				return true;
			} else {
				return false;
			}
		} else {
			if(gx <= _ego.x && _ego.x < gx + GOAL_SIZE) {
				if(gy <= _ego.y && _ego.y < gy + GOAL_SIZE) {
					return true;
				} else;
			} else;
		}
		return false;
	}
}