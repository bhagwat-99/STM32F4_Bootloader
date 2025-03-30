#include "bootloader.h"

extern uint32_t GetTickCounter();
typedef uint32_t TimerMs_t;


TimerMs_t TimerRedLed = 0;
void TaskControlRedLed(){

	if((TimerMs_t)(GetTickCounter() - TimerRedLed) < 1000) return;

	//Toggle the red led
	HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);

	TimerRedLed = GetTickCounter();
}

void bootloader_main(void){

	while(true){

		TaskControlRedLed();


	}
}
