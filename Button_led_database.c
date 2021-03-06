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
#include <stdlib.h>
#include <math.h>

#define BUFFERSIZE 20
#define RDRF_MASK 0x20	//Receive Data Register Full Flag Mask
#define RIE_MASK 0x20	//Receive Interrupt Enable Mask
#define UART_S1_TRDE_MASK 0X80
#define TIME 60
void myPrintf(float fVl);
char combo1[] = "CR_MP CR_MP CR_HP";
char combo2[] = "CR_LP ST_LP CL_LP";
char combo3[] = "CR_LP CR_MP CL_LP";


static int cr_mp[2];
static int cr_hp[2];

static int cr_lp[2];
static int s_lp[2];
static int hu_hk[2];

static int reaction_crmp=0;
static int reaction_crhp=0;

static int reaction_crlp=0;
static int reaction_crhp2=0;
static int reaction_huhk=0;

volatile int storage=0;

static int sw2_count=0;
static int sw_count=0;
static int sw3_count=0;
static int sw4_count=0;
static int sw5_count=0;
volatile int sw6_count=0;
static int sw7_count=0;
static int sw8_count=0;
static int sw9_count=0;

volatile int hit =0;
float frames;
int run =0;
volatile int delay = 10000;
int failure_flag=0;


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
double database_extraction();
double char_int_conversion(char c[], int, int);
double reaction_time(int,int);
double reaction_time2(int,int);
double reaction_time3(int,int);
void PIT_IRQHandler();
void FloatToStringNew(char *str, float f, char size);
int state =0;

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
	FRDM_KL26Z_SW6_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW7_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW8_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW9_Configure(0,FALLING_EDGE);
	FRDM_KL26Z_SW10_Configure(0,FALLING_EDGE);


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
			PRINTF("hello");
			while(sw_count==0)
			{}
			LED_set(RED,ON);
			for(int i=0;i<2000000;i++);
			LED_set(RED,OFF);
			sw_count=0;
			currentstate = FIRST;
			break;
		case FIRST:
			sw_count=0;
			sw2_count=0;
			sw5_count=0;
			while(sw2_count==0)
			{}

			LED_set(GREEN,ON);
			for(int i=0;i<2000000;i++);
			LED_set(GREEN,OFF);
			for(int i=0;i<20000;i++);
			tx_string("Start"); // add when using GUI

			//for(int i=0;i<20000;i++);
			currentstate = SECOND;
			break;
		case SECOND:
			sw_count=0;
			sw2_count=0;
			sw5_count=0;
			sw8_count=0;
			sw6_count=0;
			sw4_count=0;

			//tx_string("Start"); // add when using GUI
			if(sw2_count%2!=0)
			{
				tx_string("combo");
				state = database_extraction(combo1);
				PRINTF("%d",state);
				if (state==20)
				{
					currentstate = START;
					break;
				}

			}

			else if(sw8_count%2!=0)
			{
				tx_string("highL");
				sw2_count=0;
				sw8_count=0;
				//while(sw2_count==0)
				//{}
				PRINTF("sw\n");
				///tx_string("choic");
				while(sw8_count==0)
				{}
				//PRINTF("sw2\n");
				tx_string("choc2");
				sw2_count=0;
				sw8_count=0;
				break;

			}
			if(sw6_count%2!=0)
			{
				tx_string("combo");
				state = database_extraction(combo2);
				PRINTF("%d",state);
				if (state==20)
				{
					currentstate = START;
					break;
				}
				//sw8_count==0;
				sw6_count=0;
			}
			else if(sw_count%2!=0)
			{
				tx_string("clear");
				currentstate = SECOND;
				sw_count=0;
				break;
			}
			else if(sw5_count%2!=0)
			{
				tx_string("bread");
				//database_extraction(combo3);


			}
			else if(sw4_count%2!=0)
			{
				tx_string("combo");
				state = database_extraction(combo3);
				PRINTF("%d",state);
				if (state==20)
				{
					currentstate = START;
					break;
				}

			}


			break;
		}

	}
	return 0;
}

double database_extraction(char ch[])
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
			int state=0;
			state=char_int_conversion(ch, reaction_crmp, reaction_crhp);
			if(state ==20)
				return state;
			//PRINTF("%d",state);


		}
	}
}

