/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bas64.h
 * Author: Marko
 *
 * Created on 21. februar 2017, 23:23
 */

#ifndef BAS64_H
#define BAS64_H

#ifdef __cplusplus
extern "C" {
#endif

char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);


#ifdef __cplusplus
}
#endif

#endif /* BAS64_H */

