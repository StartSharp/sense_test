################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PLATFORM/Protocol/tcp/tcpserver.cpp 

CPP_DEPS += \
./PLATFORM/Protocol/tcp/tcpserver.d 

OBJS += \
./PLATFORM/Protocol/tcp/tcpserver.o 


# Each subdirectory must supply rules for building sources it contributes
PLATFORM/Protocol/tcp/%.o: ../PLATFORM/Protocol/tcp/%.cpp PLATFORM/Protocol/tcp/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/tct/git/repository/lee/sense_test/COMMON" -I"/home/tct/git/repository/lee/sense_test/USER" -I"/home/tct/git/repository/lee/sense_test/PLATFORM" -I"/home/tct/git/repository/lee/sense_test/LIB" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-PLATFORM-2f-Protocol-2f-tcp

clean-PLATFORM-2f-Protocol-2f-tcp:
	-$(RM) ./PLATFORM/Protocol/tcp/tcpserver.d ./PLATFORM/Protocol/tcp/tcpserver.o

.PHONY: clean-PLATFORM-2f-Protocol-2f-tcp

