/*
 * Functions.h
 *
 *  Created on: Nov 1, 2021
 *      Author: Jose A. Bernal
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdbool.h>

typedef bool StateBool[192];
typedef uint8_t StateChar[32];
typedef uint64_t StateUint[3];

StateChar RoundKeys[NR+1];
StateChar RoundCons[NR-1];

void init_buffer(unsigned char *buffer, unsigned long long numbytes);

void print_bstr(const char *label, const unsigned char *data, unsigned long long length);

void serial_out(char *label, uint8_t *dat, int *i, unsigned long long length );

void convert_stateuint_to_statechar(StateUint input, StateChar output);

void convert_statebool_to_statechar(StateBool input, StateChar output);

void print_state(StateChar input);

void prepare_round_cons();

void prepare_round_keys(StateUint Key);

void AK(StateChar input, int r);

void AC(StateChar input, int r);

void SB(StateChar input);

void SBI(StateChar input);

void MC(StateChar input);

void MCI(StateChar input);

void SC(StateChar input);

void SCI(StateChar input);

void Encrypt(StateUint Plaintext, StateUint Key, uint8_t *enc);

void Decrypt(StateUint Ciphertext, StateUint Key, uint8_t *dec);

/* Extra functions for STM M4 serial output	*/

void print_state2(StateChar input, uint8_t *output);

void serial_out2(char *label, uint8_t *dat);

/*End*/

/* ASM functions translate */
void asmSB(uint8_t *input);
/*end*/

#endif /* FUNCTIONS_H_ */
