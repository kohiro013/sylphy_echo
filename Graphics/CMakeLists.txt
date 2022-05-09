cmake_minimum_required(VERSION 3.13)
project(plt)

# includes
include_directories(
    Inc
)
 
# sources
file(GLOB SRCS
    Src/*.cpp
)
add_executable(plt ${SRCS})
 
# matplotlibcppで使うPythonとNumpyを探す
find_package(Python3 COMPONENTS Development NumPy)
target_include_directories(plt PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
target_link_libraries(plt Python3::Python Python3::NumPy)