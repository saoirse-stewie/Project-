#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"

//#define PORTA_CLK_ENABLE 0x00000200 //Port A clk enable
//#define SW1_MASK 0x8



int main()
{
	hardware_init();
	FRDM_KL26Z_LEDs_Configure();
	FRDM_KL26Z_SW2_Configure(0,0);
	FRDM_KL26Z_SW3_Configure(0,0);

	LED_set(GREEN,OFF);


	//SIM_SCGC5|=PORTA_CLK_ENABLE; //enable port module using SIM_SCGC5
	//PORTA_PCR1|= 0x1u << 8 | 0x3;//configure the port pin multiplexing using the PORTx_PCRn
								//GPIO pin with pull up
	while(1)
	{

		//if(GPIOA_PDIR & SW1_MASK)

		if(SW2_read()&&SW2_MASK)
		{
			LED_set(GREEN,ON);
		}
		//else if(SW3_read())
		//{
			//LED_set(RED,TOGGLE);
		//}
	}









	//configure the port pin direction using the PORTx_PDDR
	//use the PORTx_PDIR to read an input pin
	//use PORTx_PDOR,Portx_PSOR,PORTX_PCOR,PORTx_PTOR





	return 0;
}
