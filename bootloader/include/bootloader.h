#ifndef __STM32F4_BOOTLOADER_H__
#define __STM32F4_BOOTLOADER_H__


// include files
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"

void bootloader_main(void);
bool is_frame_ready();
bool process_frame();

void process_rxd_char(uint8_t rxchar);

#endif // __STM32F4_BOOTLOADER_H__
