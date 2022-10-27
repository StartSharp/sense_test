################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USER/Validation/subdir.mk
-include USER/UsercaseManage/subdir.mk
-include USER/SCCtrlerManage/subdir.mk
-include USER/PlatformManage/subdir.mk
-include USER/BackShare/subdir.mk
-include USER/subdir.mk
-include PLATFORM/mysql_op/subdir.mk
-include PLATFORM/Protocol/tcp/subdir.mk
-include COMMON/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := platform_core
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: platform_core

# Tool invocations
platform_core: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L"/home/tct/git/repository/lee/sense_test/LIB" -o "platform_core" $(OBJS) $(USER_OBJS) $(LIBS) -lmysqlclient
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) platform_core
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
