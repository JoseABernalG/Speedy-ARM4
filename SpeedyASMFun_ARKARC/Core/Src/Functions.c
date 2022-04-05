/*
 * Functions.c
 *
 *  Created on: Mar 15, 2022
 *      Author: sidbrun
 */



#include "Functions.h"
#include <inttypes.h>


void bslice(uint32_t *in){	//load data from stack and convert it to bitslice representation, i.e. plaintext and keys
	asm(	/*Load plaintext from stack to registers*/
			"mov r9, r0;"			//save input address
			"ldr r0, [r9];"			//load the first 32 bits i.e. Plaintext[0]
			"ldr r1, [r9, #4];"		//load Plaintext[1]
			"ldr r2, [r9, #8];"		//load Plaintext[2]
			"ldr r3, [r9, #12];"	//load Plaintext[3]
			"ldr r4, [r9, #16];"	//load Plaintext[4]
			"ldr r5, [r9, #20];"	//load Plaintext[5]

			"mov  r12, 0x0000;"
			"movt r12, 0xFC00;"


			//  here is to change b10 b11 b12 b13 b14 b15 for b60 b61 b62 b63 b64 b65
			"eor r6, r0, r1, lsr #2;"	// r6 <= r0 xor (r1 >> 2)
			"and r6, r6, r12, lsr #6;"	// r6 <= (r0 xor (r1 >> 2)) ^ 0x03F  mask

			"eor r0, r0, r6;"			// r0 <= r0 xor r6
			"eor r1, r1, r6, lsl #2;"	// r1 <= r1 xor (r6 << 2)

			// end of swap move for block 1 with block 6
			//	swap b120b121b122b123b124b125 with b20b21b22b23b24b25

			"eor r6, r2, r0, lsl #4;"	// r2 xor (r0 << 4) align blk2 with blk12 and performs xor operation
			"and r6, r6, r12, lsr #8;"	// r6 = (r2 xor (r0 << 4)) ^ r12	r2 => 00FC0000

			"eor r2, r2, r6;"			// r2 = r2 xor T
			"eor r0, r0, r6, lsr #4;"	// r0 = r0 xor (T >> 4)

			// end swap
			//swap b30b31b32b33b34b35 in r0 with b180b181b182b183b184b185 in r3

			"eor r6, r3, r0, lsl #6;"	// r3 xor (r0 << 6) align blk3 with blk18 and performs xor operation
			"and r6, r6, r12, lsr #12;" // r6 = (r3 xor (r0 << 6)) ^ r12	r12 => 000FC000

			"eor r3, r3, r6;"			// r3 = r3 xor T
			"eor r0, r0, r6, lsr #6;"	// r0 = r0 xor (T >> 6)

			//end swap
			//swap b40b41b42b43b44b45 in r0 with b240b241b242b243b244b25 in r4

			"eor r6, r4, r0, lsl #8;"	// r4 xor (r0 << 8) align blk4 with blk 24 and performs xor operation
			"and r6, r6, r12, lsr #16;"	// r6 = (r4 xor (r0 << 8)) ^ r12		r12 => 0000FC00

			"eor r4, r4, r6;"			// r4 = r4 xor T
			"eor r0, r0, r6, lsr #8;"	// r0 = r0 xor (T >> 8)
			//end swap
			//rotate register r1, r2, r3, r4 to align block b0,b1,b2,b3,b4
			"ror r1, r1, #28;"	// rotate register r1 to align the bits b10b11b12b13b14b15 with b00b01b02b03b04b05
			"ror r2, r2, #24;"	// rotate register r1 to align the bits b20b21b22b23b24b25 with b10b11b12b13b14b15
			"ror r3, r3, #20;"	// rotate register r1 to align the bits b30b31b32b33b34b35 with b20b21b22b23b24b25
			"ror r4, r4, #16;"	// rotate register r1 to align the bits b40b41b42b43b44b45 with b30b31b32b33b34b35
			//End rotation
			//	swap blocks
			//swap b80b81b82b83b84b85 in r1 with b130b131b132b133b134b135 in r2
			"eor r6, r2, r1, lsl #6;"	// r2 xor (r1 << 6) align blk8 with blk13 and performs xor operation
			"and r6, r6, r12, lsr #6;"	// r6 = (r2 xor (r1 << 6)) ^ r12		r12 => 0x03F00000

			"eor r2, r2, r6;"			// r2 = r2 xor T
			"eor r1, r1, r6, lsr #6;"	// r0 = r0 xor (T >> 6)
			//end swap

			//swap b90b91b92b93b94b95 in r1 with b190b191b192b193b194b195 in r3
			"eor r6, r3, r1, lsl #12;"	// r3 xor (r1 << 12) align blk9 wit block19 and performs xor operation
			"and r6, r6, r12, lsr #6;"	// r6 = (r2 xor (r1 << 12)) ^ r12		r12 => 0x03F00000

			"eor r3, r3, r6;"			// r3 = r3 xor T
			"eor r1, r1, r6, lsr #12;"	// r1 = r1 xor (T >> 12)
			//end swap

			//swap b150b151b152b153b154b155 in r2 with b200b201b202b203b204b205 in r3
			"eor r6, r3, r2, lsl #6;"	// r3 xor (r2 << 6) align blk15 with blk20 and performs xor operation
			"and r6, r6, r12, lsr #12;"	// r6 = (r3 xor (r2 << 6)) ^ r12 		r12 => 0x00068000

			"eor r3, r3, r6;"			// r3 = r3 xor T
			"eor r2, r2, r6, lsr #6;"	// r2 = r2 xor (T >> 6)

			//move blocks 5, 10, 21, 26 to register r11 and puts zeroes in their places

			"mov r11, r0, lsl #30;"			// move bits b50b51 to register r11 temporary
			"and r10, r1, 0x0000000F;"		// move bits b52b53b54b55 to register 10 temporary
			"eor r11, r11, r10, lsl #26;" 	// join bits b50b51b52b53b54b55 into registry r11

			"and r10, r1, 0x000000F0;"			// move bits b100b101b102b103
			"eor r11, r11, r10, lsl #18;"		// join bits b100b101b102b103 to reg11

			"and r10, r2, 0x000000C0;"			// move bits b104b105 to reg10
			"eor r11, r11, r10, lsl #14;"		// join bits b100b101b102b103 with b104b105

			"and r10, r3, 0x00003000;"			// move bits b210b211 to r10
			"eor r11, r11, r10, lsl #6;"		// join bits b210b211 wit previous bits in r11

			"and r10, r4, 0xF000;"				//
			"eor r11, r11, r10, lsl #2;"		//

			"and r10, r4, 0x000F0000;"			//
			"eor r11, r11, r10, lsr #6;"		//

			"and r10, r5, 0xC0000000;"			//
			"eor r11, r11, r10, lsr #22;"		//

			"and r0, r0, 0xFFFFFFFC;"	//clear bits b50b51
			"and r1, r1, 0xFFFFFF00;"	//clear bits b100b101b102b103 b52b53b54b55
			"and r2, r2, 0xFFFFFF3F;"	//clear bits b104b105
			"and r3, r3, 0xFFFFCFFF;"	//clear bits b210b211
			"and r4, r4, 0xFFF00FFF;"	//clear bits b260b261b262b263 b212b213b214b215
			"and r5, r5, 0x3FFFFFFF;"	//clear bits b264b265

			//rotate r5 to align all blocks and send the 00 to the end of the register
			"ror r5, r5, #30;"			//move b264b265 in 00 to the ond of register

			//swap b250b251b252b253b254b255 with 000000 in r0 aligned with b240b241b242b243b244b245
			"and r6, r12, r4, lsl #6;"		//save blk25 stored r4 to r6

			"eor r4, r4, r6, lsr #6;"		//put zeroes in blk25 position
			"eor r1, r1, r6, lsr #24;"		//move move blk25 to its position in r1

			"eor r6, r5, r11;"				//xor blk27 with blk5
			"and r6, r6, r12;"				//r6 = (blk5 xor blk27) ^ r12 	r12=>0xFC000000
			"eor r5, r5, r6;"				//put blk5 in r5
			"eor r11, r11, r6;"				//put blk27 in r11

			"eor r6, r4, r11;"				//xor blk10 in r11 with 0's in r4
			"and r6, r6, r12, lsr #6;"		//r6 = (r4 xor r11) ^ r12		r12 >> 6 => 0x03F00000
			"eor r4, r4, r6;"				//put blk10 in r4 in it's position
			"eor r11, r11, r6;"				//put 0's in r11 replacing place of blk10

			"eor r6, r5, r11;"				//xor blk28 in r5 with 0's in r11
			"and r6, r6, r12, lsr #6;"		//r6 = (r5 xor r11) ^ r12		r12 >> 6 => 0x03F00000
			"eor r5, r5, r6;"				//put 0's in r5 in place on blk28
			"eor r11, r11, r6;"				//put blk28 in r11 in place of 0's

			"eor r6, r5, r2, lsl #20;"		//xor blk11 with 0's in r5
			"and r6, r6, r12, lsr #6;"		//r6 = (r2 xor r5) ^ r12		r12 >> 6 => 0x03F00000
			"eor r5, r5, r6;"				//put blk11 in it's place in r5
			"eor r2, r2, r6, lsr #20;"		//put 0's in place of blk11

			"and r6, r3, r12, lsr #20;"		//get blk16 from r3
			"eor r4, r4, r6, lsl #8;"		//put bll16 in r4 and get aligned with blk15
			"eor r3, r3, r6;"				//clean blk16 in r3

			"and r6, r11, r12, lsr #12;"	//bet blk21 from r11
			"eor r3, r3, r6, lsr #6;"		//put blk21 in r3
			"eor r11, r11, r6;"				//clean blk211 from r11

			"and r6, r11, r12, lsr #18;"	//get blk26 from r11
			"eor r2, r2, r6, lsr #6;"		//put blk26 in r2 aligned with blk25 in r1
			"eor r11, r11, r6;"				// clear blk26 from r11

			"mov  r12, 0xFFFC;"				//new mask
			"movt r12, 0x000F;"				//new mask

			"and r6, r5, r12;"				//get blk29, blk30, blk31 from r5
			"eor r5, r5, r6;"				//clean blk29, blk30, blk31 in r5
			"eor r11, r11, r6;"				//put blk29,blk30,blk31 in r11;

			"mov r12, 0x0000;"				//put mask on r12
			"movt r12, 0xFC00;"				//put mask on r12

			"and r6, r3, r12, lsr #26;"		//get blk17 bits with the mask to r6 as temporary reg
			"eor r3, r3, r6;"				//clear r3 blk17 bits in r3
			"eor r5, r5, r6, lsl #14;"		//put blk17 bits in its position in r6 aligned with r16

			"and r6, r4, r12, lsr #20;"		//get blk22 from r4
			"eor r4, r4, r6;"				//clear blk22 from r4
			"eor r4, r4, r6, lsl #2;"		//paste blk22 to r4 just moving the two places to the left

			"and r6, r4, r12, lsr #26;"		//get blk23 from r4
			"eor r4, r4, r6;"				//clear blk23 from r4
			"eor r5, r5, r6, lsl #8;"		//put blk23 to r5 aligned to blk22 in r4

			//move blks 27, 28, 29 to r3, r4, r5 respectively

			"and r6, r11, r12;"				//get blk27 from r11
			"eor r11, r11, r6;"				//clear blk27 from r11
			"eor r3, r3, r6, lsr #24;"		//put blk27 in r3 and get aligned with blk26

			"and r6, r11, r12, lsr #6;"		//get blk28 from r11
			"eor r11, r11, r6;"				//clear blk28 from r11
			"eor r4, r4, r6, lsr #18;"		//put blk28 in r4 and get aligned with blk27 in r3

			"and r6, r11, r12, lsr #12;"	//get blk29 from r11
			"eor r11, r11, r6;"				//clear blk29 from r11
			"eor r5, r5, r6, lsr #12;"		//put blk29 in r5 and get aligned with blk28 in r4

			// move the last blks30 and blk31 in bit pairs

			"mov r12, 0x2000;"				//mask of 1 bit

			"and r6, r11, r12;"				//get b300 from r11
			"eor r0, r0, r6, lsr #12;"		//put b300 in r0 position 30

			"and r6, r11, r12, lsr #1;"		//get b301 from r11
			"eor r1, r1, r6, lsr #11;"		//put b301 in r1 position 30

			"and r6, r11, r12, lsr #2;"		//get b302 from r11
			"eor r2, r2, r6, lsr #10;"		//put b302 in r2 position 30

			"and r6, r11, r12, lsr #3;"		//get b303 from r11
			"eor r3, r3, r6, lsr #9;"		//put b303 in r3 position 30

			"and r6, r11, r12, lsr #4;"		//get b304 from r11
			"eor r4, r4, r6, lsr #8;"		//put b304 in r4 position 30

			"and r6, r11, r12, lsr #5;"		//get b305 from r11
			"eor r5, r5, r6, lsr #7;"		//put b305 in r5 position 30

			/*		blk31		*/

			"and r6, r11, r12, lsr #6;"		//get b310 from r11
			"eor r0, r0, r6, lsr #7;"		//put b310 in r0 position 31

			"and r6, r11, r12, lsr #7;"		//get b311 from r11
			"eor r1, r1, r6, lsr #6;"		//put b311 in r1 position 31

			"and r6, r11, r12, lsr #8;"		//get b312 from r11
			"eor r2, r2, r6, lsr #5;"		//put b312 in r2 position 31

			"and r6, r11, r12, lsr #9;"		//get b313 from r11
			"eor r3, r3, r6, lsr #4;"		//put g313 in r3 position 31

			"and r6, r11, r12, lsr #10;"	//get b314 from r11
			"eor r4, r4, r6, lsr #3;"		//put b314 in r4 position 31

			"and r6, r11, r12, lsr #11;"	//get b315 from r11
			"eor r5, r5, r6, lsr #2;"		//put b315 in r5 position 31

			"eor r6, r6, r6;"				//clear r6 registry
			"eor r11, r11, r11;"			//clear r11 registry
			"eor r12, r12, r12;"			//clear r12 registry

			/*		Final step for bitslice representation		*/
			"mov r12,  0x2080;"			//mask low part
			"movt r12, 0x8208;"			//mask high part

			"eor r6, r1, r0, lsl #1;"	//recover the b1 from all 5 blks in r0 i.e. blk01, blk61, blk121, blk181, blk241, and make a xor with r1 blk10, blk70, blk131, blk190, blk250
			"and r6, r6, r12;"			//just get the bit b0 from all blks xored here
			"eor r1, r1, r6;"			//put bx1's bits in bx0's place in r1
			"eor r0, r0, r6, lsr #1;"	//put bx0's bits in bx1's place in r0

			"eor r6, r2, r0, lsl #2;"	//xor r2 with (r0 << 2)			r6 <= r2 ^ (r0 << 2)
			"and r6, r6, r12;"			//get the bits b0 xor b1 here
			"eor r2, r2, r6;"			//paste blkx2's bits in r2 b0 place
			"eor r0, r0, r6, lsr #2;"	//paste blkx0's bits in r0 b2 place

			"eor r6, r3, r0, lsl #3;"
			"and r6, r6, r12;"
			"eor r3, r3, r6;"
			"eor r0, r0, r6, lsr #3;"

			"eor r6, r4, r0, lsl #4;"
			"and r6, r6, r12;"
			"eor r4, r4, r6;"
			"eor r0, r0, r6, lsr #4;"

			"eor r6, r5, r0, lsl #5;"
			"and r6, r6, r12;"
			"eor r5, r5, r6;"
			"eor r0, r0, r6, lsr #5;"

			"ror r12, r12, #1;"			//rotate to the right the mask to get one place to the right

			"eor r6, r2, r1, lsl #1;"
			"and r6, r6, r12;"
			"eor r2, r2, r6;"
			"eor r1, r1, r6, lsr #1;"

			"eor r6, r3, r1, lsl #2;"
			"and r6, r6, r12;"
			"eor r3, r3, r6;"
			"eor r1, r1, r6, lsr #2;"

			"eor r6, r4, r1, lsl #3;"
			"and r6, r6, r12;"
			"eor r4, r4, r6;"
			"eor r1, r1, r6, lsr #3;"

			"eor r6, r5, r1, lsl #4;"
			"and r6, r6, r12;"
			"eor r5, r5, r6;"
			"eor r1, r1, r6, lsr #4;"

			"ror r12, r12, #1;"			//rotate to the right the mask to get one place to the right

			"eor r6, r3, r2, lsl #1;"
			"and r6, r6, r12;"
			"eor r3, r3, r6;"
			"eor r2, r2, r6, lsr #1;"

			"eor r6, r4, r2, lsl #2;"
			"and r6, r6, r12;"
			"eor r4, r4, r6;"
			"eor r2, r2, r6, lsr #2;"

			"eor r6, r5, r2, lsl #3;"
			"and r6, r6, r12;"
			"eor r5, r5, r6;"
			"eor r2, r2, r6, lsr #3;"

			"ror r12, r12, #1;"			//rotate to the right the mask to get one place to the right

			"eor r6, r4, r3, lsl #1;"
			"and r6, r6, r12;"
			"eor r4, r4, r6;"
			"eor r3, r3, r6, lsr #1;"

			"eor r6, r5, r3, lsl #2;"
			"and r6, r6, r12;"
			"eor r5, r5, r6;"
			"eor r3, r3, r6, lsr #2;"

			"ror r12, r12, #1;"			//rotate to the right the mask to get on place to the right

			"eor r6, r5, r4, lsl #1;"
			"and r6, r6, r12;"
			"eor r5, r5, r6;"
			"eor r4, r4, r6, lsr #1;"


			"str r0, [r9, #0];"
			"str r1, [r9, #4];"
			"str r2, [r9, #8];"
			"str r3, [r9, #12];"
			"str r4, [r9, #16];"
			"str r5, [r9, #20];"

			"mov r11, r0;"				//store r0 in r11
			"mov r12, r3;"				//store r3 in r12
			"mov r10, r1;"				//store r1 in r10
			"mov r9, r2;"				//store r2 in r9
			/* Bitslice conversion end*/
		);
}

