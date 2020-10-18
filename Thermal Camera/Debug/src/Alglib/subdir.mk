################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Alglib/alglibinternal.cpp \
../src/Alglib/alglibmisc.cpp \
../src/Alglib/ap.cpp \
../src/Alglib/dataanalysis.cpp \
../src/Alglib/fasttransforms.cpp \
../src/Alglib/integration.cpp \
../src/Alglib/interpolation.cpp \
../src/Alglib/linalg.cpp \
../src/Alglib/optimization.cpp \
../src/Alglib/solvers.cpp \
../src/Alglib/specialfunctions.cpp \
../src/Alglib/statistics.cpp 

OBJS += \
./src/Alglib/alglibinternal.o \
./src/Alglib/alglibmisc.o \
./src/Alglib/ap.o \
./src/Alglib/dataanalysis.o \
./src/Alglib/fasttransforms.o \
./src/Alglib/integration.o \
./src/Alglib/interpolation.o \
./src/Alglib/linalg.o \
./src/Alglib/optimization.o \
./src/Alglib/solvers.o \
./src/Alglib/specialfunctions.o \
./src/Alglib/statistics.o 

CPP_DEPS += \
./src/Alglib/alglibinternal.d \
./src/Alglib/alglibmisc.d \
./src/Alglib/ap.d \
./src/Alglib/dataanalysis.d \
./src/Alglib/fasttransforms.d \
./src/Alglib/integration.d \
./src/Alglib/interpolation.d \
./src/Alglib/linalg.d \
./src/Alglib/optimization.d \
./src/Alglib/solvers.d \
./src/Alglib/specialfunctions.d \
./src/Alglib/statistics.d 


# Each subdirectory must supply rules for building sources it contributes
src/Alglib/%.o: ../src/Alglib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wextra  -g3 -ggdb -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I/usr/include/gsl -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


