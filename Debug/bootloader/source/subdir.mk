################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bootloader/source/bootloader.c 

OBJS += \
./bootloader/source/bootloader.o 

C_DEPS += \
./bootloader/source/bootloader.d 


# Each subdirectory must supply rules for building sources it contributes
bootloader/source/%.o bootloader/source/%.su: ../bootloader/source/%.c bootloader/source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/bhagwat/bws/Embedded/projects/stm32/baremetal/STM32F4_Bootloader/bootloader/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-bootloader-2f-source

clean-bootloader-2f-source:
	-$(RM) ./bootloader/source/bootloader.d ./bootloader/source/bootloader.o ./bootloader/source/bootloader.su

.PHONY: clean-bootloader-2f-source

