################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USER/SCCtrlerManage/SCCtrlerManage.cpp \
../USER/SCCtrlerManage/comm.cpp 

CPP_DEPS += \
./USER/SCCtrlerManage/SCCtrlerManage.d \
./USER/SCCtrlerManage/comm.d 

OBJS += \
./USER/SCCtrlerManage/SCCtrlerManage.o \
./USER/SCCtrlerManage/comm.o 


# Each subdirectory must supply rules for building sources it contributes
USER/SCCtrlerManage/%.o: ../USER/SCCtrlerManage/%.cpp USER/SCCtrlerManage/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/tct/git/repository/platform_core/COMMON" -I"/home/tct/git/repository/platform_core/USER" -I"/home/tct/git/repository/platform_core/PLATFORM" -I"/home/tct/git/repository/platform_core/LIB" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-USER-2f-SCCtrlerManage

clean-USER-2f-SCCtrlerManage:
	-$(RM) ./USER/SCCtrlerManage/SCCtrlerManage.d ./USER/SCCtrlerManage/SCCtrlerManage.o ./USER/SCCtrlerManage/comm.d ./USER/SCCtrlerManage/comm.o

.PHONY: clean-USER-2f-SCCtrlerManage

