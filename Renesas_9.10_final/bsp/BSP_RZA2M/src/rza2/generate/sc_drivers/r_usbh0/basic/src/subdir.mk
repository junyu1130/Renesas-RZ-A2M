################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hdriver.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hdriver_api.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hehci_main.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hehci_memory.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hehci_transfer.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hhci.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hhubsys.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hinthandler.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hmanager.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hohci_main.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hohci_memory.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hohci_transfer.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hscheduler.c \
../src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_rtos.c 

C_DEPS += \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hdriver.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hdriver_api.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hehci_main.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hehci_memory.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hehci_transfer.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hhci.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hhubsys.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hinthandler.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hmanager.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hohci_main.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hohci_memory.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hohci_transfer.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hscheduler.d \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_rtos.d 

OBJS += \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hdriver.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hdriver_api.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hehci_main.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hehci_memory.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hehci_transfer.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hhci.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hhubsys.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hinthandler.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hmanager.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hohci_main.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hohci_memory.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hohci_transfer.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_hscheduler.o \
./src/rza2/generate/sc_drivers/r_usbh0/basic/src/r_usbh0_rtos.o 


# Each subdirectory must supply rules for building sources it contributes
src/rza2/generate/sc_drivers/r_usbh0/basic/src/%.o: ../src/rza2/generate/sc_drivers/r_usbh0/basic/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Os -Wuninitialized -Wnull-dereference -g -D_DEFAULT_SOURCE -D_DEVICE_RZA2_ -DR_USE_ANSI_STDIO_MODE_CFG=1 -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\system\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\system\inc\iobitmasks" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_cbuffer\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_ceu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_drp\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_jcu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_mipi\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_ostm\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_riic\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_rvapi\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_scifa\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_vdc\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\msc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\msc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\basic" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\basic" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\basic\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\basic\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\basic\userdef" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\basic\userdef" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\msc\userdef" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\msc\userdef" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\msc\src\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\msc\src\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh1\basic\src\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_usbh0\basic\src\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_sdhi_simplified\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_sdhi_simplified\src\sd\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_sdhi_simplified\src\sd\inc\access" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\sc_drivers\r_sdhi_simplified\src\sd\access" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_cache\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_cpg\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_gpio\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_intc\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_mmu\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\drivers\r_stb\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\configuration" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\compiler\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\src\rza2\generate\os_abstraction\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\inc" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\inc\private" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\FreeRTOS\Include" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\FreeRTOS\Portable\gcc\arm_ca9" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\Reliance-Edge\include" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\Reliance-Edge\core\include" -I"D:\Projects\e2_studio\RZ_A2M\workspace\RZA2M_DRP_Benchmark_V1.0.0\bsp\os\Reliance-Edge\os\freertos\include" -I"C:\Program Files (x86)\GNU Tools ARM Embedded\7 2017-q4-major\/bin/newlib/libc/sys/arm" -fabi-version=0 -Wimplicit -Wswitch -Wsign-compare -Wstrict-prototypes -Wredundant-decls -Wreturn-type -fno-gcse -funit-at-a-time -funroll-loops -fdiagnostics-parseable-fixits -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


