################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/zf_components/seekfree_assistant.c \
../libraries/zf_components/seekfree_assistant_interface.c 

OBJS += \
./libraries/zf_components/seekfree_assistant.o \
./libraries/zf_components/seekfree_assistant_interface.o 

C_DEPS += \
./libraries/zf_components/seekfree_assistant.d \
./libraries/zf_components/seekfree_assistant_interface.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/zf_components/%.o: ../libraries/zf_components/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\doc" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_components" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Core" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\user\inc" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_common" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_device" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\CODE" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

