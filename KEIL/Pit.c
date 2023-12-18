#include "Pit.h"
#include "gpio.h"

volatile uint8_t flag = 0;


void PIT_Init(void) {
	
	/*Activarea semnalului de ceas pentru perifericul PIT */
	
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	/*Utilizarea semnalului de ceas pentru tabloul de timere */
	
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;
	
	/* Oprirea decrementarii valorilor numaratoarelor in modul debug */
	
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	
	/* Setarea valoarea numaratorului de pe canalul 0 la o perioada de 1.476 secunde */
	
	PIT->CHANNEL[0].LDVAL = 0xEC28F4;

	
  /*Activarea intreruperilor pe canalul 0 */
	
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	
	/* Activarea timerului de pe canalul 0 */
	
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	
	/* Activarea intreruperii mascabile si setarea prioritatii */
	
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler(void) {
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
	{
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		if(flag == 0) /* roz */
		{
			GPIOB_PCOR |= (1 << 18);	
		}
		if(flag == 1) /* cian */
		{
			GPIOB_PSOR |= (1 << 18); 
			GPIOB_PCOR |= (1<<19);
		}
		if(flag == 2) /* stins */
		{
			GPIOD_PSOR |= (1 << 1);
			GPIOB_PSOR |= (1<<19);
		}
		if(flag == 3) /* albastru */
		{
			GPIOD_PCOR |= (1 << 1);
			flag = (uint8_t)-1;
		}
		flag++;
	}
}

