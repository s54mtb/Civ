/* 
 * File:   main.c
 * Author: s54mtb
 *
 * Created on 27. January 2017, 8:49
 */

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "rhtp.h"
#include <stdio.h>
#include "bas64.h"

extern int testing(int argc, char** argv);
//extern void RHTP_get_readout(char *port, char *cmd, char *response, int *rxed, int verbose);

void print_usage() {
    printf("Usage: rhtp [vb] [nf] [e] -p port -a address -c cmd -k keyfile -A apikey\n");
}


/*
 * 
 */
int main(int argc, char** argv) {
  
  char response[256];
  int length;
  //char port[] = "/dev/ttyAMA0";
  
  
  //RHTP_Report_Find_Sensors(port, 1);
  //rhtp_cmd(port,"@7:ID\n\r", response, &length, 1);  
  //printf("Sprejeto.......%s", response);
  //rhtp_cmd(port,"@7:hdc1080 h\n\r", response, &length, 1);  
  //printf("Sprejeto.......%s", response);
  
  int opt= 0;
  int verbose_flag = 0, numeric_flag = 0, encode_flag = 0;
  char port[256];
  int address, len;
  char cmd[256];
  char fullcmd[256];
  char keyfile[1024];
  char *ptr;
  unsigned char buf[1024];
  
  /* Init parameters */
  cmd[0] = 0;
  port[0] = 0;
  address = 0xff;

  //Specifying the expected options
   //The two options l and b expect numbers as argument
   static struct option long_options[] = {
	   {"verbose",   no_argument,       0,  'v' },
	   {"brief",	 no_argument,       0,  'b' },
	   {"numeric",	 no_argument,       0,  'n' },
	   {"full",		 no_argument,       0,  'f' },
	   {"encode",	 no_argument,       0,  'e' },
	   {"keyfile",   required_argument, 0,  'k' },
	   {"port",		 required_argument, 0,  'p' },
	   {"address",   required_argument, 0,  'a' },
	   {"cmd",       required_argument, 0,  'c' },
	   {0,           0,                 0,  0   }
   };

   int long_index =0;
   while ((opt = getopt_long(argc, argv,"vbnfek:p:a:c:", 
				  long_options, &long_index )) != -1) {
	switch (opt) {
		 case 'v' : verbose_flag = 1;
			 break;
		 case 'b' : verbose_flag = 0;
			 break;
		 case 'n' : numeric_flag = 1;
			 break;
		 case 'f' : numeric_flag = 0;
			 break;
		 case 'e' : encode_flag = 1;
			 break;
		 case 'a' : address = atoi(optarg); 
			 break;
		 case 'p' : snprintf(port, 255, "%s", optarg);
			 break;
		 case 'k' : snprintf(keyfile, 1023, "%s", optarg);
			 break;
		 case 'c' : snprintf(cmd, 255, "%s", optarg);
			 break;
		 default: print_usage(); 
			 exit(EXIT_FAILURE);
	}
  }
  
  if (verbose_flag)
  {
	puts("Input parameters....:\n\r"); 
	printf("Address: %d \n\r", address);
	printf("Port: %s\n\r", port);
	printf("Cmd: %s\n\r", cmd);
	printf("Flags: V=%d, N=%d, encode=%d\n\r",verbose_flag, numeric_flag, encode_flag);
	if (encode_flag)
	{
	  printf("Keyfile: %s", keyfile);
	}
  }
   
  if (strlen(port)<2)
  {
	 puts("Invalid port.\n\r");
	 print_usage(); 
	 exit(EXIT_FAILURE);
  }
   
  if (strlen(cmd)<2)
  {
	 puts("Invalid command.\n\r");
	 print_usage(); 
	 exit(EXIT_FAILURE);
  }
   
  if (address>7)
  {
	puts("Invalid address.\n\r");
	 print_usage(); 
	 exit(EXIT_FAILURE);
  }
   
   if (encode_flag)
   {
	 if (strlen(keyfile)<2)
	 {
	   puts("Invalid keyfile.\n\r");
	   print_usage();
	   exit(EXIT_FAILURE);
	 }
	 /* Read key from file */
	 ReadPrivateKey(keyfile, verbose_flag);
   }
  
  snprintf(fullcmd, 256, "@%1d:%s\n\r", address, cmd);
  rhtp_cmd(port, fullcmd, response, &length, verbose_flag);
  
  if (encode_flag)
  {
	if (numeric_flag)
	{
	  strtok_r (response, "=", &ptr);
	  encode_result(ptr, buf);
	  puts(base64_encode(buf, strlen(ptr), &len));	
	}
	else
	{
	  puts(response);
	}
	
  }
  else
  {
	if (numeric_flag)
	{
	  strtok_r (response, "=", &ptr);
	  puts(ptr);	
	}
	else
	{
	  puts(response);
	}
	
  }
    
  return (EXIT_SUCCESS);
}


