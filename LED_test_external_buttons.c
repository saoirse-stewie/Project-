/*
a * LED_test_external_buttons.c
 *
 *  Created on: 10 Nov 2015
 *      Author: g00238234
 */
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"
volatile int sw6_count=0;
volatile int sw7_count=0;
volatile int sw3_count=0;
volatile int tick_count;
volatile int lp = 300;
volatile int hp = 667;

volatile int hit =0;
volatile int temp = 50;
float frames;

int run=0;
volatile int delay = 10000;

enum STATES{READY,PROCESS,RETURN,THIRD};
enum STATES currentstate = READY;

void PORTC_PORTD_IRQHandler();
void PORTA_IRQHandler();
void PIT_IRQHandler();

int main()
{


	hardware_init();

	FRDM_KL26Z_LEDs_Configure();

	//FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
	//FRDM_KL26Z_SW3_Configure(0,FALLING_EDGE);
	//FRDM_KL26Z_SW4_Configure(0,FALLING_EDGE);#
	FRDM_KL26Z_SW5_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW7_Configure(0,FALLING_EDGE);
	//FRDM_KL26Z_SW4_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW8_Configure(0,FALLING_EDGE);

	PIT_Configure_interrupt_mode(0.001);

	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);

	NVIC_ClearPendingIRQ(22);
	NVIC_EnableIRQ(22);

	NVIC_ClearPendingIRQ(30);
	NVIC_EnableIRQ(30);

	LED_set(ALL,OFF);
	PRINTF("\n");
	int r=0;
	while(1)
	{

		PRINTF("Beginning reaction Calculation\n");
		volatile int temp;
		volatile int temp2;
		float reaction;
		float reaction2;
		int frame;
		int count =0;

		char cr_lp_time[50]={0};
		char cr_lp_frame[50]={0};
		char s_lp_time[50]={0};
		char s_lp_frame[50]={0};

		char output[100]={0};

		lp = 300;
		//PRINTF("%d",storage);

		PIT_Configure_interrupt_mode(0.001);



		switch(currentstate)//better naming convention
		{
		case READY:

			while(delay/1000>0)
				PRINTF("\r%3d", --delay / 1000);
			currentstate = PROCESS;
			break;
		case PROCESS:
			PRINTF("\n");

			cr_lp_time[50] = 0;
			cr_lp_frame[50] = 0;
			s_lp_time[50] = 0;
			s_lp_frame[50]= 0;

			run=0;
			sw6_count=0;
			sw7_count=0;

			while((sw6_count==0))
			{}
			hit=0;
			run=1;

			while(lp>=0)
			{
				PRINTF("\r%d", lp);
			}
			PRINTF("\n");
			hit=0;
			sw6_count=0;
			sw7_count=0;

			run=2;
			while(sw6_count==0)
			{}
			run=0;

			hit = hit;
			PRINTF("%d",hit);
			hit=0;
			lp = 300;
			sw6_count=0;
			sw7_count=0;

			currentstate = RETURN;

			break;

		case RETURN:
			sw6_count=0;
			sw7_count=0;

			sw3_count=0;
			LED_set(GREEN,ON);
			for(int i=0;i<2000000;i++);
			LED_set(GREEN,OFF);
			hit = 0;
			run =0;
			currentstate = READY;
			break;


		}


		return 0;
	}
}


void PORTC_PORTD_IRQHandler()
{

	if(PORTD_ISFR & SW7_MASK)//cr_lp
	{
		PORTD_ISFR|= SW7_MASK; //clear interrupt flag for ptc1
		sw6_count++; //x

	}
	if(PORTD_ISFR & SW8_MASK)
	{
		PORTD_ISFR|= SW8_MASK; //clear interrupt flag for ptc1
		sw7_count++;

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

void PIT_IRQHandler()
{
	PIT_TFLG0 = 0x01ul;
	if(run==1)
	{
		lp--;
	}
	else if(run==2)
	{
		hit++;
		//tick_count++;
	}
	//Clear interrupt flag
	//LED_set(RED,ON);
	//for(int i=0;i<2000000;i++);
	//LED_set(RED,OFF);
	//toggle 3

}

