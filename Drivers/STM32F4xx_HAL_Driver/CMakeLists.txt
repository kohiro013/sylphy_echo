# suppress the warning "unused parameter" in HAL Driver
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter")

include_directories(Inc)
include_directories(../CMSIS/Include)
include_directories(../CMSIS/Device/ST/STM32F4xx/Include)

set(SRCS 
    Src/stm32f4xx_ll_gpio.c
    Src/stm32f4xx_ll_adc.c
    Src/stm32f4xx_ll_dma.c
    Src/stm32f4xx_ll_rcc.c
    Src/stm32f4xx_ll_utils.c
    Src/stm32f4xx_ll_exti.c
    Src/stm32f4xx_ll_spi.c
    Src/stm32f4xx_ll_tim.c
    Src/stm32f4xx_ll_usart.c
)
add_library(hal STATIC ${SRCS})
