################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PS_IIC/PS_IIC.c 

OBJS += \
./PS_IIC/PS_IIC.o 

C_DEPS += \
./PS_IIC/PS_IIC.d 


# Each subdirectory must supply rules for building sources it contributes
PS_IIC/%.o: ../PS_IIC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"E:\ZYNQ_NEW\project\RGB_TFT\RGB_TFT.sdk\RGB_TFT\ACZ702_Lib\LCD" -I"E:\ZYNQ_NEW\project\RGB_TFT\RGB_TFT.sdk\RGB_TFT\ACZ702_Lib\SCU" -I"E:\ZYNQ_NEW\project\RGB_TFT\RGB_TFT.sdk\RGB_TFT\src" -I"E:\ZYNQ_NEW\project\RGB_TFT\RGB_TFT.sdk\RGB_TFT_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


