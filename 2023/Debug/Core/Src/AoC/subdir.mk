################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AoC/AdventOfCode.c 

OBJS += \
./Core/Src/AoC/AdventOfCode.o 

C_DEPS += \
./Core/Src/AoC/AdventOfCode.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/AoC/%.o Core/Src/AoC/%.su Core/Src/AoC/%.cyclo: ../Core/Src/AoC/%.c Core/Src/AoC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F745xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-AoC

clean-Core-2f-Src-2f-AoC:
	-$(RM) ./Core/Src/AoC/AdventOfCode.cyclo ./Core/Src/AoC/AdventOfCode.d ./Core/Src/AoC/AdventOfCode.o ./Core/Src/AoC/AdventOfCode.su

.PHONY: clean-Core-2f-Src-2f-AoC

