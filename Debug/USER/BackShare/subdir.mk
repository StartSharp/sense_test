################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USER/BackShare/BackShare.cpp \
../USER/BackShare/comm_back.cpp 

CPP_DEPS += \
./USER/BackShare/BackShare.d \
./USER/BackShare/comm_back.d 

OBJS += \
./USER/BackShare/BackShare.o \
./USER/BackShare/comm_back.o 


# Each subdirectory must supply rules for building sources it contributes
USER/BackShare/%.o: ../USER/BackShare/%.cpp USER/BackShare/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/tct/git/repository/lee/sense_test/COMMON" -I"/home/tct/git/repository/lee/sense_test/USER" -I"/home/tct/git/repository/lee/sense_test/PLATFORM" -I"/home/tct/git/repository/lee/sense_test/LIB" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-USER-2f-BackShare

clean-USER-2f-BackShare:
	-$(RM) ./USER/BackShare/BackShare.d ./USER/BackShare/BackShare.o ./USER/BackShare/comm_back.d ./USER/BackShare/comm_back.o

.PHONY: clean-USER-2f-BackShare

