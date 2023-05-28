#include <iostream>
#include <vector>
#include "position.hpp"
#include "maze.hpp"
#include "potential.hpp"
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
	application::maze::getInstance().setDebugData();

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

int main() {
	plotWallAll();

	std::vector<int> ticks;
	std::vector<std::string> labels;

	for(int i = 0; i < MAZE_SIZE; i++) {
		ticks.push_back(i * SECTION_SIZE);
		labels.push_back(std::to_string(i));
	}
	plt::xticks(ticks, labels);
	plt::yticks(ticks, labels);

	application::potential::getInstance().makeMap(GOAL_X, GOAL_Y);
	for(int j = 0; j < MAZE_Y; j++) {
		for(int i = 0; i < MAZE_X; i++) {
			char ch[2];
			sprintf(ch, "%02x", application::potential::getInstance().getAroundSection(i, j) & 0x00ff);
			plt::text(i * 90 - 30, j * 90 - 20, {ch});
		}
	}
	
	plt::set_aspect_equal();
	plt::tight_layout();
	plt::show();

	return 0;
}