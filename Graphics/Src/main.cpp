#include <iostream>
#include <vector>
#include "position.hpp"
#include "maze.hpp"
#include "potential.hpp"
#include "path.hpp"
#include "adachi.hpp"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

const int HALF_SECTION_SIZE = 45;	// 半区画の長さ
const int SECTION_SIZE = 2 * HALF_SECTION_SIZE;		// 区画の長さ
const int WALL_SIZE = 6;			// 壁の厚さ
const int MAZE_SIZE = 32;			// 表示する区画数

void plotAdditionalLine(void)
{
	// 区画線の表示
	std::map<std::string, std::string> args = {
		{"color", "silver"}, 
		{"linestyle", "--"}
	};
	for (int i = 0; i <= MAZE_SIZE * 2; i++) {
		double x_min = -HALF_SECTION_SIZE;
		double x_max = MAZE_SIZE * SECTION_SIZE - HALF_SECTION_SIZE;
		double x_min2max = x_min + i * HALF_SECTION_SIZE;
		double x_max2min = x_max - i * HALF_SECTION_SIZE;

		// 直線の表示
		plt::plot({x_min, x_max}, {x_min2max, x_min2max}, args);
		plt::plot({x_min2max, x_min2max}, {x_min, x_max}, args);

		// 斜め線の表示
		if (i % 2 == 1) {
			plt::plot({x_min, x_max2min}, {x_min2max, x_max}, args);
			plt::plot({x_min2max, x_max}, {x_min, x_max2min}, args);
			plt::plot({x_min, x_min2max}, {x_min2max, x_min}, args);
			plt::plot({x_max2min, x_max}, {x_max, x_max2min}, args);
		} else;
	}
}

void plotPillar(void)
{
	// 柱の表示
	for(int j = 0; j <= MAZE_SIZE; j++) {
		for(int i = 0; i <= MAZE_SIZE; i++ ) {
			double x_center = i * SECTION_SIZE - HALF_SECTION_SIZE;
			double y_center = j * SECTION_SIZE - HALF_SECTION_SIZE;
			std::vector<double> x = {x_center - WALL_SIZE/2, x_center - WALL_SIZE/2, x_center + WALL_SIZE/2, x_center + WALL_SIZE/2};
			std::vector<double> y = {y_center - WALL_SIZE/2, y_center + WALL_SIZE/2, y_center + WALL_SIZE/2, y_center - WALL_SIZE/2};
			plt::fill(x, y, {{"color", "red"}});
		}
	}
}

void plotWall(int x, int y, int dir)
{
	double x_center;
	double y_center;
	std::vector<double> x_plot;
	std::vector<double> y_plot;

	if(dir == EAST || dir == WEST) {
		y_center = y * SECTION_SIZE;
		if(dir == WEST) {
			x_center = x * SECTION_SIZE - HALF_SECTION_SIZE;
		} else {
			x_center = x * SECTION_SIZE + HALF_SECTION_SIZE;
		}
		x_plot = {x_center - WALL_SIZE/2, x_center - WALL_SIZE/2, x_center + WALL_SIZE/2, x_center + WALL_SIZE/2};
		y_plot = {y_center - HALF_SECTION_SIZE, y_center + HALF_SECTION_SIZE, y_center + HALF_SECTION_SIZE, y_center - HALF_SECTION_SIZE};
		
	} else if(dir == NORTH || dir == SOUTH) {
		x_center = x * SECTION_SIZE;
		if(dir == SOUTH) {
			y_center = y * SECTION_SIZE - HALF_SECTION_SIZE;
		} else {
			y_center = y * SECTION_SIZE + HALF_SECTION_SIZE;
		}
		x_plot = {x_center - HALF_SECTION_SIZE, x_center - HALF_SECTION_SIZE, x_center + HALF_SECTION_SIZE, x_center + HALF_SECTION_SIZE};
		y_plot = {y_center - WALL_SIZE/2, y_center + WALL_SIZE/2, y_center + WALL_SIZE/2, y_center - WALL_SIZE/2};
	}
	plt::fill(x_plot, y_plot, {{"color", "red"}});
}

