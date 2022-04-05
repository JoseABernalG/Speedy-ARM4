/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <inttypes.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ARM_CM_DEMCR      (volatile uint32_t *)0xE000EDFC

#define ARM_CM_DWT_CYCCNT (volatile uint32_t *)0xE0001004

#define ARM_CM_DWT_CTRL   (volatile uint32_t *)0xE0001000

#define MAX_BUFF 110
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef hlpuart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void serial_out(char *label, uint8_t *dat0, uint8_t *dat1, uint8_t *dat2, uint8_t *dat3, uint8_t *dat4, uint8_t *dat5, int *i);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t startB, stopB, startE, stopE;
int deltaE = 0;
int deltaB = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  *ARM_CM_DEMCR     =  (*ARM_CM_DEMCR) | 0x01000000;  // Set bit 24
  *ARM_CM_DWT_CTRL = (*ARM_CM_DWT_CTRL) | 0x00000001 ;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  /* USER CODE BEGIN 2 */

  uint32_t Plaintext[6] = {0xA13A6324, 0x51070E43, 0x82A27F26, 0xA40682F3, 0xFE9FF680, 0x28D24FDB};
  /*All the keys are in bitslice representation. key[0] and key[7] aren't xored with round constant   */
  uint32_t Keys[8][6] = {{0X65CB7895, 0XAA4CAA9B, 0X98F8709C, 0XD4C43B89, 0X12C93259, 0XB7C604AB},		//k[0]	//master key
		  	  	  	  	 {0xB1FCF836, 0x6D539883, 0xE10FA686, 0xAEC8DCEB, 0x545667E6, 0xDD273099}, 		//k[1] ^ constant[1]   {0x88FFA82A, 0x4F47C286, 0x5568A336, 0x8CA19C61, 0x0FCF3328, 0x3C774987},		//k[1]
						 {0x5B90B703, 0xABA31D43, 0x107AAA7B, 0x0B18143C, 0x3FF2230D, 0x4419642A}, 		//k[2] ^ constant[2]   {0X61B1B178, 0X992302FD, 0X25B26492, 0X062BDD4D, 0XC27DB705, 0X66AB3AA8},		//k[2]
						 {0x51FD86DB, 0xE68AA6D5, 0x6A40F885, 0x41CE473C, 0x62B8A0D3, 0x014281FD}, 		//k[3] ^ constant[3]   {0XEEC40279, 0X4339C318, 0X37144EDA, 0X4E39AED2, 0X22AAA1FE, 0X1B1F1D28},		//k[3]
						 {0xC59C6692, 0xD3C34B7B, 0x3380F871, 0xF541CA00, 0x949CB233, 0x83D1D98D}, 		//k[4] ^ constant[4]   {0X4B24C964, 0X12AEDF18, 0X2E5BC4AB, 0X54DD5265, 0XC7C384E4, 0XDC4EA621},		//k[4]
						 {0xDE46126E, 0x1AD8BD5E, 0xC23199B0, 0xD381C2EB, 0x4FC419DF, 0xAFCB0C10}, 		//k[5] ^ constant[5]   {0X3F3CCCA0, 0X261CF1DD, 0X47FD4154, 0X14327A3E, 0XAB4519B2, 0XE30C650C},		//k[5]
						 {0x71854A2B, 0x232B011F, 0x8D8B7324, 0x789072B8, 0x607C2E8D, 0xEDEDE8F0}, 		//k[6] ^ constant[6]   {0X09F6DC17, 0XEAA19AAC, 0X0D8D8BC3, 0X17ECC918, 0X2D932491, 0XEA68315E},		//k[6]
     					 {0X8AAA87F8, 0X74A06C7C, 0X762013CF, 0X18C219CE, 0XB8A276D1, 0X769271CD} };	//k[7]	//final key

     serial_out("Speedy encryption begin using bitslice and ASM", NULL, NULL, NULL, NULL, NULL, NULL, NULL);
     serial_out("Plaintext[0] = ", (uint8_t*)&Plaintext[0], (uint8_t*)&Plaintext[1], (uint8_t*)&Plaintext[2], (uint8_t*)&Plaintext[3], (uint8_t*)&Plaintext[4], (uint8_t*)&Plaintext[5], NULL);
     serial_out("Key = ", (uint8_t*)&Keys[0][0], (uint8_t*)&Keys[0][1], (uint8_t*)&Keys[0][2], (uint8_t*)&Keys[0][3], (uint8_t*)&Keys[0][4], (uint8_t*)&Keys[0][5], NULL);


    *ARM_CM_DWT_CYCCNT = 0;
    startB = *ARM_CM_DWT_CYCCNT;

    asm(	/*Load plaintext from stack to registers r0 - r5*/
  		"ldr r0, [sp, #208];"				//load the first 32 bits i.e. Plaintext[0]
  		"ldr r1, [sp, #212];"				//load Plaintext[1]
  		"ldr r2, [sp, #216];"				//load Plaintext[2]
  		"ldr r3, [sp, #220];"				//load Plaintext[3]
  		"ldr r4, [sp, #224];"				//load Plaintext[4]
  		"ldr r5, [sp, #228];"				//load Plaintext[5]
  		"mov r12, 0x0000;"					//mask lower 16 bits
  		"movt r12, 0xFC00;"					//mask upper 16 bits
    );

    asm(	/*	convert plaintext to bitslice  */
  		//  here is to change b10 b11 b12 b13 b14 b15 for b60 b61 b62 b63 b64 b65
  		"lsl r6, r1, #4;"		  			// r6 <= (b << 4)   temporary
  		"eor r7, r6, r0, lsl #6;"  			// r7 <= (b << 4) xor (a << 6) temporary

  		"and r8 , r7, r12;"		  			// r8 => T <= ((b << 4) xor (a << 6)) ^ (M)
  		"eor r1, r1, r8, lsr #4;" 			// r1 <= (r1 xor (T >> 4));  //B = B xor (T >> 4);
  		"eor r0, r0, r8, lsr #6;" 			// r0 <= (r0 xor (T >> 6));  //A = A xor (T >> 6);

  		// end of swap move for block 1 with block 6
  		//	swap b120b121b122b123b124b125 with b20b21b22b23b24b25

  		"eor r6, r2, r0, lsl #4;"			// r2 xor (r0 << 4) align blk2 with blk12 and performs xor operation
  		"and r6, r6, r12, lsr #8;"			// r6 = (r2 xor (r0 << 4)) ^ r12	r2 => 00FC0000

  		"eor r2, r2, r6;"					// r2 = r2 xor T
  		"eor r0, r0, r6, lsr #4;"			// r0 = r0 xor (T >> 4)

  		// end swap
  		//swap b30b31b32b33b34b35 in r0 with b180b181b182b183b184b185 in r3

  		"eor r6, r3, r0, lsl #6;"			// r3 xor (r0 << 6) align blk3 with blk18 and performs xor operation
  		"and r6, r6, r12, lsr #12;" 		// r6 = (r3 xor (r0 << 6)) ^ r12	r12 => 000FC000

  		"eor r3, r3, r6;"					// r3 = r3 xor T
  		"eor r0, r0, r6, lsr #6;"			// r0 = r0 xor (T >> 6)

  		//end swap
  		//swap b40b41b42b43b44b45 in r0 with b240b241b242b243b244b25 in r4

  		"eor r6, r4, r0, lsl #8;"			// r4 xor (r0 << 8) align blk4 with blk 24 and performs xor operation
  		"and r6, r6, r12, lsr #16;"			// r6 = (r4 xor (r0 << 8)) ^ r12		r12 => 0000FC00

  		"eor r4, r4, r6;"					// r4 = r4 xor T
  		"eor r0, r0, r6, lsr #8;"			// r0 = r0 xor (T >> 8)
  		//end swap
  		//rotate register r1, r2, r3, r4 to align block b0,b1,b2,b3,b4
  		"ror r1, r1, #28;"					// rotate register r1 to align the bits b10b11b12b13b14b15 with b00b01b02b03b04b05
  		"ror r2, r2, #24;"					// rotate register r2 to align the bits b20b21b22b23b24b25 with b10b11b12b13b14b15
  		"ror r3, r3, #20;"					// rotate register r3 to align the bits b30b31b32b33b34b35 with b20b21b22b23b24b25
  		"ror r4, r4, #16;"					// rotate register r4 to align the bits b40b41b42b43b44b45 with b30b31b32b33b34b35
  		//End rotation
  		//	swap blocks
  		//swap b80b81b82b83b84b85 in r1 with b130b131b132b133b134b135 in r2
  		"eor r6, r2, r1, lsl #6;"			// r2 xor (r1 << 6) align blk8 with blk13 and performs xor operation
  		"and r6, r6, r12, lsr #6;"			// r6 = (r2 xor (r1 << 6)) ^ r12		r12 => 0x03F00000

  		"eor r2, r2, r6;"					// r2 = r2 xor T
  		"eor r1, r1, r6, lsr #6;"			// r0 = r0 xor (T >> 6)
  		//end swap

  		//swap b90b91b92b93b94b95 in r1 with b190b191b192b193b194b195 in r3
  		"eor r6, r3, r1, lsl #12;"			// r3 xor (r1 << 12) align blk9 wit block19 and performs xor operation
  		"and r6, r6, r12, lsr #6;"			// r6 = (r2 xor (r1 << 12)) ^ r12		r12 => 0x03F00000

  		"eor r3, r3, r6;"					// r3 = r3 xor T
  		"eor r1, r1, r6, lsr #12;"			// r1 = r1 xor (T >> 12)
  		//end swap

  		//swap b150b151b152b153b154b155 in r2 with b200b201b202b203b204b205 in r3
  		"eor r6, r3, r2, lsl #6;"			// r3 xor (r2 << 6) align blk15 with blk20 and performs xor operation
  		"and r6, r6, r12, lsr #12;"			// r6 = (r3 xor (r2 << 6)) ^ r12 		r12 => 0x00068000

  		"eor r3, r3, r6;"					// r3 = r3 xor T
  		"eor r2, r2, r6, lsr #6;"			// r2 = r2 xor (T >> 6)

  		//move blocks 5, 10, 21, 26 to register r11 and puts zeroes in their places

  		"mov r11, r0, lsl #30;"				// move bits b50b51 to register r11 temporary
  		"and r10, r1, 0x0000000F;"			// move bits b52b53b54b55 to register 10 temporary
  		"eor r11, r11, r10, lsl #26;" 		// join bits b50b51b52b53b54b55 into registry r11

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

  		"and r0, r0, 0xFFFFFFFC;"			//clear bits b50b51
  		"and r1, r1, 0xFFFFFF00;"			//clear bits b100b101b102b103 b52b53b54b55
  		"and r2, r2, 0xFFFFFF3F;"			//clear bits b104b105
  		"and r3, r3, 0xFFFFCFFF;"			//clear bits b210b211
  		"and r4, r4, 0xFFF00FFF;"			//clear bits b260b261b262b263 b212b213b214b215
  		"and r5, r5, 0x3FFFFFFF;"			//clear bits b264b265

  		//rotate r5 to align all blocks and send the 00 to the end of the register
  		"ror r5, r5, #30;"					//move b264b265 in 00 to the ond of register

  		//swap b250b251b252b253b254b255 with 000000 in r0 aligned with b240b241b242b243b244b245
  		"and r6, r12, r4, lsl #6;"			//save blk25 stored r4 to r6

  		"eor r4, r4, r6, lsr #6;"			//put zeroes in blk25 position
  		"eor r1, r1, r6, lsr #24;"			//move move blk25 to its position in r1

  		"eor r6, r5, r11;"					//xor blk27 with blk5
  		"and r6, r6, r12;"					//r6 = (blk5 xor blk27) ^ r12 	r12=>0xFC000000
  		"eor r5, r5, r6;"					//put blk5 in r5
  		"eor r11, r11, r6;"					//put blk27 in r11

  		"eor r6, r4, r11;"					//xor blk10 in r11 with 0's in r4
  		"and r6, r6, r12, lsr #6;"			//r6 = (r4 xor r11) ^ r12		r12 >> 6 => 0x03F00000
  		"eor r4, r4, r6;"					//put blk10 in r4 in it's position
  		"eor r11, r11, r6;"					//put 0's in r11 replacing place of blk10

  		"eor r6, r5, r11;"					//xor blk28 in r5 with 0's in r11
  		"and r6, r6, r12, lsr #6;"			//r6 = (r5 xor r11) ^ r12		r12 >> 6 => 0x03F00000
  		"eor r5, r5, r6;"					//put 0's in r5 in place on blk28
  		"eor r11, r11, r6;"					//put blk28 in r11 in place of 0's

  		"eor r6, r5, r2, lsl #20;"			//xor blk11 with 0's in r5
  		"and r6, r6, r12, lsr #6;"			//r6 = (r2 xor r5) ^ r12		r12 >> 6 => 0x03F00000
  		"eor r5, r5, r6;"					//put blk11 in it's place in r5
  		"eor r2, r2, r6, lsr #20;"			//put 0's in place of blk11

  		"and r6, r3, r12, lsr #20;"			//get blk16 from r3
  		"eor r4, r4, r6, lsl #8;"			//put bll16 in r4 and get aligned with blk15
  		"eor r3, r3, r6;"					//clean blk16 in r3

  		"and r6, r11, r12, lsr #12;"		//bet blk21 from r11
  		"eor r3, r3, r6, lsr #6;"			//put blk21 in r3
  		"eor r11, r11, r6;"					//clean blk211 from r11

  		"and r6, r11, r12, lsr #18;"		//get blk26 from r11
  		"eor r2, r2, r6, lsr #6;"			//put blk26 in r2 aligned with blk25 in r1
  		"eor r11, r11, r6;"					// clear blk26 from r11

  		"mov  r12, 0xFFFC;"					//new mask
  		"movt r12, 0x000F;"					//new mask

  		"and r6, r5, r12;"					//get blk29, blk30, blk31 from r5
  		"eor r5, r5, r6;"					//clean blk29, blk30, blk31 in r5
  		"eor r11, r11, r6;"					//put blk29,blk30,blk31 in r11;

  		"mov r12, 0x0000;"					//put mask on r12
  		"movt r12, 0xFC00;"					//put mask on r12

  		"and r6, r3, r12, lsr #26;"			//get blk17 bits with the mask to r6 as temporary reg
  		"eor r3, r3, r6;"					//clear r3 blk17 bits in r3
  		"eor r5, r5, r6, lsl #14;"			//put blk17 bits in its position in r6 aligned with r16

  		"and r6, r4, r12, lsr #20;"			//get blk22 from r4
  		"eor r4, r4, r6;"					//clear blk22 from r4
  		"eor r4, r4, r6, lsl #2;"			//paste blk22 to r4 just moving the two places to the left

  		"and r6, r4, r12, lsr #26;"			//get blk23 from r4
  		"eor r4, r4, r6;"					//clear blk23 from r4
  		"eor r5, r5, r6, lsl #8;"			//put blk23 to r5 aligned to blk22 in r4

  		//move blks 27, 28, 29 to r3, r4, r5 respectively

  		"and r6, r11, r12;"					//get blk27 from r11
  		"eor r11, r11, r6;"					//clear blk27 from r11
  		"eor r3, r3, r6, lsr #24;"			//put blk27 in r3 and get aligned with blk26

  		"and r6, r11, r12, lsr #6;"			//get blk28 from r11
  		"eor r11, r11, r6;"					//clear blk28 from r11
  		"eor r4, r4, r6, lsr #18;"			//put blk28 in r4 and get aligned with blk27 in r3

  		"and r6, r11, r12, lsr #12;"		//get blk29 from r11
  		"eor r11, r11, r6;"					//clear blk29 from r11
  		"eor r5, r5, r6, lsr #12;"			//put blk29 in r5 and get aligned with blk28 in r4

  		// move the last blks30 and blk31 in bit pairs

  		"mov r12, 0x2000;"					//mask of 1 bit

  		"and r6, r11, r12;"					//get b300 from r11
  		"eor r0, r0, r6, lsr #12;"			//put b300 in r0 position 30

  		"and r6, r11, r12, lsr #1;"			//get b301 from r11
  		"eor r1, r1, r6, lsr #11;"			//put b301 in r1 position 30

  		"and r6, r11, r12, lsr #2;"			//get b302 from r11
  		"eor r2, r2, r6, lsr #10;"			//put b302 in r2 position 30

  		"and r6, r11, r12, lsr #3;"			//get b303 from r11
  		"eor r3, r3, r6, lsr #9;"			//put b303 in r3 position 30

  		"and r6, r11, r12, lsr #4;"			//get b304 from r11
  		"eor r4, r4, r6, lsr #8;"			//put b304 in r4 position 30

  		"and r6, r11, r12, lsr #5;"			//get b305 from r11
  		"eor r5, r5, r6, lsr #7;"			//put b305 in r5 position 30

  		/*		blk31		*/

  		"and r6, r11, r12, lsr #6;"			//get b310 from r11
  		"eor r0, r0, r6, lsr #7;"			//put b310 in r0 position 31

  		"and r6, r11, r12, lsr #7;"			//get b311 from r11
  		"eor r1, r1, r6, lsr #6;"			//put b311 in r1 position 31

  		"and r6, r11, r12, lsr #8;"			//get b312 from r11
  		"eor r2, r2, r6, lsr #5;"			//put b312 in r2 position 31

  		"and r6, r11, r12, lsr #9;"			//get b313 from r11
  		"eor r3, r3, r6, lsr #4;"			//put g313 in r3 position 31

  		"and r6, r11, r12, lsr #10;"		//get b314 from r11
  		"eor r4, r4, r6, lsr #3;"			//put b314 in r4 position 31

  		"and r6, r11, r12, lsr #11;"		//get b315 from r11
  		"eor r5, r5, r6, lsr #2;"			//put b315 in r5 position 31

  		"eor r6, r6, r6;"					//clear r6 registry
  		"eor r11, r11, r11;"				//clear r11 registry
  		"eor r12, r12, r12;"				//clear r12 registry

  	/*		Final step for bitslice representation		*/

  		"mov r12,  0x2080;"					//mask low part
  		"movt r12, 0x8208;"					//mask high part

  		"eor r6, r1, r0, lsl #1;"			//recover the b1 from all 5 blks in r0 i.e. blk01, blk61, blk121, blk181, blk241, and make a xor with r1 blk10, blk70, blk131, blk190, blk250
  		"and r6, r6, r12;"					//just get the bit b0 from all blks xored here
  		"eor r1, r1, r6;"					//put bx1's bits in bx0's place in r1
  		"eor r0, r0, r6, lsr #1;"			//put bx0's bits in bx1's place in r0

  		"eor r6, r2, r0, lsl #2;"			//xor r2 with (r0 << 2)			r6 <= r2 ^ (r0 << 2)
  		"and r6, r6, r12;"					//get the bits b0 xor b1 here
  		"eor r2, r2, r6;"					//paste blkx2's bits in r2 b0 place
  		"eor r0, r0, r6, lsr #2;"			//paste blkx0's bits in r0 b2 place

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

  		"ror r12, r12, #1;"					//rotate to the right the mask to get one place to the right

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

  		"ror r12, r12, #1;"					//rotate to the right the mask to get one place to the right

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

  		"ror r12, r12, #1;"					//rotate to the right the mask to get one place to the right

  		"eor r6, r4, r3, lsl #1;"
  		"and r6, r6, r12;"
  		"eor r4, r4, r6;"
  		"eor r3, r3, r6, lsr #1;"

  		"eor r6, r5, r3, lsl #2;"
  		"and r6, r6, r12;"
  		"eor r5, r5, r6;"
  		"eor r3, r3, r6, lsr #2;"

  		"ror r12, r12, #1;"					//rotate to the right the mask to get on place to the right

  		"eor r6, r5, r4, lsl #1;"
  		"and r6, r6, r12;"
  		"eor r5, r5, r6;"
  		"eor r4, r4, r6, lsr #1;"
  		/* Bitslice conversion end*/
  	);

    stopB  = *ARM_CM_DWT_CYCCNT;
    deltaB = stopB - startB;

    *ARM_CM_DWT_CYCCNT = 0;
    startE = *ARM_CM_DWT_CYCCNT;

  ///////////// round  0 //////////////
  	asm(/*	ARK	 the key 0 must be in bitslice*/
  		"ldr r6, [sp, #16];"					//load bitslice key [0]  only bits "b_0"
  		"ldr r7, [sp, #20];"					//load bitslice key [0]  only bits "b_1"
  		"ldr r8, [sp, #24];"					//load bitslice key [0]  only bits "b_2"
  		"ldr r9, [sp, #28];"				//load bitslice key [0]  only bits "b_3"
  		"ldr r10, [sp, #32];"				//load bitslice key [0]  only bits "b_4"
  		"ldr r11, [sp, #36];"				//load bitslice key [0]  only bits "b_5"
  		/* XOR each register with the key position bit*/
  		"eor r0, r0, r6;"
  		"eor r1, r1, r7;"
  		"eor r2, r2, r8;"
  		"eor r3, r3, r9;"
  		"eor r4, r4, r10;"
  		"eor r5, r5, r11;"
  		/*END of ARK*/
  	);

  	asm(	/*		Sbox implementation in assembly		*/
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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(	/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;"  			   	// r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(	/* mix columns*/
  		"eor r0, r0, r6, ror #31;" 			//here the mix columns for the r0 register is calculated
  		"eor r0, r0, r6, ror #27;"
  		"eor r0, r0, r6, ror #23;"
  		"eor r0, r0, r6, ror #17;"
  		"eor r0, r0, r6, ror #11;"
  		"eor r0, r0, r6, ror #6;"

  		"eor r1, r1, r7, ror #30;"			//mix columns for the r1 register is calculated
  		"eor r1, r1, r7, ror #26;"
  		"eor r1, r1, r7, ror #22;"
  		"eor r1, r1, r7, ror #16;"
  		"eor r1, r1, r7, ror #10;"
  		"eor r1, r1, r7, ror #5;"

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
  	);

  ///////////// round  1 //////////////
  	asm(/*	ARK	 the key[1] xor round constant [1] must be in bitslice*/
  		"ldr r6, [sp, #40];"				//load bitslice key [1]  only bit "b_0"
  		"ldr r7, [sp, #44];"				//load bitslice key [1]  only bit "b_1"
  		"ldr r8, [sp, #48];"				//load bitslice key [1]  only bit "b_2"
  		"ldr r9, [sp, #52];"				//load bitslice key [1]  only bit "b_3"
  		"ldr r10, [sp, #56];"				//load bitslice key [1]  only bit "b_4"
  		"ldr r11, [sp, #60];"				//load bitslice key [1]  only bit "b_5"
  		/* XOR each register with the key position bit*/
  		"eor r0, r0, r6;"
  		"eor r1, r1, r7;"
  		"eor r2, r2, r8;"
  		"eor r3, r3, r9;"
  		"eor r4, r4, r10;"
  		"eor r5, r5, r11;"
  		/*EN of ARK*/
  	);

  	asm(	/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;"  		     	//	r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;"   			  	//	r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(	/* mix columns*/
  		"eor r0, r0, r6, ror #31;" 			//here the mix columns for the r0 register is calculated
  		"eor r0, r0, r6, ror #27;"
  		"eor r0, r0, r6, ror #23;"
  		"eor r0, r0, r6, ror #17;"
  		"eor r0, r0, r6, ror #11;"
  		"eor r0, r0, r6, ror #6;"

  		"eor r1, r1, r7, ror #30;"			//mix columns for the r1 register is calculated
  		"eor r1, r1, r7, ror #26;"
  		"eor r1, r1, r7, ror #22;"
  		"eor r1, r1, r7, ror #16;"
  		"eor r1, r1, r7, ror #10;"
  		"eor r1, r1, r7, ror #5;"

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
  	);

  ///////////// round  2 //////////////
  	asm(	/*	ARK	 the key[2] xor round constant [2] must be in bitslice*/
  		"ldr r6, [sp, #64];"				//load bitslice key [2]  only bit "b_0"
  		"ldr r7, [sp, #68];"				//load bitslice key [2]  only bit "b_1"
  		"ldr r8, [sp, #72];"				//load bitslice key [2]  only bit "b_2"
  		"ldr r9, [sp, #76];"				//load bitslice key [2]  only bit "b_3"
  		"ldr r10, [sp, #80];"				//load bitslice key [2]  only bit "b_4"
  		"ldr r11, [sp, #84];"				//load bitslice key [2]  only bit "b_5"
  		/* XOR each register with the key position bit*/
  		"eor r0, r0, r6;"
  		"eor r1, r1, r7;"
  		"eor r2, r2, r8;"
  		"eor r3, r3, r9;"
  		"eor r4, r4, r10;"
  		"eor r5, r5, r11;"
  		/*EN of ARK*/
  	);

  	asm(/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;"  		     	//	r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;"   			  	// r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(	/* mix columns*/
  		"eor r0, r0, r6, ror #31;" 			//here the mix columns for the r0 register is calculated
  		"eor r0, r0, r6, ror #27;"
  		"eor r0, r0, r6, ror #23;"
  		"eor r0, r0, r6, ror #17;"
  		"eor r0, r0, r6, ror #11;"
  		"eor r0, r0, r6, ror #6;"

  		"eor r1, r1, r7, ror #30;"			//mix columns for the r1 register is calculated
  		"eor r1, r1, r7, ror #26;"
  		"eor r1, r1, r7, ror #22;"
  		"eor r1, r1, r7, ror #16;"
  		"eor r1, r1, r7, ror #10;"
  		"eor r1, r1, r7, ror #5;"

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
  	);

  ///////////// round  3 //////////////
  	asm(/*	ARK	 the key[3] xor round constant [3] must be in bitslice*/
  		"ldr r6, [sp, #88];"				//load bitslice key [3]  only bit "b_0"
  		"ldr r7, [sp, #92];"				//load bitslice key [3]  only bit "b_1"
  		"ldr r8, [sp, #96];"				//load bitslice key [3]  only bit "b_2"
  		"ldr r9, [sp, #100];"				//load bitslice key [3]  only bit "b_3"
  		"ldr r10, [sp, #104];"				//load bitslice key [3]  only bit "b_4"
  		"ldr r11, [sp, #108];"				//load bitslice key [3]  only bit "b_5"
  		/* XOR each register with the key position bit*/
  		"eor r0, r0, r6;"
  		"eor r1, r1, r7;"
  		"eor r2, r2, r8;"
  		"eor r3, r3, r9;"
  		"eor r4, r4, r10;"
  		"eor r5, r5, r11;"
  		/*EN of ARK*/
  	);

  	asm(/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;" 		      	//	r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;"  			   	//	r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(	/* mix columns*/
  		"eor r0, r0, r6, ror #31;" 			//here the mix columns for the r0 register is calculated
  		"eor r0, r0, r6, ror #27;"
  		"eor r0, r0, r6, ror #23;"
  		"eor r0, r0, r6, ror #17;"
  		"eor r0, r0, r6, ror #11;"
  		"eor r0, r0, r6, ror #6;"

  		"eor r1, r1, r7, ror #30;"			//mix columns for the r1 register is calculated
  		"eor r1, r1, r7, ror #26;"
  		"eor r1, r1, r7, ror #22;"
  		"eor r1, r1, r7, ror #16;"
  		"eor r1, r1, r7, ror #10;"
  		"eor r1, r1, r7, ror #5;"

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
  );

  ///////////// round  4 //////////////
  	asm(/*	ARK	 the key[4] xor round constant [4] must be in bitslice*/
  		"ldr r6, [sp, #112];"				//load bitslice key [4]  only bit "b_0"
  		"ldr r7, [sp, #116];"				//load bitslice key [4]  only bit "b_1"
  		"ldr r8, [sp, #120];"				//load bitslice key [4]  only bit "b_2"
  		"ldr r9, [sp, #124];"				//load bitslice key [4]  only bit "b_3"
  		"ldr r10, [sp, #128];"				//load bitslice key [4]  only bit "b_4"
  		"ldr r11, [sp, #132];"				//load bitslice key [4]  only bit "b_5"
  /* XOR each register with the key position bit*/
  		"eor r0, r0, r6;"
  		"eor r1, r1, r7;"
  		"eor r2, r2, r8;"
  		"eor r3, r3, r9;"
  		"eor r4, r4, r10;"
  		"eor r5, r5, r11;"
  		/*EN of ARK*/
  	);

  	asm(	/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;"  		     	//	r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;" 			   	//	r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(	/* mix columns*/
  		"eor r0, r0, r6, ror #31;" 			//here the mix columns for the r0 register is calculated
  		"eor r0, r0, r6, ror #27;"
  		"eor r0, r0, r6, ror #23;"
  		"eor r0, r0, r6, ror #17;"
  		"eor r0, r0, r6, ror #11;"
  		"eor r0, r0, r6, ror #6;"

  		"eor r1, r1, r7, ror #30;"			//mix columns for the r1 register is calculated
  		"eor r1, r1, r7, ror #26;"
  		"eor r1, r1, r7, ror #22;"
  		"eor r1, r1, r7, ror #16;"
  		"eor r1, r1, r7, ror #10;"
  		"eor r1, r1, r7, ror #5;"

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
  	);

  ///////////// round  5 //////////////
  	asm(	/*	ARK	 the key[5] xor round constant [5] must be in bitslice*/
  		"ldr r6, [sp, #136];"				//load bitslice key [5]  only bit "b_0"
  		"ldr r7, [sp, #140];"				//load bitslice key [5]  only bit "b_1"
  		"ldr r8, [sp, #144];"				//load bitslice key [5]  only bit "b_2"
  		"ldr r9, [sp, #148];"				//load bitslice key [5]  only bit "b_3"
  		"ldr r10, [sp, #152];"				//load bitslice key [5]  only bit "b_4"
  		"ldr r11, [sp, #156];"				//load bitslice key [5]  only bit "b_5"
  			/* XOR each register with the key position bit*/
  		"eor r0, r0, r6;"
  		"eor r1, r1, r7;"
  		"eor r2, r2, r8;"
  		"eor r3, r3, r9;"
  		"eor r4, r4, r10;"
  		"eor r5, r5, r11;"
  		/*EN of ARK*/
  	);

  	asm(	/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;"				//	r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;" 		    	//	r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(	/* mix columns*/
  		"eor r0, r0, r6, ror #31;" 			//here the mix columns for the r0 register is calculated
  		"eor r0, r0, r6, ror #27;"
  		"eor r0, r0, r6, ror #23;"
  		"eor r0, r0, r6, ror #17;"
  		"eor r0, r0, r6, ror #11;"
  		"eor r0, r0, r6, ror #6;"

  		"eor r1, r1, r7, ror #30;"			//mix columns for the r1 register is calculated
  		"eor r1, r1, r7, ror #26;"
  		"eor r1, r1, r7, ror #22;"
  		"eor r1, r1, r7, ror #16;"
  		"eor r1, r1, r7, ror #10;"
  		"eor r1, r1, r7, ror #5;"

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
  	);

  ///////////// round  6 //////////////
  	asm(	/*	ARK	 the key [6] xor round constant [6] must be in bitslice*/
  		"ldr r6, [sp, #160];"				//load bitslice key [6]  only bit "b_0"
  		"ldr r7, [sp, #164];"				//load bitslice key [6]  only bit "b_1"
  		"ldr r8, [sp, #168];"				//load bitslice key [6]  only bit "b_2"
  		"ldr r9, [sp, #172];"				//load bitslice key [6]  only bit "b_3"
  		"ldr r10, [sp, #176];"				//load bitslice key [6]  only bit "b_4"
  		"ldr r11, [sp, #180];"				//load bitslice key [6]  only bit "b_5"
  			/* XOR each register with the key position bit*/
  		"eor r0, r0, r6;"
  		"eor r1, r1, r7;"
  		"eor r2, r2, r8;"
  		"eor r3, r3, r9;"
  		"eor r4, r4, r10;"
  		"eor r5, r5, r11;"
  		/*EN of ARK*/
  	);

  	asm(	/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;" 		      	//	r12 <= 0's

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
  	);

  	asm(	/* shift columns */
  		"mov r0, r6;"
  		"mov r1, r7, ror #31;"
  		"mov r2, r8, ror #30;"
  		"mov r3, r9, ror #29;"
  		"mov r4, r10, ror #28;"
  		"mov r5, r11, ror #27;"
  		/* shift columns end */
  	);

  	asm(/*		Sbox implementation in assembly		*/
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

  		"eor r12, r12, r12;" 		    	//	r12 <= 0's

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
  	);

  	asm(	/*		LAST KEY in bitslice representation		*/
  		/*		register from r6 to r12 haven't valid data erase them	*/
  		"ldr r0, [sp, #184];"				//load bitslice key [6]  only bit "b_0"
  		"ldr r1, [sp, #188];"				//load bitslice key [6]  only bit "b_1"
  		"ldr r2, [sp, #192];"				//load bitslice key [6]  only bit "b_2"
  		"ldr r3, [sp, #196];"				//load bitslice key [6]  only bit "b_3"
  		"ldr r4, [sp, #200];"				//load bitslice key [6]  only bit "b_4"
  		"ldr r5, [sp, #204];"				//load bitslice key [6]  only bit "b_5"
  		/*	LAST add round KEY to the state i.e. blk i xor constant i		*/
  		"eor r0, r0, r6;"
  		"eor r1, r1, r7;"
  		"eor r2, r2, r8;"
  		"eor r3, r3, r9;"
  		"eor r4, r4, r10;"
  		"eor r5, r5, r11;"
  		"eor r12, r12;"
  	);
  	/*at this point the 6th round is done the results are in r0 - r5 then r6 - r12 can be clean*/

  	stopE  = *ARM_CM_DWT_CYCCNT;
  	deltaE = stopE - startE;

  	serial_out("Done! ", NULL, NULL, NULL, NULL, NULL, NULL, NULL );

  	serial_out("Bitslice Clock cycles = ", NULL, NULL, NULL, NULL, NULL, NULL, &deltaB);
  	serial_out("Encryption Clock cycles = ", NULL, NULL, NULL, NULL, NULL, NULL, &deltaE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 71;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void serial_out(char *label, uint8_t *dat0, uint8_t *dat1, uint8_t *dat2, uint8_t *dat3, uint8_t *dat4, uint8_t *dat5, int *i){

	uint16_t sz = 0;
	char m[MAX_BUFF];

	if((label != NULL) && (dat0 != NULL) && (dat1 != NULL) && (dat2 != NULL) && (dat3 != NULL) && (dat4 != NULL) && (dat5 != NULL) && (i == NULL)){
		sprintf(m, "%s %.2X%.2X%.2X%.2X %.2X%.2X%.2X%.2X %.2X%.2X%.2X%.2X %.2X%.2X%.2X%.2X %.2X%.2X%.2X%.2X %.2X%.2X%.2X%.2X \r\n",
		label, dat0[3], dat0[2], dat0[1], dat0[0],
			   dat1[3], dat1[2], dat1[1], dat1[0],
			   dat2[3], dat2[2], dat2[1], dat2[0],
			   dat3[3], dat3[2], dat3[1], dat3[0],
			   dat4[3], dat4[2], dat4[1], dat4[0],
			   dat5[3], dat5[2], dat5[1], dat5[0] );
	}

	if((label != NULL) && (i != NULL)){    // print string and integer //&& (dat0 = NULL) && (dat1 = NULL) && (dat2 = NULL) && (dat3 = NULL) && (dat4 = NULL) && (dat5 = NULL) && (i != NULL)){    // print string and integer
		sprintf(m, "%s %i \r\n", label, *i);
	}

	if((label != NULL) && (dat0 == NULL) && (dat1 == NULL) && (dat2 == NULL) && (dat3 == NULL) && (dat4 == NULL) && (dat5 == NULL) && (i == NULL)){    // print the string label
		sprintf(m, "%s \r\n", label);
	}

	sz = 0;
	do{
		sz++;
	}while(m[sz]!='\0');

	HAL_UART_Transmit(&hlpuart1, (uint8_t *)m, sz, HAL_MAX_DELAY);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
