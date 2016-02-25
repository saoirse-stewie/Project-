/*
 * jostick.c

 *
 *  Created on: 23 Feb 2016
 *      Author: g00238234
 */

#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"


void PORTC_PORTD_IRQHandler();
int sw_count=0;
int sw2_count=0;
int main()
{

	hardware_init();

	FRDM_KL26Z_LEDs_Configure();

	FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW3_Configure(0,FALLING_EDGE);

	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
	NVIC_ClearPendingIRQ(30);
	NVIC_EnableIRQ(30);

	////LED_set(GREEN,OFF);
	//LED_set(RED,OFF);
	//LED_set(BLUE,OFF);

	while(1)
	{
		if(sw_count>0)
		{

			PRINTF("UP\n");
			sw_count=0;
		}
		else if(sw2_count>0)
		{
			PRINTF("LEFT\n");
			sw2_count=0;
		}
	}


	return 0;
}
void PORTC_PORTD_IRQHandler()
{

	if(SW2_read()&&SW2_MASK)
	{
		PORTC_ISFR|= SW2_MASK; //clear interrupt flag for ptc1
		for(int i=0;i<200000;i++);
		sw_count++;

	}
	else if(SW3_read()&&SW3_MASK)
	{
		PORTC_ISFR|= SW3_MASK; //clear interrupt flag for ptc1
		for(int i=0;i<200000;i++);
		sw2_count++;

	}
	//PORTD_ISFR|= SW4_MASK; //clear interrupt flag for ptc1
	//LED_set(BLUE,TOGGLE);

	//}


}


