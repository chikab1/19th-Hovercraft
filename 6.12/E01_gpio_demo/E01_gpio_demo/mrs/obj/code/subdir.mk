################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/admin/Desktop/E01_gpio_demo/E01_gpio_demo/code/circle.c \
C:/Users/admin/Desktop/E01_gpio_demo/E01_gpio_demo/code/cross.c \
C:/Users/admin/Desktop/E01_gpio_demo/E01_gpio_demo/code/image_process.c \
C:/Users/admin/Desktop/E01_gpio_demo/E01_gpio_demo/code/pid.c 

OBJS += \
./code/circle.o \
./code/cross.o \
./code/image_process.o \
./code/pid.o 

C_DEPS += \
./code/circle.d \
./code/cross.d \
./code/image_process.d \
./code/pid.d 


# Each subdirectory must supply rules for building sources it contributes
code/circle.o: C:/Users/admin/Desktop/E01_gpio_demo/E01_gpio_demo/code/circle.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\E01_gpio_demo\Libraries\doc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_components" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Core" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\user\inc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_common" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_device" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\code" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/cross.o: C:/Users/admin/Desktop/E01_gpio_demo/E01_gpio_demo/code/cross.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\E01_gpio_demo\Libraries\doc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_components" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Core" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\user\inc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_common" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_device" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\code" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/image_process.o: C:/Users/admin/Desktop/E01_gpio_demo/E01_gpio_demo/code/image_process.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\E01_gpio_demo\Libraries\doc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_components" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Core" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\user\inc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_common" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_device" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\code" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/pid.o: C:/Users/admin/Desktop/E01_gpio_demo/E01_gpio_demo/code/pid.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\E01_gpio_demo\Libraries\doc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_components" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Core" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\user\inc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_common" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_device" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\code" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

