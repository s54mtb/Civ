/* 
 * File:   testing.c
 * Author: s54mtb
 *
 * Created on 27. januar 2017, 8:49
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <time.h>


#define CBC 1
#define ECB 1

#include "encdec.h"

#include "bas64.h"

extern void phex(char* str, int len);


/*
 * 
 */
int testing(int argc, char** argv) {

  /* Encription vars */
  
  int l;
  uint8_t i, buf[64], buf2[64];
  // 128bit key
  
   uint8_t key[16] =  { (uint8_t) 0xcd, (uint8_t) 0xbf, (uint8_t) 0x35, 
                     (uint8_t) 0x18, (uint8_t) 0x9a, (uint8_t) 0xee, 
                     (uint8_t) 0x03, (uint8_t) 0xb6, (uint8_t) 0xcb, 
                     (uint8_t) 0xb7, (uint8_t) 0xa5, (uint8_t) 0x18, 
                     (uint8_t) 0x29, (uint8_t) 0xc3, (uint8_t) 0x44, 
                     (uint8_t) 0xfc };
   
   //GeneratePrivateKey("//home//pi//mojprivatnikljuc.bin", key, 16, 1);

   // 512bit plaintext buffer
    uint8_t plain_text[64] ;

   // 1024bit hex ciphertext request
    uint8_t enc_hex_text[64] ;


//-------------------------
	//----- SETUP USB virtual COM port with RS485 attached -----
	//-------------------------

	int uart0_filestream = -1;
	
	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n\r");
	}
	
	//CONFIGURE THE UART
	// The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

//----- TX BYTES -----
	unsigned char tx_buffer[20];
	unsigned char *p_tx_buffer;
	
	sprintf(tx_buffer, "@7:hdc1080 h\n\r");
	tcdrain(uart0_filestream);
	
	
	if (uart0_filestream != -1)
	{
		int count = write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));		//Filestream, bytes to write, number of bytes to write
		printf ("Written >>> %d bytes\n\r", count);
		if (count < 0)
		{
			printf("UART TX error\n\r");
		}		
	}
	
	usleep(1000000);
	
	//----- CHECK FOR ANY RX BYTES -----
	if (uart0_filestream != -1)
	{
		// Read up to 255 characters from the port if they are there
		char rx_buffer[100];
		int rx_length = 0;
		
		rx_length = read(uart0_filestream, rx_buffer, 100);		//Filestream, buffer to store in, number of bytes to read (max)
		if (rx_length > 0)
		{
 		  rx_buffer[rx_length] = 0;
		  printf("Rx: %s \n\r",rx_buffer);
		  
		  memset(buf, 0, 64);
          memset(buf2, 0, 64);
		  memset(plain_text, 0, 64);
		  
		  snprintf(plain_text,64,"|||Tralala hopsasa|||");
		  
		  printf("ECB encrypt verbose:\n\r\n\r");
		  printf("plain text:\n\r");
		  for(i = (uint8_t) 0; i < (uint8_t) 4; ++i)
		  {
			  phex(plain_text + i * (uint8_t) 16, 16);
		  }
		  printf("\n\r");

		  printf("key:\n\r");
		  phex(key, 16);
		  printf("\n\r");
		  

		  // print the resulting cipher as 4 x 16 byte strings
		  printf("ciphertext:\n\r");
		  for(i = 0; i < 4; ++i)
		  {
			  AES128_ECB_encrypt(plain_text + (i*16), key, buf+(i*16));
			  phex(buf + (i*16), 16);
		  }
		  printf("\n\r");
		  
		  
		  
		  
		  
		}
	}

	

    //----- CLOSE THE UART -----
	close(uart0_filestream);

	
  return (EXIT_SUCCESS);
}


