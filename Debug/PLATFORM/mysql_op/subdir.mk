################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PLATFORM/mysql_op/mysql_op.cpp 

CPP_DEPS += \
./PLATFORM/mysql_op/mysql_op.d 

OBJS += \
./PLATFORM/mysql_op/mysql_op.o 


# Each subdirectory must supply rules for building sources it contributes
PLATFORM/mysql_op/%.o: ../PLATFORM/mysql_op/%.cpp PLATFORM/mysql_op/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/tct/eclipse-workspace/platform_core/COMMON" -I"/home/tct/eclipse-workspace/platform_core/USER" -I"/home/tct/eclipse-workspace/platform_core/PLATFORM" -I"/home/tct/eclipse-workspace/platform_core/LIB" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-PLATFORM-2f-mysql_op

clean-PLATFORM-2f-mysql_op:
	-$(RM) ./PLATFORM/mysql_op/mysql_op.d ./PLATFORM/mysql_op/mysql_op.o

.PHONY: clean-PLATFORM-2f-mysql_op

