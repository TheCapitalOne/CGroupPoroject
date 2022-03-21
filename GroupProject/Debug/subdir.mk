################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GroupProject.c \
../SendPage.c \
../Server.c \
../Socket.c 

OBJS += \
./GroupProject.o \
./SendPage.o \
./Server.o \
./Socket.o 

C_DEPS += \
./GroupProject.d \
./SendPage.d \
./Server.d \
./Socket.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -Werror -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


