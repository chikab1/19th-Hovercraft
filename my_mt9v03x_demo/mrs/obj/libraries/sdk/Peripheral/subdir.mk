################################################################################
# MRS Version: 1.9.1
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/sdk/Peripheral/ch32v30x_adc.c \
../libraries/sdk/Peripheral/ch32v30x_bkp.c \
../libraries/sdk/Peripheral/ch32v30x_can.c \
../libraries/sdk/Peripheral/ch32v30x_crc.c \
../libraries/sdk/Peripheral/ch32v30x_dac.c \
../libraries/sdk/Peripheral/ch32v30x_dbgmcu.c \
../libraries/sdk/Peripheral/ch32v30x_dma.c \
../libraries/sdk/Peripheral/ch32v30x_dvp.c \
../libraries/sdk/Peripheral/ch32v30x_eth.c \
../libraries/sdk/Peripheral/ch32v30x_exti.c \
../libraries/sdk/Peripheral/ch32v30x_flash.c \
../libraries/sdk/Peripheral/ch32v30x_fsmc.c \
../libraries/sdk/Peripheral/ch32v30x_gpio.c \
../libraries/sdk/Peripheral/ch32v30x_i2c.c \
../libraries/sdk/Peripheral/ch32v30x_iwdg.c \
../libraries/sdk/Peripheral/ch32v30x_opa.c \
../libraries/sdk/Peripheral/ch32v30x_pwr.c \
../libraries/sdk/Peripheral/ch32v30x_rcc.c \
../libraries/sdk/Peripheral/ch32v30x_rng.c \
../libraries/sdk/Peripheral/ch32v30x_rtc.c \
../libraries/sdk/Peripheral/ch32v30x_sdio.c \
../libraries/sdk/Peripheral/ch32v30x_spi.c \
../libraries/sdk/Peripheral/ch32v30x_tim.c \
../libraries/sdk/Peripheral/ch32v30x_usart.c \
../libraries/sdk/Peripheral/ch32v30x_usbotg_device.c \
../libraries/sdk/Peripheral/ch32v30x_wwdg.c 

OBJS += \
./libraries/sdk/Peripheral/ch32v30x_adc.o \
./libraries/sdk/Peripheral/ch32v30x_bkp.o \
./libraries/sdk/Peripheral/ch32v30x_can.o \
./libraries/sdk/Peripheral/ch32v30x_crc.o \
./libraries/sdk/Peripheral/ch32v30x_dac.o \
./libraries/sdk/Peripheral/ch32v30x_dbgmcu.o \
./libraries/sdk/Peripheral/ch32v30x_dma.o \
./libraries/sdk/Peripheral/ch32v30x_dvp.o \
./libraries/sdk/Peripheral/ch32v30x_eth.o \
./libraries/sdk/Peripheral/ch32v30x_exti.o \
./libraries/sdk/Peripheral/ch32v30x_flash.o \
./libraries/sdk/Peripheral/ch32v30x_fsmc.o \
./libraries/sdk/Peripheral/ch32v30x_gpio.o \
./libraries/sdk/Peripheral/ch32v30x_i2c.o \
./libraries/sdk/Peripheral/ch32v30x_iwdg.o \
./libraries/sdk/Peripheral/ch32v30x_opa.o \
./libraries/sdk/Peripheral/ch32v30x_pwr.o \
./libraries/sdk/Peripheral/ch32v30x_rcc.o \
./libraries/sdk/Peripheral/ch32v30x_rng.o \
./libraries/sdk/Peripheral/ch32v30x_rtc.o \
./libraries/sdk/Peripheral/ch32v30x_sdio.o \
./libraries/sdk/Peripheral/ch32v30x_spi.o \
./libraries/sdk/Peripheral/ch32v30x_tim.o \
./libraries/sdk/Peripheral/ch32v30x_usart.o \
./libraries/sdk/Peripheral/ch32v30x_usbotg_device.o \
./libraries/sdk/Peripheral/ch32v30x_wwdg.o 

C_DEPS += \
./libraries/sdk/Peripheral/ch32v30x_adc.d \
./libraries/sdk/Peripheral/ch32v30x_bkp.d \
./libraries/sdk/Peripheral/ch32v30x_can.d \
./libraries/sdk/Peripheral/ch32v30x_crc.d \
./libraries/sdk/Peripheral/ch32v30x_dac.d \
./libraries/sdk/Peripheral/ch32v30x_dbgmcu.d \
./libraries/sdk/Peripheral/ch32v30x_dma.d \
./libraries/sdk/Peripheral/ch32v30x_dvp.d \
./libraries/sdk/Peripheral/ch32v30x_eth.d \
./libraries/sdk/Peripheral/ch32v30x_exti.d \
./libraries/sdk/Peripheral/ch32v30x_flash.d \
./libraries/sdk/Peripheral/ch32v30x_fsmc.d \
./libraries/sdk/Peripheral/ch32v30x_gpio.d \
./libraries/sdk/Peripheral/ch32v30x_i2c.d \
./libraries/sdk/Peripheral/ch32v30x_iwdg.d \
./libraries/sdk/Peripheral/ch32v30x_opa.d \
./libraries/sdk/Peripheral/ch32v30x_pwr.d \
./libraries/sdk/Peripheral/ch32v30x_rcc.d \
./libraries/sdk/Peripheral/ch32v30x_rng.d \
./libraries/sdk/Peripheral/ch32v30x_rtc.d \
./libraries/sdk/Peripheral/ch32v30x_sdio.d \
./libraries/sdk/Peripheral/ch32v30x_spi.d \
./libraries/sdk/Peripheral/ch32v30x_tim.d \
./libraries/sdk/Peripheral/ch32v30x_usart.d \
./libraries/sdk/Peripheral/ch32v30x_usbotg_device.d \
./libraries/sdk/Peripheral/ch32v30x_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/sdk/Peripheral/%.o: ../libraries/sdk/Peripheral/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\doc" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_components" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Core" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\user\inc" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_common" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_device" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\CODE" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