void sbox(){				//substitution box in ASM and bitslice
	asm(	/*		Sbox implementation in assembly		*/
			"mov r7, r12;"						//restore r7 value
			"eor r12, r12, r12;"				//clean r12 register

			"and r9, r4, r2;"					//  r9  <= r2 and r4
			"orn r9, r9, r5;"					//  r9  <= not((r2 and r4) or r5)
			"and r9, r9, r3;"					//	r9  <= r3 and not((r2 and r4) or r5)
			"and r10, r5, r4;"					//	r10 <= (r5 and r4)
			"orn r11, r3, r0;"					//	r11 <= not(r3 or r0)
			"orn r10, r10, r11;"				//	r10 <= not((r5 and r4) or (not(r3 or r0)))
			"and r10, r1, r10;"					//	r10 <= (not((r5 and r4) or (not(r3 or r0)))) and (r1)
			"orr r6, r10, r9;"					//	r6  <= (((not((r5 and r4) or (not(r3 or r0)))) and (r1)) or (r3 and not((r2 and r4) or r5)))

			"and r9, r0, r2;"					//	r9  <= (r0 and r2)			r9 	overwrite
			"orn r10, r2, r3;"					//	r10 <= not(r2 or r3)		r10 overwrite
			"orn r9, r9, r10;"					//	r9	<= not((r0 and r0) or (not(r2 or r3)))
			"and r9, r9, r5;"					//	r9	<= ((not((r0 and r0) or (not(r2 or r3)))) and r5)
			"orr r10, r5, r4;"					//	r10 <= (r5 or r4)			r10 overwrite
			"orn r10, r10, r3;"					//	r10 <= not((not(r5 or r4)) or r3)
			"orn r9, r9, r10;"					//	r9 	<= not(((not((r0 and r0) or (not(r2 or r3)))) and r5) or (not((not(r5 or r4)) or r3)))
			"orr r10, r0, r3;"					//	r10 <= (r0 or r3)			r10	overwrite
			"orn r10, r10, r1;"					//	r10	<= not((r0 or r3) or r1)
			"orn r7, r9, r10;"					//	r7	<= not((not(((not((r0 and r0) or (not(r2 or r3)))) and r5) or (not((not(r5 or r4)) or r3)))) or (not((r0 or r3) or r1)) )

			"and r9, r1, r3;"					//	r9 	<= (r1 and r3)			r9	overwrite
			"orn r10, r3, r4;"					//	r10 <= not(r3 or r4)		r10	overwrite
			"orn r9, r9, r10;"					//	r9	<= not((not(r3 or r4)) or (r1 and r3))
			"and r9, r0, r9;"					//	r9	<= ((not((not(r3 or r4)) or (r1 and r3))) and r0)
			"orr r10, r3, r4;"					//	r10 <= (r3 or r4)			r10	overwrite
			"orn r10, r10, r2;"					//	r10	<= not((r3 or r4) or r2)
			"orn r9, r9, r10;"					//	r9	<= not(((not((not(r3 or r4)) or (r1 and r3))) and r0) or (not((r3 or r4) or r2)) )
			"orr r10, r0, r2;"					//	r10	<= (r0 or r2)			r10	overwrite
			"orr r10, r10, r5;"					//	r10 <= ((r0 or r2) or r5)
			"orn r8, r9, r10;"					//	r8	<= not((not(((not((not(r3 or r4)) or (r1 and r3))) and r0) or (not((r3 or r4) or r2)))) or (((r0 or r2) or r5)))

			"and r9, r0, r4;"					//	r9	<= (r0 and r4)			r9	overwrite
			"orr r10, r0, r3;"					//	r10	<= (r0 or r3)			r10 overwrite
			"orn r9, r9, r10;"					//	r9	<= not((r0 and r4) or (r0 or r3))
			"and r9, r9, r2;"					//	r9	<= ((not((r0 and r4) or (r0 or r3))) and r2)
			"and r10, r0, r5;"					//	r10 <= (r0 and r5)			r10	overwrite
			"orn r10, r2, r10;"					//	r10	<= not((r0 and r5) or r2)
			"orn r9, r9, r10;"					//	r9	<= not(((not((r0 and r4) or (r0 or r3))) and r2) or (not((r0 and r5) or r2)))
			"and r10, r1, r3;"					//	r10 <= (r1 and r3)			r10 overwrite
			"orn r10, r0, r10;"					//	r10	<= not((r1 and r3) or r0)
			"orn r9, r9, r10;"					//	r9	<= not((not(((not((r0 and r4) or (r0 or r3))) and r2) or (not((r0 and r5) or r2)))) or (not((r1 and r3) or r0)))

			"eor r12, r12, r12;" 	    	  	//  put 0's to the reg 12

			"orr r10, r4, r2;"					//	r10	<= (r4 or r2)
			"orr r11, r0, r1;"					//	r11 <= (r0 or r1)
			"orn r10, r10, r11;"				//	r10	<= not((r4 or r2) or (r0 or r1))
			"orn r10, r12, r10;"				//	r10	<= not(() or (not((r4 or r2) or (r0 or r1))))
			"orn r11, r3, r0;"					//	r11	<= not( r3 or r0)		r11	overwrite
			"orn r10, r10, r11;"				//	r10	<= not((not(() or (not((r4 or r2) or (r0 or r1))))) or (not( r3 or r0)))
			"and r11, r4, r5;"					//	r11	<= (r4 and r5)			r11	overwrite
			"orn r11, r0, r11;"					//	r11 <= not((r4 and r5) or r0)
			"orn r10, r10, r11;"				//	r10 <= not((not((not(() or (not((r4 or r2) or (r0 or r1))))) or (not( r3 or r0)))) or (not((r4 and r5) or r0)))

			"and r12, r0, r3;"					//  r12 <= (r0 and r3)			r12	overwrite
			"orn r11, r2, r4;"					//	r11	<= not(r2 or r4)		r11	overwrite
			"orn r12, r12, r11;"				//	r12	<= not(((r0 and r3)) or (not(r2 or r4)))
			"orn r12, r1, r12;"					//	r12	<= not((not(((r0 and r3)) or (not(r2 or r4)))) or r1)
			"and r11, r1, r0;"					//	r11	<= (r1 and r0)
			"orr r11, r11, r5;"					//	r11	<= ((r1 and r0) or r5)
			"and r11, r11, r2;"					//	r11	<= (((r1 and r0) or r5) and r2)
			"orn r11, r11, r12;"				//	r11	<= not((((r1 and r0) or r5) and r2) or (not((not(((r0 and r3)) or (not(r2 or r4)))) or r1)))
			/*		Sbox implementation assembly end	*/
			"mov r12, r7;"						//save r7 value to r12
			"mov r7, sp;"						//move sp to r7
		);
}

