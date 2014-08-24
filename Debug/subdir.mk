################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AppGame.cpp \
../globalVars.cpp \
../main.cpp 

OBJS += \
./AppGame.o \
./globalVars.o \
./main.o 

CPP_DEPS += \
./AppGame.d \
./globalVars.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/SDL -O0 -g3 -Wall -c -fmessage-length=0 -D_GNU_SOURCE=1 -D_REENTRANT -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


