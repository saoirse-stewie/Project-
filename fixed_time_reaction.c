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

enum STATES{STARTUP,ATTACK,RECOVERY};
enum STATES currentstate = STARTUP;

volatile float start_up= 66;//start time;
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
		PIT_Configure_interrupt_mode(0.001);

		FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
		FRDM_KL26Z_SW3_Configure(0,FALLING_EDGE);

		NVIC_ClearPendingIRQ(31);
		NVIC_EnableIRQ(31);




		while(1)
		{

			switch(currentstate)
			{

			case STARTUP:

				sw_count=0;
				sw2_count=0;
				temp=(start_up/1000);
				PRINTF("\r%.4f", temp);

				while((sw_count==0) && (start_up/1000-temp)<(start_up/1000))
				{
					//PRINTF("%.4f",start_up/1000-temp);
				}
				//if ((start_up/1000-temp)==0)
				//{
				//	PRINTF("\rtoo slow\n");
				//	currentstate=STARTUP;
				//}
				//else
					PRINTF("\ryour reaction time was %.4f", (start_up/1000 - temp));
					currentstate=STARTUP;

					break;
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

	if(SW2_read()&&SW2_MASK)
		{
			PORTC_ISFR|= SW2_MASK;
			sw_count++;
		}
	// if(SW3_read()&&SW3_MASK){

	//	PORTC_ISFR|= SW3_MASK;

	//		sw2_count++;;
			//PRINTF("\r%3d\n",timer_tick1/1000);

		//	}



}

void PIT_IRQHandler()
{
	PIT_TFLG0 = 0X01ul;
	--start_up;

}





