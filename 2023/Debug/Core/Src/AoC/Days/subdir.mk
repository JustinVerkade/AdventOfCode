################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AoC/Days/Day01.c \
../Core/Src/AoC/Days/Day02.c \
../Core/Src/AoC/Days/Day03.c \
../Core/Src/AoC/Days/Day04.c \
../Core/Src/AoC/Days/Day05.c \
../Core/Src/AoC/Days/Day06.c \
../Core/Src/AoC/Days/Day07.c \
../Core/Src/AoC/Days/Day08.c \
../Core/Src/AoC/Days/Day09.c \
../Core/Src/AoC/Days/Day10.c \
../Core/Src/AoC/Days/Day11.c \
../Core/Src/AoC/Days/Day12.c \
../Core/Src/AoC/Days/Day13.c \
../Core/Src/AoC/Days/Day14.c \
../Core/Src/AoC/Days/Day15.c \
../Core/Src/AoC/Days/Day16.c \
../Core/Src/AoC/Days/Day17.c \
../Core/Src/AoC/Days/Day18.c \
../Core/Src/AoC/Days/Day19.c \
../Core/Src/AoC/Days/Day20.c \
../Core/Src/AoC/Days/Day21.c \
../Core/Src/AoC/Days/Day22.c \
../Core/Src/AoC/Days/Day23.c \
../Core/Src/AoC/Days/Day24.c \
../Core/Src/AoC/Days/Day25.c \
../Core/Src/AoC/Days/DayTemplace.c 

OBJS += \
./Core/Src/AoC/Days/Day01.o \
./Core/Src/AoC/Days/Day02.o \
./Core/Src/AoC/Days/Day03.o \
./Core/Src/AoC/Days/Day04.o \
./Core/Src/AoC/Days/Day05.o \
./Core/Src/AoC/Days/Day06.o \
./Core/Src/AoC/Days/Day07.o \
./Core/Src/AoC/Days/Day08.o \
./Core/Src/AoC/Days/Day09.o \
./Core/Src/AoC/Days/Day10.o \
./Core/Src/AoC/Days/Day11.o \
./Core/Src/AoC/Days/Day12.o \
./Core/Src/AoC/Days/Day13.o \
./Core/Src/AoC/Days/Day14.o \
./Core/Src/AoC/Days/Day15.o \
./Core/Src/AoC/Days/Day16.o \
./Core/Src/AoC/Days/Day17.o \
./Core/Src/AoC/Days/Day18.o \
./Core/Src/AoC/Days/Day19.o \
./Core/Src/AoC/Days/Day20.o \
./Core/Src/AoC/Days/Day21.o \
./Core/Src/AoC/Days/Day22.o \
./Core/Src/AoC/Days/Day23.o \
./Core/Src/AoC/Days/Day24.o \
./Core/Src/AoC/Days/Day25.o \
./Core/Src/AoC/Days/DayTemplace.o 

C_DEPS += \
./Core/Src/AoC/Days/Day01.d \
./Core/Src/AoC/Days/Day02.d \
./Core/Src/AoC/Days/Day03.d \
./Core/Src/AoC/Days/Day04.d \
./Core/Src/AoC/Days/Day05.d \
./Core/Src/AoC/Days/Day06.d \
./Core/Src/AoC/Days/Day07.d \
./Core/Src/AoC/Days/Day08.d \
./Core/Src/AoC/Days/Day09.d \
./Core/Src/AoC/Days/Day10.d \
./Core/Src/AoC/Days/Day11.d \
./Core/Src/AoC/Days/Day12.d \
./Core/Src/AoC/Days/Day13.d \
./Core/Src/AoC/Days/Day14.d \
./Core/Src/AoC/Days/Day15.d \
./Core/Src/AoC/Days/Day16.d \
./Core/Src/AoC/Days/Day17.d \
./Core/Src/AoC/Days/Day18.d \
./Core/Src/AoC/Days/Day19.d \
./Core/Src/AoC/Days/Day20.d \
./Core/Src/AoC/Days/Day21.d \
./Core/Src/AoC/Days/Day22.d \
./Core/Src/AoC/Days/Day23.d \
./Core/Src/AoC/Days/Day24.d \
./Core/Src/AoC/Days/Day25.d \
./Core/Src/AoC/Days/DayTemplace.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/AoC/Days/%.o Core/Src/AoC/Days/%.su Core/Src/AoC/Days/%.cyclo: ../Core/Src/AoC/Days/%.c Core/Src/AoC/Days/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F745xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-AoC-2f-Days

