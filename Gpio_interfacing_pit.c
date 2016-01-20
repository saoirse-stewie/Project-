#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"

//#define PORTA_CLK_ENABLE 0x00000200 //Port A clk enable
//#define SW1_MASK 0x8
void PIT_Configure_interrupt_mode(float time_delay_secs);
void PIT_IRQ();
void sw_ISR();
void sw2_ISR();

volatile int timer_tick=0;
volatile int sw_count=0;
volatile int sw2_count=0;

enum STATES{INIT,PLAY};
enum STATES currentstate = INIT;



int main()
{

	volatile int temp;
	volatile int rand_no;
	int x;

	hardware_init();

	FRDM_KL26Z_LEDs_Configure();

	FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW1_Configure(PULLUP,FALLING_EDGE);

	PIT_Configure_interrupt_mode(0.01);

	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);

	LED_set(GREEN,OFF);

	while(1)
	{
		switch(currentstate)
		{
		case INIT:
			sw_count=0;
			sw2_count=0;
			PRINTF("SW1 to start");
			while(sw_count==0)
			{}
			LED_set(GREEN,OFF);
			currentstate = PLAY;
			break;
		case PLAY:
			PRINTF("when led on, press sw2");
			srand(timer_tick);
			rand_no = rand()%5000;
			temp=timer_tick;

			while((timer_tick-temp<rand_no) && (sw2_count==0) )
			{}
			if(sw2_count>0)
			{
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

void PIT_IRQ()
{
	PIT_TFLG0=0x01ul;
	PIT_TCTRL0 |= 0x03ul;
	timer_tick++;
}
void sw_ISR()
{
	PORTC_ISFR|= SW2_MASK; //clear interrupt flag for ptc1
	sw_count++;
}
void sw2_ISR()
{
	PORTC_ISFR|= SW1_MASK; //clear interrupt flag for ptc1
	sw2_count++;
}
