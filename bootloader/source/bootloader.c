#include "bootloader.h"

#define USER_APP_ADDRESS		0x08040000
#define USER_APP_RESET_VECTOR	(USER_APP_ADDRESS + 0x4) // reset vector is at offset by 4 bytes. First four bytes store top of stack value.

extern uint32_t GetTickCounter();
typedef uint32_t TimerMs_t;

#define MAX_FRAME_SIZE	1024

uint8_t start_byte		= 	0x5E;
uint8_t	end_byte		= 	0x5F;
uint8_t esc_byte		=	0x5D;

uint8_t msg_rx_count	=	0;

uint8_t rxd_buf[1024];
uint8_t	frame_buf[1024];
bool frame_received = false;


void Jump_to_application(){

	void (*user_app_reset_handler)(void) = (void *)(*((uint32_t *)(USER_APP_RESET_VECTOR)));

	user_app_reset_handler();

}

TimerMs_t timer_main_start = 0;

void bootloader_main(void){

	timer_main_start = 0;

	while(true){

		if((TimerMs_t)(GetTickCounter() - timer_main_start) > 15000){ // wait 15 sec in bootloader before proceeding to

			//Jump_to_application();
		}

		if(is_frame_ready()){

			process_frame();
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

		memcpy(frame_buf, rxd_buf, msg_rx_count);
		memset(rxd_buf, 0, sizeof(rxd_buf));
		msg_rx_count = 0;

		frame_received = true;

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

    if (msg_rx_count >= sizeof(rxd_buf))
    {
    	start_byte_rcvd = false;
        return ;
    }

    rxd_buf[msg_rx_count] = rxchar;
    msg_rx_count++;
}



bool is_frame_ready(){

	return frame_received;
}

bool process_frame(){

	bool ret_val = false;

	if(!frame_received){
		return ret_val;
	}

	return ret_val;
}





