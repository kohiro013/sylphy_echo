#include "path.hpp"

namespace application
{
	path::path():
		_num{0},
		_path{(255)}
	{}

	/* ----------------------------------------------------------------------------------
		パスの初期設定
	-----------------------------------------------------------------------------------*/
	void path::reset(void)
	{
		for(uint16_t i = 0; i < _path.size(); i++) {
			_path[i].straight = 0;
			_path[i].direction = 0;
			_path[i].type = 0;
		}
		_num = 0;
	}

	/* ----------------------------------------------------------------------------------
		直線パスのセット
	-----------------------------------------------------------------------------------*/
	void path::setStraightSection(int8_t n)
	{
		_path[_num].straight += n;
	}

	/* ----------------------------------------------------------------------------------
		ターンパスのセット
	-----------------------------------------------------------------------------------*/
	void path::setTurnSection(int8_t type, int8_t dir)
	{
		_path[_num].direction = dir;
		_path[_num].type = type;
		_num ++;
	}

	/* ----------------------------------------------------------------------------------
		生成したパスの参照
	-----------------------------------------------------------------------------------*/
	t_path path::getSequence(uint8_t n)
	{
		return _path[n];
	}

	t_path path::getReturnSequence(uint8_t n)
	{
		t_path return_path;
		if((_num-1) - n == 0) {
			return_path.straight	= _path[0].straight;
			return_path.type		= t_path_type::GOAL;
			return_path.direction	= 0;

		} else if((_num-1) - n-1 < 0) {
			return_path.straight	= 0;
			return_path.type		= t_path_type::TURN_0;
			return_path.direction	= -1;

		} else {
			return_path.straight	= _path[(_num-1)-n].straight;

			if(_path[(_num-1)-n-1].type == TURN_45IN) {
				return_path.type = TURN_45OUT;
			} else if(_path[(_num-1)-n-1].type == TURN_135IN) {
				return_path.type = TURN_135OUT;
			} else if(_path[(_num-1)-n-1].type == TURN_45OUT ) {
				return_path.type = TURN_45IN;
			} else if(_path[(_num-1)-n-1].type == TURN_135OUT ) {
				return_path.type = TURN_135IN;
			} else {
				return_path.type = _path[(_num-1)-n-1].type;
			}

			if(_path[(_num-1)-n-1].direction == RIGHT) {
				return_path.direction = LEFT;
			} else if(_path[(_num-1)-n-1].direction == LEFT) {
				return_path.direction = RIGHT;
			} else {
				return_path.direction = -1;
			}
		}
		return return_path;
	}

	/* ----------------------------------------------------------------------------------
		生成したパス数の参照
	-----------------------------------------------------------------------------------*/
	uint8_t path::getSequenceNumber(void)
	{
		uint16_t i;
		for(i = 0; i < _path.size(); i++) {
			if(_path[i].type == TURN_0 || _path[i].type == GOAL) {
				break;
			} else;
		}
		return i;
	}

	/* ----------------------------------------------------------------------------------
		パスの圧縮
	-----------------------------------------------------------------------------------*/
	void path::compress(uint16_t num)
	{
		for(uint16_t i = num + 1; i < _path.size(); i++) {
			if(_path[i].type == t_path_type::GOAL) {
				break;
			} else;
			_path[i] = _path[i+1];
		}
	}

	/* ----------------------------------------------------------------------------------
		大回りターンのパス変換
	-----------------------------------------------------------------------------------*/
	void path::convertTurnLarge(void)
	{
		for(uint16_t i = 0; i < _path.size(); i++) {
			if(_path[i].type == GOAL) {
				_num = i + 1;
				break;
			} else;

			if(_path[i].type == TURN_90) {
				if(_path[i].straight > 0 && _path[i+1].straight > 0) {
					_path[i].straight --;
					_path[i].type = TURN_90L;
					_path[i+1].straight --;
				} else;
			} else;
		}
	}

	/* ----------------------------------------------------------------------------------
		180度ターンのパス変換
	-----------------------------------------------------------------------------------*/
	void path::convertTurn180(void)
	{
		for(uint16_t i = 0; i < _path.size(); i++) {
			if(_path[i].type == GOAL) {
				_num = i + 1;
				break;
			} else;

			if(_path[i].straight > 0 && _path[i+1].straight == 0 && _path[i+2].straight > 0) {
				if(_path[i].type == TURN_90 && _path[i+1].type == TURN_90 && _path[i].direction == _path[i+1].direction) {
					_path[i].straight --;
					_path[i].type = TURN_180;
					_path[i+2].straight --;
					path::compress(i);
				} else;
			} else;
		}
	}

