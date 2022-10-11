################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USER/UsercaseManage/UsercaseManage.cpp 

CPP_DEPS += \
./USER/UsercaseManage/UsercaseManage.d 

OBJS += \
./USER/UsercaseManage/UsercaseManage.o 


# Each subdirectory must supply rules for building sources it contributes
USER/UsercaseManage/%.o: ../USER/UsercaseManage/%.cpp USER/UsercaseManage/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/tct/eclipse-workspace/platform_core/COMMON" -I"/home/tct/eclipse-workspace/platform_core/USER" -I"/home/tct/eclipse-workspace/platform_core/PLATFORM" -I"/home/tct/eclipse-workspace/platform_core/LIB" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-USER-2f-UsercaseManage

clean-USER-2f-UsercaseManage:
	-$(RM) ./USER/UsercaseManage/UsercaseManage.d ./USER/UsercaseManage/UsercaseManage.o

.PHONY: clean-USER-2f-UsercaseManage

