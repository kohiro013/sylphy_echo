set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_SYSTEM_NAME Generic)

# tool chain
set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_EXE_LINKER   arm-none-eabi-g++)
set(OBJCOPY            arm-none-eabi-objcopy)
set(SIZE               arm-none-eabi-size)

# mpu
set(CPU       cortex-m4)
set(FPU       -mfpu=fpv4-sp-d16)
set(FLOAT_ABI -mfloat-abi=hard)
set(LDSCRIPT  ../STM32F411CEUx_FLASH.ld)

# compiler options
add_compile_options(-Og)
add_compile_options(-Wall)
add_compile_options(-ffunction-sections)
add_compile_options(-fdata-sections)
add_compile_options(-fstack-usage)

set(CMAKE_C_FLAGS "-mcpu=${CPU} -std=gnu11 ${FPU} ${FLOAT_ABI} -mthumb")
set(CMAKE_CXX_FLAGS "-mcpu=${CPU} -std=gnu++17 -fno-use-cxa-atexit ${FPU} ${FLOAT_ABI} -mthumb")
set(CMAKE_AS_FLAGS "-mcpu=${CPU} -x assembler-with-cpp ${FPU} ${FLOAT_ABI} -mthumb")
set(CMAKE_EXE_LINKER_FLAGS "-mcpu=${CPU} -T${CMAKE_CURRENT_SOURCE_DIR}/${LDSCRIPT} --specs=nosys.specs -Wl,--gc-sections -static 
	--specs=nano.specs ${FPU} ${FLOAT_ABI} -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group -Wl,--print-memory-usage -u _printf_float -u _scanf_float")