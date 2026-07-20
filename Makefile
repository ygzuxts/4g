TARGET := 4GCatModule
TOOLCHAIN_BIN ?= $(shell if command -v arm-none-eabi-gcc >/dev/null 2>&1; then dirname "$$(command -v arm-none-eabi-gcc)"; else dirname "$$(find /mnt/c/Program\ Files\ \(x86\) /mnt/c/Program\ Files -type f -iname arm-none-eabi-gcc.exe -print -quit 2>/dev/null)"; fi)
WINDOWS_TEMP_WSL := $(shell temp="$$(cmd.exe /C echo %TEMP% 2>/dev/null | tail -n 1 | tr -d '\r')"; if [ -n "$$temp" ]; then wslpath -u "$$temp" 2>/dev/null; fi)
BUILD_DIR ?= $(shell if printf "%s" "$(TOOLCHAIN_BIN)" | grep -q '^/mnt/c/' && [ -n "$(WINDOWS_TEMP_WSL)" ]; then printf "%s/4g-module-gcc" "$(WINDOWS_TEMP_WSL)"; else printf "build/gcc"; fi)
OUTPUT_DIR ?= build/gcc
LD_WRAPPER_DIR := $(BUILD_DIR)/toolwrap
LINKER_SCRIPT := STM32F407VETx_FLASH.ld
BUILD_LINKER_SCRIPT := $(BUILD_DIR)/$(notdir $(LINKER_SCRIPT))

TOOLCHAIN_BIN_WIN := $(shell wslpath -w "$(TOOLCHAIN_BIN)" 2>/dev/null || printf "%s" "$(TOOLCHAIN_BIN)")
TOOLCHAIN_LIBEXEC_WIN := $(shell wslpath -w "$(TOOLCHAIN_BIN)/../libexec/gcc/arm-none-eabi/12.2.1" 2>/dev/null || printf "%s" "$(TOOLCHAIN_BIN)/../libexec/gcc/arm-none-eabi/12.2.1")
TOOLCHAIN_SYSROOT_WIN := $(shell wslpath -w "$(TOOLCHAIN_BIN)/../arm-none-eabi" 2>/dev/null || printf "%s" "$(TOOLCHAIN_BIN)/../arm-none-eabi")
TOOLCHAIN_GCC_INCLUDE_WIN := $(shell wslpath -w "$(TOOLCHAIN_BIN)/../lib/gcc/arm-none-eabi/12.2.1/include" 2>/dev/null || printf "%s" "$(TOOLCHAIN_BIN)/../lib/gcc/arm-none-eabi/12.2.1/include")
TOOLCHAIN_GCC_INCLUDE_FIXED_WIN := $(shell wslpath -w "$(TOOLCHAIN_BIN)/../lib/gcc/arm-none-eabi/12.2.1/include-fixed" 2>/dev/null || printf "%s" "$(TOOLCHAIN_BIN)/../lib/gcc/arm-none-eabi/12.2.1/include-fixed")
TOOLCHAIN_NEWLIB_INCLUDE_WIN := $(shell wslpath -w "$(TOOLCHAIN_BIN)/../arm-none-eabi/include" 2>/dev/null || printf "%s" "$(TOOLCHAIN_BIN)/../arm-none-eabi/include")
TOOLCHAIN_GCC_LIB_WIN := $(shell wslpath -w "$(TOOLCHAIN_BIN)/../lib/gcc/arm-none-eabi/12.2.1/thumb/v7e-m+fp/hard" 2>/dev/null || printf "%s" "$(TOOLCHAIN_BIN)/../lib/gcc/arm-none-eabi/12.2.1/thumb/v7e-m+fp/hard")
TOOLCHAIN_NEWLIB_LIB_WIN := $(shell wslpath -w "$(TOOLCHAIN_BIN)/../arm-none-eabi/lib/thumb/v7e-m+fp/hard" 2>/dev/null || printf "%s" "$(TOOLCHAIN_BIN)/../arm-none-eabi/lib/thumb/v7e-m+fp/hard")
LD_WRAPPER_DIR_WIN := $(shell wslpath -w "$(abspath $(LD_WRAPPER_DIR))" 2>/dev/null || printf "%s" "$(abspath $(LD_WRAPPER_DIR))")
TARGET_ELF_WIN := $(shell wslpath -w "$(abspath $(BUILD_DIR)/$(TARGET).elf)" 2>/dev/null || printf "%s" "$(abspath $(BUILD_DIR)/$(TARGET).elf)")
TARGET_HEX_WIN := $(shell wslpath -w "$(abspath $(BUILD_DIR)/$(TARGET).hex)" 2>/dev/null || printf "%s" "$(abspath $(BUILD_DIR)/$(TARGET).hex)")
TARGET_BIN_WIN := $(shell wslpath -w "$(abspath $(BUILD_DIR)/$(TARGET).bin)" 2>/dev/null || printf "%s" "$(abspath $(BUILD_DIR)/$(TARGET).bin)")
TARGET_MAP_WIN := $(shell wslpath -w "$(abspath $(BUILD_DIR)/$(TARGET).map)" 2>/dev/null || printf "%s" "$(abspath $(BUILD_DIR)/$(TARGET).map)")
LINKER_SCRIPT_WIN := $(shell wslpath -w "$(abspath $(BUILD_LINKER_SCRIPT))" 2>/dev/null || printf "%s" "$(abspath $(BUILD_LINKER_SCRIPT))")

