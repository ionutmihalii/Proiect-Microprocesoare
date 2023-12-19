#include "uart.h"
#include "gpio.h"
#include "Pit.h"
#include "Adc.h"

int main(void) 
{
	UART0_Init(115200);
	RGBLed_Init();
	PIT_Init();
	ADC0_Init();
	for(;;)
	{
	}

}
