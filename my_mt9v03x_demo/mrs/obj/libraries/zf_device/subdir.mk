################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/zf_device/zf_device_absolute_encoder.c \
../libraries/zf_device/zf_device_aht20.c \
../libraries/zf_device/zf_device_bluetooth_ch9141.c \
../libraries/zf_device/zf_device_camera.c \
../libraries/zf_device/zf_device_dl1a.c \
../libraries/zf_device/zf_device_dl1b.c \
../libraries/zf_device/zf_device_dm1xa.c \
../libraries/zf_device/zf_device_gnss.c \
../libraries/zf_device/zf_device_icm20602.c \
../libraries/zf_device/zf_device_imu660ra.c \
../libraries/zf_device/zf_device_imu963ra.c \
../libraries/zf_device/zf_device_ips114.c \
../libraries/zf_device/zf_device_ips200.c \
../libraries/zf_device/zf_device_k24c02.c \
../libraries/zf_device/zf_device_key.c \
../libraries/zf_device/zf_device_mpu6050.c \
../libraries/zf_device/zf_device_mt9v03x_dvp.c \
../libraries/zf_device/zf_device_oled.c \
../libraries/zf_device/zf_device_scc8660_dvp.c \
../libraries/zf_device/zf_device_tft180.c \
../libraries/zf_device/zf_device_type.c \
../libraries/zf_device/zf_device_virtual_oscilloscope.c \
../libraries/zf_device/zf_device_w25q32.c \
../libraries/zf_device/zf_device_wifi_spi.c \
../libraries/zf_device/zf_device_wifi_uart.c \
../libraries/zf_device/zf_device_wireless_ch573.c \
../libraries/zf_device/zf_device_wireless_uart.c 

OBJS += \
./libraries/zf_device/zf_device_absolute_encoder.o \
./libraries/zf_device/zf_device_aht20.o \
./libraries/zf_device/zf_device_bluetooth_ch9141.o \
./libraries/zf_device/zf_device_camera.o \
./libraries/zf_device/zf_device_dl1a.o \
./libraries/zf_device/zf_device_dl1b.o \
./libraries/zf_device/zf_device_dm1xa.o \
./libraries/zf_device/zf_device_gnss.o \
./libraries/zf_device/zf_device_icm20602.o \
./libraries/zf_device/zf_device_imu660ra.o \
./libraries/zf_device/zf_device_imu963ra.o \
./libraries/zf_device/zf_device_ips114.o \
./libraries/zf_device/zf_device_ips200.o \
./libraries/zf_device/zf_device_k24c02.o \
./libraries/zf_device/zf_device_key.o \
./libraries/zf_device/zf_device_mpu6050.o \
./libraries/zf_device/zf_device_mt9v03x_dvp.o \
./libraries/zf_device/zf_device_oled.o \
./libraries/zf_device/zf_device_scc8660_dvp.o \
./libraries/zf_device/zf_device_tft180.o \
./libraries/zf_device/zf_device_type.o \
./libraries/zf_device/zf_device_virtual_oscilloscope.o \
./libraries/zf_device/zf_device_w25q32.o \
./libraries/zf_device/zf_device_wifi_spi.o \
./libraries/zf_device/zf_device_wifi_uart.o \
./libraries/zf_device/zf_device_wireless_ch573.o \
./libraries/zf_device/zf_device_wireless_uart.o 

C_DEPS += \
./libraries/zf_device/zf_device_absolute_encoder.d \
./libraries/zf_device/zf_device_aht20.d \
./libraries/zf_device/zf_device_bluetooth_ch9141.d \
./libraries/zf_device/zf_device_camera.d \
./libraries/zf_device/zf_device_dl1a.d \
./libraries/zf_device/zf_device_dl1b.d \
./libraries/zf_device/zf_device_dm1xa.d \
./libraries/zf_device/zf_device_gnss.d \
./libraries/zf_device/zf_device_icm20602.d \
./libraries/zf_device/zf_device_imu660ra.d \
./libraries/zf_device/zf_device_imu963ra.d \
./libraries/zf_device/zf_device_ips114.d \
./libraries/zf_device/zf_device_ips200.d \
./libraries/zf_device/zf_device_k24c02.d \
./libraries/zf_device/zf_device_key.d \
./libraries/zf_device/zf_device_mpu6050.d \
./libraries/zf_device/zf_device_mt9v03x_dvp.d \
./libraries/zf_device/zf_device_oled.d \
./libraries/zf_device/zf_device_scc8660_dvp.d \
./libraries/zf_device/zf_device_tft180.d \
./libraries/zf_device/zf_device_type.d \
./libraries/zf_device/zf_device_virtual_oscilloscope.d \
./libraries/zf_device/zf_device_w25q32.d \
./libraries/zf_device/zf_device_wifi_spi.d \
./libraries/zf_device/zf_device_wifi_uart.d \
./libraries/zf_device/zf_device_wireless_ch573.d \
./libraries/zf_device/zf_device_wireless_uart.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/zf_device/%.o: ../libraries/zf_device/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\doc" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_components" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Core" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\user\inc" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_common" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_device" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\CODE" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

