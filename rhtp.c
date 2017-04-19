/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "rhtp.h"

#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

#include <stdio.h>
#include <string.h>

// prints string as hex
void phex(char* str, int len)
{
    unsigned char i;
    for(i = 0; i < len; ++i)
        printf("%.2x ", (unsigned char)str[i]);
    printf("\n\r");
}

const char *RHTP_UART_PORT =  "/dev/ttyAMA0";
#define RHTP_BAUDRATE		  B9600

RHTP_STATUS_t rhtp_cmd(char *port, char *cmd, char *response, int *rxed, int verbose)
{
  int uart0_filestream = -1;
  struct termios options;
  unsigned char tx_buffer[20];
	
  if (verbose)
  {
	printf("Opening: %s\n\r",port);
  }
  uart0_filestream = open(port, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
  if (verbose)
  {
	printf("Status: %d\n\r", uart0_filestream);
  }
  if (uart0_filestream == -1)
  {
	  //ERROR - CAN'T OPEN SERIAL PORT
	if (verbose)
	{
	  printf("Error - Unable to open UART.  Ensure it is not in use by another application\n\r");
	}
	return RHTP_CANT_OPEN_UART;
  }
  tcgetattr(uart0_filestream, &options);
  options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(uart0_filestream, TCIFLUSH);
  tcsetattr(uart0_filestream, TCSANOW, &options);
  
  if (verbose)
  {
	printf("Sending: \"%s\"",cmd);
  }
  sprintf(tx_buffer, cmd);
  tcdrain(uart0_filestream);
  
  if (uart0_filestream != -1)
	{
		int count = write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));		//Filestream, bytes to write, number of bytes to write
		if (verbose)
		{
		  printf ("Written >>> %d bytes\n\r", count);
		}
		if (count < 0)
		{
		  if (verbose)
		  {
			printf("UART TX error\n\r");
		  }
		  return RHTP_TX_ERROR;
		}		
	}
	
  if (verbose)
  {
	puts("Waiting for response\n\r");
  }
  usleep(1000000);
	
  *rxed = 0;
  //----- CHECK FOR ANY RX BYTES -----
  if (uart0_filestream != -1)
  {
	  // Read up to 255 characters from the port if they are there
	  char rx_buffer[256];
	  int rx_length = 0;
	  
	  if (verbose)
	  {
		printf("Reading bytes from %s\n\r",port);
	  }
	  rx_length = read(uart0_filestream, rx_buffer, 100);		//Filestream, buffer to store in, number of bytes to read (max)
	  if (verbose)
	  {
		printf("Read %d bytes.\n\r", rx_length);
	  }
	  if (rx_length > 0)
	  {
		rx_buffer[rx_length] = 0;
		if (verbose)
		{
		  puts("Rx: ");
		  phex(rx_buffer, rx_length);
		  //printf("Rx: \"%s\" \n\r",rx_buffer); 
		}
		*rxed = rx_length;
		sprintf(response, rx_buffer);
	  }
  }

	
  if (verbose)
  {
	puts("Closing port.\n\r");
  }
  //----- CLOSE THE UART -----
  close(uart0_filestream);
  return RHTP_OK;
}



/**
 Iterate all addresses from 0 to 7 and query response for RHTP sensors
 output: addr is 8 byte array with number > 0  when sensor was found at 
         the address. Value is sensor version times 10
 return value: number of sensors found
 */
int RHTP_Find_Sensors(char *port, unsigned char *addr, int verbose)
{
  int i;
  RHTP_STATUS_t status;
  unsigned char cmd[20], response[20];
  int len, v1, v2, num = 0;
  
  // Ask for ID for all addresses
  for (i=0; i<8; i++)
  {
	if (verbose)
	{
	  printf("Checking sensor #%d\n\r", i);
	}
	snprintf(cmd, 20, "@%1d:ID\n\r",i);
	status = rhtp_cmd(port,cmd, response, &len, verbose);
	if (verbose)
	{
	  printf("Address %d, status: %d", i, status);
	}
	if ((status == RHTP_OK) & (len>0))
	{
	  if (sscanf(response,"RHTP V%1d.%1d",&v1,&v2) == 2)
	  {
	    addr[i] = v1*10+v2;
	    num++;
	  }
	  if (verbose)
	  {
	    printf("Found sensor %s\n\r",response);
	  }
	}
	else
	{
	  addr[i] = 0;
	}
  }
  if (verbose)
  {
	printf("Found total %d sensors.\n\r",num);
  }
  return num;  
}



void RHTP_Report_Find_Sensors(char *port, int verbose)
{
  unsigned char addr;
  RHTP_STATUS_t status = RHTP_Find_Sensors(port, &addr, verbose);
  
  if (verbose)
  {
	puts ("Available sensors:");
	printf("%02x \n", addr);
  }
  
}


