################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ACZ702_Lib/SII9022/SII9022.c 

OBJS += \
./ACZ702_Lib/SII9022/SII9022.o 

C_DEPS += \
./ACZ702_Lib/SII9022/SII9022.d 


# Each subdirectory must supply rules for building sources it contributes
ACZ702_Lib/SII9022/%.o: ../ACZ702_Lib/SII9022/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"E:\ACZ702V1_V2\RGB_TFT_HDMI\RGB_TFT_HDMI.sdk\RGB_TFT\ACZ702_Lib\LCD" -I"E:\ACZ702V1_V2\RGB_TFT_HDMI\RGB_TFT_HDMI.sdk\RGB_TFT\ACZ702_Lib\SCU" -I"E:\ACZ702V1_V2\RGB_TFT_HDMI\RGB_TFT_HDMI.sdk\RGB_TFT\src" -I"E:\ACZ702V1_V2\RGB_TFT_HDMI\RGB_TFT_HDMI.sdk\RGB_TFT_bsp\ps7_cortexa9_0\include" -I"E:\ACZ702V1_V2\RGB_TFT_HDMI\RGB_TFT_HDMI.sdk\RGB_TFT\ACZ702_Lib\PS_IIC" -I"E:\ACZ702V1_V2\RGB_TFT_HDMI\RGB_TFT_HDMI.sdk\RGB_TFT\ACZ702_Lib\SII9022" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


