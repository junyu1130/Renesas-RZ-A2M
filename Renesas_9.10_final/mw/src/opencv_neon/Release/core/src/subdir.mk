################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../core/src/algorithm.cpp \
../core/src/alloc.cpp \
../core/src/arithm.cpp \
../core/src/array.cpp \
../core/src/command_line_parser.cpp \
../core/src/conjugate_gradient.cpp \
../core/src/convert.cpp \
../core/src/copy.cpp \
../core/src/cuda_gpu_mat.cpp \
../core/src/cuda_host_mem.cpp \
../core/src/cuda_info.cpp \
../core/src/cuda_stream.cpp \
../core/src/datastructs.cpp \
../core/src/directx.cpp \
../core/src/downhill_simplex.cpp \
../core/src/dxt.cpp \
../core/src/gl_core_3_1.cpp \
../core/src/glob.cpp \
../core/src/hal_internal.cpp \
../core/src/kmeans.cpp \
../core/src/lapack.cpp \
../core/src/lda.cpp \
../core/src/lpsolver.cpp \
../core/src/mathfuncs.cpp \
../core/src/mathfuncs_core.cpp \
../core/src/matmul.cpp \
../core/src/matop.cpp \
../core/src/matrix.cpp \
../core/src/matrix_decomp.cpp \
../core/src/merge.cpp \
../core/src/ocl.cpp \
../core/src/opengl.cpp \
../core/src/out.cpp \
../core/src/ovx.cpp \
../core/src/parallel.cpp \
../core/src/parallel_pthreads.cpp \
../core/src/pca.cpp \
../core/src/persistence.cpp \
../core/src/rand.cpp \
../core/src/split.cpp \
../core/src/stat.cpp \
../core/src/stl.cpp \
../core/src/system.cpp \
../core/src/tables.cpp \
../core/src/types.cpp \
../core/src/umatrix.cpp \
../core/src/va_intel.cpp 

OBJS += \
./core/src/algorithm.o \
./core/src/alloc.o \
./core/src/arithm.o \
./core/src/array.o \
./core/src/command_line_parser.o \
./core/src/conjugate_gradient.o \
./core/src/convert.o \
./core/src/copy.o \
./core/src/cuda_gpu_mat.o \
./core/src/cuda_host_mem.o \
./core/src/cuda_info.o \
./core/src/cuda_stream.o \
./core/src/datastructs.o \
./core/src/directx.o \
./core/src/downhill_simplex.o \
./core/src/dxt.o \
./core/src/gl_core_3_1.o \
./core/src/glob.o \
./core/src/hal_internal.o \
./core/src/kmeans.o \
./core/src/lapack.o \
./core/src/lda.o \
./core/src/lpsolver.o \
./core/src/mathfuncs.o \
./core/src/mathfuncs_core.o \
./core/src/matmul.o \
./core/src/matop.o \
./core/src/matrix.o \
./core/src/matrix_decomp.o \
./core/src/merge.o \
./core/src/ocl.o \
./core/src/opengl.o \
./core/src/out.o \
./core/src/ovx.o \
./core/src/parallel.o \
./core/src/parallel_pthreads.o \
./core/src/pca.o \
./core/src/persistence.o \
./core/src/rand.o \
./core/src/split.o \
./core/src/stat.o \
./core/src/stl.o \
./core/src/system.o \
./core/src/tables.o \
./core/src/types.o \
./core/src/umatrix.o \
./core/src/va_intel.o 