	/* ----------------------------------------------------------------------------------
		斜め侵入型45度ターンのパス判定
	-----------------------------------------------------------------------------------*/
	bool path::isTurn45in(uint16_t num)
	{
		if(_path[num].straight > 0 && _path[num+1].straight == 0) {
			if(_path[num].type == TURN_90 && _path[num+1].type == TURN_90) {
				return _path[num].direction != _path[num+1].direction;
			} else;
		} else;
		return false;
	}

	/* ----------------------------------------------------------------------------------
		斜め脱出型45度ターンのパス判定
	-----------------------------------------------------------------------------------*/
	bool path::isTurn45out(uint16_t num)
	{
		if(_path[num].straight == 0 && _path[num+1].straight > 0) {
			if((_path[num-1].type == TURN_45IN || _path[num-1].type == TURN_90) && _path[num].type == TURN_90) {
				return _path[num-1].direction != _path[num].direction;
			} else;
		} else;
		return false;
	}
	
	/* ----------------------------------------------------------------------------------
		90度ターンのパス判定
	-----------------------------------------------------------------------------------*/
	bool path::isTurn90v(uint16_t num)
	{
		if(_path[num].straight == 0 && _path[num+1].straight == 0 && _path[num+2].straight == 0) {
			if(_path[num].type == TURN_90 && _path[num+1].type == TURN_90) {
				return _path[num].direction == _path[num+1].direction;
			} else;
		} else;
		return false;
	}

	/* ----------------------------------------------------------------------------------
		斜め侵入型135度ターンのパス判定
	-----------------------------------------------------------------------------------*/
	bool path::isTurn135in(uint16_t num)
	{
		if(_path[num].straight > 0 && _path[num+1].straight == 0 && _path[num+2].straight == 0) {
			if(_path[num].type == TURN_90 && _path[num+1].type == TURN_90) {
				return _path[num].direction == _path[num+1].direction;
			} else;
		} else;
		return false;
	}

	/* ----------------------------------------------------------------------------------
		斜め脱出型135度ターンのパス判定
	-----------------------------------------------------------------------------------*/
	bool path::isTurn135out(uint16_t num)
	{
		if(_path[num].straight == 0 && _path[num+1].straight == 0 && _path[num+2].straight > 0) {
			if(_path[num].type == TURN_90 && _path[num+1].type == TURN_90) {
				return _path[num].direction == _path[num+1].direction;
			} else;
		} else;
		return false;
	}

	/* ----------------------------------------------------------------------------------
		斜め侵入型45度ターンのパス変換
	-----------------------------------------------------------------------------------*/
	void path::convertTurn45in(uint16_t num)
	{
		_path[num].straight --;
		_path[num].type = TURN_45IN;
	}

	/* ----------------------------------------------------------------------------------
		斜め脱出型45度ターンのパス変換
	-----------------------------------------------------------------------------------*/
	void path::convertTurn45out(uint16_t num)
	{
		_path[num+1].straight --;
		_path[num].type = TURN_45OUT;
	}

	/* ----------------------------------------------------------------------------------
		90度ターンのパス変換
	-----------------------------------------------------------------------------------*/
	void path::convertTurn90v(uint16_t num)
	{
		_path[num].type = TURN_90V;
		path::compress(num);
	}

	/* ----------------------------------------------------------------------------------
		斜め侵入型135度ターンのパス変換
	-----------------------------------------------------------------------------------*/
	void path::convertTurn135in(uint16_t num)
	{
		_path[num].straight --;
		_path[num].type = TURN_135IN;
		path::compress(num);
	}

	/* ----------------------------------------------------------------------------------
		斜め脱出型135度ターンのパス変換
	-----------------------------------------------------------------------------------*/
	void path::convertTurn135out(uint16_t num)
	{
		_path[num+2].straight --;
		_path[num].type = TURN_135OUT;
		path::compress(num);
	}

