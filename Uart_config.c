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


#define RDRF_MASK 0x20	//Receive Data Register Full Flag Mask
#define RIE_MASK 0x20	//Receive Interrupt Enable Mask
#define UART_S1_TRDE_MASK 0X80

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
char char_received();
void enable_UART0_receive_interrupt();
void put_char(char c);
/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * UART0 Interrupt Handler
 * Echos received character
 */
void UART0_IRQHandler(void)
{
    if(UART0_S1 & RDRF_MASK)
    {
    	PUTCHAR(UART0_D);
    	//PUTCHAR('A');
    }
}

//this is not needed as hardware init executes this for UART
void uart_config()
{
		PORTA_PCR1|=0x2u<<8;//mux - ALT2 UART0_RX
		PORTA_PCR2|=0x2u<<8;//mux - ALT2 UART0_TX

		SIM_SCGC4|=SIM_SCGC4_UART0_MASK;//enabling clock
		UART0_C2&=0XF3;//diabling transmittion and receiving until config complete
		SIM_SOPT2|=0x1u<<26;
		uart_clock=CLOCK_SYS_GetPllFllClockFreq();
		OSR=15;
		SBR=uart_clock/((OSR + 1)*BAUD_RATE);
		UART0_C4=OSR;
		UART0_BDL=SBR&OXFF;
		UART0_BDH|=((SBR&0XFF00)>>8);

		UART0_C1=0;
		UART_C2|=0X0C;
		UART0_C3=0;
}


int main()
{
	hardware_init();
	enable_UART0_receive_interrupt();

	PRINTF("UART0 Test Code\n\r");
	PRINTF("Any entered character will be echoed\r\n\n");

	while(1)
	{

	}
}


char char_received()
{
	if(UART0_S1 & RDRF_MASK)
		return 1;
	else
		return 0;
}


void enable_UART0_receive_interrupt()
{
	//Configure NVIC
	NVIC_ClearPendingIRQ(12);
	NVIC_EnableIRQ(12);
	UART0_C2 |= RIE_MASK;	//set RIE to enable receive interrupt
}
//void put_char(char c)
//{
	//while((UART0_S1 & TDRE_MASK)==0)
	//{}
	//UART0_D=c;
//}





