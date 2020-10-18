################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/include/stm32f4-hal/stm32f4xx_hal_exti.c 

OBJS += \
./system/include/stm32f4-hal/stm32f4xx_hal_exti.o 

C_DEPS += \
./system/include/stm32f4-hal/stm32f4xx_hal_exti.d 


# Each subdirectory must supply rules for building sources it contributes
system/include/stm32f4-hal/%.o: ../system/include/stm32f4-hal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wextra  -g3 -ggdb -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I/usr/include/gsl -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