clean-Core-2f-Src-2f-AoC-2f-Days:
	-$(RM) ./Core/Src/AoC/Days/Day01.cyclo ./Core/Src/AoC/Days/Day01.d ./Core/Src/AoC/Days/Day01.o ./Core/Src/AoC/Days/Day01.su ./Core/Src/AoC/Days/Day02.cyclo ./Core/Src/AoC/Days/Day02.d ./Core/Src/AoC/Days/Day02.o ./Core/Src/AoC/Days/Day02.su ./Core/Src/AoC/Days/Day03.cyclo ./Core/Src/AoC/Days/Day03.d ./Core/Src/AoC/Days/Day03.o ./Core/Src/AoC/Days/Day03.su ./Core/Src/AoC/Days/Day04.cyclo ./Core/Src/AoC/Days/Day04.d ./Core/Src/AoC/Days/Day04.o ./Core/Src/AoC/Days/Day04.su ./Core/Src/AoC/Days/Day05.cyclo ./Core/Src/AoC/Days/Day05.d ./Core/Src/AoC/Days/Day05.o ./Core/Src/AoC/Days/Day05.su ./Core/Src/AoC/Days/Day06.cyclo ./Core/Src/AoC/Days/Day06.d ./Core/Src/AoC/Days/Day06.o ./Core/Src/AoC/Days/Day06.su ./Core/Src/AoC/Days/Day07.cyclo ./Core/Src/AoC/Days/Day07.d ./Core/Src/AoC/Days/Day07.o ./Core/Src/AoC/Days/Day07.su ./Core/Src/AoC/Days/Day08.cyclo ./Core/Src/AoC/Days/Day08.d ./Core/Src/AoC/Days/Day08.o ./Core/Src/AoC/Days/Day08.su ./Core/Src/AoC/Days/Day09.cyclo ./Core/Src/AoC/Days/Day09.d ./Core/Src/AoC/Days/Day09.o ./Core/Src/AoC/Days/Day09.su ./Core/Src/AoC/Days/Day10.cyclo ./Core/Src/AoC/Days/Day10.d ./Core/Src/AoC/Days/Day10.o ./Core/Src/AoC/Days/Day10.su ./Core/Src/AoC/Days/Day11.cyclo ./Core/Src/AoC/Days/Day11.d ./Core/Src/AoC/Days/Day11.o ./Core/Src/AoC/Days/Day11.su ./Core/Src/AoC/Days/Day12.cyclo ./Core/Src/AoC/Days/Day12.d ./Core/Src/AoC/Days/Day12.o ./Core/Src/AoC/Days/Day12.su ./Core/Src/AoC/Days/Day13.cyclo ./Core/Src/AoC/Days/Day13.d ./Core/Src/AoC/Days/Day13.o ./Core/Src/AoC/Days/Day13.su ./Core/Src/AoC/Days/Day14.cyclo ./Core/Src/AoC/Days/Day14.d ./Core/Src/AoC/Days/Day14.o ./Core/Src/AoC/Days/Day14.su ./Core/Src/AoC/Days/Day15.cyclo ./Core/Src/AoC/Days/Day15.d ./Core/Src/AoC/Days/Day15.o ./Core/Src/AoC/Days/Day15.su ./Core/Src/AoC/Days/Day16.cyclo ./Core/Src/AoC/Days/Day16.d ./Core/Src/AoC/Days/Day16.o ./Core/Src/AoC/Days/Day16.su ./Core/Src/AoC/Days/Day17.cyclo ./Core/Src/AoC/Days/Day17.d ./Core/Src/AoC/Days/Day17.o ./Core/Src/AoC/Days/Day17.su ./Core/Src/AoC/Days/Day18.cyclo ./Core/Src/AoC/Days/Day18.d ./Core/Src/AoC/Days/Day18.o ./Core/Src/AoC/Days/Day18.su ./Core/Src/AoC/Days/Day19.cyclo ./Core/Src/AoC/Days/Day19.d ./Core/Src/AoC/Days/Day19.o ./Core/Src/AoC/Days/Day19.su ./Core/Src/AoC/Days/Day20.cyclo ./Core/Src/AoC/Days/Day20.d ./Core/Src/AoC/Days/Day20.o ./Core/Src/AoC/Days/Day20.su ./Core/Src/AoC/Days/Day21.cyclo ./Core/Src/AoC/Days/Day21.d ./Core/Src/AoC/Days/Day21.o ./Core/Src/AoC/Days/Day21.su ./Core/Src/AoC/Days/Day22.cyclo ./Core/Src/AoC/Days/Day22.d ./Core/Src/AoC/Days/Day22.o ./Core/Src/AoC/Days/Day22.su ./Core/Src/AoC/Days/Day23.cyclo ./Core/Src/AoC/Days/Day23.d ./Core/Src/AoC/Days/Day23.o ./Core/Src/AoC/Days/Day23.su ./Core/Src/AoC/Days/Day24.cyclo ./Core/Src/AoC/Days/Day24.d ./Core/Src/AoC/Days/Day24.o ./Core/Src/AoC/Days/Day24.su ./Core/Src/AoC/Days/Day25.cyclo ./Core/Src/AoC/Days/Day25.d ./Core/Src/AoC/Days/Day25.o ./Core/Src/AoC/Days/Day25.su ./Core/Src/AoC/Days/DayTemplace.cyclo ./Core/Src/AoC/Days/DayTemplace.d ./Core/Src/AoC/Days/DayTemplace.o ./Core/Src/AoC/Days/DayTemplace.su

.PHONY: clean-Core-2f-Src-2f-AoC-2f-Days

