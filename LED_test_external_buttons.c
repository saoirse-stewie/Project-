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


void PORTC_PORTD_IRQHandler();

int main()
{
	hardware_init();

	FRDM_KL26Z_LEDs_Configure();

	FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW3_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW4_Configure(0,FALLING_EDGE);

	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);

	////LED_set(GREEN,OFF);
	//LED_set(RED,OFF);
	//LED_set(BLUE,OFF);

	while(1)
	{}


	return 0;
}


void PORTC_PORTD_IRQHandler()
{

	if(SW2_read()==0)
	{
		PORTC_ISFR|= SW2_MASK; //clear interrupt flag for ptc1
		LED_set(GREEN,TOGGLE);
	}
	else if (SW3_read()==0)
	{
		PORTC_ISFR|= SW3_MASK; //clear interrupt flag for ptc1
		LED_set(RED,TOGGLE);
	}
	else if (SW4_read()==0)
	{
		PORTD_ISFR|= SW4_MASK; //clear interrupt flag for ptc1
		LED_set(BLUE,TOGGLE);
	}


}