double char_int_conversion(char c[], int reaction_crmp, int reaction_crhp)
{


	int i;
	int n=0;
	int recovery_crhp;

	if(strcmp(c,combo1)==0)//compare both the strings
	{
		cr_mp[0] = received_char[0]-'0';//startup//4
		cr_mp[1] = received_char[2]-'0';//active//4
		cr_mp[2] = received_char[5]-'0';//recovery //17
		cr_mp[2] += 1 ;//recovery //17

		for(i=0;i<3;i++)
		{
			reaction_crmp = reaction_crmp + cr_mp[i];
			//reaction_crhp = reaction_crhp +
		}
		reaction_crmp=(reaction_crmp*1000)/60;
		//PRINTF("%d",reaction_crmp);

		cr_hp[0] = received_char[1]-'0';//startup//4
		cr_hp[1] = received_char[3]-'0';//active//8
		cr_hp[2] = received_char[6]-'0';//recovery//20
		cr_hp[2] *=10;


		for(i=0;i<3;i++)
		{
			reaction_crhp = reaction_crhp + cr_hp[i];
		}
		reaction_crhp=reaction_crhp*1000/60;
		int state=0;
		state = reaction_time(reaction_crmp,reaction_crhp);
		if (state ==20)
			return state;


	}
	else if(strcmp(c,combo2)==0)//compare both the strings
	{
		cr_lp[0] = received_char[0]-'0';//startup//4
		cr_lp[1] = received_char[3]-'0';//active//4
		cr_lp[2] = received_char[6]-'0';//recovery //17

		for(i=0;i<3;i++)
		{

			reaction_crlp  += cr_lp[i];
		}

		reaction_crlp=(reaction_crlp*1000)/60;


		s_lp[0] = received_char[1]-'0';//startup//4
		s_lp[1] = received_char[4]-'0';//active//8
		s_lp[2] = received_char[7]-'0';//recovery//20

		for(i=0;i<3;i++)
		{


			reaction_crhp2 +=  s_lp[i];
			PRINTF("%d", s_lp[i]);

		}
		PRINTF("%d", reaction_crhp2);

		reaction_crhp2=(reaction_crhp2*1000)/60;
		double state;

		state = reaction_time2(reaction_crmp,reaction_crhp);
		if (state ==20)
			return state;



	}
	else if(strcmp(c,combo3)==0)//compare both the strings
	{
		cr_mp[0] = received_char[1]-'0';//startup//4
		cr_mp[1] = received_char[5]-'0';//active//4
		cr_mp[2] = received_char[6]-'0';//recovery //17
		cr_mp[2] +=2;

		for(i=0;i<3;i++)
		{
			reaction_crmp = reaction_crmp + cr_mp[i];
			//reaction_crhp = reaction_crhp +
		}
		PRINTF("%d",reaction_crmp);
		reaction_crmp=(reaction_crmp*1000)/60;
		cr_lp[0] = received_char[0]-'0';//startup//4
		cr_lp[1] = received_char[3]-'0';//active//4
		cr_lp[2] = received_char[6]-'0';//recovery //17

		for(i=0;i<3;i++)
		{

			reaction_crlp  += cr_lp[i];
		}
		PRINTF("%d",reaction_crlp);
		reaction_crlp=(reaction_crlp*1000)/60;

		state = reaction_time3(reaction_crmp,reaction_crhp);
		if (state ==20)
			return state;
	}


}
//TODO FIX TIMINGS
double reaction_time(int reaction_crmp, int reaction_crhp)
{
	PRINTF("Beginning reaction Calculation\n");
	storage = reaction_crmp;
	PRINTF("%d", storage);
	int count =0;

	char cr_mp_time[50]={0};
	char cr_mp_frame[50]={0};
	char cr_hp_time[50]={0};
	char cr_hp_frame[50]={0};

	char output[100]={0};

	volatile int temp=0;
	PIT_Configure_interrupt_mode(0.001);

	while(count<=5)
	{

		switch(thisstate)//better naming convention
		{
		case READY:
			while(delay/1000>0)
				PRINTF("\r%3d", --delay / 1000);
			thisstate = PROCESS;
			break;
		case PROCESS:


			cr_mp_time[50] = 0;
			cr_mp_frame[50] = 0;
			cr_hp_time[50] = 0;
			cr_hp_frame[50]= 0;

			sw4_count =0;
			sw3_count=0;
			run=0;
			while(sw4_count==0)
			{}

			if(sw9_count>0)
			{
				sw9_count=0;
				thisstate=PROCESS;
				break;
			}

			run =1;
			hit=0;
			while(storage>=0)
			{
				PRINTF("\r%d",storage);
			}
			PRINTF("\n");
			run=2;
			sw4_count =0;
			sw3_count=0;
			while(sw4_count==0)
			{}
			if(sw9_count>0)
			{
				sw9_count=0;
				thisstate=PROCESS;
				break;
			}
			run=0;
			hit = hit-120;

			sprintf(cr_mp_time,"%d", hit);

			frames = (float)hit*60/1000;
			PRINTF("\n MSec: %d, frames: %.2f",hit,frames);
			temp = frames;
			float f2 = frames - temp;
			f2 = fabs(f2);
			int d2 = (int)(f2*10000);

			sprintf(cr_mp_frame,"%d.%02d\n",temp,d2);

			hit =0;
			storage = reaction_crmp;
			run=1;
			while(storage >= 0)
			{
				PRINTF("\r%d",storage);
			}

			run=2;
			sw4_count =0;
			sw3_count=0;
			while(sw3_count==0)
			{}
			run=0;
			hit=hit-137;

			sprintf(cr_hp_time,"%d",hit);

			frames=(float)hit*60/1000;
			PRINTF("\n MSec: %d, frames: %.2f",hit,frames);
			temp = frames;
			float f3 = frames - temp;
			f3 = fabs(f3);
			int d3 = (int)(f3*10000);

			sprintf(cr_hp_frame,"%d.%02d\n",temp,d3);


			hit=0;
			storage = reaction_crmp;
			sw3_count=0;
			sw4_count=0;

			if(sw9_count>0)
			{
				sw9_count=0;
				thisstate=PROCESS;
				break;
			}

			thisstate = RETURN;
			break;

		case RETURN:

			strcat(output,cr_mp_time);
			strcat(output,",");
			strcat(output,cr_hp_time);
			strcat(output,",");
			strcat(output,cr_mp_frame);
			strcat(output,",");
			strcat(output,cr_hp_frame);
			strcat(output,",");
			strcat(output,"cr_mp_cr_mp_cr_hk");

			//PRINTF("\n%s",output);
			tx_string(output);

			memset(output, 0, sizeof(output));

			count+=1;
			thisstate = PROCESS;
			break;
		}


	}
	PRINTF("here");
	while(1)
	{
		sw5_count=0;
		sw2_count=0;
		sw8_count=0;
		sw6_count=0;

		if(sw2_count%2!=0)
		{
			PRINTF("here");
			tx_string("ye");
			reaction_time(reaction_crmp, reaction_crhp);
			sw2_count=0;
			sw8_count=0;
			sw6_count=0;
		}
		else if(sw8_count%2!=0)
		{
			sw8_count=0;
			tx_string("hi");
			while(sw2_count==0){}
			tx_string("no");
			sw8_count=0;
			sw2_count=0;
			sw6_count=0;
			while(sw6_count==0)
			{}
			tx_string("st");
			break;
		}
		//	else if(sw6_count%2!=0)
		//{
		//tx_string("st");
		//break;
		//}

		sw6_count=0;
		sw5_count=0;
		sw2_count=0;
		sw8_count=0;

	}
	PRINTF("brak");
	int state = 20;
	return state;


}
double reaction_time2(int reaction_crlp, int reaction_crhp2)
{
	//FIX
	PRINTF("Beginning reaction Calculation\n");
	storage = reaction_crlp;
	PRINTF("%d",storage);

	volatile int temp;
	volatile int temp2;
	float reaction;
	float reaction2;
	float frames;
	int count =0;

	char cr_lp_time[50]={0};
	char cr_lp_frame[50]={0};
	char s_lp_time[50]={0};
	char s_lp_frame[50]={0};

	char output[100]={0};

	PIT_Configure_interrupt_mode(0.001);

	while(count<=5)
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

			cr_lp_time[50] = 0;
			cr_lp_frame[50] = 0;
			s_lp_time[50] = 0;
			s_lp_frame[50]= 0;

			sw6_count=0;
			sw7_count=0;
			run=0;
			while((sw6_count==0))
			{}

			run=1;
			hit=0;
			while(storage>=0)
			{
				PRINTF("\r%d", storage);
			}
			PRINTF("\n");
			sw6_count=0;
			sw7_count=0;
			run=2;
			while(sw6_count==0)
			{}
			run=0;
			hit = hit-137;
			PRINTF("%d",hit);

			sprintf(cr_lp_time,"%d", hit);

			frames = (float)hit*60/1000;

			PRINTF("\n MSec: %d, frames: %.2f",hit,frames);
			temp = frames;
			float f2 = frames - temp;
			f2 = fabs(f2);
			int d2 = (int)(f2*10000);

			sprintf(cr_lp_frame,"%d.%02d\n",temp,d2);

			hit=0;
			storage = reaction_crlp;
			sw6_count=0;
			sw7_count=0;

			thisstate = RETURN;

			break;

		case RETURN:
			sw_count=0;
			sw2_count=0;
			sw3_count=0;
			sw6_count=0;
			sw7_count=0;

			LED_set(GREEN,ON);
			for(int i=0;i<2000000;i++);
			LED_set(GREEN,OFF);

			strcat(output,cr_lp_time);
			strcat(output,",");
			strcat(output,"0");
			strcat(output,",");
			strcat(output,cr_lp_frame);
			strcat(output,",");
			strcat(output,"0");
			strcat(output,",");
			strcat(output,"cr_lp_s_lp_cr_oks");
			tx_string(output);

			memset(output, 0, sizeof(output));

			count+=1;
			hit = 0;
			run =0;

			delay =10000;
			thisstate = PROCESS;
			break;
		}

	}
	PRINTF("here");
	while(1)
	{
		sw5_count=0;
		sw2_count=0;
		sw8_count=0;
		sw6_count=0;

		if(sw2_count%2!=0)
		{
			PRINTF("here");
			tx_string("ye");
			reaction_time2(reaction_crmp, reaction_crhp);
			sw2_count=0;
			sw8_count=0;
			sw6_count=0;
		}
		else if(sw8_count%2!=0)
		{
			sw8_count=0;
			tx_string("hi");
			while(sw2_count==0){}
			tx_string("no");
			sw8_count=0;
			sw2_count=0;
			sw6_count=0;
			while(sw6_count==0)
			{}
			tx_string("st");
			break;
		}
		//	else if(sw6_count%2!=0)
		//{
		//tx_string("st");
		//break;
		//}

		sw6_count=0;
		sw5_count=0;
		sw2_count=0;
		sw8_count=0;

	}
	PRINTF("brak");
	int state = 20;
	return state;



}
double reaction_time3(int reaction_crlp, int reaction_crmp2)
{
	//FIX
	PRINTF("Beginning reaction Calculation\n");
	storage = reaction_crlp;
	PRINTF("%d",storage);

	volatile int temp;
	volatile int temp2;
	float reaction;
	float reaction2;
	float frames;
	int count =0;

	char cr_lp_time[50]={0};
	char cr_lp_frame[50]={0};
	char s_lp_time[50]={0};
	char s_lp_frame[50]={0};

	char output[100]={0};

	PIT_Configure_interrupt_mode(0.001);

	while(count<=5)
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

			cr_lp_time[50] = 0;
			cr_lp_frame[50] = 0;
			s_lp_time[50] = 0;
			s_lp_frame[50]= 0;

			sw6_count=0;
			sw4_count=0;
			run=0;
			while((sw6_count==0))
			{}

			run=1;
			hit=0;
			while(storage>=0)
			{
				PRINTF("\r%d", storage);
			}
			PRINTF("\n");
			sw6_count=0;
			sw4_count=0;
			run=2;
			while(sw4_count==0)
			{}
			run=0;
			hit = hit-50;
			PRINTF("%d",hit);

			sprintf(cr_lp_time,"%d", hit);

			frames = (float)hit*60/1000;

			PRINTF("\n MSec: %d, frames: %.2f",hit,frames);
			temp = frames;
			float f2 = frames - temp;
			f2 = fabs(f2);
			int d2 = (int)(f2*10000);

			sprintf(cr_lp_frame,"%d.%02d\n",temp,d2);

			hit=0;
			storage = reaction_crlp;
			sw6_count=0;
			sw4_count=0;

			thisstate = RETURN;

			break;

		case RETURN:
			sw_count=0;
			sw2_count=0;
			sw3_count=0;
			sw6_count=0;
			sw7_count=0;

			strcat(output,cr_lp_time);
			strcat(output,",");
			strcat(output,"0");
			strcat(output,",");
			strcat(output,cr_lp_frame);
			strcat(output,",");
			strcat(output,"0");
			strcat(output,",");
			strcat(output,"cr_lp_s_lp_cr_oks");
			tx_string(output);

			memset(output, 0, sizeof(output));

			count+=1;
			hit = 0;
			run =0;

			delay =10000;
			thisstate = READY;
			break;
		}

	}
	PRINTF("here");
	while(1)
	{
		sw5_count=0;
		sw2_count=0;
		sw8_count=0;
		sw6_count=0;

		if(sw2_count%2!=0)
		{
			PRINTF("here");
			tx_string("ye");
			reaction_time3(reaction_crmp, reaction_crhp);
			sw2_count=0;
			sw8_count=0;
			sw6_count=0;
		}
		else if(sw8_count%2!=0)
		{
			sw8_count=0;
			tx_string("hi");
			while(sw2_count==0){}
			tx_string("no");
			sw8_count=0;
			sw2_count=0;
			sw6_count=0;
			while(sw6_count==0)
			{}
			tx_string("st");
			break;
		}
		//	else if(sw6_count%2!=0)
		//{
		//tx_string("st");
		//break;
		//}

		sw6_count=0;
		sw5_count=0;
		sw2_count=0;
		sw8_count=0;

	}
	PRINTF("brak");
	int state = 20;
	return state;

}