	/* ----------------------------------------------------------------------------------
		斜めのパス変換
	-----------------------------------------------------------------------------------*/
	void path::convertDiagonal(void)
	{
		uint16_t i, j;

		// ターンの判定と変換
		for(i = 0; i < _path.size(); i++) {
			if(_path[i].type == t_path_type::GOAL) {
				break;
			} else;

			if(path::isTurn45in(i) == true || path::isTurn135in(i) == true) {
				for(j = i; j < _path.size(); j++) {
					if(_path[j].type == t_path_type::GOAL) {
						break;
					} else if(path::isTurn45out(j) == true || path::isTurn135out(j) == true) {
						break;
					} else;
				}

				if(_path[j].type != t_path_type::GOAL) {
					if(path::isTurn45in(i) == true)		{path::convertTurn45in(i);	}
					if(path::isTurn45out(j) == true)	{path::convertTurn45out(j);	}
					if(path::isTurn135out(j) == true)	{path::convertTurn135out(j);}
					if(path::isTurn135in(i) == true)	{path::convertTurn135in(i);	}
					for(uint16_t k = i + 1; k < j; k++) {
						if(path::isTurn90v(k) == true) {
							path::convertTurn90v(k);
							j --;
						} else;
					}
				}
			}
		}

		// 斜めへの侵入と脱出ターンの間を斜め直進へ変換
		for(i = 0; i < _path.size(); i++) {
			if(_path[i].type == t_path_type::GOAL) {
				break;
			} else;

			if(_path[i].type >= t_path_type::TURN_45IN) {
				i ++;
				for(j = i; _path[j].type < t_path_type::TURN_45OUT; j++) {
					if(_path[j].type == t_path_type::TURN_90V) {
						_path[j].straight = j - i;
						for(uint16_t k = i; k < _path.size(); k++) {
							if(_path[k].type == t_path_type::GOAL) {
								break;
							} else;
							_path[k] = _path[k + j - i];
						}
						j = i;
						i ++;
					} else;
				}
				_path[j].straight = j - i;
				for(uint16_t k = i; k < _path.size(); k++) {
					if(_path[k].type == t_path_type::GOAL) {
						break;
					} else;
					_path[k] = _path[k + j - i];
				}
			} else;
		}
	}

	/* ----------------------------------------------------------------------------------
		拡張ターンのパス変換
	-----------------------------------------------------------------------------------*/
	void path::convertAdvancedTurn(void)
	{
		for(uint16_t i = 0; i < _path.size(); i++) {
			if(_path[i].type == t_path_type::GOAL) {
				_num = i + 1;
				break;
			} else;

			if(_path[i].type == TURN_45OUT && _path[i + 1].straight == 0 && _path[i + 1].type == TURN_45IN) {
				if(_path[i].direction == _path[i + 1].direction ) {
					_path[i].type == TURN_KOJIMA;
					path::compress(i);
				} else;
			} else;
		}
	}

	/* ----------------------------------------------------------------------------------
		パス変換
	-----------------------------------------------------------------------------------*/
	void path::convert(void)
	{
		path::convertTurnLarge();
		path::convertTurn180();
		path::convertDiagonal();
		path::convertAdvancedTurn();
	}

	/* ----------------------------------------------------------------------------------
		パスの表示
	-----------------------------------------------------------------------------------*/
	void path::display(t_path temp_path)
	{
		// 直線区間の表示
		if(temp_path.straight != 0) {
			if(temp_path.type >= TURN_90V && temp_path.type <= TURN_135OUT) {
				printf("Diagonal\t : %d\n\r", temp_path.straight);
			} else {
				printf("Straight\t : %d\n\r", temp_path.straight);
			}
		}

		// ターンの表示
		switch (temp_path.type) {
			case TURN_0:		printf("Turn_0");		break;
			case TURN_90:		printf("Turn_90");		break;
			case TURN_90L:		printf("Turn_90L");		break;
			case TURN_180:		printf("Turn_180");		break;
			case TURN_90V:		printf("Turn_90v");		break;
			case TURN_45IN:		printf("Turn_45in");	break;
			case TURN_135IN:	printf("Turn_135in");	break;
			case TURN_45OUT:	printf("Turn_45out");	break;
			case TURN_135OUT:	printf("Turn_135out");	break;
			case TURN_KOJIMA:	printf("Turn_Kojima");	break;
			case GOAL:			printf("Goal!");		break;
			default:			printf("ERROR!\n\r");	break;
		}

		if((temp_path.type == TURN_0) || (temp_path.type == GOAL)) {
			// 何もしない
		} else {
			if(temp_path.direction == RIGHT) 		printf("_RIGHT");
			else if(temp_path.direction == LEFT)	printf("_LEFT");
			else									printf("_ERROR\r\n");
		}
	}

	void path::displayAll(void)
	{
		t_path temp_path;

		printf("\n\rStart!\n\r");
		for(uint16_t i = 0; i < _path.size(); i++) {
			temp_path = path::getSequence(i);
			//temp_path = path::getReturnSequence(i);

			path::display(temp_path);
			printf("\n\r");
			if((temp_path.type == TURN_0) || (temp_path.type == GOAL)) {
				break;
			} else;
		}
		printf("\n\r");
	}
}