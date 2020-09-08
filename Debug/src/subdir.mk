################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/init.c \
../src/uart.c 

CPP_SRCS += \
../src/main.cpp 

C_DEPS += \
./src/init.d \
./src/uart.d 

OBJS += \
./src/init.o \
./src/main.o \
./src/uart.o 

CPP_DEPS += \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/init.o: ../src/init.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/Users/xieyu/Documents/Microprocessor/lab1/inc" -I"C:/Users/xieyu/Documents/Microprocessor/stm32lib/CMSIS/Include" -I"C:/Users/xieyu/Documents/Microprocessor/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/xieyu/Documents/Microprocessor/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"src/init.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
src/main.o: ../src/main.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/Users/xieyu/Documents/Microprocessor/lab1/inc" -I"C:/Users/xieyu/Documents/Microprocessor/stm32lib/CMSIS/Include" -I"C:/Users/xieyu/Documents/Microprocessor/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/xieyu/Documents/Microprocessor/stm32lib/STM32F7xx_HAL_Driver/Inc" -O0 -ffunction-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
src/uart.o: ../src/uart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/Users/xieyu/Documents/Microprocessor/lab1/inc" -I"C:/Users/xieyu/Documents/Microprocessor/stm32lib/CMSIS/Include" -I"C:/Users/xieyu/Documents/Microprocessor/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/xieyu/Documents/Microprocessor/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"src/uart.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

