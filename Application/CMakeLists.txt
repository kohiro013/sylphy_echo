if(DEFINED "linux")
	include_directories(
		../Library/MyLibrary
		Inc
	)
else()
	include_directories(
		../Drivers/STM32F4xx_HAL_Driver/Inc
		../Drivers/CMSIS/Device/ST/STM32F4xx/Include
		../Drivers/CMSIS/Include
		../Library/CMSIS-DSP/Include
		../Library/CMSIS-DSP/Include/dsp
		../Library/CMSIS-DSP/PrivateInclude
		../Library/MyLibrary
		Inc
	)
endif()

file(GLOB SRCS
    Src/*.cpp
)
add_library(application STATIC ${SRCS})
