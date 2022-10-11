################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USER/Validation/Validation.cpp 

CPP_DEPS += \
./USER/Validation/Validation.d 

OBJS += \
./USER/Validation/Validation.o 


# Each subdirectory must supply rules for building sources it contributes
USER/Validation/%.o: ../USER/Validation/%.cpp USER/Validation/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/tct/git/repository/platform_core/COMMON" -I"/home/tct/git/repository/platform_core/USER" -I"/home/tct/git/repository/platform_core/PLATFORM" -I"/home/tct/git/repository/platform_core/LIB" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-USER-2f-Validation

clean-USER-2f-Validation:
	-$(RM) ./USER/Validation/Validation.d ./USER/Validation/Validation.o

.PHONY: clean-USER-2f-Validation

