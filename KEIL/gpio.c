#include "gpio.h"

#define RED_LED_PIN (18) /* PORT B*/
#define GREEN_LED_PIN (19) /* PORT B*/
#define BLUE_LED_PIN (1) /* PORT D*/

#define GREEN (12) /* PORT A*/
#define YELLOW (1) /* PORT B*/
#define RED (3) /* PORT B*/

void RGBLed_Init(void){
	
	/*Activarea semnalului de ceas pentru pinii folositi in cadrul led-ului RGB*/
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTA_MASK;
	
	/*--- RED LED ---*/
	
	/*Utilizare GPIO ca varianta de multiplexare*/
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	/*Configurare pin pe post de output*/
	GPIOB_PDDR |= (1<<RED_LED_PIN);
	
	/*Stingerea LED-ului (punerea pe 0 logic)*/
	GPIOB_PSOR |= (1<<RED_LED_PIN);
	
	/*--- GREEN LED ---*/
	
	/*Utilizare GPIO ca varianta de multiplexare*/
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	/*Configurare pin pe post de output*/
	GPIOB_PDDR |= (1<<GREEN_LED_PIN);
	
	/*Stingerea LED-ului (punerea pe 0 logic)*/
	GPIOB_PSOR |= (1<<GREEN_LED_PIN);
	
	/*--- BLUE LED ---*/
	
		/*Utilizare GPIO ca varianta de multiplexare*/
	PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(1);
	
	/*Configurare pin pe post de output*/
	GPIOD_PDDR |= (1<<BLUE_LED_PIN);
	
	/*Stingerea LED-ului (punerea pe 0 logic)*/
	/*GPIOD_PSOR |= (1<<BLUE_LED_PIN);*/
	
	/*--- EXTERN GREEN LED ---*/
	
	/*Utilizare GPIO ca varianta de multiplexare*/
	PORTA->PCR[GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[GREEN] |= PORT_PCR_MUX(1);
	
	/*Configurare pin pe post de output*/
	GPIOA_PDDR |= (1<<GREEN);
	
	/*Stingerea LED-ului (punerea pe 0 logic)*/
	GPIOA_PSOR |= (1<<GREEN);
	
	/*--- EXTERN YELLOW LED ---*/
	
	/*Utilizare GPIO ca varianta de multiplexare*/
	PORTB->PCR[YELLOW] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[YELLOW] |= PORT_PCR_MUX(1);
	
	/*Configurare pin pe post de output*/
	GPIOB_PDDR |= (1<<YELLOW);
	
	/*Stingerea LED-ului (punerea pe 0 logic)*/
	GPIOB_PSOR |= (1<<YELLOW);
	
	/*--- EXTERN RED LED ---*/
	
	/*Utilizare GPIO ca varianta de multiplexare*/
	PORTB->PCR[RED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED] |= PORT_PCR_MUX(1);
	
	/*Configurare pin pe post de output*/
	GPIOB_PDDR |= (1<<RED);
	
	/*Stingerea LED-ului (punerea pe 0 logic)*/
	GPIOB_PSOR |= (1<<RED);
}

void ChangeColor(int state)
{
	GPIOB_PSOR |= (1<<RED);
	GPIOB_PSOR |= (1<<YELLOW);
	GPIOA_PSOR |= (1<<GREEN);
	
	if(state == 0)
	{
		GPIOA_PTOR |= (1<<GREEN);
	}
	
	if(state == 1)
	{
		GPIOA_PTOR |= (1<<GREEN);
		GPIOB_PTOR |= (1<<YELLOW);
	}
	
	if(state == 2)
	{
		GPIOA_PTOR |= (1<<GREEN);
		GPIOB_PTOR |= (1<<YELLOW);
		GPIOB_PTOR |= (1<<RED);
	} 
}
