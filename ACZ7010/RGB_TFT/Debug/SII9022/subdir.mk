################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SII9022/SII9022.c 

OBJS += \
./SII9022/SII9022.o 

C_DEPS += \
./SII9022/SII9022.d 


# Each subdirectory must supply rules for building sources it contributes
SII9022/%.o: ../SII9022/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"E:\ZYNQ_NEW\project\RGB_TFT\RGB_TFT.sdk\RGB_TFT\ACZ702_Lib\LCD" -I"E:\ZYNQ_NEW\project\RGB_TFT\RGB_TFT.sdk\RGB_TFT\ACZ702_Lib\SCU" -I"E:\ZYNQ_NEW\project\RGB_TFT\RGB_TFT.sdk\RGB_TFT\src" -I"E:\ZYNQ_NEW\project\RGB_TFT\RGB_TFT.sdk\RGB_TFT_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