void shiftcolumns(){		//Shift columns in ASM with bitslice
	asm(	/* shift columns */
			"mov r0, r6;"
			"mov r1, r12, ror #31;"				//"mov r1, r7, ror #31;"				// Original
			"mov r2, r8, ror #30;"
			"mov r3, r9, ror #29;"
			"mov r4, r10, ror #28;"
			"mov r5, r11, ror #27;"
			/* shift columns end */
		);
}

void mixcolumns(){			//Mix columns in ASM with bitslice
	asm(	/* mix columns*/
			"eor r0, r0, r6, ror #31;" 			//here the mix columns for the r0 register is calculated
			"eor r0, r0, r6, ror #27;"
			"eor r0, r0, r6, ror #23;"
			"eor r0, r0, r6, ror #17;"
			"eor r0, r0, r6, ror #11;"
			"eor r0, r0, r6, ror #6;"

			"eor r1, r1, r12, ror #30;"			//mix columns for the r1 register is calculated
			"eor r1, r1, r12, ror #26;"			// r1, r1, r7 was the original
			"eor r1, r1, r12, ror #22;"
			"eor r1, r1, r12, ror #16;"
			"eor r1, r1, r12, ror #10;"
			"eor r1, r1, r12, ror #5;"

			"eor r2, r2, r8, ror #29;"			//mix columns for the r2 register is calculated
			"eor r2, r2, r8, ror #25;"
			"eor r2, r2, r8, ror #21;"
			"eor r2, r2, r8, ror #15;"
			"eor r2, r2, r8, ror #9;"
			"eor r2, r2, r8, ror #4;"

			"eor r3, r3, r9, ror #28;"			//mix columns for the r3 register is calculated
			"eor r3, r3, r9, ror #24;"
			"eor r3, r3, r9, ror #20;"
			"eor r3, r3, r9, ror #14;"
			"eor r3, r3, r9, ror #8;"
			"eor r3, r3, r9, ror #3;"

			"eor r4, r4, r10, ror #27;"			//mix columns for the r4 register is calculated
			"eor r4, r4, r10, ror #23;"
			"eor r4, r4, r10, ror #19;"
			"eor r4, r4, r10, ror #13;"
			"eor r4, r4, r10, ror #7;"
			"eor r4, r4, r10, ror #2;"

			"eor r5, r5, r11, ror #26;"			//mix columns for the r5 register is calculated
			"eor r5, r5, r11, ror #22;"
			"eor r5, r5, r11, ror #18;"
			"eor r5, r5, r11, ror #12;"
			"eor r5, r5, r11, ror #6;"
			"eor r5, r5, r11, ror #1;"
			/* mix columns end*/

			"mov r11, r0;"				//store r0 in r11
			"mov r12, r3;"				//store r3 in r12
			"mov r10, r1;"				//store r1 in r10
			"mov r9, r2;"				//store r2 in r9

			"mov r7, sp;"			//put stack pointer back to register 7
		);
}

