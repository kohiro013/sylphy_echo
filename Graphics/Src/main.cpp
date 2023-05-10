#include <iostream>
 
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
 
int main() {
    plt::plot({1, 2, 4, 8, 16});
    plt::show();

    printf("Hello C++ World!\r\n");
    
    return 0;
}