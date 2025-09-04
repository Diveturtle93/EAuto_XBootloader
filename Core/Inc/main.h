/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#ifdef DEBUG
	#define EAuto
	#warning "Das Symbol EAuto muss deaktiviert werden, wenn eine PCB programmiert werden soll"
#endif
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#if (BMS == 255) && (MOTOR == 255)
#warning "Es ist keine PCB definiert. Build ist fehlerhaft und fuehrt ggf. zu Beschaedigungen an der PCB."
#elif (BMS != 255) && (MOTOR != 255)
#error "Es sind zwei PCBs definiert. Falsche Pinbelegung fuehrt zu Beschaedigungen an der PCB."
#elif (BMS != 255) && (MOTOR == 255)
#warning "PCB Batteriemanagement-System ausgewaehlt"
#elif (BMS == 255) && (MOTOR != 255)
#warning "PCB Motorsteuergeraet ausgewaehlt"
#endif
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#ifdef EAuto
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RED_LED_Pin GPIO_PIN_2
#define RED_LED_GPIO_Port GPIOB
#define GREEN_LED_Pin GPIO_PIN_14
#define GREEN_LED_GPIO_Port GPIOB
#define BLUE_LED_Pin GPIO_PIN_7
#define BLUE_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#endif

// Definiere Pinout fuer Motorsteuergeraet 1.0
//----------------------------------------------------------------------
#if (MOTOR == 1) || (BMS == 1)
	#define RED_LED_Pin GPIO_PIN_2
	#define RED_LED_GPIO_Port GPIOB
	#define GREEN_LED_Pin GPIO_PIN_14
	#define GREEN_LED_GPIO_Port GPIOB
	#define BLUE_LED_Pin GPIO_PIN_7
	#define BLUE_LED_GPIO_Port GPIOB
#endif
//----------------------------------------------------------------------

// Definiere Bootloader Versionsnummern
//----------------------------------------------------------------------
#define MAJOR					0												// Bootloader Major Number
#define MINOR					1												// Bootloader Minor Number
//----------------------------------------------------------------------

// Definiere Flash Startadresse
//----------------------------------------------------------------------
#define FLASH_APP_START_ADDRESS			(0x8008000)
//----------------------------------------------------------------------

// Definiere Flash Validationadresse
//----------------------------------------------------------------------
#define FLASH_APP_VALID_ADDRESS			(0x803FFF8)
//----------------------------------------------------------------------

// Definiere Flash Endadresse
//----------------------------------------------------------------------
#define FLASH_APP_END_ADDRESS			0x8040000-1
//----------------------------------------------------------------------

// Typedefine Boolean definieren
//----------------------------------------------------------------------
typedef enum
{
	false,
	true,
} bool;
//----------------------------------------------------------------------
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
