################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/renesas/compiler/init/_exit.c \
../src/renesas/compiler/init/_kill.c \
../src/renesas/compiler/init/dso_handle.c \
../src/renesas/compiler/init/fini.c \
../src/renesas/compiler/init/init.c \
../src/renesas/compiler/init/r_compiler_abstraction.c \
../src/renesas/compiler/init/resetprg.c \
../src/renesas/compiler/init/sections.c \
../src/renesas/compiler/init/syscalls.c 

C_DEPS += \
./src/renesas/compiler/init/_exit.d \
./src/renesas/compiler/init/_kill.d \
./src/renesas/compiler/init/dso_handle.d \
./src/renesas/compiler/init/fini.d \
./src/renesas/compiler/init/init.d \
./src/renesas/compiler/init/r_compiler_abstraction.d \
./src/renesas/compiler/init/resetprg.d \
./src/renesas/compiler/init/sections.d \
./src/renesas/compiler/init/syscalls.d 

OBJS += \
./src/renesas/compiler/init/_exit.o \
./src/renesas/compiler/init/_kill.o \
./src/renesas/compiler/init/dso_handle.o \
./src/renesas/compiler/init/fini.o \
./src/renesas/compiler/init/init.o \
./src/renesas/compiler/init/r_compiler_abstraction.o \
./src/renesas/compiler/init/resetprg.o \
./src/renesas/compiler/init/sections.o \
./src/renesas/compiler/init/syscalls.o 


# Each subdirectory must supply rules for building sources it contributes
src/renesas/compiler/init/%.o: ../src/renesas/compiler/init/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -O0 -ffunction-sections -fdata-sections -Wnull-dereference -Wmaybe-uninitialized -g3 -Wstack-usage=4096 -DNO_ICONV -D__MBED__ -D_DEVICE_RZA2_ -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\compiler\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\configuration" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\os_abstraction\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\system\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_cache\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_cpg\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_gpio\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_intc\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_mmu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_stb\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_cbuffer\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_ceu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_drp\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_jcu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_mipi\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_ostm\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_riic\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_rvapi\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_scifa\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_vdc\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\lib\drp" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\os\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\os\inc\private" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\os\FreeRTOS\Include" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\os\FreeRTOS\Portable\gcc\arm_ca9" -I"D:/Projects/e2_studio/RZ_A2M/workspace/Renesas_9.10_final/app/RZA2M_DRP_Benchmark/src/renesas/application/inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\Translator" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car\binarization" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car\syn6288" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car\Track" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car\ParallelSendData" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car\NRF24L01" -fabi-version=0 -Wa,-adhlns="$@.lst" -fdiagnostics-parseable-fixits -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


