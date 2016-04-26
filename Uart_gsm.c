//Name: Saoirse Stewart
//Date:18/11/14
//Description: GSM modem interfacing
#include "board.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include <stdlib.h>
#include <string.h>

#define BufferSize 20 //size of circular buffer

//response codes for AT command function
#define SUCCESS 0
#define FAIL -1
#define ERROR -2

#define RDRF_MASK 0x20	//Receive Data Register Full Flag Mask
#define RIE_MASK 0x20	//Receive Interrupt Enable Mask
#define UART_S1_TRDE_MASK 0X80

//Function Prototypes
void enable_UART0_receive_interrupt();
void buffer_init();
char buffer_get();
void buffer_put(char data);
int send_command(char * command_ptr, char * response_ptr, int size, int wait_time);
int send_sms(char * number, char * message);
void printf_response(char * ptr);
void PIT_Configure_interrupt_mode(float time_delay_secs);
void PIT_IRQ();

//Global Variables used for circular buffer
char ch;
char buffer[BufferSize];
char * wr_ptr;
char * rd_ptr;
volatile unsigned long tick_count;
int result = 0;
volatile int CurrentTick;

int main (){

	int x;
	//char * ptr;
	char * AT = "\r\nAT\r\n";
	char * PIN_CHECK = "\r\nAT+CPIN?\r\n";
	char * ENTER_PIN = "\r\nAT+CPIN=\"1234\"\r\n";
	char * CREG = "\r\nAT+CREG?\r\n";
	char response[20];

	char * ptr;
	enum STATES {INIT, CHECK_PIN, SEND_PIN, CHECK_NETWORK_REG, SEND_SMS, CONNECTED};
	enum STATES CurrentState = INIT;

	hardware_init();
	enable_UART0_receive_interrupt();
	buffer_init();
	PIT_Configure_interrupt_mode(1);    //PIT interrupt every second
	enable_UART0_receive_interrupt();

	PRINTF("\nGSM Modem Control\n");
	//Configuration

	while(1)
	{
		switch(CurrentState)
		{
		case INIT:  //Check connection to MODEM by sending AT. Expected response is OK
			PRINTF("Testing Modem Connection\n");
			result = send_command(AT, response, sizeof(response), 10);
			if(result == SUCCESS || result == ERROR)
			{
				printf_response(response);
			}
			if(result == SUCCESS)   //"OK" was returned by MODEM
				CurrentState = CHECK_PIN;
			else    //incorrect response or timeout. Delay and try again
			{
				CurrentTick = tick_count;
				while((tick_count - CurrentTick) < 5)
				{}
			}
			break;

		case CHECK_PIN: //Check if SIM card is ready
			result = send_command(PIN_CHECK, response, sizeof(response), 10);
			if(result == SUCCESS || result == ERROR)
			{
				printf_response(response);
			}
			if(result == SUCCESS)   //"OK" returned, check response string for "READY" or "SIM_PIN"
			{
				if(strstr(response, "READY")) //if ready ok is typed into terminal
				{

					CurrentState = CHECK_NETWORK_REG; //move to check_network_reg
				}
				else if(strstr(response, "SIM_PIN")) // sim_pin ok entered
				{

					CurrentState = SEND_PIN; //move to send_pin
				}
			}
			else
				CurrentState = INIT;
			break;

		case SEND_PIN:  //Send PIN code. "OK" response if PIN is correct
			result = send_command(ENTER_PIN, response, sizeof(response), 10);
			if(result == SUCCESS || result == ERROR)
			{
				printf_response(response);
			}
			if(result==SUCCESS) //if logic '0'
					{
				//if(strstr(response, ""))
				CurrentState = CHECK_NETWORK_REG; //move to check_network_reg
					}
			else
			{
				CurrentState = CHECK_PIN; //if '-1' move back to check_pin again
			}
			break;


		case CHECK_NETWORK_REG: //check if registered on mobile network
			result = send_command(CREG, response, sizeof(response), 20);
			if(result == SUCCESS || result == ERROR)
			{
				printf_response(response);
			}
			if(result==SUCCESS)
			{
				if(strstr(response,"+CREG")) //if +creg entered
						{
					ptr = (char*)strchr(response,':');//point to the colon
					ptr+=4; //move 4 places
					switch(*ptr)//switch contents of address pointed to
					{
					case '1'://if  +creg: 0,1 entered
						CurrentState = INIT;// move back to init
						PRINTF("Invalid;Try again");
						break;
					case '2'://if  +creg: 0,2 entered
						CurrentState = SEND_SMS;//move to send_sms
						PRINTF("Proceed to send SMS");
						break;
					case '3'://if  +creg: 0,3 entered
						CurrentTick = tick_count;//wait for "network"
						while((tick_count - CurrentTick) < 5)
						{}
						PRINTF("Checking for Network, Please Wait");
						break;
					case '4'://if  +creg: 0,4 entered
						CurrentState = INIT;//move back to init
						PRINTF("Invalid,Try Again");
						break;
					case '5': //if  +creg: 0,5 entered
						CurrentState = SEND_SMS;//send sms
						PRINTF("Proceed to send SMS");
						break;
					default:
						CurrentState = INIT;
						PRINTF("Invalid,Try Again");
						break;
					}
						}
				else
				{
					CurrentState = INIT;
				}
				printf_response(response);
				break;

		case SEND_SMS:  //Send a text message
			send_sms("\"0871234567\"","\"Testing 123\""); //send number

			send_sms("\"0171112233\"","\"Success\""); // the message
			if(result==SUCCESS) // if logic '0'
			{
				PRINTF("Message has been sent");
				CurrentState = CONNECTED;//move to connected
				break;
			}
			else
			{
				CurrentTick = tick_count;// wait for input
				while((tick_count - CurrentTick) < 5)
				{}
				break;
			}
		case CONNECTED:
			while(1)    //dummy loop
			{}
			break;
		default:
			break;
			}//end switch-case
		}//end while(1)
	}
}
/**********************************************************************
Function to initialise the circular buffer to 0's
 ***********************************************************************/
