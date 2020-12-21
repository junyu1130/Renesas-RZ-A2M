################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/rza2/generate/sc_drivers/r_vdc/src/r_spea.c \
../src/rza2/generate/sc_drivers/r_vdc/src/r_spea_check_parameter.c \
../src/rza2/generate/sc_drivers/r_vdc/src/r_spea_register.c \
../src/rza2/generate/sc_drivers/r_vdc/src/r_spea_register_address.c \
../src/rza2/generate/sc_drivers/r_vdc/src/r_vdc.c \
../src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_check_parameter.c \
../src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_interrupt.c \
../src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_register.c \
../src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_register_address.c \
../src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_shared_param.c 

C_DEPS += \
./src/rza2/generate/sc_drivers/r_vdc/src/r_spea.d \
./src/rza2/generate/sc_drivers/r_vdc/src/r_spea_check_parameter.d \
./src/rza2/generate/sc_drivers/r_vdc/src/r_spea_register.d \
./src/rza2/generate/sc_drivers/r_vdc/src/r_spea_register_address.d \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc.d \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_check_parameter.d \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_interrupt.d \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_register.d \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_register_address.d \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_shared_param.d 

OBJS += \
./src/rza2/generate/sc_drivers/r_vdc/src/r_spea.o \
./src/rza2/generate/sc_drivers/r_vdc/src/r_spea_check_parameter.o \
./src/rza2/generate/sc_drivers/r_vdc/src/r_spea_register.o \
./src/rza2/generate/sc_drivers/r_vdc/src/r_spea_register_address.o \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc.o \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_check_parameter.o \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_interrupt.o \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_register.o \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_register_address.o \
./src/rza2/generate/sc_drivers/r_vdc/src/r_vdc_shared_param.o 


# Each subdirectory must supply rules for building sources it contributes
src/rza2/generate/sc_drivers/r_vdc/src/%.o: ../src/rza2/generate/sc_drivers/r_vdc/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Os -Wuninitialized -Wnull-dereference -g -D_DEFAULT_SOURCE -D_DEVICE_RZA2_ -DR_USE_ANSI_STDIO_MODE_CFG=1 -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\system\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\system\inc\iobitmasks" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_cbuffer\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_ceu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_drp\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_jcu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_mipi\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_ostm\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_riic\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_rvapi\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_scifa\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_vdc\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\msc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\msc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\basic" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\basic" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\basic\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\basic\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\basic\userdef" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\basic\userdef" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\msc\userdef" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\msc\userdef" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\msc\src\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\msc\src\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\basic\src\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\basic\src\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_sdhi_simplified\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_sdhi_simplified\src\sd\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_sdhi_simplified\src\sd\inc\access" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_sdhi_simplified\src\sd\access" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_cache\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_cpg\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_gpio\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_intc\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_mmu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_stb\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\configuration" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\compiler\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\os_abstraction\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\inc\private" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\FreeRTOS\Include" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\FreeRTOS\Portable\gcc\arm_ca9" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\Reliance-Edge\include" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\Reliance-Edge\core\include" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\Reliance-Edge\os\freertos\include" -I"C:\Program Files (x86)\GNU Tools ARM Embedded\7 2017-q4-major\/bin/newlib/libc/sys/arm" -fabi-version=0 -Wimplicit -Wswitch -Wsign-compare -Wstrict-prototypes -Wredundant-decls -Wreturn-type -fno-gcse -funit-at-a-time -funroll-loops -fdiagnostics-parseable-fixits -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


