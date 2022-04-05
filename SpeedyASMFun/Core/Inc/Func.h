/*
 * Func.h
 *
 *  Created on: Mar 15, 2022
 *      Author: sidbrun
 */

#ifndef FUNC_H_
#define FUNC_H_

#include <inttypes.h>

void bslice(uint32_t *in);					//load data from stack and convert it to bitslice representation, i.e. plaintext and keys

void sbox();								//substitution box in ASM and bitslice

void shiftcolumns();						//Shift columns in ASM with bitslice

void mixcolumns();							//Mix columns in ASM with bitslice

void ark(uint32_t *key);					//Add round key in ASM with bitslice

void lark(uint32_t *key, uint32_t *Ctxt);					//lAST Add round key in ASM with bitslice

void arc(uint32_t *rc);						//Add round constant with bitslice

#endif /* FUNC_H_ */
