################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/renesas/application/dat.asm 

C_SRCS += \
../src/renesas/application/CLI_commands.c \
../src/renesas/application/debug.c \
../src/renesas/application/r_bcd_ae.c \
../src/renesas/application/r_bcd_camera.c \
../src/renesas/application/r_bcd_lcd.c \
../src/renesas/application/r_bcd_main.c \
../src/renesas/application/r_drp.c 

C_DEPS += \
./src/renesas/application/CLI_commands.d \
./src/renesas/application/debug.d \
./src/renesas/application/r_bcd_ae.d \
./src/renesas/application/r_bcd_camera.d \
./src/renesas/application/r_bcd_lcd.d \
./src/renesas/application/r_bcd_main.d \
./src/renesas/application/r_drp.d 

OBJS += \
./src/renesas/application/CLI_commands.o \
./src/renesas/application/dat.o \
./src/renesas/application/debug.o \
./src/renesas/application/r_bcd_ae.o \
./src/renesas/application/r_bcd_camera.o \
./src/renesas/application/r_bcd_lcd.o \
./src/renesas/application/r_bcd_main.o \
./src/renesas/application/r_drp.o 

ASM_DEPS += \
./src/renesas/application/dat.d 


# Each subdirectory must supply rules for building sources it contributes
src/renesas/application/%.o: ../src/renesas/application/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -O0 -ffunction-sections -fdata-sections -Wnull-dereference -Wmaybe-uninitialized -g3 -Wstack-usage=4096 -DNO_ICONV -D__MBED__ -D_DEVICE_RZA2_ -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\compiler\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\configuration" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\os_abstraction\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\system\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_cache\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_cpg\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_gpio\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_intc\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_mmu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\drivers\r_stb\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_cbuffer\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_ceu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_drp\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_jcu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_mipi\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_ostm\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_riic\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_rvapi\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_scifa\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\sc_drivers\r_vdc\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\lib\drp" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\os\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\os\inc\private" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\os\FreeRTOS\Include" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\os\FreeRTOS\Portable\gcc\arm_ca9" -I"D:/Projects/e2_studio/RZ_A2M/workspace/Renesas_9.10_final/app/RZA2M_DRP_Benchmark/src/renesas/application/inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\Translator" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car\binarization" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car\syn6288" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car\Track" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car\ParallelSendData" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application\smart_car\NRF24L01" -fabi-version=0 -Wa,-adhlns="$@.lst" -fdiagnostics-parseable-fixits -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/renesas/application/%.o: ../src/renesas/application/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -O0 -ffunction-sections -fdata-sections -Wnull-dereference -Wmaybe-uninitialized -g3 -Wstack-usage=4096 -x assembler-with-cpp -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\lib\drp" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\app\RZA2M_DRP_Benchmark\src\renesas\application" -I"D:\Projects\e2_studio\RZ_A2M\workspace\Renesas_9.10_final\bsp\src\rza2\generate\os_abstraction\inc" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


