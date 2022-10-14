################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../protocol/tcp/tcpserver.cpp 

CPP_DEPS += \
./protocol/tcp/tcpserver.d 

OBJS += \
./protocol/tcp/tcpserver.o 


# Each subdirectory must supply rules for building sources it contributes
protocol/tcp/%.o: ../protocol/tcp/%.cpp protocol/tcp/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/tct/eclipse-workspace/platform_core/COMMON" -I"/home/tct/eclipse-workspace/platform_core/USER" -I"/home/tct/eclipse-workspace/platform_core/PLATFORM" -I"/home/tct/eclipse-workspace/platform_core/LIB" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-protocol-2f-tcp

clean-protocol-2f-tcp:
	-$(RM) ./protocol/tcp/tcpserver.d ./protocol/tcp/tcpserver.o

.PHONY: clean-protocol-2f-tcp

