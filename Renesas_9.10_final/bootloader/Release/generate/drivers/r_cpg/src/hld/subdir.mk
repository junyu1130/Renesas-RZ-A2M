################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../generate/drivers/r_cpg/src/hld/r_cpg_drv_api.c 

C_DEPS += \
./generate/drivers/r_cpg/src/hld/r_cpg_drv_api.d 

OBJS += \
./generate/drivers/r_cpg/src/hld/r_cpg_drv_api.o 


# Each subdirectory must supply rules for building sources it contributes
generate/drivers/r_cpg/src/hld/%.o: ../generate/drivers/r_cpg/src/hld/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Os -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g3 -Wstack-usage=100 -I"D:/Projects/e2_studio/RZ_A2M/workspace/Renesas-9.9/Renesas-9.7-TrafficLight/bootloader/generate/compiler/inc" -I"D:/Projects/e2_studio/RZ_A2M/workspace/Renesas-9.9/Renesas-9.7-TrafficLight/bootloader/generate/drivers/r_cpg/inc" -I"D:/Projects/e2_studio/RZ_A2M/workspace/Renesas-9.9/Renesas-9.7-TrafficLight/bootloader/generate/drivers/r_spibsc/inc" -I"D:/Projects/e2_studio/RZ_A2M/workspace/Renesas-9.9/Renesas-9.7-TrafficLight/bootloader/generate/drivers/r_stb/inc" -I"D:/Projects/e2_studio/RZ_A2M/workspace/Renesas-9.9/Renesas-9.7-TrafficLight/bootloader/generate/system/inc" -I"D:/Projects/e2_studio/RZ_A2M/workspace/Renesas-9.9/Renesas-9.7-TrafficLight/bootloader/generate/configuration" -fabi-version=0 -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


