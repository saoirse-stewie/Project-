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
#define BUFFERSIZE 10
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
char buffer[BUFFERSIZE];
char *wr;
char * rd;
void init_buffer();
void put_buffer(char ch);		//function prototypes
char get_buffer();
char char_received();
void enable_UART1_receive_interrupt();
void put_char(char c);
void tx_string(char *ptr);
char received;
char command_received=0;
static char startup;
char received_char[BUFFERSIZE];
int x =0;
int done = 0;
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
		received=UART1_D;
		put_buffer(received);

		if(received == 'n')
		{
			command_received=1;
		}

		//PUTCHAR(received);
		//PRINTF("%d",counter);

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
	init_buffer();
	//tx_string("CR_MP CR_HP");
	while(1)
	{

		tx_string("CR_MP CR_HP");

		if(command_received)
		{
			command_received =0;
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
				//
			}
			PRINTF("%c",received_char[0]);
		}
			//command_received =0;
			//memset(received_char,0,BUFFERSIZE); //sets elements back to zero
			//done =0; x =0;
			//while(done==0)
			//{
			//	received_char[x] = get_buffer();//first element of array = get buffer function
			//	if(received_char[x]=='\r')//if enter detected
				//{
				//	done =1;//initialise done to 1
				//}
				//else x++;//increment x
			//}
			//PRINTF("%c\n",received_char);

		//}

		//put_char('h;');
		//put_char('a  b');

		//for(int x=0;x<10000;x++);
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
void tx_string(char *ptr){
	while(*ptr != '\0')
		put_char(*ptr++);
}

void init_buffer()
{
	wr= buffer; //write pointer points to address of first element in buffer
	rd=buffer;	//read pointer points to address of
	memset(buffer,0,sizeof(buffer)); //sets elements back to zero
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





