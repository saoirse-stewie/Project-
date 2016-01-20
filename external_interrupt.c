/*
 * external_interrupt.c
 *
 *  Created on: 6 Nov 2015
 *      Author: g00238234
 */

#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"
volatile int x=0;
void PORTC_PORTD_IRQHandler();

int main()
{


	hardware_init();
	FRDM_KL26Z_LEDs_Configure();

	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
	FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW1_Configure(PULLUP,FALLING_EDGE);

	while(1)
	{
		PRINTF("Press sw1\n");
	}

	return 0;
}

void PORTC_PORTD_IRQHandler()
{

	x++;

	if(SW1_read())
	{
		PORTC_ISFR|= SW1_MASK;
		LED_set(RED,TOGGLE);
	}
	else
	{
		PORTC_ISFR|= SW2_MASK; //clear interrupt flag for ptc1
		LED_set(GREEN,TOGGLE);
	}
}

