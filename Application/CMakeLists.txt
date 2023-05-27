include_directories(
	../Library/MyLibrary
    Inc
)
file(GLOB SRCS
    Src/*.cpp
)
add_library(application STATIC ${SRCS})
