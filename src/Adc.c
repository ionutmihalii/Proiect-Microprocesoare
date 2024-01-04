#include "Adc.h"
#include "uart.h"
#include "gpio.h"

#define ADC_CHANNEL (15) /* PORT C PIN 1 */

void ADC0_Init(void) {
	
	/*Activarea semnalului de ceas pentru modulul periferic ADC*/
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	
	/*Functia de calibrare*/
	//ADC0_Calibrate();
	
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

int ADC0_Calibrate(void) {
	
	/*===== For best calibration results =====*/
	
	ADC0_CFG1 |= ADC_CFG1_MODE(2)  |  				 /*10 bits mode*/
                ADC_CFG1_ADICLK(0)|  /*Input Bus Clock divided by 2*/
                ADC_CFG1_ADIV(0);   /*Clock divide by 8*/
	
	/*The calibration will automatically begin if the SC2[ADTRG] is 0. (PG. 495)*/
	ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;
	
	/*Set hardware averaging to maximum, that is, SC3[AVGE]=1 and SC3[AVGS]=0x11 for an average of 32 (PG. 494)*/
	ADC0->SC3 |= (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3));
	
	/*To initiate calibration, the user sets SC3[CAL] (PG. 495)*/
	ADC0->SC3 |= ADC_SC3_CAL_MASK;
	
	/*At the end of a calibration sequence, SC1n[COCO] will be set (PG. 495)*/
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
	
	/*At the end of the calibration routine, if SC3[CALF] is not*/
	/*set, the automatic calibration routine is completed successfully. (PG. 495)*/
	if(ADC0->SC3 & ADC_SC3_CALF_MASK){
		return (1);
	}
	
	/*====== CALIBRATION FUNCTION (PG.495) =====*/
	
	/*1. Initialize or clear a 16-bit variable in RAM.*/
	uint16_t calibration_var = 0x0000;
	
	/*2. Add the plus-side calibration results CLP0, CLP1, CLP2, CLP3, CLP4, and CLPS to the variable.*/
	calibration_var += ADC0->CLP0;
	calibration_var += ADC0->CLP1;
	calibration_var += ADC0->CLP2;
	calibration_var += ADC0->CLP3;
	calibration_var += ADC0->CLP4;
	calibration_var += ADC0->CLPS;
	
	/*3. Divide the variable by two.*/
	calibration_var /= 2;
	
	/*4. Set the MSB of the variable. */
	calibration_var |= 0x8000;
	
	/*5. Store the value in the plus-side gain calibration register PG.*/
	ADC0->PG = ADC_PG_PG(calibration_var);
	
	/*6. Repeat the procedure for the minus-side gain calibration value.*/
	calibration_var = 0x0000;
	
	calibration_var += ADC0->CLM0;
	calibration_var += ADC0->CLM1;
	calibration_var += ADC0->CLM2;
	calibration_var += ADC0->CLM3;
	calibration_var += ADC0->CLM4;
	calibration_var += ADC0->CLMS;
	
	calibration_var /= 2;
	
	calibration_var |= 0x8000;
	
	ADC0->MG = ADC_MG_MG(calibration_var);
	
	/*Incheierea calibrarii*/
	ADC0->SC3 &= ~ADC_SC3_CAL_MASK;
	
	return (0);
}

uint16_t ADC0_Read(void){
	
	/*A conversion is initiated following a write to SC1A, with SC1n[ADCH] not all 1's (PG. 485)*/
	ADC0->SC1[0] |= ADC_SC1_ADCH(ADC_CHANNEL);
	
	/* ADACT is set when a conversion is initiated*/
	/* and cleared when a conversion is completed or aborted.*/
	while(ADC0->SC2 & ADC_SC2_ADACT_MASK);
	
	/* A conversion is completed when the result of the conversion is transferred */
	/* into the data result registers, Rn (PG. 486)*/
	
	/* If the compare functions are disabled, this is indicated by setting of SC1n[COCO]*/
	/* If hardware averaging is enabled, the respective SC1n[COCO] sets only if*/
	/* the last of the selected number of conversions is completed (PG. 486)*/
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
	
	return (uint16_t) ADC0->R[0];
	
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
