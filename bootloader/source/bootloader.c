#include "bootloader.h"

#define USER_APP_ADDRESS		0x08040000
#define USER_APP_RESET_VECTOR	(USER_APP_ADDRESS + 0x4) // reset vector is at offset by 4 bytes. First four bytes store top of stack value.

extern uint32_t GetTickCounter();
typedef uint32_t TimerMs_t;

enum state_t state;

#define MAX_FRAME_SIZE	1024

uint8_t start_byte		= 	0x5E;
uint8_t	end_byte		= 	0x5F;
uint8_t esc_byte		=	0x5D;

uint8_t msgRxCount	=	0;

uint8_t rxdBuf[1024];
uint8_t	frameBuf[1024];
uint16_t frameBufLength;

bool frameReceived = false;
bool frameVerified = false;

bool retVal = false;


void Jump_to_application(){

	void (*user_app_reset_handler)(void) = (void *)(*((uint32_t *)(USER_APP_RESET_VECTOR)));

	user_app_reset_handler();

}

TimerMs_t timer_main_start = 0;

void bootloader_main(void){

	timer_main_start = 0;

	state = BL_INIT;


	while(true){


		switch(state){

			case BL_INIT:
				// wait 15 seconds in bootloader before proceeding to
				if((TimerMs_t)(GetTickCounter() - timer_main_start) > 15000){

					state = BL_TIMEOUT;
				}
				// if frame received at last moment still give chance
				if(frameReceived){

					state = BL_FRAME_PROCESS;
				}
				break;
			case BL_TIMEOUT:
				state = BL_VERIFY_APP;
				break;

			case BL_JUMP_TO_APP:
				Jump_to_application(); // No return function
				state = BL_INIT; // control should not reach here
				break;

			case BL_FRAME_PROCESS:
				retVal = VerifyFrame();
				if(!retVal){
					//clean up buffer
					frameBufLength = 0;
					memset(frameBuf, 0, sizeof(frameBuf));
					state = BL_INIT;
				}

				//s//tate = BL_
				break;
			case BL_ERASE:
				// erase application
				retVal = BL_eraseApp();
				if(!retVal){
					BL_eraseApp();
				}

				if(frameReceived){
					state = BL_WRITE;
				}
				state = BL_INIT;

				break;
			case BL_WRITE:
				break;
			case BL_VERIFY_APP:
				retVal = BL_VerifyApplication();
				if(!retVal){
					// Bad Application
					state = BL_ERASE;
				}
				break;
			default:
				break;
		}
	}
}


void process_rxd_char(uint8_t rxchar){

	static bool start_byte_rcvd 	= false;
	static bool esc_byte_rcvd	= false;

	//TODO : CRC calculation

	if(start_byte == rxchar){

		// clean up
		esc_byte_rcvd = false;
		start_byte_rcvd = true;
		return;
	}

	if(!start_byte_rcvd) return;

	if(end_byte == rxchar){

		start_byte_rcvd = false;

		memcpy(frameBuf, rxdBuf, msgRxCount);
		memset(rxdBuf, 0, sizeof(rxdBuf));
		frameBufLength = msgRxCount;
		msgRxCount = 0;

		frameReceived = true;

		return;

	}

	if(esc_byte ==  rxchar){
		if(esc_byte_rcvd){
			// two escape byte is not expected
			start_byte_rcvd = false;
		}
		esc_byte_rcvd = true;
		return;
	}

	if(esc_byte_rcvd == true){

		rxchar = (uint8_t)(rxchar ^ 0x20);
		esc_byte_rcvd = false;
	}

    if (msgRxCount >= sizeof(rxdBuf))
    {
    	start_byte_rcvd = false;
        return ;
    }

    rxdBuf[msgRxCount] = rxchar;
    msgRxCount++;
}



bool IsFrameReceived(){

	return frameReceived;
}

bool flashErased = false;

bool VerifyFrame(){

	if(!frameReceived){

		return false;
	}

	// todo crc check

	frameVerified = true;

	return true;
}


bool BL_eraseApp(){

	uint32_t sectorError;
	HAL_StatusTypeDef status = HAL_OK;
	FLASH_EraseInitTypeDef eraseStruct;

	status = HAL_FLASH_Unlock();
	if(status != HAL_OK) return false;

	eraseStruct.Sector = 6; // application start at 0x0804 0000; end at 0x080f ffff
	eraseStruct.NbSectors = 6; // sector 6 to sector 11;

	eraseStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	eraseStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	status = HAL_FLASHEx_Erase(&eraseStruct, &sectorError);
	if(status != HAL_OK) return false;

	status = HAL_FLASH_Lock();
	if(!retVal) return false;

	return true;
}

bool BL_VerifyApplication(void){

	return false;
}




