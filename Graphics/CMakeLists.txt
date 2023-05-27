cmake_minimum_required(VERSION 3.13)
project(plt)

add_subdirectory(../Application ./build/Application)

# includes
include_directories(
	../Library/MyLibrary
	../Application/Inc
    Inc
)

# sources
file(GLOB SRCS
    Src/*.cpp
)
add_executable(main ${SRCS})

# target_link
target_link_libraries(main
	application
)
 
# matplotlibcppで使うPythonとNumpyを探す
find_package(Python3 COMPONENTS Development NumPy)
target_include_directories(main PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
target_link_libraries(main Python3::Python Python3::NumPy)