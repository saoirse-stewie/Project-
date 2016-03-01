/*
 * LED_test_external_buttons.c
 *
 *  Created on: 10 Nov 2015
 *      Author: g00238234
 */
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"

#define RDRF_MASK 0x20	//Receive Data Register Full Flag Mask
#define RIE_MASK 0x20	//Receive Interrupt Enable Mask
#define UART_S1_TRDE_MASK 0X80


void PORTC_PORTD_IRQHandler();
void PORTA_IRQHandler();
char char_received();
void enable_UART1_receive_interrupt();
void put_char(char c);
void tx_string(char *ptr);
void UART1_IRQHandler(void);
void uart_config();
int sw_count=0;
int sw2_count=0;
int i;
char recieved_char[3];

enum STATES{START,FIRST,SECOND,THIRD};
enum STATES currentstate = START;


int main()
{

	hardware_init();
	uart_config();
	enable_UART1_receive_interrupt();
	FRDM_KL26Z_LEDs_Configure();

	FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW3_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW4_Configure(0,FALLING_EDGE);

	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);

	NVIC_ClearPendingIRQ(30);
	NVIC_EnableIRQ(30);

	////LED_set(GREEN,OFF);
	//LED_set(RED,OFF);
	//LED_set(BLUE,OFF);

	while(1)
	{
		switch(currentstate)
		{
		case START:
			sw_count=0;
			sw2_count=0;
			while(sw_count==0)
			{}
			tx_string("Start");
			sw_count=0;
			currentstate = FIRST;
			break;
		case FIRST:
			sw2_count=0;
			while(sw2_count==0)
			{}
			tx_string("Combo");
			sw2_count=0;
			currentstate = START;
			break;

		}
	}


	return 0;
}


void PORTC_PORTD_IRQHandler()
{

	if(PORTC_ISFR & SW3_MASK)
		{
			PORTC_ISFR|= SW3_MASK; //clear interrupt flag for ptc1
			LED_set(BLUE,ON);
			for(int i=0;i<2000000;i++);
			LED_set(BLUE,OFF);
			sw2_count++;
		}
		 if(PORTC_ISFR & SW2_MASK)
			{
			PORTC_ISFR|= SW2_MASK; //clear interrupt flag for ptc1
			LED_set(GREEN,ON);
			for(int i=0;i<2000000;i++);
			LED_set(GREEN,OFF);
				sw_count++;
			}

}
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
void UART1_IRQHandler(void)
{
    if(UART1_S1 & RDRF_MASK)
    {
    	//PUTCHAR(UART1_D);
    	//PUTCHAR('A');
    }
}
char char_received()
{
	if(UART1_S1 & RDRF_MASK)
	{

		return 1;
	}
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
void tx_string(char *ptr){
	while(*ptr != '\0')
		put_char(*ptr++);
}

