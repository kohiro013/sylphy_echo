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
add_executable(main ${SRCS})
 
# matplotlibcppで使うPythonとNumpyを探す
find_package(Python3 COMPONENTS Development NumPy)
target_include_directories(main PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
target_link_libraries(main Python3::Python Python3::NumPy)