################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/zf_driver/zf_driver_adc.c \
../libraries/zf_driver/zf_driver_delay.c \
../libraries/zf_driver/zf_driver_dvp.c \
../libraries/zf_driver/zf_driver_encoder.c \
../libraries/zf_driver/zf_driver_exti.c \
../libraries/zf_driver/zf_driver_flash.c \
../libraries/zf_driver/zf_driver_gpio.c \
../libraries/zf_driver/zf_driver_iic.c \
../libraries/zf_driver/zf_driver_pit.c \
../libraries/zf_driver/zf_driver_pwm.c \
../libraries/zf_driver/zf_driver_soft_iic.c \
../libraries/zf_driver/zf_driver_soft_spi.c \
../libraries/zf_driver/zf_driver_spi.c \
../libraries/zf_driver/zf_driver_timer.c \
../libraries/zf_driver/zf_driver_uart.c \
../libraries/zf_driver/zf_driver_usb_cdc.c 

OBJS += \
./libraries/zf_driver/zf_driver_adc.o \
./libraries/zf_driver/zf_driver_delay.o \
./libraries/zf_driver/zf_driver_dvp.o \
./libraries/zf_driver/zf_driver_encoder.o \
./libraries/zf_driver/zf_driver_exti.o \
./libraries/zf_driver/zf_driver_flash.o \
./libraries/zf_driver/zf_driver_gpio.o \
./libraries/zf_driver/zf_driver_iic.o \
./libraries/zf_driver/zf_driver_pit.o \
./libraries/zf_driver/zf_driver_pwm.o \
./libraries/zf_driver/zf_driver_soft_iic.o \
./libraries/zf_driver/zf_driver_soft_spi.o \
./libraries/zf_driver/zf_driver_spi.o \
./libraries/zf_driver/zf_driver_timer.o \
./libraries/zf_driver/zf_driver_uart.o \
./libraries/zf_driver/zf_driver_usb_cdc.o 

C_DEPS += \
./libraries/zf_driver/zf_driver_adc.d \
./libraries/zf_driver/zf_driver_delay.d \
./libraries/zf_driver/zf_driver_dvp.d \
./libraries/zf_driver/zf_driver_encoder.d \
./libraries/zf_driver/zf_driver_exti.d \
./libraries/zf_driver/zf_driver_flash.d \
./libraries/zf_driver/zf_driver_gpio.d \
./libraries/zf_driver/zf_driver_iic.d \
./libraries/zf_driver/zf_driver_pit.d \
./libraries/zf_driver/zf_driver_pwm.d \
./libraries/zf_driver/zf_driver_soft_iic.d \
./libraries/zf_driver/zf_driver_soft_spi.d \
./libraries/zf_driver/zf_driver_spi.d \
./libraries/zf_driver/zf_driver_timer.d \
./libraries/zf_driver/zf_driver_uart.d \
./libraries/zf_driver/zf_driver_usb_cdc.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/zf_driver/%.o: ../libraries/zf_driver/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\doc" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_components" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Core" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\user\inc" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_common" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_device" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\CODE" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