CPP_DEPS += \
./core/src/algorithm.d \
./core/src/alloc.d \
./core/src/arithm.d \
./core/src/array.d \
./core/src/command_line_parser.d \
./core/src/conjugate_gradient.d \
./core/src/convert.d \
./core/src/copy.d \
./core/src/cuda_gpu_mat.d \
./core/src/cuda_host_mem.d \
./core/src/cuda_info.d \
./core/src/cuda_stream.d \
./core/src/datastructs.d \
./core/src/directx.d \
./core/src/downhill_simplex.d \
./core/src/dxt.d \
./core/src/gl_core_3_1.d \
./core/src/glob.d \
./core/src/hal_internal.d \
./core/src/kmeans.d \
./core/src/lapack.d \
./core/src/lda.d \
./core/src/lpsolver.d \
./core/src/mathfuncs.d \
./core/src/mathfuncs_core.d \
./core/src/matmul.d \
./core/src/matop.d \
./core/src/matrix.d \
./core/src/matrix_decomp.d \
./core/src/merge.d \
./core/src/ocl.d \
./core/src/opengl.d \
./core/src/out.d \
./core/src/ovx.d \
./core/src/parallel.d \
./core/src/parallel_pthreads.d \
./core/src/pca.d \
./core/src/persistence.d \
./core/src/rand.d \
./core/src/split.d \
./core/src/stat.d \
./core/src/stl.d \
./core/src/system.d \
./core/src/tables.d \
./core/src/types.d \
./core/src/umatrix.d \
./core/src/va_intel.d 


# Each subdirectory must supply rules for building sources it contributes
core/src/%.o: ../core/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ftree-vectorize -mvectorize-with-neon-quad -Wnull-dereference -DDEVICE_ERROR_PATTERN=1 -DARM_MATH_CA9 -D__FPU_PRESENT -D__MBED__=1 -DDEVICE_I2CSLAVE=1 -DTARGET_LIKE_MBED -DDEVICE_PORTINOUT=1 -DDEVICE_PORTIN=1 -DTOOLCHAIN_object -DDEVICE_SERIAL_ASYNCH=1 -D__CMSIS_RTOS -D__EVAL -DTOOLCHAIN_GCC -DTARGET_CORTEX_A -DDEVICE_CAN=1 -DMBED_BUILD_TIMESTAMP=1487815611.57 -DDEVICE_I2C_ASYNCH=1 -DTARGET_UVISOR_UNSUPPORTED -D__MBED_CMSIS_RTOS_CA9 -DTARGET_LIKE_CORTEX_A9 -DDEVICE_SERIAL=1 -DTARGET_MBRZA1LU -DDEVICE_INTERRUPTIN=1 -DDEVICE_I2C=1 -DDEVICE_PORTOUT=1 -DDEVICE_STDIO_MESSAGES=1 -DDEVICE_SPI_ASYNCH=1 -DTARGET_RENESAS -DTARGET_RELEASE -DDEVICE_SERIAL_FC=1 -DTARGET_GR_LYCHEE -DTARGET_A9 -D__CORTEX_A9 -DTARGET_RZ_A1XX -DTARGET_FF_ARDUINO -DTOOLCHAIN_GCC_ARM -DDEVICE_SPI=1 -DDEVICE_SPISLAVE=1 -DDEVICE_ANALOGIN=1 -DDEVICE_PWMOUT=1 -D__OPENCV_BUILD -D__ARM_NEON__ -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\calib3d\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\calib3d\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\core\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\core\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\core\include\opencv2\core" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\features2d\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\features2d\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\flann\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\flann\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgcodecs\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgcodecs\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgcodecs\include\opencv2\imgcodecs" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgproc\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgproc\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\imgproc\include\opencv2\imgproc" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\ml\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\ml\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\ml\include\opencv2\ml" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\objdetect\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\objdetect\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\objdetect\include\opencv2\objdetect" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\opencv_contrib\modules\face\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\photo\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\photo\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\shape\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\shape\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\stitching\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\stitching\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\superres\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\superres\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\video\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\video\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\videostab\include" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\videostab\include\opencv2" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\mbed-headers" -I"D:\Project\e2_studio\workspace2\RZA2M_DRP_Benchmark_V1.0.0\RZA2M_DRP_Benchmark_V1.0.0\mw\src\opencv_neon\3rdparty\libjpeg" -I"D:/Project/e2_studio/workspace2/RZA2M_DRP_Benchmark_V1.0.0/RZA2M_DRP_Benchmark_V1.0.0/mw/src/opencv_neon/generate" -I"D:/Project/e2_studio/workspace2/RZA2M_DRP_Benchmark_V1.0.0/RZA2M_DRP_Benchmark_V1.0.0/mw/src/opencv_neon/src" -std=gnu++11 -fabi-version=0 -fpermissive -fdiagnostics-parseable-fixits -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


