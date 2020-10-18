################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/alglib/src/alglibinternal.cpp \
../src/alglib/src/alglibmisc.cpp \
../src/alglib/src/ap.cpp \
../src/alglib/src/diffequations.cpp \
../src/alglib/src/fasttransforms.cpp \
../src/alglib/src/integration.cpp \
../src/alglib/src/interpolation.cpp \
../src/alglib/src/linalg.cpp \
../src/alglib/src/optimization.cpp \
../src/alglib/src/solvers.cpp \
../src/alglib/src/specialfunctions.cpp \
../src/alglib/src/statistics.cpp 

OBJS += \
./src/alglib/src/alglibinternal.o \
./src/alglib/src/alglibmisc.o \
./src/alglib/src/ap.o \
./src/alglib/src/diffequations.o \
./src/alglib/src/fasttransforms.o \
./src/alglib/src/integration.o \
./src/alglib/src/interpolation.o \
./src/alglib/src/linalg.o \
./src/alglib/src/optimization.o \
./src/alglib/src/solvers.o \
./src/alglib/src/specialfunctions.o \
./src/alglib/src/statistics.o 

CPP_DEPS += \
./src/alglib/src/alglibinternal.d \
./src/alglib/src/alglibmisc.d \
./src/alglib/src/ap.d \
./src/alglib/src/diffequations.d \
./src/alglib/src/fasttransforms.d \
./src/alglib/src/integration.d \
./src/alglib/src/interpolation.d \
./src/alglib/src/linalg.d \
./src/alglib/src/optimization.d \
./src/alglib/src/solvers.d \
./src/alglib/src/specialfunctions.d \
./src/alglib/src/statistics.d 


# Each subdirectory must supply rules for building sources it contributes
src/alglib/src/%.o: ../src/alglib/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wextra  -g3 -ggdb -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DAE_NO_EXCEPTIONS -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=c++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


