################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USER/main.cpp 

CPP_DEPS += \
./USER/main.d 

OBJS += \
./USER/main.o 


# Each subdirectory must supply rules for building sources it contributes
USER/%.o: ../USER/%.cpp USER/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/tct/git/repository/lee/sense_test/COMMON" -I"/home/tct/git/repository/lee/sense_test/USER" -I"/home/tct/git/repository/lee/sense_test/PLATFORM" -I"/home/tct/git/repository/lee/sense_test/LIB" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-USER

clean-USER:
	-$(RM) ./USER/main.d ./USER/main.o

.PHONY: clean-USER