export COMPILER_PATH := $(TOOLCHAIN_BIN_WIN);$(TOOLCHAIN_LIBEXEC_WIN)
export LIBRARY_PATH := $(TOOLCHAIN_GCC_LIB_WIN);$(TOOLCHAIN_NEWLIB_LIB_WIN)

CC := "$(TOOLCHAIN_BIN)/arm-none-eabi-gcc.exe"
AS := "$(TOOLCHAIN_BIN)/arm-none-eabi-gcc.exe"
OBJCOPY := "$(TOOLCHAIN_BIN)/arm-none-eabi-objcopy.exe"
SIZE := "$(TOOLCHAIN_BIN)/arm-none-eabi-size.exe"
GCC_DRIVER_FLAGS := -B"$(LD_WRAPPER_DIR_WIN)\\" -B"$(TOOLCHAIN_LIBEXEC_WIN)\\" -B"$(TOOLCHAIN_BIN_WIN)\\arm-none-eabi-" -B"$(TOOLCHAIN_BIN_WIN)\\" --sysroot="$(TOOLCHAIN_SYSROOT_WIN)"
GCC_SYSTEM_INCLUDES := -isystem "$(TOOLCHAIN_GCC_INCLUDE_WIN)" -isystem "$(TOOLCHAIN_GCC_INCLUDE_FIXED_WIN)" -isystem "$(TOOLCHAIN_NEWLIB_INCLUDE_WIN)"

CPU := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
DEFS := -DUSE_HAL_DRIVER -DSTM32F407xx

INCLUDES := \
  -ICore/Inc \
  -IDrivers/STM32F4xx_HAL_Driver/Inc \
  -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
  -IMiddlewares/Third_Party/FreeRTOS/Source/include \
  -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS \
  -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
  -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
  -IDrivers/CMSIS/Include \
  -IBSP \
  -IMiddlewares/Mavlink \
  -IMiddlewares/Mavlink/common \
  -IMiddlewares/Mavlink/minimal \
  -IMiddlewares/FatFs \
  -IMiddlewares/FatFs/drivers \
  -IMiddlewares/FatFs/option