void ark(uint32_t *key){//Add round key in ASM with bitslice
	asm(/*	ARK	 the key 0 must be in bitslice*/

			"mov r1, r10;"				//recover value of ptxt stored previously in r1 fron r10 bkup
			"mov r2, r9;"				//recover value of ptxt stored previously in r2 from r9 bkup
			"mov r3, r12;"				//return back the value of plaintext stored in r3 from r12
			"mov r12, r0;"				//store *key address
			"mov r0, r11;"				//return back the value of plaintext stored in r0 from r11

			"ldr r6, [r12];"			//load bitslice key only bits "b_0"
			"ldr r7, [r12, #4];"		//load bitslice key only bits "b_1"
			"ldr r8, [r12, #8];"		//load bitslice key only bits "b_2"
			"ldr r9, [r12, #12];"		//load bitslice key only bits "b_3"
			"ldr r10, [r12, #16];"		//load bitslice key only bits "b_4"
			"ldr r11, [r12, #20];"		//load bitslice key only bits "b_5"

			/* XOR each register with the key position bit*/
			"eor r0, r0, r6;"
			"eor r1, r1, r7;"
			"eor r2, r2, r8;"
			"eor r3, r3, r9;"
			"eor r4, r4, r10;"
			"eor r5, r5, r11;"
			/*END of ARK*/
			//"mov r12, r7;"			//save r7 to r12
			"mov r7, sp;"			//put stack pointer back to register 7
	);
}

