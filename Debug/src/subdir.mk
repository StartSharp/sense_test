################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/platform_core.cpp 

CPP_DEPS += \
./src/platform_core.d 

OBJS += \
./src/platform_core.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/tct/eclipse-workspace/platform_core/COMMON" -I"/home/tct/eclipse-workspace/platform_core/USER" -I"/home/tct/eclipse-workspace/platform_core/PLATFORM" -I"/home/tct/eclipse-workspace/platform_core/LIB" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/platform_core.d ./src/platform_core.o

.PHONY: clean-src

