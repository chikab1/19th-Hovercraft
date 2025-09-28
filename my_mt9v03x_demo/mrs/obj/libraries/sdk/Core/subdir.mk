################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/sdk/Core/core_riscv.c 

OBJS += \
./libraries/sdk/Core/core_riscv.o 

C_DEPS += \
./libraries/sdk/Core/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/sdk/Core/%.o: ../libraries/sdk/Core/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\doc" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_components" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Core" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Ld" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Peripheral" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Startup" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\user\inc" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_common" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_device" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\CODE" -I"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

