#include <iostream>
#include <vector>
#include "maze.hpp"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

const int HALF_SECTION_SIZE = 45;	// 半区画の長さ
const int SECTION_SIZE = 2 * HALF_SECTION_SIZE;		// 区画の長さ
const int WALL_SIZE = 6;			// 壁の厚さ
const int MAZE_SIZE = 16;			// 表示する区画数

int main() {
	// 区画線の表示
	std::map<std::string, std::string> args = {
		{"color", "silver"}, 
		{"linestyle", "--"}
	};
	for (int i = 0; i <= MAZE_SIZE * 2 + 1; i++) {
		double x_min = -HALF_SECTION_SIZE;
		double x_max = MAZE_SIZE * SECTION_SIZE;
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

	plt::axis("equal");
	plt::tight_layout();
	plt::show();

	return 0;
}