################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../videostab/src/deblurring.cpp \
../videostab/src/fast_marching.cpp \
../videostab/src/frame_source.cpp \
../videostab/src/global_motion.cpp \
../videostab/src/inpainting.cpp \
../videostab/src/log.cpp \
../videostab/src/motion_stabilizing.cpp \
../videostab/src/optical_flow.cpp \
../videostab/src/outlier_rejection.cpp \
../videostab/src/stabilizer.cpp \
../videostab/src/wobble_suppression.cpp 

OBJS += \
./videostab/src/deblurring.o \
./videostab/src/fast_marching.o \
./videostab/src/frame_source.o \
./videostab/src/global_motion.o \
./videostab/src/inpainting.o \
./videostab/src/log.o \
./videostab/src/motion_stabilizing.o \
./videostab/src/optical_flow.o \
./videostab/src/outlier_rejection.o \
./videostab/src/stabilizer.o \
./videostab/src/wobble_suppression.o 

CPP_DEPS += \
./videostab/src/deblurring.d \
./videostab/src/fast_marching.d \
./videostab/src/frame_source.d \
./videostab/src/global_motion.d \
./videostab/src/inpainting.d \
./videostab/src/log.d \
./videostab/src/motion_stabilizing.d \
./videostab/src/optical_flow.d \
./videostab/src/outlier_rejection.d \
./videostab/src/stabilizer.d \
./videostab/src/wobble_suppression.d 


# Each subdirectory must supply rules for building sources it contributes
videostab/src/%.o: ../videostab/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ftree-vectorize -mvectorize-with-neon-quad -Wnull-dereference -DDEVICE_ERROR_PATTERN=1 -DARM_MATH_CA9 -D__FPU_PRESENT -D__MBED__=1 -DDEVICE_I2CSLAVE=1 -DTARGET_LIKE_MBED -DDEVICE_PORTINOUT=1 -DDEVICE_PORTIN=1 -DTOOLCHAIN_object -DDEVICE_SERIAL_ASYNCH=1 -D__CMSIS_RTOS -D__EVAL -DTOOLCHAIN_GCC -DTARGET_CORTEX_A -DDEVICE_CAN=1 -DMBED_BUILD_TIMESTAMP=1487815611.57 -DDEVICE_I2C_ASYNCH=1 -DTARGET_UVISOR_UNSUPPORTED -D__MBED_CMSIS_RTOS_CA9 -DTARGET_LIKE_CORTEX_A9 -DDEVICE_SERIAL=1 -DTARGET_MBRZA1LU -DDEVICE_INTERRUPTIN=1 -DDEVICE_I2C=1 -DDEVICE_PORTOUT=1 -DDEVICE_STDIO_MESSAGES=1 -DDEVICE_SPI_ASYNCH=1 -DTARGET_RENESAS -DTARGET_RELEASE -DDEVICE_SERIAL_FC=1 -DTARGET_GR_LYCHEE -DTARGET_A9 -D__CORTEX_A9 -DTARGET_RZ_A1XX -DTARGET_FF_ARDUINO -DTOOLCHAIN_GCC_ARM -DDEVICE_SPI=1 -DDEVICE_SPISLAVE=1 -DDEVICE_ANALOGIN=1 -DDEVICE_PWMOUT=1 -D__OPENCV_BUILD -D__ARM_NEON__ -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\calib3d\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\calib3d\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\core\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\core\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\core\include\opencv2\core" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\features2d\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\features2d\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\flann\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\flann\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgcodecs\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgcodecs\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgcodecs\include\opencv2\imgcodecs" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgproc\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgproc\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgproc\include\opencv2\imgproc" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\ml\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\ml\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\ml\include\opencv2\ml" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\objdetect\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\objdetect\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\objdetect\include\opencv2\objdetect" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\opencv_contrib\modules\face\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\photo\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\photo\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\shape\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\shape\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\stitching\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\stitching\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\superres\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\superres\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\video\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\video\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\videostab\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\videostab\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\mbed-headers" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\3rdparty\libjpeg" -I"D:/Project/e2_studio/workspace2/RZA2M_DRP_Benchmark_V1.0.0/RZA2M_DRP_Benchmark_V1.0.0/mw/src/opencv_neon/generate" -I"D:/Project/e2_studio/workspace2/RZA2M_DRP_Benchmark_V1.0.0/RZA2M_DRP_Benchmark_V1.0.0/mw/src/opencv_neon/src" -std=gnu++11 -fabi-version=0 -fpermissive -fdiagnostics-parseable-fixits -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


