################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../generate/compiler/asm/cpu.asm \
../generate/compiler/asm/initsect.asm \
../generate/compiler/asm/reset_handler.asm \
../generate/compiler/asm/vector_table.asm \
../generate/compiler/asm/vfp_init.asm 

OBJS += \
./generate/compiler/asm/cpu.o \
./generate/compiler/asm/initsect.o \
./generate/compiler/asm/reset_handler.o \
./generate/compiler/asm/vector_table.o \
./generate/compiler/asm/vfp_init.o 

ASM_DEPS += \
./generate/compiler/asm/cpu.d \
./generate/compiler/asm/initsect.d \
./generate/compiler/asm/reset_handler.d \
./generate/compiler/asm/vector_table.d \
./generate/compiler/asm/vfp_init.d 


# Each subdirectory must supply rules for building sources it contributes
generate/compiler/asm/%.o: ../generate/compiler/asm/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Os -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g3 -Wstack-usage=100 -x assembler-with-cpp -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