void buffer_init()
{
	char x;
	rd_ptr = buffer;
	wr_ptr = buffer;
	for(x=0; x<BufferSize; x++)
		buffer[x]=0;
}

/***********************************************************************************
Function to write a character to the circular buffer
If the buffer is full, old data is overwritten and the read pointer is incremented
 ************************************************************************************/
void buffer_put(char data)
{
	*wr_ptr = data;
	if(wr_ptr == &buffer[BufferSize-1])
		wr_ptr = buffer;
	else
		wr_ptr++;

	if(wr_ptr == rd_ptr)
	{
		if(++rd_ptr == &buffer[BufferSize-1])
			rd_ptr = buffer;
	}
}

/***********************************************************************************
Function to read a character from the circular buffer
Returns character or 0 if the buffer is empty
 ************************************************************************************/
char buffer_get()
{
	char ch;
	if(rd_ptr != wr_ptr)
	{
		ch = *rd_ptr;
		if(rd_ptr == &buffer[BufferSize-1])
			rd_ptr = buffer;
		else
			rd_ptr++;
		return ch;
	}
	else
	{
		//PRINTF("\nBuffer Empty");
		return 0;
	}
}




/******************************************************************************************
 * Function to send a command to a GSM modem and to check response
 *
 * Passed Parameters:
 *  Pointer to command string
 *  Pointer to response buffer
 *  Size of response buffer
 *  Modem timeout time in seconds
 *
 * Return Value:
 *  0: OK response received from modem
 *  -1: ERROR: response received from MODEM
 *  -2: Modem timed out
 *******************************************************************************************/
