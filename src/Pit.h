#include "MKL25Z4.h"

void PIT_Init(void); /*Functie de initializarea a modulului periferic*/
void PIT_IRQHandler(void); /*Functia de tratarea a intreruperilor pentru perifericul PIT*/
uint8_t UTILS_PrintTimer(uint32_t value); /*Functie menita sa printeze o valoare numarata in formatul MM:SS*/

extern volatile uint8_t flag;
extern volatile uint8_t flag2;
