/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   koder.h
 * Author: Marko
 *
 * Created on 05. marec 2017, 22:18
 */

#ifndef KODER_H
#define KODER_H

#ifdef __cplusplus
extern "C" {
#endif


int ReadPrivateKey(char *filename, int verbose_flag);
void encode_result(char *plaintext, char *coded, int verbose_flag);

#ifdef __cplusplus
}
#endif

#endif /* KODER_H */

