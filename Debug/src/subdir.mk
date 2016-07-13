################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/build_kdtree.cpp \
../src/kdtree.cpp \
../src/query_kdtree.cpp \
../src/test.cpp 

OBJS += \
./src/build_kdtree.o \
./src/kdtree.o \
./src/query_kdtree.o \
./src/test.o 

CPP_DEPS += \
./src/build_kdtree.d \
./src/kdtree.d \
./src/query_kdtree.d \
./src/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


