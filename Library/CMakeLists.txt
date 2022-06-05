cmake_minimum_required(VERSION 3.13)

# includes
include_directories(
    CMSIS-DSP/Include
	CMSIS-DSP/Include/dsp
	CMSIS-DSP/PrivateInclude
	ntshell-v0.3.1/lib/core
	ntshell-v0.3.1/lib/util
    ../Drivers/CMSIS/Include
)

# sources
file(GLOB SRCS
    CMSIS-DSP/Source/**/*.c
	ntshell-v0.3.1/lib/core/*.c
	ntshell-v0.3.1/lib/util/*.c
)

add_library(library STATIC ${SRCS})