void lark(uint32_t *key, uint32_t *Ctxt){//Add round key in ASM with bitslice
	asm(/*	ARK	 the key 0 must be in bitslice*/

			"mov r7, r12;"				//restore value of r7 register previously used in sbox function

			"mov r12, r1;"				//save Ctext address

			"ldr r1, [r0, #4];"			//load bitslice key only bits "b_1"
			"ldr r2, [r0, #8];"			//load bitslice key only bits "b_2"
			"ldr r3, [r0, #12];"		//load bitslice key only bits "b_3"
			"ldr r4, [r0, #16];"		//load bitslice key only bits "b_4"
			"ldr r5, [r0, #20];"		//load bitslice key only bits "b_5"
			"ldr r0, [r0];"				//load bitslice key only bits "b_0"

			/* XOR each register with the key position bit*/
			"eor r0, r0, r6;"
			"eor r1, r1, r7;"
			"eor r2, r2, r8;"
			"eor r3, r3, r9;"
			"eor r4, r4, r10;"
			"eor r5, r5, r11;"
			/*END of ARK*/

			"str r0, [r12, #0];"
			"str r1, [r12, #4];"
			"str r2, [r12, #8];"
			"str r3, [r12, #12];"
			"str r4, [r12, #16];"
			"str r5, [r12, #20];"

			"mov r7, sp;"			//put stack pointer back to register 7
	);
}

