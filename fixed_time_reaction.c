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

enum STATES{START,STARTUP,ATTACK,RECOVERY};
enum STATES currentstate = START;

volatile float start_up= 66000;//start time;
volatile float attack = 50;
volatile float recovery = 100;
volatile float run=0;

int sw_count=0;
int sw2_count=0;

volatile float timer_tick;


int main()
{

	float temp;
	volatile int rand_no;
	int x;

    	hardware_init();

	PRINTF("\rStanding Heavy Punch\n");
	PRINTF("\rStart_up Time: %.4f,Attack_time:  %.4f,Recovery_time %.4f\n\r",(start_up/1000),(attack/1000),(recovery/1000));




	FRDM_KL26Z_LEDs_Configure();
	PIT_Configure_interrupt_mode(1);

	FRDM_KL26Z_SW1_Configure(PULLUP,FALLING_EDGE);
	FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW3_Configure(0,FALLING_EDGE);

	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);




	while(1)
	{

		switch(currentstate)
		{
		case START:
			sw_count=0;
			sw2_count=0;
			PRINTF("\rPress sw1\n");

			while(sw2_count==0)
			{}

			currentstate=STARTUP;
			break;

		case STARTUP:

			temp=start_up;
			PRINTF("\r%.4f", temp);

			while((sw_count==0) && (start_up-temp)<(start_up))
			{}

			if ((start_up-temp)>=start_up)
			{
				PRINTF("\rtoo slow\n");
				currentstate=START;
			}
			else
			{
				PRINTF("\ryour reaction time was %.4f\n", (temp-start_up)/1000);
				currentstate=START;

			}


			break;
		case ATTACK:

			while((sw2_count==0)&& (attack-temp)<(start_up))
			{}
			if ((attack-temp)>=attack)
			{
				PRINTF("\rtoo slow\n");
				currentstate=STARTUP;
			}
			else
			{
				PRINTF("\ryour reaction time was %.4f\n", (temp-attack)/1000);
				currentstate=ATTACK;
			}

			//	else

			//while(timer_tick-temp<(start_up/1000) && (sw2_count==0))
			//{}
			//LED_set(GREEN,ON);
			//}

		}
	}

	return 0;
}

void PORTC_PORTD_IRQHandler()
{

	if(SW1_read()&&SW1_MASK){

		PORTC_ISFR|= SW1_MASK;
		sw2_count++;


	}
	if(SW2_read()&&SW2_MASK)
	{
		PORTC_ISFR|= SW2_MASK;
		sw_count++;

	}




}

void PIT_IRQHandler()
{
	PIT_TFLG0 = 0X01ul;
	--start_up;

}





