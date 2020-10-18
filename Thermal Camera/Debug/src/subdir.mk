################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BlinkLed.c \
../src/Communication.c \
../src/MLX90640_API.c \
../src/MLX90640_I2C_Driver.c \
../src/Timer.c \
../src/_write.c \
../src/font12.c \
../src/font16.c \
../src/font20.c \
../src/font22.c \
../src/font22_spec_char.c \
../src/font24.c \
../src/font36.c \
../src/font8.c \
../src/ili9341_cube.c \
../src/ringbuff.c \
../src/stm32f4xx_hal_msp.c 

CPP_SRCS += \
../src/main.cpp 

OBJS += \
./src/BlinkLed.o \
./src/Communication.o \
./src/MLX90640_API.o \
./src/MLX90640_I2C_Driver.o \
./src/Timer.o \
./src/_write.o \
./src/font12.o \
./src/font16.o \
./src/font20.o \
./src/font22.o \
./src/font22_spec_char.o \
./src/font24.o \
./src/font36.o \
./src/font8.o \
./src/ili9341_cube.o \
./src/main.o \
./src/ringbuff.o \
./src/stm32f4xx_hal_msp.o 

C_DEPS += \
./src/BlinkLed.d \
./src/Communication.d \
./src/MLX90640_API.d \
./src/MLX90640_I2C_Driver.d \
./src/Timer.d \
./src/_write.d \
./src/font12.d \
./src/font16.d \
./src/font20.d \
./src/font22.d \
./src/font22_spec_char.d \
./src/font24.d \
./src/font36.d \
./src/font8.d \
./src/ili9341_cube.d \
./src/ringbuff.d \
./src/stm32f4xx_hal_msp.d 

CPP_DEPS += \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wextra  -g3 -ggdb -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I/usr/include/gsl -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wextra  -g3 -ggdb -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I/usr/include/gsl -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/stm32f4xx_hal_msp.o: ../src/stm32f4xx_hal_msp.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wextra  -g3 -ggdb -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I/usr/include/gsl -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -Wno-missing-prototypes -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/stm32f4xx_hal_msp.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