void PORTC_PORTD_IRQHandler()
{

	if(PORTC_ISFR & SW3_MASK)
	{
		PORTC_ISFR|= SW3_MASK; //clear interrupt flag for ptc1

		sw_count++;

	}
	if(PORTC_ISFR & SW2_MASK)//a
	{
		PORTC_ISFR|= SW2_MASK; //clear interrupt flag for ptc1
		sw2_count++;

	}
	if(PORTC_ISFR & SW4_MASK)//cr_hk
	{
		PORTC_ISFR|= SW4_MASK; //clear interrupt flag for ptc1
		sw3_count++;

	}
	if(PORTC_ISFR & SW5_MASK)//cr_mp
	{
		PORTC_ISFR|= SW5_MASK; //clear interrupt flag for ptc1
		sw4_count++;

	}
	if(PORTD_ISFR & SW6_MASK)
	{
		PORTD_ISFR|= SW6_MASK; //clear interrupt flag for ptc1
		sw5_count++;

	}
	if(PORTD_ISFR & SW7_MASK)//cr_lp
	{
		PORTD_ISFR|= SW7_MASK; //clear interrupt flag for ptc1
		sw6_count++; //x

	}
	if(PORTD_ISFR & SW8_MASK)
	{
		PORTD_ISFR|= SW8_MASK; //clear interrupt flag for ptc1
		sw7_count++;

	}
	if(PORTD_ISFR & SW9_MASK)
	{
		PORTD_ISFR|= SW9_MASK; //clear interrupt flag for ptc1
		sw8_count++;

	}
	if(PORTD_ISFR & SW10_MASK)
	{
		PORTD_ISFR|= SW10_MASK; //clear interrupt flag for ptc1
		sw9_count++;
		LED_set(GREEN,ON);
		for(int i=0;i<2000000;i++);
		LED_set(GREEN,OFF);

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

	//delay--;

	if(run==1)
	{
		storage--;
	}
	else if(run==2)
	{
		hit++;
		//tick_count++;
	}


}