C_SOURCES := \
  Core/Src/main.c \
  Core/Src/freertos.c \
  Core/Src/stm32f4xx_it.c \
  Core/Src/stm32f4xx_hal_msp.c \
  Core/Src/stm32f4xx_hal_timebase_tim.c \
  Core/Src/gcc_syscalls.c \
  BSP/cJSON.c \
  BSP/usart_4gmoudle.c \
  BSP/usart_debug.c \
  BSP/usart_mavlink.c \
  BSP/track_queue.c \
  BSP/track_json.c \
  BSP/rtcm_parse.c \
  BSP/bsp_sdio_sd.c \
  BSP/bsp_led.c \
  BSP/bsp_can.c \
  BSP/bsp_Flash.c \
  BSP/mavlink_usart_fifo.c \
  Middlewares/Mavlink/mavlink_parse.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_mmc.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c \
  Core/Src/system_stm32f4xx.c \
  Middlewares/FatFs/diskio.c \
  Middlewares/FatFs/ff.c \
  Middlewares/FatFs/ff_gen_drv.c \
  Middlewares/FatFs/drivers/sd_diskio.c \
  Middlewares/FatFs/option/ccsbcs.c \
  Middlewares/FatFs/option/syscall.c \
  Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
  Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
  Middlewares/Third_Party/FreeRTOS/Source/list.c \
  Middlewares/Third_Party/FreeRTOS/Source/queue.c \
  Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
  Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
  Middlewares/Third_Party/FreeRTOS/Source/timers.c \
  Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c \
  Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
  Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c

ASM_SOURCES := Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s

OBJECTS := $(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.o))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(ASM_SOURCES:.s=.o))
OBJECTS_WIN := $(foreach obj,$(OBJECTS),"$(shell wslpath -w "$(abspath $(obj))" 2>/dev/null || printf "%s" "$(abspath $(obj))")")
DEPS := $(OBJECTS:.o=.d)

CFLAGS := $(GCC_DRIVER_FLAGS) $(CPU) $(DEFS) $(INCLUDES) $(GCC_SYSTEM_INCLUDES) -Og -g3 -ffunction-sections -fdata-sections -Wall -Wno-unused-parameter
ASFLAGS := $(GCC_DRIVER_FLAGS) $(CPU) $(DEFS) $(INCLUDES) $(GCC_SYSTEM_INCLUDES) -x assembler-with-cpp
LDFLAGS := $(GCC_DRIVER_FLAGS) $(CPU) -nostartfiles -T"$(LINKER_SCRIPT_WIN)" -L"$(TOOLCHAIN_GCC_LIB_WIN)" -L"$(TOOLCHAIN_NEWLIB_LIB_WIN)" --specs=nano.specs --specs=nosys.specs -Wl,-Map="$(TARGET_MAP_WIN)",--cref -Wl,--gc-sections
LIBS := -lc -lm -lnosys

.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin
	$(SIZE) "$(TARGET_ELF_WIN)"
	@if [ "$(abspath $(BUILD_DIR))" != "$(abspath $(OUTPUT_DIR))" ]; then \
		mkdir -p "$(OUTPUT_DIR)"; \
		cp "$(BUILD_DIR)/$(TARGET).elf" "$(BUILD_DIR)/$(TARGET).hex" "$(BUILD_DIR)/$(TARGET).bin" "$(BUILD_DIR)/$(TARGET).map" "$(OUTPUT_DIR)/"; \
	fi

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) $(BUILD_LINKER_SCRIPT) $(LD_WRAPPER_DIR)/ld.exe
	$(CC) $(OBJECTS_WIN) $(LDFLAGS) $(LIBS) -o "$(TARGET_ELF_WIN)"

$(BUILD_LINKER_SCRIPT): $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	cp $< $@

$(LD_WRAPPER_DIR)/ld.exe:
	@mkdir -p $(dir $@)
	cp "$(TOOLCHAIN_BIN)/arm-none-eabi-ld.exe" $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O ihex "$(TARGET_ELF_WIN)" "$(TARGET_HEX_WIN)"

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O binary -S "$(TARGET_ELF_WIN)" "$(TARGET_BIN_WIN)"

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -MF "$$(wslpath -w "$$(realpath -m "$(@:.o=.d)")")" -MT "$@" -c $< -o "$$(wslpath -w "$$(realpath -m "$@")")"

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -MMD -MP -MF "$$(wslpath -w "$$(realpath -m "$(@:.o=.d)")")" -MT "$@" -c $< -o "$$(wslpath -w "$$(realpath -m "$@")")"

clean:
	rm -rf $(BUILD_DIR)
	@if [ "$(abspath $(BUILD_DIR))" != "$(abspath $(OUTPUT_DIR))" ]; then rm -rf "$(OUTPUT_DIR)"; fi

-include $(DEPS)