void arc(uint32_t *k, uint32_t *rc){					//Add round constant with bitslice
	asm(
			"mov r11, r1;"				//save address of key
			"mov r12, r0;"				//save address of Rc

			"ldr r0, [r11];"			//load bitslice key only bits "b_0"
			"ldr r1, [r11, #4];"		//load bitslice key only bits "b_1"
			"ldr r2, [r11, #8];"		//load bitslice key only bits "b_2"
			"ldr r3, [r11, #12];"		//load bitslice key only bits "b_3"
			"ldr r4, [r11, #16];"		//load bitslice key only bits "b_4"
			"ldr r5, [r11, #20];"		//load bitslice key only bits "b_5"

			"ldr r6, [r12];"			//load bitslice key only bits "b_0"
			"ldr r7, [r12, #4];"		//load bitslice key only bits "b_1"
			"ldr r8, [r12, #8];"		//load bitslice key only bits "b_2"
			"ldr r9, [r12, #12];"		//load bitslice key only bits "b_3"
			"ldr r10, [r12, #16];"		//load bitslice key only bits "b_4"
			"ldr r11, [r12, #20];"		//load bitslice key only bits "b_5"

			/* XOR each register with the key position bit*/
			"eor r0, r0, r6;"
			"eor r1, r1, r7;"
			"eor r2, r2, r8;"
			"eor r3, r3, r9;"
			"eor r4, r4, r10;"
			"eor r5, r5, r11;"
			/*END of ARK*/
			/*store K[i] ^ Rc[i] into K address*/
			"str r0, [r12, #0];"
			"str r1, [r12, #4];"
			"str r2, [r12, #8];"
			"str r3, [r12, #12];"
			"str r4, [r12, #16];"
			"str r5, [r12, #20];"

			"mov r7, sp;"			//put stack pointer back to register 7
	);
}
