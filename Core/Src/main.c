/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BasicUart.h"
#include "flash.h"
#include "XModem.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void welcome_screen (void);
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // Willkommensbildschirm
  welcome_screen();

  // LEDs definieren
  HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET);

  // Abfrage ob Application vorhanden
  // Abfrage nur wenn aus Bootloader gebootet wird. Sollte ueber die Option-Bytes die
  // Bootadresse geaendert werden, kann dieser Schritt uebersprungen werden.
  if (X_OK == app_validation(FLASH_APP_VALID_ADDRESS))
  {
	  // Sprung zu Application, wenn vorhanden
	  uartTransmitString("Application vorhanden.\n");
	  uartTransmitString("Springe zu Benutzer Application...\n");
	  flash_jump_to_app();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  // xmodem dauerhaft Abfragen und auf Upload warten
	  // Abbruch durch Benutzer mit "a" oder "A"
	  // Abbruch durch Bootloader wenn Timeout erreicht und Application vorhanden
	  uartTransmitString("Bitte sende eine neue Binaerdatei mit Xmodemprotokoll um ein Update der Firmware durchzufuehren.\n");
	  uartTransmitString("Update kann mit \"a\" or \"A\" abgebrochen werden.\n");
	  xmodem_receive();

	  // Wenn Xmodem_receive fehlerhaft, dann Fehlermeldung ausgeben und erneut probieren
	  uartTransmitString("\nFehler... bitte erneut versuchen.\n");
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void welcome_screen (void)
{
	uartTransmitString("\n\033[2J\033[H\033[38;5;208m");
	uartTransmitString("####################################################################################################\n");
	uartTransmitString("####################################################################################################\n");
	uartTransmitString("####################################################################################################\n");
	uartTransmitString("####################################################################################################\n");
	uartTransmitString("####     ###    ##  ###  ##      ##      ##  ##  ##     ###      ##  ######      ###    ####    ####\n");
	uartTransmitString("####  ##  ###  ###  ###  ##  ########  ####  ##  ##  ##  ####  ####  ######  ######  ##  ##  ##  ###\n");
	uartTransmitString("####  ##  ###  ###  ###  ##  ########  ####  ##  ##  ##  ####  ####  ######  ######  ##  ######  ###\n");
	uartTransmitString("####  ##  ###  ###  ###  ##    ######  ####  ##  ##     #####  ####  ######    #####     ####   ####\n");
	uartTransmitString("####  ##  ###  ####  #  ###  ########  ####  ##  ##  #  #####  ####  ######  ##########  ######  ###\n");
	uartTransmitString("####  ##  ###  ####  #  ###  ########  ####  ##  ##  ##  ####  ####  ######  ##########  ##  ##  ###\n");
	uartTransmitString("####     ###    ####   ####      ####  #####    ###  ##  ####  ####      ##      ###    ####    ####\n");
	uartTransmitString("####################################################################################################\n");
	uartTransmitString("####################################################################################################\n");
	uartTransmitString("####################################################################################################\n");
	uartTransmitString("####################################################################################################\n");
	uartTransmitString("====================================================================================================\n");
	uartTransmitString("----------------------------------------> EAuto Bootloader <----------------------------------------\n");
	uartTransmitString("\tVersion ");
	uartTransmitNumber(MAJOR, 10);
	uartTransmitChar('.');
	uartTransmitNumber(MINOR, 10);
	uartTransmitChar('.');
	uartTransmitNumber(PATCH, 10);
	uartTransmitChar('.');
	uartTransmitNumber(DEV, 10);
	uartTransmitString("\n\tMikrocontroller:\t\tSTM32F767ZIT\n");
	uartTransmitString("====================================================================================================\n");
	uartTransmitString("\n\033[0;37m");
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
#ifdef USE_FULL_ASSERT
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
