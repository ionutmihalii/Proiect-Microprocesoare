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
			Voltage_Read();
			if(UART0->S1 & UART0_S1_RDRF_MASK) 
			{
				char c = UART0->D;
				if(c == 'f')
					flag2 = !flag2;
			}
	}

}
