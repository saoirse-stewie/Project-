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
void PIT_IRQHandler();

enum STATES{INIT,PLAY};
enum STATES currentstate = INIT;

volatile int timer_tick=0;
volatile int sw_count=0;
volatile int sw2_count=0;


int main()
{

	volatile int temp;
	volatile int rand_no;
	int x;

	hardware_init();
	FRDM_KL26Z_LEDs_Configure();
	PIT_Configure_interrupt_mode(0.01);

	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);

	FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW1_Configure(PULLUP,FALLING_EDGE);


	LED_set(GREEN,OFF);

	while(1)
	{
		switch(currentstate)
		{
		case INIT:
			sw_count=0;
			sw2_count=0;
			PRINTF("\rPress sw1\n");
			while(sw_count==0)
			{}
			LED_set(GREEN,OFF);
			currentstate=PLAY;
			break;
		case PLAY:
			PRINTF("\rwhen led on press switch 2\n");
			srand(timer_tick);
			rand_no = rand()%5000;
			temp=timer_tick;

			while((timer_tick-temp<rand_no) && (sw2_count==0) )
			{}
			if(sw2_count>0)
			{
				PRINTF("BOO\n");
				currentstate=INIT;
				break;
			}
			LED_set(GREEN,ON);
			temp=timer_tick;
			while(((sw2_count==0)&&timer_tick-temp<5000))
			{}
			if(timer_tick-temp>=5000)
			{
				PRINTF("too slow");
				currentstate=INIT;
			}
			else
			{
				PRINTF("reaction time: %d MSec \n" ,(timer_tick-temp));
				LED_set(GREEN,OFF);
				currentstate = INIT;
			}
			break;
		default:
			break;
		}


	}

	return 0;
}

void PORTC_PORTD_IRQHandler()
{

	if(SW1_read()==0)
	{
		PORTC_ISFR|= SW1_MASK;
		sw_count++;

	}
	else if(SW2_read()==0)
	{
		PORTC_ISFR|= SW2_MASK; //clear interrupt flag for ptc1
		sw2_count++;
	}
}

void PIT_IRQHandler()
{
	PIT_TFLG0 = 0X01ul;
	timer_tick++;
}





