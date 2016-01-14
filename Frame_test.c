#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"

//void PORTC_PORTD_IRQHandler();
void PIT_ISR();//ISR function prototype
char char_received();
int tick_count=0;//initialise tick_count to 0
void PIT_Configure_interrupt_mode(float time_delay_secs);
void LED_set(int LED_code, int setting);
void PIT_IRQ();


int main()
{

	hardware_init();
	FRDM_KL26Z_LEDs_Configure();
	PIT_Configure_interrupt_mode(1);

	while(1)
	{}
	return 0;

}
void PIT_IRQ()
{
	PIT_TFLG0=0x01ul;
	LED_set(ALL,TOGGLE);
}
