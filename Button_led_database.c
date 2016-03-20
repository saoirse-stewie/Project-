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
#include <string.h>
#include <stdio.h>

#define BUFFERSIZE 10
#define RDRF_MASK 0x20	//Receive Data Register Full Flag Mask
#define RIE_MASK 0x20	//Receive Interrupt Enable Mask
#define UART_S1_TRDE_MASK 0X80

char cr_mp[2];
char cr_hp[3];
char test[10] = {'1','2','3'};
char buff[10];
char v[] = {'i','h','t'};
char tmpstr;

static int sw2_count=0;
static int sw_count=0;
char *wr;
char * rd;
char received;
char command_received=0;
char received_char[BUFFERSIZE];
static int x =0;
int done = 0;

char buffer[BUFFERSIZE];



enum STATES{START,FIRST,SECOND,THIRD};
enum STATES currentstate = START;

void PORTC_PORTD_IRQHandler();
void PORTA_IRQHandler();
char get_buffer();
void put_buffer(char ch);
void tx_string(char *ptr);
void put_char(char c);
void enable_UART1_receive_interrupt();
char char_received();
void init_buffer();
void UART1_IRQHandler(void);
void uart_config();
char database_extraction();

int main()
{


	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);

	sw_count=0;
	sw2_count=0;


	hardware_init();
	uart_config();
	enable_UART1_receive_interrupt();
	init_buffer();
	FRDM_KL26Z_LEDs_Configure();

	FRDM_KL26Z_SW2_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW3_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW4_Configure(0,FALLING_EDGE);



	PRINTF("hello");
	//LED_set(GREEN,OFF);
	LED_set(ALL,OFF);
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
			LED_set(RED,ON);
			for(int i=0;i<2000000;i++);
			LED_set(RED,OFF);
			currentstate = FIRST;
			break;
		case FIRST:
			sw_count=0;
			sw2_count=0;
			while(sw2_count==0)
			{}
			LED_set(GREEN,ON);
			for(int i=0;i<2000000;i++);
			LED_set(GREEN,OFF);
			//	tx_string("Start"); // add when using GUI
			currentstate = SECOND;
			break;
		case SECOND:
			sw_count=0;
			sw2_count=0;
			while(sw2_count==0)
			{}
			database_extraction();
			int i;
			cr_mp[0] = received_char[0];
			cr_hp[0] = received_char[1];
			cr_mp[1] = received_char[2];
			cr_hp[1] = received_char[3];
			cr_mp[2] = received_char[4];
			cr_hp[2] = received_char[6];

			strncat ((char*)&cr_mp[2], (char*)&received_char[5], 1);//this must be improved
			strncat ((char*)&cr_hp[2], (char*)&received_char[7], 1);

			for(i=0;i<=3;i++)
			{
				PRINTF("%c",cr_mp[i]);
			}
			for(i=0;i<=3;i++)
			{
				PRINTF("%c",cr_hp[i]);
			}

			//TODO
			//add three buttons and extract values for reaction time.... maybe try in an extra function .... this can be main file



			currentstate = FIRST;
			break;
		}

	}
	return 0;
}

char database_extraction()
{

	while(1)
	{
		tx_string("CR_MP CR_HP");

		if(command_received)
		{
			command_received = 0;
			done =0; x =0;
			//memset(received_char,0,BUFFERSIZE); //sets elements back to zero
			while(done==0)
			{
				received_char[x] = get_buffer();
				if(received_char[x]=='n')//if enter detected
				{
					done=1;
				}
				else
					x++;

			}
			int i;
			for(i=0;i<=7;i++)
			{
				return received_char[i];
			}
			///return received_char[0];
			//PRINTF("%c",received_char[0]);
			//PRINTF("%c",received_char[1]);
			//PRINTF("%c",received_char[2]);
			//PRINTF("%c",received_char[3]);
			//PRINTF("%c",received_char[4]);
			//PRINTF("%c",received_char[5]);
			//PRINTF("%c",received_char[6]);
			//PRINTF("%c",received_char[7]);

		}


	}

	return received_char[x];
}


void PORTC_PORTD_IRQHandler()
{

	if(PORTC_ISFR & SW3_MASK)
	{
		PORTC_ISFR|= SW3_MASK; //clear interrupt flag for ptc1
		//for(int i=0;i<2000000;i++);
		//LED_set(BLUE,OFF);
		sw_count++;
	}
	if(PORTC_ISFR & SW2_MASK)
	{
		PORTC_ISFR|= SW2_MASK; //clear interrupt flag for ptc1
		//LED_set(GREEN,ON);
		//for(int i=0;i<2000000;i++);
		//LED_set(GREEN,OFF);
		//LED_set(BLUE,ON);
		//for(int i=0;i<2000000;i++);
		//LED_set(BLUE,OFF);

		//for(int i=0;i<2000000;i++);

		sw2_count++;
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
	int i=0;
	if(UART1_S1 & RDRF_MASK)
	{
		received=UART1_D;
		put_buffer(received);

		if(received == 'n')
		{
			command_received=1;
		}

	}
}
void init_buffer()
{
	wr= buffer; //write pointer points to address of first element in buffer
	rd=buffer;	//read pointer points to address of
	memset(buffer,0,sizeof(buffer)); //sets elements back to zero
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
void put_buffer(char ch)
{
	*wr =ch;//points to the contents in the address in buffer
	//(start address) + (last address)
	if(++wr==(buffer+BUFFERSIZE))//increment write, if its equal to start and end address
		wr = buffer;//point to first address
	if(rd==wr)//if rd pointer and wr pointer are equal
	{
		if(++rd==(buffer+BUFFERSIZE))//increment rd, if its equal to start and end address
			rd= buffer;//point to first address
	}
}
char get_buffer()
{
	char c;
	if(rd==wr)//if the read and write are equal
		return 0; // return 0;
	else
	{
		c = *rd;//points to the contents in the address in buffer
		if(++rd == (buffer+BUFFERSIZE))//increment read, if its equal to start and end address
		{
			rd=0;//reset
		}
		return c;
	}
}




