################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../cpu.cpp \
../main.cpp \
../memory.cpp \
../register.cpp \
../stage.cpp 

OBJS += \
./cpu.o \
./main.o \
./memory.o \
./register.o \
./stage.o 

CPP_DEPS += \
./cpu.d \
./main.d \
./memory.d \
./register.d \
./stage.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


