################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../COMMON/CommonMemory.c \
../COMMON/EndianChange.c \
../COMMON/LinkList.c \
../COMMON/crc32.c 

C_DEPS += \
./COMMON/CommonMemory.d \
./COMMON/EndianChange.d \
./COMMON/LinkList.d \
./COMMON/crc32.d 

OBJS += \
./COMMON/CommonMemory.o \
./COMMON/EndianChange.o \
./COMMON/LinkList.o \
./COMMON/crc32.o 


# Each subdirectory must supply rules for building sources it contributes
COMMON/%.o: ../COMMON/%.c COMMON/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/tct/git/repository/platform_core/COMMON" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-COMMON

clean-COMMON:
	-$(RM) ./COMMON/CommonMemory.d ./COMMON/CommonMemory.o ./COMMON/EndianChange.d ./COMMON/EndianChange.o ./COMMON/LinkList.d ./COMMON/LinkList.o ./COMMON/crc32.d ./COMMON/crc32.o

.PHONY: clean-COMMON

