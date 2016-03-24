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
#define TIME 60

char combo1[] = "CR_MP CR_HP";


static int cr_mp[2];
static int cr_hp[2];

static int reaction_crmp=0;
static int reaction_crhp=0;
volatile int storage=0;
volatile int storage2=0;


static int sw2_count=0;
static int sw_count=0;
static int sw3_count=0;
static int sw4_count=0;

volatile int tick_count=0;
volatile int delay = 10000;


char *wr;
char * rd;
char received;
char command_received=0;
char received_char[BUFFERSIZE];
static int x =0;
int done = 0;
int run=0;
int run2=0;

char buffer[BUFFERSIZE];





enum STATES{START,FIRST,SECOND,THIRD};
enum STATES currentstate = START;

enum COMBOSTATES{READY, PROCESS, RETURN};
enum COMBOSTATES thisstate = READY;

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
void database_extraction();
void char_int_conversion(char c[], int, int);
double reaction_time(int,int);
void PIT_IRQHandler();

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
	FRDM_KL26Z_SW5_Configure(0,FALLING_EDGE);

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
			tx_string("Start"); // add when using GUI
			currentstate = SECOND;
			break;
		case SECOND:
			sw_count=0;
			sw2_count=0;
			while(sw2_count==0)
			{}
			tx_string("Combo"); // add when using GUI
			database_extraction(combo1);
			currentstate = FIRST;
			break;
		}

	}
	return 0;
}

void database_extraction(char ch[])
{

	while(1)
	{

		tx_string(ch);

		if(command_received)
		{
			command_received = 0;
			done =0; x =0;
			memset(received_char,0,BUFFERSIZE); //sets elements back to zero
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

			char_int_conversion(ch, reaction_crmp, reaction_crhp);

		}
	}
}

void char_int_conversion(char c[], int reaction_crmp, int reaction_crhp)
{

	int i;
	int n=0;
	PRINTF("HERE");
	if(strcmp(c,combo1)==0)//compare both the strings
	{
		cr_mp[0] = received_char[0]-'0';//startup//4
		cr_mp[1] = received_char[2]-'0';//active//4
		cr_mp[2] = received_char[5]-'0';//recovery //17
		cr_mp[2]  += 10;


		for(i=0;i<3;i++)
		{
			//PRINTF("%d", cr_mp[i]);
			reaction_crmp = reaction_crmp + cr_mp[i];
			//reaction_crhp = reaction_crhp +
		}
		reaction_crmp=(reaction_crmp*1000)/TIME;

		cr_hp[0] = received_char[1]-'0';//startup//4
		cr_hp[1] = received_char[3]-'0';//active//8
		cr_hp[2] = received_char[6]-'0';//recovery//20
		cr_hp[2] *=10;
		PRINTF("%d",cr_hp[2]);

		for(i=0;i<3;i++)
		{
			reaction_crhp = reaction_crhp + cr_hp[i];
		}
		reaction_crhp=reaction_crhp/TIME;

		reaction_time(reaction_crmp,reaction_crhp);

	}

}

double reaction_time(int reaction_crmp, int reaction_crhp)
{
	PRINTF("Beginning reaction Calculation\n");
	volatile int temp;
	volatile int temp2;


	float reaction;
	char *buffer2=(char *)&reaction;
	int hitstun = 2*1000/TIME;

	storage = reaction_crmp;
	storage2 = reaction_crhp;
	PRINTF("%d", storage);
	PIT_Configure_interrupt_mode(0.001);
	//TODO add new table to db, 2 frame link!!!!!!!


	while(1)
	{

		switch(thisstate)//better naming convention
		{
		case READY:

			while(delay/1000>0)
				PRINTF("\r%3d", --delay / 1000);
			thisstate = PROCESS;
			break;
		case PROCESS:
			PRINTF("\n");
			delay=10000;
			storage = reaction_crmp;
			temp2 = reaction_crhp;
			sw3_count=0;
			sw4_count=0;
			run=0;
			while((sw4_count==0))
			{}
			run=1;
			while(storage>0)
			{
				PRINTF("\r        %d", storage);
			}
			run=0;
			sw4_count=0;
			if(storage==0)
			{
				storage = reaction_crmp;
				tick_count=0;
				run=2;
			}
			while((sw4_count==0))
			{}
			run=1;
			reaction = (float)(tick_count-hitstun)/1000;
			int i;
			//strcpy(buffer2[0], reaction);
			//for (i = 0; i < sizeof(float); i++)
				//	PRINTF("%c",buffer2[i]);
			//buffer2[0]=reaction;

			//put_char(buffer2); // add when using GUI
			PRINTF("\n%f", reaction);//CR_MP CR_MP next CR_MP CR_HP

			storage = reaction_crmp;
			//run=1;
			PRINTF("\n");
			while(storage>0)
			{
				PRINTF("\r        %d", storage);
			}
			if(storage==0)
			{
				storage = reaction_crmp;
				tick_count=0;
				run=2;
			}
			sw4_count=0;
			sw3_count=0;

			while((sw3_count==0))
			{}
			run=0;
			reaction = (float)(tick_count-hitstun)/1000;
			PRINTF("\n%f", reaction);//CR_MP CR_MP next CR_MP CR_HP

			thisstate = READY;
			break;




		}

	}


}
void PORTC_PORTD_IRQHandler()
{

	if(PORTC_ISFR & SW3_MASK)
	{
		PORTC_ISFR|= SW3_MASK; //clear interrupt flag for ptc1

		sw_count++;
	}
	if(PORTC_ISFR & SW2_MASK)
	{
		PORTC_ISFR|= SW2_MASK; //clear interrupt flag for ptc1
		sw2_count++;
	}
	if(PORTC_ISFR & SW4_MASK)
	{
		PORTC_ISFR|= SW4_MASK; //clear interrupt flag for ptc1
		sw3_count++;

	}
	if(PORTC_ISFR & SW5_MASK)
	{
		PORTC_ISFR|= SW5_MASK; //clear interrupt flag for ptc1
		sw4_count++;

	}


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

void PIT_IRQHandler()
{
	PIT_TFLG0 = 0X01ul;

	delay--;

	if(run == 1)
	{
		storage--;
	}
	else if(run == 2)
	{
		tick_count++;
	}

}


