################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/admin/Desktop/E01_gpio_demo/libraries/zf_components/seekfree_assistant.c \
C:/Users/admin/Desktop/E01_gpio_demo/libraries/zf_components/seekfree_assistant_interface.c 

OBJS += \
./zf_components/seekfree_assistant.o \
./zf_components/seekfree_assistant_interface.o 

C_DEPS += \
./zf_components/seekfree_assistant.d \
./zf_components/seekfree_assistant_interface.d 


# Each subdirectory must supply rules for building sources it contributes
zf_components/seekfree_assistant.o: C:/Users/admin/Desktop/E01_gpio_demo/libraries/zf_components/seekfree_assistant.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\E01_gpio_demo\Libraries\doc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_components" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Core" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\user\inc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_common" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_device" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\code" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_components/seekfree_assistant_interface.o: C:/Users/admin/Desktop/E01_gpio_demo/libraries/zf_components/seekfree_assistant_interface.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\E01_gpio_demo\Libraries\doc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_components" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Core" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\user\inc" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_common" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_device" -I"C:\Users\admin\Desktop\E01_gpio_demo\E01_gpio_demo\code" -I"C:\Users\admin\Desktop\E01_gpio_demo\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

