################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/renesas/compiler/asm/cpu.asm \
../src/renesas/compiler/asm/initsect.asm \
../src/renesas/compiler/asm/irq.asm \
../src/renesas/compiler/asm/irqfiq_handler.asm \
../src/renesas/compiler/asm/mmu_operation.asm \
../src/renesas/compiler/asm/r_cache_l1_rza2.asm \
../src/renesas/compiler/asm/reset_handler.asm \
../src/renesas/compiler/asm/vector_mirrortable.asm \
../src/renesas/compiler/asm/vector_table.asm \
../src/renesas/compiler/asm/vfp_init.asm 

OBJS += \
./src/renesas/compiler/asm/cpu.o \
./src/renesas/compiler/asm/initsect.o \
./src/renesas/compiler/asm/irq.o \
./src/renesas/compiler/asm/irqfiq_handler.o \
./src/renesas/compiler/asm/mmu_operation.o \
./src/renesas/compiler/asm/r_cache_l1_rza2.o \
./src/renesas/compiler/asm/reset_handler.o \
./src/renesas/compiler/asm/vector_mirrortable.o \
./src/renesas/compiler/asm/vector_table.o \
./src/renesas/compiler/asm/vfp_init.o 

ASM_DEPS += \
./src/renesas/compiler/asm/cpu.d \
./src/renesas/compiler/asm/initsect.d \
./src/renesas/compiler/asm/irq.d \
./src/renesas/compiler/asm/irqfiq_handler.d \
./src/renesas/compiler/asm/mmu_operation.d \
./src/renesas/compiler/asm/r_cache_l1_rza2.d \
./src/renesas/compiler/asm/reset_handler.d \
./src/renesas/compiler/asm/vector_mirrortable.d \
./src/renesas/compiler/asm/vector_table.d \
./src/renesas/compiler/asm/vfp_init.d 


# Each subdirectory must supply rules for building sources it contributes
src/renesas/compiler/asm/%.o: ../src/renesas/compiler/asm/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -O0 -ffunction-sections -fdata-sections -Wnull-dereference -Wmaybe-uninitialized -g3 -Wstack-usage=4096 -x assembler-with-cpp -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\lib\drp" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\os_abstraction\inc" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


