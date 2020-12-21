################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../generate/drivers/r_spibsc/src/lld/r_spibsc_adjust_timing_data.c \
../generate/drivers/r_spibsc/src/lld/r_spibsc_lld_rza2m.c \
../generate/drivers/r_spibsc/src/lld/r_spibsc_sflash_userdef.c 

C_DEPS += \
./generate/drivers/r_spibsc/src/lld/r_spibsc_adjust_timing_data.d \
./generate/drivers/r_spibsc/src/lld/r_spibsc_lld_rza2m.d \
./generate/drivers/r_spibsc/src/lld/r_spibsc_sflash_userdef.d 

OBJS += \
./generate/drivers/r_spibsc/src/lld/r_spibsc_adjust_timing_data.o \
./generate/drivers/r_spibsc/src/lld/r_spibsc_lld_rza2m.o \
./generate/drivers/r_spibsc/src/lld/r_spibsc_sflash_userdef.o 


# Each subdirectory must supply rules for building sources it contributes
generate/drivers/r_spibsc/src/lld/%.o: ../generate/drivers/r_spibsc/src/lld/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Og -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g3 -Wstack-usage=100 -I"D:/Project/e2_studio/workspace2/RZA2M_DRP_Benchmark_V1.0.0/RZA2M_DRP_Benchmark_V1.0.0/bootloader/generate/compiler/inc" -I"D:/Project/e2_studio/workspace2/RZA2M_DRP_Benchmark_V1.0.0/RZA2M_DRP_Benchmark_V1.0.0/bootloader/generate/drivers/r_cpg/inc" -I"D:/Project/e2_studio/workspace2/RZA2M_DRP_Benchmark_V1.0.0/RZA2M_DRP_Benchmark_V1.0.0/bootloader/generate/drivers/r_spibsc/inc" -I"D:/Project/e2_studio/workspace2/RZA2M_DRP_Benchmark_V1.0.0/RZA2M_DRP_Benchmark_V1.0.0/bootloader/generate/drivers/r_stb/inc" -I"D:/Project/e2_studio/workspace2/RZA2M_DRP_Benchmark_V1.0.0/RZA2M_DRP_Benchmark_V1.0.0/bootloader/generate/system/inc" -I"D:/Project/e2_studio/workspace2/RZA2M_DRP_Benchmark_V1.0.0/RZA2M_DRP_Benchmark_V1.0.0/bootloader/generate/configuration" -fabi-version=0 -Wa,-adhlns="$@.lst" -fdiagnostics-parseable-fixits -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


