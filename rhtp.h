/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   rhtp.h
 * Author: Marko
 *
 * Created on 11. februar 2017, 16:36
 */

#ifndef RHTP_H
#define RHTP_H

#ifdef __cplusplus
extern "C" {
#endif
  
  typedef enum 
  {
	RHTP_OK	= 0,
	RHTP_ERROR	= -1,
	RHTP_CANT_OPEN_UART = -100,	  // ERROR - CAN'T OPEN SERIAL PORT
	RHTP_TX_ERROR = -101,		  // UART TX error
  } RHTP_STATUS_t;

  
  
  RHTP_STATUS_t RHTP_Find_Sensors(char *port,unsigned char *addr, int verbose);
  RHTP_STATUS_t rhtp_cmd(char *port, char *cmd, char *response, int *rxed, int verbose);
  void RHTP_Report_Find_Sensors(char *port,int verbose);


#ifdef __cplusplus
}
#endif

#endif /* RHTP_H */