int send_command(char * command_ptr, char * response_ptr, int size, int wait_time)
{
	volatile int time;
	char CR_count=0;
	int x, termination_char_received = 0, command_received=0;
	char c;
	char buff[20];
	char rx_count = 0;
	char *temp_ptr = response_ptr;
	PRINTF("HERE");

	for(x=0;x<size;x++) //clear response buffer
		*temp_ptr++ = 0;
	for(x=0;x<20;x++)
		buff[x] = 0;
	//send command to serial port
	while(*command_ptr != '\0')
	{
		PUTCHAR(*command_ptr++);
	}
	//wait for a command response or a timeout
	time = tick_count;
	x=0;
	while(!command_received &&(tick_count - time < wait_time))
	{
		c = buffer_get();
		if(c)   //buffer not empty
		{
			*response_ptr++ = c;
			rx_count++;
			if(!termination_char_received)
			{
				if(strstr(response_ptr - rx_count, "OK") || strstr(response_ptr - rx_count, "ERROR:"))
				{
					PRINTF("\nOK or ERROR received\n");
					termination_char_received = 1;
				}
			}
			else if(c == '\r')
			{
				command_received = 1;
				//printf("\nResponse Received:  \n");
			}
		}
	}
	if(command_received)
	{
		if(strstr(response_ptr - rx_count, "OK"))
			return SUCCESS;
		else if(strstr(response_ptr - rx_count, "ERROR:"))
			return ERROR;
	}
	else
	{
		PRINTF("\nTimeout on serial response\n");
		return FAIL;
	}

}

/*---------------------------------------------------------------------------
 * Function to send an SMS text message
 *
 * The function is passed 2 strings - the phone number and the text message
 * Text message must be terminated with Control Z (0x1A)
 * e.g. send_sms("\"+353876477260\"","\"this is a test\"");
 *
 */

int send_sms(char * number, char * message)
{
	char sms_send_command[40] = "AT+CMGS=";
	char * ptr;
	volatile char time = 0;
	char response_received = 0;
	char ch_received=0;
	char c;
	char response[20];

	strcat(sms_send_command, number);//add number to the sms_command
	strcat(sms_send_command, "\r");

	ptr = sms_send_command;
	while(*ptr!='\0')
	{
		PUTCHAR(*ptr++); //wait for the ok
	}

	CurrentTick = tick_count;
	ch_received =0;
	while((ch_received)==0&&(tick_count - CurrentTick) < 5)// while not timeout
	{
		c=buffer_get();//get character for buffer
		if(c) //if there is character
			ch_received = 1;//character has been recieved
	}
	if((tick_count - CurrentTick) >= 5) //timeout
	{
		PRINTF("\nTimeout on serial response\n");
		return FAIL;
	}
	else
	{
		if(c == '>')//if character is a +
		{
			memset(sms_send_command,0,40); // clear buffer

			strcat(sms_send_command, message); //concatenate the message, to the sms_command
			strcat(sms_send_command, "\r\n");
			strcat(sms_send_command, "\x1A");//add ctrlz to string

			result = send_command(sms_send_command, response, sizeof(response), 20);
			return SUCCESS;
		}
		else
		{
			PRINTF("Failed to send the message");
			return FAIL;
		}
	}


}

/*********************************************************************************************************
 * Function to print a modem response to the serial terminal
 * This function is used instead of printf because the PE Micro terminal only sends \r for a keyboard entry
 * The function adds a \n to every \r received
 ************************************************************************************************************/
void printf_response(char * ptr)
{
	while(*ptr != '\0')
	{
		PUTCHAR(*ptr);
		if(*ptr++ == '\r')
			PUTCHAR('\n');
	}
}

/****************************************************************************
 * Function to enable the UART5 interrupt
 * Only the receive buffer full interrupt is enabled
 */
void enable_UART0_receive_interrupt()
{
	//Configure NVIC
	NVIC_ClearPendingIRQ(12);
	NVIC_EnableIRQ(12);
	UART0_C2 |= RIE_MASK;	//set RIE to enable receive interrupt
}

//UART5 Receive ISR
//Receives a character and writes it to response buffer
void UART0_IRQHandler(void)
{
	if(UART0_S1 & RDRF_MASK)
	{
		ch = UART0_D;   //read character. This clears RDRF flag
		PUTCHAR(UART0_D); //echo character
		buffer_put(ch);
	}
}

/*************************************************************************
 * PIT ISR
 * Increments a tick count every time the counter reaches 0
 *************************************************************************/
void PIT_IRQ()
{
	PIT_TFLG0=0x01ul;
	PIT_TCTRL0 |= 0x03ul;
	tick_count++;
}



