/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "api.h"
#include "Functions.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ARM_CM_DEMCR      (volatile uint32_t *)0xE000EDFC

#define ARM_CM_DWT_CYCCNT (volatile uint32_t *)0xE0001004

#define ARM_CM_DWT_CTRL   (volatile uint32_t *)0xE0001000

#define MAX_BUFF 150 		/*Buffer max size in bytes*/



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef hlpuart1;

/* USER CODE BEGIN PV */
uint32_t startE, stopE;
int deltaE = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
/* USER CODE BEGIN PFP */




/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  StateUint Plaintext     = {0xA13A632451070E43,0x82A27F26A40682F3,0xFE9FF68028D24FDB};
  StateUint Key           = {0x764C4F6254E1BFF2,0x08E95862428FAED0,0x1584F4207A7E8477};
  StateUint Ciphertext    = {0xED3D0EA11C427BD3,0x2570DF41C6FD66EB,0xBF4916E760ED0943};

  uint8_t enc[24] = {0};
//  uint8_t dec[24] = {0};

  serial_out("Plaintext := ", (uint8_t*)&Plaintext, NULL, 24);
  serial_out("Key := ", (uint8_t*)&Key, NULL, 24);
  serial_out("Ciphertext := ", (uint8_t*)&Ciphertext, NULL, 24);

  serial_out("Encryption", NULL, NULL, 0);

  *ARM_CM_DWT_CYCCNT = 0;
  startE = *ARM_CM_DWT_CYCCNT;
  Encrypt(Plaintext, Key, enc);						//Speedy encryption
  stopE  = *ARM_CM_DWT_CYCCNT;
  deltaE = stopE - startE;

  serial_out("Encryption Done\n", NULL, NULL, 0);
  serial_out("Encryption Clock cycles = ", NULL, &deltaE, 0);


//  serial_out("Decryption", NULL, NULL, 0);
 // Decrypt(Ciphertext, Key, dec);					//Speedy decryption
//  serial_out("Decryption Done\n", NULL, NULL, 0);



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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

}

/* USER CODE BEGIN 4 */

void serial_out(char *label, uint8_t *dat, int *i, unsigned long long length){
  char buff [MAX_BUFF];
  uint16_t sz = 0;
  if(length == 0){
      sprintf(buff,"%s", label);
  }
  if(length == 8){
    sprintf(buff, "%s%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X\r\n", label, dat[7], dat[6], dat[5], dat[4], dat[3], dat[2], dat[1], dat[0]);
  }else if(length == 16){
    sprintf(buff, "%s%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X , %.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X\r\n", label, dat[7], dat[6], dat[5], dat[4], dat[3], dat[2], dat[1], dat[0], dat[15], dat[14], dat[13], dat[12], dat[11], dat[10], dat[9], dat[8]);
  }else if(length == 24){
    sprintf(buff, "%s%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X , %.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X , %.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X\r\n", label, dat[7], dat[6], dat[5], dat[4], dat[3], dat[2], dat[1], dat[0], dat[15], dat[14], dat[13], dat[12], dat[11], dat[10], dat[9], dat[8], dat[23], dat[22], dat[21], dat[20], dat[19], dat[18], dat[17], dat[16]);
  }


  if((label != NULL) && (dat == NULL) && (i != NULL)){    // print string and integer
    sprintf(buff, "%s%i\r\n", label, *i);
  }

  if((label != NULL) && (dat == NULL) && (i == NULL)){    // print the string label
  sprintf(buff, "%s\r\n", label);
  }

  sz = 0;
  do{
    sz++;
  }while(buff[sz]!='\0');

  HAL_UART_Transmit(&hlpuart1, (uint8_t *)buff, sz, HAL_MAX_DELAY);

}

void serial_out2(char *label, uint8_t *dat){
  char buff [MAX_BUFF];
  uint16_t sz = 0;

  sprintf(buff, "%s%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X , %.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X , %.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X \r\n",
		  label, dat[0], dat[1], dat[2], dat[3], dat[4], dat[5], dat[6], dat[7], dat[8], dat[9], dat[10], dat[11], dat[12], dat[13],
		  dat[14], dat[15], dat[16], dat[17], dat[18], dat[19], dat[20], dat[21], dat[22], dat[23]);


  sz = 0;
  do{
    sz++;
  }while(buff[sz]!='\0');

  HAL_UART_Transmit(&hlpuart1, (uint8_t *)buff, sz, HAL_MAX_DELAY);

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
