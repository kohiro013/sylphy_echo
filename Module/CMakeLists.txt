include_directories(
    ../Drivers/STM32F4xx_HAL_Driver/Inc
    ../Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ../Drivers/CMSIS/Include
    ../Core/Inc
    Inc
)
file(GLOB SRCS
    Src/*.cpp
)
add_library(module STATIC ${SRCS})
