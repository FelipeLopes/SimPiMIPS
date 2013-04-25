################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../hello.o 

CPP_SRCS += \
../app.cpp \
../cpu.cpp \
../display_grid.cpp \
../file_parser.cpp \
../main_window.cpp \
../memory.cpp \
../presenter.cpp \
../register.cpp \
../stage.cpp 

OBJS += \
./app.o \
./cpu.o \
./display_grid.o \
./file_parser.o \
./main_window.o \
./memory.o \
./presenter.o \
./register.o \
./stage.o 

CPP_DEPS += \
./app.d \
./cpu.d \
./display_grid.d \
./file_parser.d \
./main_window.d \
./memory.d \
./presenter.d \
./register.d \
./stage.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 `wx-config --cppflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


