/* 
 * File:   main.c
 * Author: s54mtb
 *
 * Created on 2. march 2017, 8:49
 */


/*
 Coding the output for http transfer to the server.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "encdec.h"
#include "bas64.h"


static uint8_t private_key[256];


int ReadPrivateKey(char *filename, int verbose_flag)
{
  FILE *fp;
  size_t len;
  int error = 0;
                
//size_t fwrite(const void *ptr, size_t size_of_elements, size_t number_of_elements, FILE *a_file);

  fp = fopen(filename, "r");
  if (verbose_flag)
  {
	if (fp!=NULL)
	{
	  printf("Key file %s opened.",filename);
	}
	else
	{
	  printf("Error opening key file %s.",filename);
	  error = -1;
	}
  }
  
  if (fp!=NULL) len = fread(private_key, 1, 16, fp);

  fclose(fp);
 
  if (verbose_flag)
  {
	if (len==16)
	{
	  printf("Read %d bytes.",len);
	}
	else
	{
	  printf("Error reading from key file %s. Read length %d bytes.",filename, len);
	  error = -1;
	}
  }
  return error;
}


void encode_result(char *plaintext, char *coded, int verbose_flag)
{
  unsigned char buf[16];
  
  int i,j;
  
  if (verbose_flag)
  {
	printf("Encoding plain text '%s'\n\r",plaintext);
	puts("Using private key:");
	phex(private_key, 16);
	puts("\n\rEncoded:");
  }
  
  for (i=0; i<(strlen(plaintext)/16); i++)
  {
	AES128_ECB_encrypt(plaintext + (i*16), private_key, buf);
	phex(buf, 16);
	for (j=0; j<16; j++) coded[i*16+j] = buf[j];
  }
}

