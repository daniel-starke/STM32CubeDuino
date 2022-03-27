/**
 * @file board.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-10-01
 * @version 2022-03-20
 */
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <wiring_irq.h>


/* exported variables */
HardwareSerial Serial1(USART1, getIrqNumFor(USART1), PB_7, PB_6, GPIO_AF7_USART2, GPIO_AF7_USART2);
HardwareSerial Serial2(USART2, getIrqNumFor(USART2), PA_15, PA_2, GPIO_AF3_USART2, GPIO_AF7_USART2);
TwoWire Wire(I2C1, I2C1_EV_IRQn, I2C1_ER_IRQn, PA_9, PA_10, GPIO_AF4_I2C1, GPIO_AF4_I2C1);
SPIClass SPI(SPI1, PA_5, PA_7, PA_6, GPIO_AF5_SPI1, GPIO_AF5_SPI1, GPIO_AF5_SPI1);


/**
 * Initializes this board by configuring the system clock base.
 * 
 * @remarks NUCLEO-L432KC has an LSE of 32768Hz (SB5 and SB7 closed) and no HSE.
 * MSI is used as high speed clock source and the USB clock is calibrated via SOF.
 */
void initVariant() {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/* Configure LSE Drive Capability */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
	/* Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure. */
#ifndef STM32CUBEDUINO_DISABLE_USB
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
#else
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
#endif
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV6;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		systemErrorHandler();
	}
	/* Initializes the CPU, AHB and APB buses clocks */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		systemErrorHandler();
	}
#ifndef STM32CUBEDUINO_DISABLE_USB
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_LPTIM1|RCC_PERIPHCLK_LPTIM2|RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_ADC;
#else
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_LPTIM1|RCC_PERIPHCLK_LPTIM2|RCC_PERIPHCLK_USB|RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_ADC;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
#endif
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
	PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
	PeriphClkInit.Lptim2ClockSelection = RCC_LPTIM2CLKSOURCE_LSI;
	PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
	PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
	PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
	PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
	PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		systemErrorHandler();
	}
	/* Configure the main internal regulator output voltage */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
		systemErrorHandler();
	}
	/* Enable MSI Auto calibration */
	HAL_RCCEx_EnableMSIPLLMode();
#ifndef STM32CUBEDUINO_DISABLE_USB
	RCC_CRSInitTypeDef RCC_CRSInitStruct = {0};
	/* Enable the SYSCFG APB clock */
	__HAL_RCC_CRS_CLK_ENABLE();
	/* Configures CRS */
	RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;
	RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;
	RCC_CRSInitStruct.Polarity = RCC_CRS_SYNC_POLARITY_RISING;
	RCC_CRSInitStruct.ReloadValue = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
	RCC_CRSInitStruct.ErrorLimitValue = 34;
	RCC_CRSInitStruct.HSI48CalibrationValue = 32;
	HAL_RCCEx_CRSConfig(&RCC_CRSInitStruct);
#endif /* not STM32CUBEDUINO_DISABLE_USB */
}
