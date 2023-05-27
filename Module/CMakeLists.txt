include_directories(
    ../Drivers/STM32F4xx_HAL_Driver/Inc
    ../Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ../Drivers/CMSIS/Include
    ../Core/Inc
    ../Library/CMSIS-DSP/Include
    ../Library/CMSIS-DSP/Include/dsp
    ../Library/CMSIS-DSP/PrivateInclude
    ../Library/ntshell-v0.3.1/lib/core
    ../Library/ntshell-v0.3.1/lib/util
	../Library/MyLibrary
	../Application/Inc
    Inc
)
file(GLOB SRCS
    Src/*.cpp
)
add_library(module STATIC ${SRCS})
