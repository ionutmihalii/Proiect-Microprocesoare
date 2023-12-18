#include "uart.h"
#include "gpio.h"
#include "Pit.h"

int main() 
{
	UART0_Init(115200);
	RGBLed_Init();
	PIT_Init();
	for(;;)
	{
	}

}
