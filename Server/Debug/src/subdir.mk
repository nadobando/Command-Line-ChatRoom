################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Broker.cpp \
../src/ChatRoom.cpp \
../src/Dispatcher.cpp \
../src/Login.cpp \
../src/MThread.cpp \
../src/MultipleTCPSocketsListener.cpp \
../src/TCPMessengerServer.cpp \
../src/TCPSocket.cpp \
../src/UserDetails.cpp \
../src/main.cpp 

OBJS += \
./src/Broker.o \
./src/ChatRoom.o \
./src/Dispatcher.o \
./src/Login.o \
./src/MThread.o \
./src/MultipleTCPSocketsListener.o \
./src/TCPMessengerServer.o \
./src/TCPSocket.o \
./src/UserDetails.o \
./src/main.o 

CPP_DEPS += \
./src/Broker.d \
./src/ChatRoom.d \
./src/Dispatcher.d \
./src/Login.d \
./src/MThread.d \
./src/MultipleTCPSocketsListener.d \
./src/TCPMessengerServer.d \
./src/TCPSocket.d \
./src/UserDetails.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


