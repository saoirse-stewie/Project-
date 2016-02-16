/*
 *  Uses UART0 in interrupt mode to echo receive characters
 *
 *  Niall O'Keeffe
 *  V01
 *  11/11/15
 */

#include "board.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include <stdlib.h>
#include <string.h>


#define RDRF_MASK 0x20	//Receive Data Register Full Flag Mask
#define RIE_MASK 0x20	//Receive Interrupt Enable Mask
#define UART_S1_TRDE_MASK 0X80

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
char char_received();
void enable_UART1_receive_interrupt();
void put_char(char c);
/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * UART0 Interrupt Handler
 * Echos received character
 */
void UART1_IRQHandler(void)
{
    if(UART1_S1 & RDRF_MASK)
    {
    	PUTCHAR(UART1_D);
    	//PUTCHAR('A');
    }
}

//this is not needed as hardware init executes this for UART
void uart_config()
{
		int SBR;
		PORTE_PCR0|=0x3u<<8;//mux - ALT2 UART0_RX
		PORTE_PCR1|=0x3u<<8;//mux - ALT2 UART0_TX

		SIM_SCGC4|=0x1u<<11;//enabling clock
		UART1_C2&=0XF3;//diabling transmittion and receiving until config complete
		SIM_SOPT2|=0x1u<<26;
		//uart_clock=CLOCK_SYS_GetPllFllClockFreq();
		//OSR=15;
		SBR=78;
		//UART1_C4=OSR;
		UART1_BDL=SBR&0XFF;
		UART1_BDH|=((SBR&0XFF00)>>8);

		UART1_C1=0;
		UART1_C2|=0X0C;
		UART1_C3=0;
}


int main()
{

	hardware_init();
	uart_config();
	enable_UART1_receive_interrupt();

	PRINTF("UART0 Test Code\n\r");
	PRINTF("Any entered character will be echoed\r\n\n");

	while(1)
	{
		put_char('h');

		if(char_received())
		{
			PRINTF("received");
		}

		for(int x=0;x<10000;x++);
	}
}


char char_received()
{
	if(UART1_S1 & RDRF_MASK)
		return 1;
	else
		return 0;
}


void enable_UART1_receive_interrupt()
{
	//Configure NVIC
	NVIC_ClearPendingIRQ(13);
	NVIC_EnableIRQ(13);
	UART1_C2 |= RIE_MASK;	//set RIE to enable receive interrupt
}
void put_char(char c)
{
	while((UART1_S1 & UART_S1_TRDE_MASK)==0)
	{}
	UART1_D=c;
}





