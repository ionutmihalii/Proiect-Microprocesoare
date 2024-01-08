#include "Adc.h"
#include "uart.h"
#include "gpio.h"

#define ADC_CHANNEL (15) /* PORT C PIN 1 */

void ADC0_Init(void) {
	
	/*Activarea semnalului de ceas pentru modulul periferic ADC*/
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	
	
	ADC0->CFG1 = 0x00;

	/*Selectarea modului de conversie pe 10 biti single-ended --> MODE = 10*/
	/*Selectarea sursei de ceas pentru generarea ceasului intern --> ADICLK*/
	/*Selectarea ratei de divizare folosit de periferic pentru generarea ceasului intern --> ADIV*/
	/*Set ADC clock frequency fADCK less than or equal to 4 MHz (PG. 494)*/
	ADC0->CFG1 |= ADC_CFG1_MODE(2) |
							 ADC_CFG1_ADICLK(0) |
							 ADC_CFG1_ADIV(0);
	
	/*DIFF = 0 --> Conversii single-ended (PG. 464)*/
	ADC0->SC1[0] = 0x00;
	ADC0->SC3 = 0x00;

	/*Selectarea modului de conversii continue, */
	/*pentru a-l putea folosi in tandem cu mecanismul de intreruperi*/
	ADC0->SC3 |= ADC_SC3_ADCO_MASK;
	
	/*Activarea subsistemului de conversie prin aproximari succesive pe un anumit canal (PG.464)*/
	ADC0->SC1[0] |= ADC_SC1_ADCH(ADC_CHANNEL);
	/*Enables conversion complete interrupts*/
	ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
}


void Voltage_Read(void){
	uint16_t analog_input = (uint16_t) ((ADC0->R[0]<<6)>>6);
	
	float measured_voltage = (analog_input * 3.3f) / 1024;
	
	uint8_t parte_zecimala = (uint8_t) measured_voltage;
	uint8_t parte_fractionara1 = ((uint8_t)(measured_voltage * 10)) % 10;
	uint8_t parte_fractionara2 = ((uint8_t)(measured_voltage * 100)) % 10;
	UART0_Transmit(parte_zecimala + 0x30);
	UART0_Transmit('.');
	UART0_Transmit(parte_fractionara1 + 0x30);
	UART0_Transmit(parte_fractionara2 + 0x30);
	UART0_Transmit(0x0A);
	UART0_Transmit(0x0D);
	
		
	if(measured_voltage <= 1.1f)
	{
		ChangeColor(0);
	}
	else if(measured_voltage <= 2.2f)
	{
		ChangeColor(1);
	}
	else if(measured_voltage <= 3.3f)
	{
		ChangeColor(2);
	}
}
