#ifndef __STM32F4_BOOTLOADER_H__
#define __STM32F4_BOOTLOADER_H__


// include files
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal_flash.h"

void bootloader_main(void);
bool IsFrameReceived();
bool VerifyFrame();
bool BL_eraseApp();

void process_rxd_char(uint8_t rxchar);
bool BL_VerifyApplication(void);



enum state_t {
	BL_INIT,
	BL_TIMEOUT, // JUMP TO APP
	BL_JUMP_TO_APP,
	BL_FRAME_PROCESS,
	BL_ERASE,
	BL_WRITE,
	BL_VERIFY_APP
};

#endif // __STM32F4_BOOTLOADER_H__
