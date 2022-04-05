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
#include "Functions.h"
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
#define NR 7

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

  /*Plaintext in regular representation*/
    uint32_t Plaintext[6] = {0xA13A6324, 0x51070E43, 0x82A27F26, 0xA40682F3, 0xFE9FF680, 0x28D24FDB};

  /*All Keys are in regular representation*/
    uint32_t Keys[8][6] = {{0x764C4F62, 0x54E1BFF2, 0x08E95862, 0x428FAED0, 0x1584F420, 0x7A7E8477},//;		//k[0]	//master key
  /*  uint32_t Key1[6] =*/ {0x918049DD, 0xF49A9BBB, 0x61AB3CF7, 0xF11A8694, 0x468B444B, 0x88899E45},//;		//k[1]
  /*  uint32_t Key2[6] =*/ {0x4A3A5040, 0xD1F8A42F, 0xEA1C2777, 0x98CAE714, 0xE4E6674C, 0x78D56216},//;		//k[2]
  /*  uint32_t Key3[6] =*/ {0x834A8996, 0xCFD98A05, 0x9D5E90D5, 0x59818134, 0xDF133AE8, 0xFEEC23A0},//;		//k[3]
  /*  uint32_t Key4[6] =*/ {0x0E7215A4, 0xFEA258FC, 0x0C7756CE, 0xEFC054C1, 0xB5702A6B, 0xD062620D},//;		//k[4]
  /*  uint32_t Key5[6] =*/ {0x0C9CE48B, 0xCEEB20AB, 0x3CE7B10A, 0xC3D5569A, 0x111BC989, 0x9E55D190},//;		//k[5]
  /*  uint32_t Key6[6] =*/ {0x4514C4EC, 0xE56EFA5D, 0x6236C89A, 0xF240F1F2, 0x260D6894, 0x27571A6A},//;		//k[6]
  /*  uint32_t Key7[6] =*/ {0x8996DF99, 0x9A40DC4E, 0x818009C0, 0x8134CF53, 0x2AADBFFC, 0x32F5D30B}};//;		//k[7]	//final key

    uint32_t Rc[6][6] = {{0X3903501C,	0X22145A05,	0XB46705B0,	0X2269408A,	0X5B9954CE, 0XE150791E},
    	  	  	  	  	   {0X3A21067B,	0X32801FBE,	0X35C8CEE9,	0X0D33C971,	0XFD8F9408,	0X22B25E82},
    	  	  	  	  	   {0XBF3984A2,	0XA5B365CD,	0X5D54B65F,	0X0FF7E9EE,	0X4012012D,	0X1A5D9CD5},
    	  	  	  	  	   {0X8EB8AFF6,	0XC16D9463,	0X1DDB3CDA,	0XA19C9865,	0X535F36D7,	0X5F9F7FAC},
    	  	  	  	  	   {0XE17ADECE,	0X3CC44C83,	0X85CCD8E4,	0XC7B3B8D5,	0XE481006D,	0X4CC7691C},
    	  	  	  	  	   {0X7873963C,	0XC98A9BB3,	0X8006F8E7,	0X6F7CBBA0,	0X4DEF0A1C,	0X0785D9AE}};

    uint32_t Ciphertext[6]= {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    serial_out("Speedy encryption begin using bitslice and ASM", NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    serial_out("Plaintext = ", (uint8_t*)&Plaintext[0], (uint8_t*)&Plaintext[1], (uint8_t*)&Plaintext[2], (uint8_t*)&Plaintext[3], (uint8_t*)&Plaintext[4], (uint8_t*)&Plaintext[5], NULL);
    serial_out("Key = ", (uint8_t*)&Keys[0][0], (uint8_t*)&Keys[0][1], (uint8_t*)&Keys[0][2], (uint8_t*)&Keys[0][3], (uint8_t*)&Keys[0][4], (uint8_t*)&Keys[0][5], NULL);


    *ARM_CM_DWT_CYCCNT = 0;
    startB = *ARM_CM_DWT_CYCCNT;


    //bslice();									//convert data to bitslice representation
    bslice(Keys[0]);
    bslice(Keys[1]);
    bslice(Keys[2]);
    bslice(Keys[3]);
    bslice(Keys[4]);
    bslice(Keys[5]);
    bslice(Keys[6]);
    bslice(Keys[7]);

    arc(Keys[1], Rc[0]);						//Performs Key[i] ^ RoundConstant[i] and store it into Key variable
    arc(Keys[2], Rc[1]);
    arc(Keys[3], Rc[2]);
    arc(Keys[4], Rc[3]);
    arc(Keys[5], Rc[4]);
    arc(Keys[6], Rc[5]);

    bslice(Plaintext);

    stopB  = *ARM_CM_DWT_CYCCNT;
    deltaB = stopB - startB;

    *ARM_CM_DWT_CYCCNT = 0;
    startE = *ARM_CM_DWT_CYCCNT;

    for(int i = 0; i < NR; i++){
  	  ark(Keys[i]);								// Add round key
  	  sbox();									// substitution box
  	  shiftcolumns();							// shift columns
  	  sbox();									// substitution box
  	  if(i <= NR-2){
  		  shiftcolumns();						// shift columns
  		  mixcolumns();							// mix columns
  		  //arc(Rc[i]);							// add round constant
  	  }
  	  if(i == (NR - 1)){
  		  lark(Keys[i+1], Ciphertext);			// last add round key, here the cipher text is stored in memory
  	  }
    }

    stopE  = *ARM_CM_DWT_CYCCNT;
    deltaE = stopE - startE;


    serial_out("Bitslice Cipher Text = ", (uint8_t*)&Ciphertext[0], (uint8_t*)&Ciphertext[1], (uint8_t*)&Ciphertext[2], (uint8_t*)&Ciphertext[3], (uint8_t*)&Ciphertext[4], (uint8_t*)&Ciphertext[5], NULL);
   	serial_out("Bitslice Clock cycles = ", NULL, NULL, NULL, NULL, NULL, NULL, &deltaB);
   	serial_out("Encryption Clock cycles = ", NULL, NULL, NULL, NULL, NULL, NULL, &deltaE);
   	serial_out("Done! ", NULL, NULL, NULL, NULL, NULL, NULL, NULL );
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
