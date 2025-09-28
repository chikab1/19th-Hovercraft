################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include user_c/subdir.mk
-include libraries/zf_driver/subdir.mk
-include libraries/zf_device/subdir.mk
-include libraries/zf_components/subdir.mk
-include libraries/zf_common/subdir.mk
-include libraries/sdk/Startup/subdir.mk
-include libraries/sdk/Peripheral/subdir.mk
-include libraries/sdk/Core/subdir.mk
-include CODE/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
my_mt9v03x_demo.hex \

SECONDARY_LIST += \
my_mt9v03x_demo.lst \

SECONDARY_SIZE += \
my_mt9v03x_demo.siz \


# 所有目标
all: my_mt9v03x_demo.elf secondary-outputs

# 工具调用
my_mt9v03x_demo.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -T "C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\sdk\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -L"../" -L"C:\Users\admin\Desktop\my_mt9v03x_demo\mrs\libraries\zf_device" -Wl,-Map,"my_mt9v03x_demo.map" --specs=nano.specs -u _printf_float --specs=nosys.specs -Wl,--print-memory-usage -o "my_mt9v03x_demo.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
my_mt9v03x_demo.hex: my_mt9v03x_demo.elf
	@	riscv-none-embed-objcopy -O ihex "my_mt9v03x_demo.elf"  "my_mt9v03x_demo.hex"
	@	@
my_mt9v03x_demo.lst: my_mt9v03x_demo.elf
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "my_mt9v03x_demo.elf" > "my_mt9v03x_demo.lst"
	@	@
my_mt9v03x_demo.siz: my_mt9v03x_demo.elf
	@	riscv-none-embed-size --format=berkeley "my_mt9v03x_demo.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) my_mt9v03x_demo.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
