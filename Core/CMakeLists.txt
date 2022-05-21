cmake_minimum_required(VERSION 3.13)

# project
project(shyphy_echo)
set(ELF_FILE ${PROJECT_NAME}.elf)
set(HEX_FILE ${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_NAME}.bin)
set(MAP_FILE ${PROJECT_NAME}.map)

enable_language(ASM)

# define
add_definitions(-DDUSE_HAL_DRIVER)
add_definitions(-DUSE_FULL_LL_DRIVER)
add_definitions(-DHSE_VALUE=25000000)
add_definitions(-DHSE_STARTUP_TIMEOUT=100)
add_definitions(-DLSE_STARTUP_TIMEOUT=5000)
add_definitions(-DLSE_VALUE=32768)
add_definitions(-DEXTERNAL_CLOCK_VALUE=12288000)
add_definitions(-DHSI_VALUE=16000000)
add_definitions(-DLSI_VALUE=32000)
add_definitions(-DVDD_VALUE=3300)
add_definitions(-DPREFETCH_ENABLE=1)
add_definitions(-DINSTRUCTION_CACHE_ENABLE=1)
add_definitions(-DDATA_CACHE_ENABLE=1)
add_definitions(-DSTM32F411xE)

# sub_directory
add_subdirectory(../Drivers/STM32F4xx_HAL_Driver ../build/Drivers/STM32F4xx_HAL_Driver)

# includes
include_directories(
    Inc
    ../Drivers/STM32F4xx_HAL_Driver/Inc
    ../Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ../Drivers/CMSIS/Include
)

# sources
file(GLOB SRCS
    Src/*.c
    ../startup_stm32f411xe.s
)

# executables
add_executable(${ELF_FILE} ${SRCS})

# target_link
target_link_libraries(${ELF_FILE}
	hal
)

add_custom_command(TARGET ${ELF_FILE} POST_BUILD
    COMMAND ${OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
    COMMAND ${OBJCOPY} -Oihex   ${ELF_FILE} ${HEX_FILE}
    COMMAND ${SIZE} --format=berkeley ${ELF_FILE} ${HEX_FILE}
    COMMENT "Invoking: Cross ARM GNU Print Size"
)