void plotWallAll(void)
{
	plotPillar();
	for(int i = 0; i < MAZE_X; i++) {
		plotWall(i, 0, SOUTH);
	}

	for(int j = 0; j < MAZE_Y; j++) {
		plotWall(0, j, WEST);
	}

	for(int j = 0; j < MAZE_Y; j++) {
		for(int i = 0; i < MAZE_X; i++) {
			if(application::maze::getInstance().getGlobalData(i, j, NORTH) == true) {
				plotWall(i, j, NORTH);
			} else;

			if(application::maze::getInstance().getGlobalData(i, j, EAST) == true) {
				plotWall(i, j, EAST);
			} else;
		}
	}
}

void plotMove(int dir)
{
	t_position old_position = application::position::getInstance().getMyPlace();
	t_position new_position = application::position::getInstance().moveMyPlace(dir);
	
	std::vector<double> x = {static_cast<double>(old_position.x) * SECTION_SIZE,
							 static_cast<double>(new_position.x) * SECTION_SIZE};
	std::vector<double> y = {static_cast<double>(old_position.y) * SECTION_SIZE,
							 static_cast<double>(new_position.y) * SECTION_SIZE};

	switch (old_position.dir) {
		case EAST:	x[0] -= HALF_SECTION_SIZE;	break;
		case NORTH:	y[0] -= HALF_SECTION_SIZE;	break;
		case WEST:	x[0] += HALF_SECTION_SIZE;	break;
		case SOUTH:	y[0] += HALF_SECTION_SIZE;	break;
	}

	switch (new_position.dir) {
		case EAST:	x[1] -= HALF_SECTION_SIZE;	break;
		case NORTH:	y[1] -= HALF_SECTION_SIZE;	break;
		case WEST:	x[1] += HALF_SECTION_SIZE;	break;
		case SOUTH:	y[1] += HALF_SECTION_SIZE;	break;
	}
	plt::plot(x, y, "b-");
}

void plotAdachi(const bool is_display_map = true)
{
	application::potential::getInstance().makeMap(GOAL_X, GOAL_Y);
	if(is_display_map) {
		for(int j = 0; j < MAZE_Y; j++) {
			for(int i = 0; i < MAZE_X; i++) {
				char ch[2];
				sprintf(ch, "%02x", application::potential::getInstance().getAroundSection(i, j) & 0x00ff);
				plt::text(i * 90 - 30, j * 90 - 20, {ch});
			}
		}
	}

	application::position::getInstance().reset();
	t_position ego = application::position::getInstance().moveMyPlace(FRONT);
	plt::plot({0, 0}, {0, HALF_SECTION_SIZE}, "b-");

	while(application::position::getInstance().getIsGoal(GOAL_X, GOAL_Y) == false) {
		ego = application::position::getInstance().getMyPlace();
		int8_t next_direction = application::adachi::getInstance().getNextDirection(&ego);
		plotMove(next_direction);
	}

	for(int i = 0; i < GOAL_SIZE - 1; i++) {
		plotMove(FRONT);
	}
	ego = application::position::getInstance().getMyPlace();
	std::vector<double> x = {static_cast<double>(ego.x) * SECTION_SIZE,
							 static_cast<double>(ego.x) * SECTION_SIZE};
	std::vector<double> y = {static_cast<double>(ego.y) * SECTION_SIZE,
							 static_cast<double>(ego.y) * SECTION_SIZE};
	switch (ego.dir) {
		case EAST:	x[0] -= HALF_SECTION_SIZE;	break;
		case NORTH:	y[0] -= HALF_SECTION_SIZE;	break;
		case WEST:	x[0] += HALF_SECTION_SIZE;	break;
		case SOUTH:	y[0] += HALF_SECTION_SIZE;	break;
	}
	plt::plot(x, y, "b-");
}

void Dijkstra_DebugPrintf(int8_t, int8_t);
t_position Dijkstra_ConvertPath(int8_t, int8_t);

int main() {
	application::maze::getInstance().setDebugData();
	plotWallAll();
	plotAdachi(false);

	application::position::getInstance().reset();
	Dijkstra_ConvertPath(GOAL_X, GOAL_Y);
	application::path::getInstance().displayAll();

	std::vector<int> ticks;
	std::vector<std::string> labels;

	for(int i = 0; i < MAZE_SIZE; i++) {
		ticks.push_back(i * SECTION_SIZE);
		labels.push_back(std::to_string(i));
	}
	plt::xticks(ticks, labels);
	plt::yticks(ticks, labels);
	
	plt::set_aspect_equal();
	plt::tight_layout();
	plt::show();

	return 0;
}