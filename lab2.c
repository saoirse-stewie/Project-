#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"

void PORTC_PORTD_IRQHandler();


int main(void)
{

	hardware_init();
	FRDM_KL26Z_LEDs_Configure();
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);

	FRDM_KL26Z_SW1_Configure(PULLUP, FALLING_EDGE);

	while(1)
		{}
	return 0;
}

void PORTC_PORTD_IRQHandler()
{
	int count=0;
	PORTC_ISFR|= SW1_MASK;
	//count++;
	//PRINTF("%d", count);
	LED_set(GREEN,TOGGLE);

}
