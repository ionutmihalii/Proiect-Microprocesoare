#include "Pit.h"
#include "uart.h"
#include "gpio.h"

static uint8_t change = 0;

void PIT_Init(void) {
	
	// Activarea semnalului de ceas pentru perifericul PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	// Utilizarea semnalului de ceas pentru tabloul de timere
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;
	// Oprirea decrementarii valorilor numaratoarelor in modul debug
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	// Setarea valoarea numaratorului de pe canalul 0 la o perioada de 1 secunda
	PIT->CHANNEL[0].LDVAL = 0x9FFFFF;
	
  // Activarea întreruperilor pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	// Activarea întreruperii mascabile si setarea prioritatiis
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler(void) {
	
	if(change == 0)
	{
		ChangeColor(0, 0, 1);
		change = 1;
	}
	
	if(change == 1)
	{
		ChangeColor(1, 0, 1);
		change = 2;
	}
	
	if(change == 2)
	{
		ChangeColor(0, 1, 1);
		change = 3;
	}
	
	if(change == 3)
	{
		ChangeColor(0, 0, 0);
		change = 0;
	}
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
	}
}