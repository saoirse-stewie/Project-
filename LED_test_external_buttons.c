/*
 * LED_test_external_buttons.c
 *
 *  Created on: 10 Nov 2015
 *      Author: g00238234
 */
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"
static int sw2_count=0;
static int sw_count=0;

enum STATES{START,FIRST,SECOND,THIRD};
enum STATES currentstate = START;

void PORTC_PORTD_IRQHandler();
void PORTA_IRQHandler();

int main()
{

	hardware_init();

	FRDM_KL26Z_LEDs_Configure();

	FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW3_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW4_Configure(0,FALLING_EDGE);

	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);

	NVIC_ClearPendingIRQ(30);
	NVIC_EnableIRQ(30);

	PRINTF("hello");
	//LED_set(GREEN,OFF);
	LED_set(ALL,OFF);
	//LED_set(BLUE,OFF);

	while(1)
	{

		switch(currentstate)
		{
		case START:
			sw_count=0;
			sw2_count=0;
			while(sw_count==0)
			{}
			//if(sw_count > 0)
			//{
			LED_set(RED,ON);
			for(int i=0;i<2000000;i++);
			LED_set(RED,OFF);
			currentstate = FIRST;
			break;
			//}
		case FIRST:
			sw_count=0;
			sw2_count=0;
			while(sw2_count==0)
			{}
			LED_set(GREEN,ON);
			for(int i=0;i<2000000;i++);
			LED_set(GREEN,OFF);
			currentstate = START;
			break;




			//else if(sw2_count>0)
			//{

			//}
			//else if(sw2_count>0)
			//{
			//	sw_count=0;
			//	sw2_count=0;


		}
	}


	return 0;
}


void PORTC_PORTD_IRQHandler()
{

	if(PORTC_ISFR & SW3_MASK)
	{
		PORTC_ISFR|= SW3_MASK; //clear interrupt flag for ptc1
		//for(int i=0;i<2000000;i++);
		//LED_set(BLUE,OFF);
		sw_count++;
	}
	if(PORTC_ISFR & SW2_MASK)
	{
		PORTC_ISFR|= SW2_MASK; //clear interrupt flag for ptc1
		//LED_set(GREEN,ON);
		//for(int i=0;i<2000000;i++);
		//LED_set(GREEN,OFF);
		//LED_set(BLUE,ON);
		//for(int i=0;i<2000000;i++);
		//LED_set(BLUE,OFF);

		//for(int i=0;i<2000000;i++);

		sw2_count++;
	}
	//else if (SW3_read()==0)
	//{
	//PORTC_ISFR|= SW3_MASK; //clear interrupt flag for ptc1
	//LED_set(BLUE,TOGGLE);
	//}
	//else if (SW4_read())
	//{

	//PORTD_ISFR|= SW4_MASK; //clear interrupt flag for ptc1
	//LED_set(BLUE,TOGGLE);

	//}


}
void PORTA_IRQHandler()
{
	PORTA_ISFR|= SW3_MASK;
	LED_set(BLUE,TOGGLE);
}



