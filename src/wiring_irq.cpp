/**
 * @file wiring_irq.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-11-13
 * @version 2022-04-03
 */
#include "Arduino.h"
#include "wiring_irq.h"


#if !defined(STM32CUBEDUINO_DISABLE_TIMER) || !defined(STM32CUBEDUINO_DISABLE_PWM)
#undef STM32CUBEDUINO_DISABLE_ALL_TIMERS
#else
#define STM32CUBEDUINO_DISABLE_ALL_TIMERS
#endif


#ifdef __HAL_GET_PENDING_IT /* STM32F0 and STM32G0 specific interrupt line handling. */
#ifdef HAL_ITLINE_ADC
#define ifIsPendingIrqForADC() if (__HAL_GET_PENDING_IT(HAL_ITLINE_ADC))
#else /* not HAL_ITLINE_ADC */
#define ifIsPendingIrqForADC() if ( false )
#endif /* not HAL_ITLINE_ADC */
#ifdef HAL_ITLINE_AES
#define ifIsPendingIrqForAES() if (__HAL_GET_PENDING_IT(HAL_ITLINE_AES))
#else /* not HAL_ITLINE_AES */
#define ifIsPendingIrqForAES() if ( false )
#endif /* not HAL_ITLINE_AES */
#ifdef HAL_ITLINE_CAN
#define ifIsPendingIrqForCAN() if (__HAL_GET_PENDING_IT(HAL_ITLINE_CAN))
#else /* not HAL_ITLINE_CAN */
#define ifIsPendingIrqForCAN() if ( false )
#endif /* not HAL_ITLINE_CAN */
#ifdef HAL_ITLINE_CEC
#define ifIsPendingIrqForCEC() if (__HAL_GET_PENDING_IT(HAL_ITLINE_CEC))
#else /* not HAL_ITLINE_CEC */
#define ifIsPendingIrqForCEC() if ( false )
#endif /* not HAL_ITLINE_CEC */
#ifdef HAL_ITLINE_CLK_CTRL
#define ifIsPendingIrqForCLK_CTRL() if (__HAL_GET_PENDING_IT(HAL_ITLINE_CLK_CTRL))
#else /* not HAL_ITLINE_CLK_CTRL */
#define ifIsPendingIrqForCLK_CTRL() if ( false )
#endif /* not HAL_ITLINE_CLK_CTRL */
#ifdef HAL_ITLINE_COMP1
#define ifIsPendingIrqForCOMP1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_COMP1))
#else /* not HAL_ITLINE_COMP1 */
#define ifIsPendingIrqForCOMP1() if ( false )
#endif /* not HAL_ITLINE_COMP1 */
#ifdef HAL_ITLINE_COMP2
#define ifIsPendingIrqForCOMP2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_COMP2))
#else /* not HAL_ITLINE_COMP2 */
#define ifIsPendingIrqForCOMP2() if ( false )
#endif /* not HAL_ITLINE_COMP2 */
#ifdef HAL_ITLINE_COMP3
#define ifIsPendingIrqForCOMP3() if (__HAL_GET_PENDING_IT(HAL_ITLINE_COMP3))
#else /* not HAL_ITLINE_COMP3 */
#define ifIsPendingIrqForCOMP3() if ( false )
#endif /* not HAL_ITLINE_COMP3 */
#ifdef HAL_ITLINE_CRS
#define ifIsPendingIrqForCRS() if (__HAL_GET_PENDING_IT(HAL_ITLINE_CRS))
#else /* not HAL_ITLINE_CRS */
#define ifIsPendingIrqForCRS() if ( false )
#endif /* not HAL_ITLINE_CRS */
#ifdef HAL_ITLINE_DAC
#define ifIsPendingIrqForDAC() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DAC))
#else /* not HAL_ITLINE_DAC */
#define ifIsPendingIrqForDAC() if ( false )
#endif /* not HAL_ITLINE_DAC */
#ifdef HAL_ITLINE_DMAMUX1
#define ifIsPendingIrqForDMAMUX1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMAMUX1))
#else /* not HAL_ITLINE_DMAMUX1 */
#define ifIsPendingIrqForDMAMUX1() if ( false )
#endif /* not HAL_ITLINE_DMAMUX1 */
#ifdef HAL_ITLINE_DMA1_CH1
#define ifIsPendingIrqForDMA1_CH1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA1_CH1))
#else /* not HAL_ITLINE_DMA1_CH1 */
#define ifIsPendingIrqForDMA1_CH1() if ( false )
#endif /* not HAL_ITLINE_DMA1_CH1 */
#ifdef HAL_ITLINE_DMA1_CH2
#define ifIsPendingIrqForDMA1_CH2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA1_CH2))
#else /* not HAL_ITLINE_DMA1_CH2 */
#define ifIsPendingIrqForDMA1_CH2() if ( false )
#endif /* not HAL_ITLINE_DMA1_CH2 */
#ifdef HAL_ITLINE_DMA1_CH3
#define ifIsPendingIrqForDMA1_CH3() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA1_CH3))
#else /* not HAL_ITLINE_DMA1_CH3 */
#define ifIsPendingIrqForDMA1_CH3() if ( false )
#endif /* not HAL_ITLINE_DMA1_CH3 */
#ifdef HAL_ITLINE_DMA1_CH4
#define ifIsPendingIrqForDMA1_CH4() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA1_CH4))
#else /* not HAL_ITLINE_DMA1_CH4 */
#define ifIsPendingIrqForDMA1_CH4() if ( false )
#endif /* not HAL_ITLINE_DMA1_CH4 */
#ifdef HAL_ITLINE_DMA1_CH5
#define ifIsPendingIrqForDMA1_CH5() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA1_CH5))
#else /* not HAL_ITLINE_DMA1_CH5 */
#define ifIsPendingIrqForDMA1_CH5() if ( false )
#endif /* not HAL_ITLINE_DMA1_CH5 */
#ifdef HAL_ITLINE_DMA1_CH6
#define ifIsPendingIrqForDMA1_CH6() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA1_CH6))
#else /* not HAL_ITLINE_DMA1_CH6 */
#define ifIsPendingIrqForDMA1_CH6() if ( false )
#endif /* not HAL_ITLINE_DMA1_CH6 */
#ifdef HAL_ITLINE_DMA1_CH7
#define ifIsPendingIrqForDMA1_CH7() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA1_CH7))
#else /* not HAL_ITLINE_DMA1_CH7 */
#define ifIsPendingIrqForDMA1_CH7() if ( false )
#endif /* not HAL_ITLINE_DMA1_CH7 */
#ifdef HAL_ITLINE_DMA2_CH1
#define ifIsPendingIrqForDMA2_CH1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA2_CH1))
#else /* not HAL_ITLINE_DMA2_CH1 */
#define ifIsPendingIrqForDMA2_CH1() if ( false )
#endif /* not HAL_ITLINE_DMA2_CH1 */
#ifdef HAL_ITLINE_DMA2_CH2
#define ifIsPendingIrqForDMA2_CH2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA2_CH2))
#else /* not HAL_ITLINE_DMA2_CH2 */
#define ifIsPendingIrqForDMA2_CH2() if ( false )
#endif /* not HAL_ITLINE_DMA2_CH2 */
#ifdef HAL_ITLINE_DMA2_CH3
#define ifIsPendingIrqForDMA2_CH3() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA2_CH3))
#else /* not HAL_ITLINE_DMA2_CH3 */
#define ifIsPendingIrqForDMA2_CH3() if ( false )
#endif /* not HAL_ITLINE_DMA2_CH3 */
#ifdef HAL_ITLINE_DMA2_CH4
#define ifIsPendingIrqForDMA2_CH4() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA2_CH4))
#else /* not HAL_ITLINE_DMA2_CH4 */
#define ifIsPendingIrqForDMA2_CH4() if ( false )
#endif /* not HAL_ITLINE_DMA2_CH4 */
#ifdef HAL_ITLINE_DMA2_CH5
#define ifIsPendingIrqForDMA2_CH5() if (__HAL_GET_PENDING_IT(HAL_ITLINE_DMA2_CH5))
#else /* not HAL_ITLINE_DMA2_CH5 */
#define ifIsPendingIrqForDMA2_CH5() if ( false )
#endif /* not HAL_ITLINE_DMA2_CH5 */
#ifdef HAL_ITLINE_EWDG
#define ifIsPendingIrqForEWDG() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EWDG))
#else /* not HAL_ITLINE_EWDG */
#define ifIsPendingIrqForEWDG() if ( false )
#endif /* not HAL_ITLINE_EWDG */
#ifdef HAL_ITLINE_WWDG
#define ifIsPendingIrqForWWDG() if (__HAL_GET_PENDING_IT(HAL_ITLINE_WWDG))
#else /* not HAL_ITLINE_WWDG */
#define ifIsPendingIrqForWWDG() if ( false )
#endif /* not HAL_ITLINE_WWDG */
#ifdef HAL_ITLINE_EXTI0
#define ifIsPendingIrqForEXTI0() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI0))
#else /* not HAL_ITLINE_EXTI0 */
#define ifIsPendingIrqForEXTI0() if ( false )
#endif /* not HAL_ITLINE_EXTI0 */
#ifdef HAL_ITLINE_EXTI1
#define ifIsPendingIrqForEXTI1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI1))
#else /* not HAL_ITLINE_EXTI1 */
#define ifIsPendingIrqForEXTI1() if ( false )
#endif /* not HAL_ITLINE_EXTI1 */
#ifdef HAL_ITLINE_EXTI2
#define ifIsPendingIrqForEXTI2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI2))
#else /* not HAL_ITLINE_EXTI2 */
#define ifIsPendingIrqForEXTI2() if ( false )
#endif /* not HAL_ITLINE_EXTI2 */
#ifdef HAL_ITLINE_EXTI3
#define ifIsPendingIrqForEXTI3() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI3))
#else /* not HAL_ITLINE_EXTI3 */
#define ifIsPendingIrqForEXTI3() if ( false )
#endif /* not HAL_ITLINE_EXTI3 */
#ifdef HAL_ITLINE_EXTI4
#define ifIsPendingIrqForEXTI4() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI4))
#else /* not HAL_ITLINE_EXTI4 */
#define ifIsPendingIrqForEXTI4() if ( false )
#endif /* not HAL_ITLINE_EXTI4 */
#ifdef HAL_ITLINE_EXTI5
#define ifIsPendingIrqForEXTI5() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI5))
#else /* not HAL_ITLINE_EXTI5 */
#define ifIsPendingIrqForEXTI5() if ( false )
#endif /* not HAL_ITLINE_EXTI5 */
#ifdef HAL_ITLINE_EXTI6
#define ifIsPendingIrqForEXTI6() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI6))
#else /* not HAL_ITLINE_EXTI6 */
#define ifIsPendingIrqForEXTI6() if ( false )
#endif /* not HAL_ITLINE_EXTI6 */
#ifdef HAL_ITLINE_EXTI7
#define ifIsPendingIrqForEXTI7() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI7))
#else /* not HAL_ITLINE_EXTI7 */
#define ifIsPendingIrqForEXTI7() if ( false )
#endif /* not HAL_ITLINE_EXTI7 */
#ifdef HAL_ITLINE_EXTI8
#define ifIsPendingIrqForEXTI8() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI8))
#else /* not HAL_ITLINE_EXTI8 */
#define ifIsPendingIrqForEXTI8() if ( false )
#endif /* not HAL_ITLINE_EXTI8 */
#ifdef HAL_ITLINE_EXTI9
#define ifIsPendingIrqForEXTI9() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI9))
#else /* not HAL_ITLINE_EXTI9 */
#define ifIsPendingIrqForEXTI9() if ( false )
#endif /* not HAL_ITLINE_EXTI9 */
#ifdef HAL_ITLINE_EXTI10
#define ifIsPendingIrqForEXTI10() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI10))
#else /* not HAL_ITLINE_EXTI10 */
#define ifIsPendingIrqForEXTI10() if ( false )
#endif /* not HAL_ITLINE_EXTI10 */
#ifdef HAL_ITLINE_EXTI11
#define ifIsPendingIrqForEXTI11() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI11))
#else /* not HAL_ITLINE_EXTI11 */
#define ifIsPendingIrqForEXTI11() if ( false )
#endif /* not HAL_ITLINE_EXTI11 */
#ifdef HAL_ITLINE_EXTI12
#define ifIsPendingIrqForEXTI12() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI12))
#else /* not HAL_ITLINE_EXTI12 */
#define ifIsPendingIrqForEXTI12() if ( false )
#endif /* not HAL_ITLINE_EXTI12 */
#ifdef HAL_ITLINE_EXTI13
#define ifIsPendingIrqForEXTI13() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI13))
#else /* not HAL_ITLINE_EXTI13 */
#define ifIsPendingIrqForEXTI13() if ( false )
#endif /* not HAL_ITLINE_EXTI13 */
#ifdef HAL_ITLINE_EXTI14
#define ifIsPendingIrqForEXTI14() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI14))
#else /* not HAL_ITLINE_EXTI14 */
#define ifIsPendingIrqForEXTI14() if ( false )
#endif /* not HAL_ITLINE_EXTI14 */
#ifdef HAL_ITLINE_EXTI15
#define ifIsPendingIrqForEXTI15() if (__HAL_GET_PENDING_IT(HAL_ITLINE_EXTI15))
#else /* not HAL_ITLINE_EXTI15 */
#define ifIsPendingIrqForEXTI15() if ( false )
#endif /* not HAL_ITLINE_EXTI15 */
#ifdef HAL_ITLINE_FDCAN1_IT0
#define ifIsPendingIrqForFDCAN1_IT0() if (__HAL_GET_PENDING_IT(HAL_ITLINE_FDCAN1_IT0))
#else /* not HAL_ITLINE_FDCAN1_IT0 */
#define ifIsPendingIrqForFDCAN1_IT0() if ( false )
#endif /* not HAL_ITLINE_FDCAN1_IT0 */
#ifdef HAL_ITLINE_FDCAN1_IT1
#define ifIsPendingIrqForFDCAN1_IT1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_FDCAN1_IT1))
#else /* not HAL_ITLINE_FDCAN1_IT1 */
#define ifIsPendingIrqForFDCAN1_IT1() if ( false )
#endif /* not HAL_ITLINE_FDCAN1_IT1 */
#ifdef HAL_ITLINE_FDCAN2_IT0
#define ifIsPendingIrqForFDCAN2_IT0() if (__HAL_GET_PENDING_IT(HAL_ITLINE_FDCAN2_IT0))
#else /* not HAL_ITLINE_FDCAN2_IT0 */
#define ifIsPendingIrqForFDCAN2_IT0() if ( false )
#endif /* not HAL_ITLINE_FDCAN2_IT0 */
#ifdef HAL_ITLINE_FDCAN2_IT1
#define ifIsPendingIrqForFDCAN2_IT1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_FDCAN2_IT1))
#else /* not HAL_ITLINE_FDCAN2_IT1 */
#define ifIsPendingIrqForFDCAN2_IT1() if ( false )
#endif /* not HAL_ITLINE_FDCAN2_IT1 */
#ifdef HAL_ITLINE_FLASH_ECC
#define ifIsPendingIrqForFLASH_ECC() if (__HAL_GET_PENDING_IT(HAL_ITLINE_FLASH_ECC))
#else /* not HAL_ITLINE_FLASH_ECC */
#define ifIsPendingIrqForFLASH_ECC() if ( false )
#endif /* not HAL_ITLINE_FLASH_ECC */
#ifdef HAL_ITLINE_FLASH_ITF
#define ifIsPendingIrqForFLASH_ITF() if (__HAL_GET_PENDING_IT(HAL_ITLINE_FLASH_ITF))
#else /* not HAL_ITLINE_FLASH_ITF */
#define ifIsPendingIrqForFLASH_ITF() if ( false )
#endif /* not HAL_ITLINE_FLASH_ITF */
#ifdef HAL_ITLINE_I2C1
#define ifIsPendingIrqForI2C1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_I2C1))
#else /* not HAL_ITLINE_I2C1 */
#define ifIsPendingIrqForI2C1() if ( false )
#endif /* not HAL_ITLINE_I2C1 */
#ifdef HAL_ITLINE_I2C2
#define ifIsPendingIrqForI2C2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_I2C2))
#else /* not HAL_ITLINE_I2C2 */
#define ifIsPendingIrqForI2C2() if ( false )
#endif /* not HAL_ITLINE_I2C2 */
#ifdef HAL_ITLINE_I2C3
#define ifIsPendingIrqForI2C3() if (__HAL_GET_PENDING_IT(HAL_ITLINE_I2C3))
#else /* not HAL_ITLINE_I2C3 */
#define ifIsPendingIrqForI2C3() if ( false )
#endif /* not HAL_ITLINE_I2C3 */
#ifdef HAL_ITLINE_LPTIM1
#define ifIsPendingIrqForLPTIM1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_LPTIM1))
#else /* not HAL_ITLINE_LPTIM1 */
#define ifIsPendingIrqForLPTIM1() if ( false )
#endif /* not HAL_ITLINE_LPTIM1 */
#ifdef HAL_ITLINE_LPTIM2
#define ifIsPendingIrqForLPTIM2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_LPTIM2))
#else /* not HAL_ITLINE_LPTIM2 */
#define ifIsPendingIrqForLPTIM2() if ( false )
#endif /* not HAL_ITLINE_LPTIM2 */
#ifdef HAL_ITLINE_LPUART1
#define ifIsPendingIrqForLPUART1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_LPUART1))
#else /* not HAL_ITLINE_LPUART1 */
#define ifIsPendingIrqForLPUART1() if ( false )
#endif /* not HAL_ITLINE_LPUART1 */
#ifdef HAL_ITLINE_LPUART2
#define ifIsPendingIrqForLPUART2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_LPUART2))
#else /* not HAL_ITLINE_LPUART2 */
#define ifIsPendingIrqForLPUART2() if ( false )
#endif /* not HAL_ITLINE_LPUART2 */
#ifdef HAL_ITLINE_PVDOUT
#define ifIsPendingIrqForPVDOUT() if (__HAL_GET_PENDING_IT(HAL_ITLINE_PVDOUT))
#else /* not HAL_ITLINE_PVDOUT */
#define ifIsPendingIrqForPVDOUT() if ( false )
#endif /* not HAL_ITLINE_PVDOUT */
#ifdef HAL_ITLINE_PVMOUT
#define ifIsPendingIrqForPVMOUT() if (__HAL_GET_PENDING_IT(HAL_ITLINE_PVMOUT))
#else /* not HAL_ITLINE_PVMOUT */
#define ifIsPendingIrqForPVMOUT() if ( false )
#endif /* not HAL_ITLINE_PVMOUT */
#ifdef HAL_ITLINE_VDDIO2
#define ifIsPendingIrqForVDDIO2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_VDDIO2))
#else /* not HAL_ITLINE_VDDIO2 */
#define ifIsPendingIrqForVDDIO2() if ( false )
#endif /* not HAL_ITLINE_VDDIO2 */
#ifdef HAL_ITLINE_RNG
#define ifIsPendingIrqForRNG() if (__HAL_GET_PENDING_IT(HAL_ITLINE_RNG))
#else /* not HAL_ITLINE_RNG */
#define ifIsPendingIrqForRNG() if ( false )
#endif /* not HAL_ITLINE_RNG */
#ifdef HAL_ITLINE_RTC
#define ifIsPendingIrqForRTC() if (__HAL_GET_PENDING_IT(HAL_ITLINE_RTC))
#else /* not HAL_ITLINE_RTC */
#define ifIsPendingIrqForRTC() if ( false )
#endif /* not HAL_ITLINE_RTC */
#ifdef HAL_ITLINE_RTC_ALRA
#define ifIsPendingIrqForRTC_ALRA() if (__HAL_GET_PENDING_IT(HAL_ITLINE_RTC_ALRA))
#else /* not HAL_ITLINE_RTC_ALRA */
#define ifIsPendingIrqForRTC_ALRA() if ( false )
#endif /* not HAL_ITLINE_RTC_ALRA */
#ifdef HAL_ITLINE_RTC_TSTAMP
#define ifIsPendingIrqForRTC_TSTAMP() if (__HAL_GET_PENDING_IT(HAL_ITLINE_RTC_TSTAMP))
#else /* not HAL_ITLINE_RTC_TSTAMP */
#define ifIsPendingIrqForRTC_TSTAMP() if ( false )
#endif /* not HAL_ITLINE_RTC_TSTAMP */
#ifdef HAL_ITLINE_RTC_WAKEUP
#define ifIsPendingIrqForRTC_WAKEUP() if (__HAL_GET_PENDING_IT(HAL_ITLINE_RTC_WAKEUP))
#else /* not HAL_ITLINE_RTC_WAKEUP */
#define ifIsPendingIrqForRTC_WAKEUP() if ( false )
#endif /* not HAL_ITLINE_RTC_WAKEUP */
#ifdef HAL_ITLINE_SPI1
#define ifIsPendingIrqForSPI1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_SPI1))
#else /* not HAL_ITLINE_SPI1 */
#define ifIsPendingIrqForSPI1() if ( false )
#endif /* not HAL_ITLINE_SPI1 */
#ifdef HAL_ITLINE_SPI2
#define ifIsPendingIrqForSPI2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_SPI2))
#else /* not HAL_ITLINE_SPI2 */
#define ifIsPendingIrqForSPI2() if ( false )
#endif /* not HAL_ITLINE_SPI2 */
#ifdef HAL_ITLINE_SPI3
#define ifIsPendingIrqForSPI3() if (__HAL_GET_PENDING_IT(HAL_ITLINE_SPI3))
#else /* not HAL_ITLINE_SPI3 */
#define ifIsPendingIrqForSPI3() if ( false )
#endif /* not HAL_ITLINE_SPI3 */
#ifdef HAL_ITLINE_TAMPER
#define ifIsPendingIrqForTAMPER() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TAMPER))
#else /* not HAL_ITLINE_TAMPER */
#define ifIsPendingIrqForTAMPER() if ( false )
#endif /* not HAL_ITLINE_TAMPER */
#ifdef HAL_ITLINE_TIM1_BRK
#define ifIsPendingIrqForTIM1_BRK() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM1_BRK))
#else /* not HAL_ITLINE_TIM1_BRK */
#define ifIsPendingIrqForTIM1_BRK() if ( false )
#endif /* not HAL_ITLINE_TIM1_BRK */
#ifdef HAL_ITLINE_TIM1_CC
#define ifIsPendingIrqForTIM1_CC() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM1_CC))
#else /* not HAL_ITLINE_TIM1_CC */
#define ifIsPendingIrqForTIM1_CC() if ( false )
#endif /* not HAL_ITLINE_TIM1_CC */
#ifdef HAL_ITLINE_TIM1_CCU
#define ifIsPendingIrqForTIM1_CCU() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM1_CCU))
#else /* not HAL_ITLINE_TIM1_CCU */
#define ifIsPendingIrqForTIM1_CCU() if ( false )
#endif /* not HAL_ITLINE_TIM1_CCU */
#ifdef HAL_ITLINE_TIM1_TRG
#define ifIsPendingIrqForTIM1_TRG() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM1_TRG))
#else /* not HAL_ITLINE_TIM1_TRG */
#define ifIsPendingIrqForTIM1_TRG() if ( false )
#endif /* not HAL_ITLINE_TIM1_TRG */
#ifdef HAL_ITLINE_TIM1_UPD
#define ifIsPendingIrqForTIM1_UPD() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM1_UPD))
#else /* not HAL_ITLINE_TIM1_UPD */
#define ifIsPendingIrqForTIM1_UPD() if ( false )
#endif /* not HAL_ITLINE_TIM1_UPD */
#ifdef HAL_ITLINE_TIM2
#define ifIsPendingIrqForTIM2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM2))
#else /* not HAL_ITLINE_TIM2 */
#define ifIsPendingIrqForTIM2() if ( false )
#endif /* not HAL_ITLINE_TIM2 */
#ifdef HAL_ITLINE_TIM3
#define ifIsPendingIrqForTIM3() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM3))
#else /* not HAL_ITLINE_TIM3 */
#define ifIsPendingIrqForTIM3() if ( false )
#endif /* not HAL_ITLINE_TIM3 */
#ifdef HAL_ITLINE_TIM4
#define ifIsPendingIrqForTIM4() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM4))
#else /* not HAL_ITLINE_TIM4 */
#define ifIsPendingIrqForTIM4() if ( false )
#endif /* not HAL_ITLINE_TIM4 */
#ifdef HAL_ITLINE_TIM6
#define ifIsPendingIrqForTIM6() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM6))
#else /* not HAL_ITLINE_TIM6 */
#define ifIsPendingIrqForTIM6() if ( false )
#endif /* not HAL_ITLINE_TIM6 */
#ifdef HAL_ITLINE_TIM7
#define ifIsPendingIrqForTIM7() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM7))
#else /* not HAL_ITLINE_TIM7 */
#define ifIsPendingIrqForTIM7() if ( false )
#endif /* not HAL_ITLINE_TIM7 */
#ifdef HAL_ITLINE_TIM14
#define ifIsPendingIrqForTIM14() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM14))
#else /* not HAL_ITLINE_TIM14 */
#define ifIsPendingIrqForTIM14() if ( false )
#endif /* not HAL_ITLINE_TIM14 */
#ifdef HAL_ITLINE_TIM15
#define ifIsPendingIrqForTIM15() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM15))
#else /* not HAL_ITLINE_TIM15 */
#define ifIsPendingIrqForTIM15() if ( false )
#endif /* not HAL_ITLINE_TIM15 */
#ifdef HAL_ITLINE_TIM16
#define ifIsPendingIrqForTIM16() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM16))
#else /* not HAL_ITLINE_TIM16 */
#define ifIsPendingIrqForTIM16() if ( false )
#endif /* not HAL_ITLINE_TIM16 */
#ifdef HAL_ITLINE_TIM17
#define ifIsPendingIrqForTIM17() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TIM17))
#else /* not HAL_ITLINE_TIM17 */
#define ifIsPendingIrqForTIM17() if ( false )
#endif /* not HAL_ITLINE_TIM17 */
#ifdef HAL_ITLINE_TSC_EOA
#define ifIsPendingIrqForTSC_EOA() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TSC_EOA))
#else /* not HAL_ITLINE_TSC_EOA */
#define ifIsPendingIrqForTSC_EOA() if ( false )
#endif /* not HAL_ITLINE_TSC_EOA */
#ifdef HAL_ITLINE_TSC_MCE
#define ifIsPendingIrqForTSC_MCE() if (__HAL_GET_PENDING_IT(HAL_ITLINE_TSC_MCE))
#else /* not HAL_ITLINE_TSC_MCE */
#define ifIsPendingIrqForTSC_MCE() if ( false )
#endif /* not HAL_ITLINE_TSC_MCE */
#ifdef HAL_ITLINE_UCPD1
#define ifIsPendingIrqForUCPD1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_UCPD1))
#else /* not HAL_ITLINE_UCPD1 */
#define ifIsPendingIrqForUCPD1() if ( false )
#endif /* not HAL_ITLINE_UCPD1 */
#ifdef HAL_ITLINE_UCPD2
#define ifIsPendingIrqForUCPD2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_UCPD2))
#else /* not HAL_ITLINE_UCPD2 */
#define ifIsPendingIrqForUCPD2() if ( false )
#endif /* not HAL_ITLINE_UCPD2 */
#ifdef HAL_ITLINE_USART1
#define ifIsPendingIrqForUSART1() if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART1))
#else /* not HAL_ITLINE_USART1 */
#define ifIsPendingIrqForUSART1() if ( false )
#endif /* not HAL_ITLINE_USART1 */
#ifdef HAL_ITLINE_USART2
#define ifIsPendingIrqForUSART2() if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART2))
#else /* not HAL_ITLINE_USART2 */
#define ifIsPendingIrqForUSART2() if ( false )
#endif /* not HAL_ITLINE_USART2 */
#ifdef HAL_ITLINE_USART3
#define ifIsPendingIrqForUSART3() if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART3))
#else /* not HAL_ITLINE_USART3 */
#define ifIsPendingIrqForUSART3() if ( false )
#endif /* not HAL_ITLINE_USART3 */
#ifdef HAL_ITLINE_USART4
#define ifIsPendingIrqForUSART4() if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART4))
#else /* not HAL_ITLINE_USART4 */
#define ifIsPendingIrqForUSART4() if ( false )
#endif /* not HAL_ITLINE_USART4 */
#ifdef HAL_ITLINE_USART5
#define ifIsPendingIrqForUSART5() if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART5))
#else /* not HAL_ITLINE_USART5 */
#define ifIsPendingIrqForUSART5() if ( false )
#endif /* not HAL_ITLINE_USART5 */
#ifdef HAL_ITLINE_USART6
#define ifIsPendingIrqForUSART6() if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART6))
#else /* not HAL_ITLINE_USART6 */
#define ifIsPendingIrqForUSART6() if ( false )
#endif /* not HAL_ITLINE_USART6 */
#ifdef HAL_ITLINE_USART7
#define ifIsPendingIrqForUSART7() if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART7))
#else /* not HAL_ITLINE_USART7 */
#define ifIsPendingIrqForUSART7() if ( false )
#endif /* not HAL_ITLINE_USART7 */
#ifdef HAL_ITLINE_USART8
#define ifIsPendingIrqForUSART8() if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART8))
#else /* not HAL_ITLINE_USART8 */
#define ifIsPendingIrqForUSART8() if ( false )
#endif /* not HAL_ITLINE_USART8 */
#ifdef HAL_ITLINE_USB
#define ifIsPendingIrqForUSB() if (__HAL_GET_PENDING_IT(HAL_ITLINE_USB))
#else /* not HAL_ITLINE_USB */
#define ifIsPendingIrqForUSB() if ( false )
#endif /* not HAL_ITLINE_USB */
#else /* not __HAL_GET_PENDING_IT */
#define ifIsPendingIrqForADC()
#define ifIsPendingIrqForAES()
#define ifIsPendingIrqForCAN()
#define ifIsPendingIrqForCEC()
#define ifIsPendingIrqForCLK_CTRL()
#define ifIsPendingIrqForCOMP1()
#define ifIsPendingIrqForCOMP2()
#define ifIsPendingIrqForCOMP3()
#define ifIsPendingIrqForCRS()
#define ifIsPendingIrqForDAC()
#define ifIsPendingIrqForDMAMUX1()
#define ifIsPendingIrqForDMA1_CH1()
#define ifIsPendingIrqForDMA1_CH2()
#define ifIsPendingIrqForDMA1_CH3()
#define ifIsPendingIrqForDMA1_CH4()
#define ifIsPendingIrqForDMA1_CH5()
#define ifIsPendingIrqForDMA1_CH6()
#define ifIsPendingIrqForDMA1_CH7()
#define ifIsPendingIrqForDMA2_CH1()
#define ifIsPendingIrqForDMA2_CH2()
#define ifIsPendingIrqForDMA2_CH3()
#define ifIsPendingIrqForDMA2_CH4()
#define ifIsPendingIrqForDMA2_CH5()
#define ifIsPendingIrqForEWDG()
#define ifIsPendingIrqForWWDG()
#define ifIsPendingIrqForEXTI0()
#define ifIsPendingIrqForEXTI1()
#define ifIsPendingIrqForEXTI2()
#define ifIsPendingIrqForEXTI3()
#define ifIsPendingIrqForEXTI4()
#define ifIsPendingIrqForEXTI5()
#define ifIsPendingIrqForEXTI6()
#define ifIsPendingIrqForEXTI7()
#define ifIsPendingIrqForEXTI8()
#define ifIsPendingIrqForEXTI9()
#define ifIsPendingIrqForEXTI10()
#define ifIsPendingIrqForEXTI11()
#define ifIsPendingIrqForEXTI12()
#define ifIsPendingIrqForEXTI13()
#define ifIsPendingIrqForEXTI14()
#define ifIsPendingIrqForEXTI15()
#define ifIsPendingIrqForFDCAN1_IT0()
#define ifIsPendingIrqForFDCAN1_IT1()
#define ifIsPendingIrqForFDCAN2_IT0()
#define ifIsPendingIrqForFDCAN2_IT1()
#define ifIsPendingIrqForFLASH_ECC()
#define ifIsPendingIrqForFLASH_ITF()
#define ifIsPendingIrqForI2C1()
#define ifIsPendingIrqForI2C2()
#define ifIsPendingIrqForI2C3()
#define ifIsPendingIrqForLPTIM1()
#define ifIsPendingIrqForLPTIM2()
#define ifIsPendingIrqForLPUART1()
#define ifIsPendingIrqForLPUART2()
#define ifIsPendingIrqForPVDOUT()
#define ifIsPendingIrqForPVMOUT()
#define ifIsPendingIrqForVDDIO2()
#define ifIsPendingIrqForRNG()
#define ifIsPendingIrqForRTC()
#define ifIsPendingIrqForRTC_ALRA()
#define ifIsPendingIrqForRTC_TSTAMP()
#define ifIsPendingIrqForRTC_WAKEUP()
#define ifIsPendingIrqForSPI1()
#define ifIsPendingIrqForSPI2()
#define ifIsPendingIrqForSPI3()
#define ifIsPendingIrqForTAMPER()
#define ifIsPendingIrqForTIM1_BRK()
#define ifIsPendingIrqForTIM1_CC()
#define ifIsPendingIrqForTIM1_CCU()
#define ifIsPendingIrqForTIM1_TRG()
#define ifIsPendingIrqForTIM1_UPD()
#define ifIsPendingIrqForTIM2()
#define ifIsPendingIrqForTIM3()
#define ifIsPendingIrqForTIM4()
#define ifIsPendingIrqForTIM6()
#define ifIsPendingIrqForTIM7()
#define ifIsPendingIrqForTIM14()
#define ifIsPendingIrqForTIM15()
#define ifIsPendingIrqForTIM16()
#define ifIsPendingIrqForTIM17()
#define ifIsPendingIrqForTSC_EOA()
#define ifIsPendingIrqForTSC_MCE()
#define ifIsPendingIrqForUCPD1()
#define ifIsPendingIrqForUCPD2()
#define ifIsPendingIrqForUSART1()
#define ifIsPendingIrqForUSART2()
#define ifIsPendingIrqForUSART3()
#define ifIsPendingIrqForUSART4()
#define ifIsPendingIrqForUSART5()
#define ifIsPendingIrqForUSART6()
#define ifIsPendingIrqForUSART7()
#define ifIsPendingIrqForUSART8()
#define ifIsPendingIrqForUSB()
#endif /* not __HAL_GET_PENDING_IT */


/* Ensure that there are no macro re-mappings enabled. */
#undef ADC_IRQHandler
#undef ADC1_COMP_IRQHandler
#undef ADC1_IRQHandler
#undef ADC1_2_IRQHandler
#undef ADC2_IRQHandler
#undef ADC3_IRQHandler
#undef ADC4_IRQHandler
#undef ADC5_IRQHandler

#undef AES_IRQHandler
#undef AES_RNG_IRQHandler
#undef AES_RNG_LPUART1_IRQHandler
#undef AES1_IRQHandler
#undef AES2_IRQHandler

#undef BDMA_Channel0_IRQHandler
#undef BDMA_Channel1_IRQHandler
#undef BDMA_Channel2_IRQHandler
#undef BDMA_Channel3_IRQHandler
#undef BDMA_Channel4_IRQHandler
#undef BDMA_Channel5_IRQHandler
#undef BDMA_Channel6_IRQHandler
#undef BDMA_Channel7_IRQHandler
#undef BDMA1_IRQHandler
#undef BDMA2_Channel0_IRQHandler
#undef BDMA2_Channel1_IRQHandler
#undef BDMA2_Channel2_IRQHandler
#undef BDMA2_Channel3_IRQHandler
#undef BDMA2_Channel4_IRQHandler
#undef BDMA2_Channel5_IRQHandler
#undef BDMA2_Channel6_IRQHandler
#undef BDMA2_Channel7_IRQHandler

#undef CAN_RX0_IRQHandler
#undef CAN_RX1_IRQHandler
#undef CAN_SCE_IRQHandler
#undef CAN_TX_IRQHandler
#undef CAN1_RX0_IRQHandler
#undef CAN1_RX1_IRQHandler
#undef CAN1_SCE_IRQHandler
#undef CAN1_TX_IRQHandler
#undef CAN2_RX0_IRQHandler
#undef CAN2_RX1_IRQHandler
#undef CAN2_SCE_IRQHandler
#undef CAN2_TX_IRQHandler
#undef CAN3_RX0_IRQHandler
#undef CAN3_RX1_IRQHandler
#undef CAN3_SCE_IRQHandler
#undef CAN3_TX_IRQHandler

#undef CEC_CAN_IRQHandler
#undef CEC_IRQHandler

#undef COMP_ACQ_IRQHandler
#undef COMP_IRQHandler
#undef COMP1_2_IRQHandler
#undef COMP1_2_3_IRQHandler
#undef COMP2_IRQHandler
#undef COMP4_IRQHandler
#undef COMP4_5_6_IRQHandler
#undef COMP4_6_IRQHandler
#undef COMP7_IRQHandler

#undef CORDIC_IRQHandler

#undef CRS_IRQHandler

#undef CRYP_IRQHandler
#undef CRYP1_IRQHandler
#undef CRYP2_IRQHandler

#undef DAC_IRQHandler
#undef DAC2_IRQHandler

#undef DCMI_IRQHandler
#undef DCMI_PSSI_IRQHandler

#undef DFSDM1_FLT0_IRQHandler
#undef DFSDM1_FLT1_IRQHandler
#undef DFSDM1_FLT2_IRQHandler
#undef DFSDM1_FLT3_IRQHandler
#undef DFSDM1_FLT4_IRQHandler
#undef DFSDM1_FLT5_IRQHandler
#undef DFSDM1_FLT6_IRQHandler
#undef DFSDM1_FLT7_IRQHandler
#undef DFSDM2_FLT0_IRQHandler
#undef DFSDM2_FLT1_IRQHandler
#undef DFSDM2_FLT2_IRQHandler
#undef DFSDM2_FLT3_IRQHandler
#undef DFSDM2_IRQHandler

#undef DMAMUX_OVR_IRQHandler
#undef DMAMUX1_IRQHandler
#undef DMAMUX1_OVR_IRQHandler
#undef DMAMUX1_S_IRQHandler
#undef DMAMUX2_OVR_IRQHandler

#undef DMA1_Channel1_IRQHandler
#undef DMA1_Channel2_IRQHandler
#undef DMA1_Channel2_3_IRQHandler
#undef DMA1_Channel3_IRQHandler
#undef DMA1_Channel4_IRQHandler
#undef DMA1_Channel4_5_IRQHandler
#undef DMA1_Channel4_5_6_7_IRQHandler
#undef DMA1_Channel5_IRQHandler
#undef DMA1_Channel6_IRQHandler
#undef DMA1_Channel7_IRQHandler
#undef DMA1_Channel8_IRQHandler
#undef DMA1_Ch1_IRQHandler
#undef DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler
#undef DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler
#undef DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler
#undef DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler
#undef DMA1_Stream0_IRQHandler
#undef DMA1_Stream1_IRQHandler
#undef DMA1_Stream2_IRQHandler
#undef DMA1_Stream3_IRQHandler
#undef DMA1_Stream4_IRQHandler
#undef DMA1_Stream5_IRQHandler
#undef DMA1_Stream6_IRQHandler
#undef DMA1_Stream7_IRQHandler
#undef DMA2D_IRQHandler
#undef DMA2_Channel1_IRQHandler
#undef DMA2_Channel2_IRQHandler
#undef DMA2_Channel3_IRQHandler
#undef DMA2_Channel4_IRQHandler
#undef DMA2_Channel4_5_IRQHandler
#undef DMA2_Channel5_IRQHandler
#undef DMA2_Channel6_IRQHandler
#undef DMA2_Channel7_IRQHandler
#undef DMA2_Channel8_IRQHandler
#undef DMA2_Stream0_IRQHandler
#undef DMA2_Stream1_IRQHandler
#undef DMA2_Stream2_IRQHandler
#undef DMA2_Stream3_IRQHandler
#undef DMA2_Stream4_IRQHandler
#undef DMA2_Stream5_IRQHandler
#undef DMA2_Stream6_IRQHandler
#undef DMA2_Stream7_IRQHandler

#undef DSI_IRQHandler

#undef DTS_IRQHandler

#undef ETH_IRQHandler
#undef ETH_WKUP_IRQHandler
#undef ETH1_IRQHandler
#undef ETH1_LPI_IRQHandler
#undef ETH1_WKUP_IRQHandler

#undef EXTI0_IRQHandler
#undef EXTI0_1_IRQHandler
#undef EXTI1_IRQHandler
#undef EXTI2_IRQHandler
#undef EXTI2_TSC_IRQHandler
#undef EXTI2_3_IRQHandler
#undef EXTI3_IRQHandler
#undef EXTI4_IRQHandler
#undef EXTI4_15_IRQHandler
#undef EXTI5_IRQHandler
#undef EXTI6_IRQHandler
#undef EXTI7_IRQHandler
#undef EXTI8_IRQHandler
#undef EXTI9_IRQHandler
#undef EXTI9_5_IRQHandler
#undef EXTI10_IRQHandler
#undef EXTI11_IRQHandler
#undef EXTI12_IRQHandler
#undef EXTI13_IRQHandler
#undef EXTI14_IRQHandler
#undef EXTI15_IRQHandler
#undef EXTI15_10_IRQHandler

#undef FDCAN_CAL_IRQHandler
#undef FDCAN1_IT0_IRQHandler
#undef FDCAN1_IT1_IRQHandler
#undef FDCAN2_IT0_IRQHandler
#undef FDCAN2_IT1_IRQHandler
#undef FDCAN3_IT0_IRQHandler
#undef FDCAN3_IT1_IRQHandler

#undef FLASH_IRQHandler
#undef FLASH_S_IRQHandler

#undef FMAC_IRQHandler

#undef FMC_IRQHandler

#undef FMPI2C1_ER_IRQHandler
#undef FMPI2C1_EV_IRQHandler

#undef FSMC_IRQHandler

#undef GFXMMU_IRQHandler

#undef GPU_IRQHandler

#undef GTZC_IRQHandler

#undef HASH_CRS_IRQHandler
#undef HASH_IRQHandler
#undef HASH_RNG_IRQHandler
#undef HASH1_IRQHandler
#undef HASH2_IRQHandler

#undef HRTIM1_FLT_IRQHandler
#undef HRTIM1_Master_IRQHandler
#undef HRTIM1_TIMA_IRQHandler
#undef HRTIM1_TIMB_IRQHandler
#undef HRTIM1_TIMC_IRQHandler
#undef HRTIM1_TIMD_IRQHandler
#undef HRTIM1_TIME_IRQHandler
#undef HRTIM1_TIMF_IRQHandler

#undef HSEM_IRQHandler

#undef ICACHE_IRQHandler

#undef IPCC_C1_RX_IRQHandler
#undef IPCC_C1_TX_IRQHandler
#undef IPCC_RX0_IRQHandler
#undef IPCC_RX1_IRQHandler
#undef IPCC_TX0_IRQHandler
#undef IPCC_TX1_IRQHandler

#undef I2C1_ER_IRQHandler
#undef I2C1_EV_IRQHandler
#undef I2C1_IRQHandler
#undef I2C2_ER_IRQHandler
#undef I2C2_EV_IRQHandler
#undef I2C2_IRQHandler
#undef I2C3_ER_IRQHandler
#undef I2C3_EV_IRQHandler
#undef I2C3_IRQHandler
#undef I2C4_ER_IRQHandler
#undef I2C4_EV_IRQHandler
#undef I2C5_ER_IRQHandler
#undef I2C5_EV_IRQHandler
#undef I2C6_ER_IRQHandler
#undef I2C6_EV_IRQHandler

#undef JPEG_IRQHandler

#undef LCD_IRQHandler

#undef LPTIM1_IRQHandler
#undef LPTIM2_IRQHandler
#undef LPTIM3_IRQHandler
#undef LPTIM4_IRQHandler
#undef LPTIM5_IRQHandler

#undef LPUART1_IRQHandler

#undef LTDC_ER_IRQHandler
#undef LTDC_IRQHandler

#undef MDIOS_IRQHandler
#undef MDIOS_WKUP_IRQHandler

#undef MDMA_IRQHandler

#undef OCTOSPI1_IRQHandler
#undef OCTOSPI2_IRQHandler

#undef OTFDEC1_IRQHandler
#undef OTFDEC2_IRQHandler

#undef OTG_FS_EP1_IN_IRQHandler
#undef OTG_FS_EP1_OUT_IRQHandler
#undef OTG_FS_IRQHandler
#undef OTG_FS_WKUP_IRQHandler
#undef OTG_HS_EP1_IN_IRQHandler
#undef OTG_HS_EP1_OUT_IRQHandler
#undef OTG_HS_IRQHandler
#undef OTG_HS_WKUP_IRQHandler
#undef OTG_IRQHandler

#undef PKA_IRQHandler

#undef PVD_AVD_IRQHandler

#undef QUADSPI_IRQHandler

#undef RCC_CRS_IRQHandler
#undef RCC_IRQHandler
#undef RCC_S_IRQHandler
#undef RCC_WAKEUP_IRQHandler

#undef RNG_IRQHandler
#undef RNG_LPUART1_IRQHandler
#undef RNG1_IRQHandler
#undef RNG2_IRQHandler

#undef RTC_Alarm_IRQHandler
#undef RTC_IRQHandler
#undef RTC_S_IRQHandler
#undef RTC_TAMP_IRQHandler
#undef RTC_TAMP_LSECSS_IRQHandler
#undef RTC_TAMP_STAMP_CSS_LSE_IRQHandler
#undef RTC_TIMESTAMP_IRQHandler
#undef RTC_WKUP_ALARM_IRQHandler
#undef RTC_WKUP_IRQHandler

#undef SAI1_IRQHandler
#undef SAI2_IRQHandler
#undef SAI3_IRQHandler
#undef SAI4_IRQHandler

#undef SDADC1_IRQHandler
#undef SDADC2_IRQHandler
#undef SDADC3_IRQHandler

#undef SDIO_IRQHandler

#undef SDMMC1_IRQHandler
#undef SDMMC2_IRQHandler
#undef SDMMC3_IRQHandler

#undef SPDIF_RX_IRQHandler

#undef SPI1_IRQHandler
#undef SPI2_IRQHandler
#undef SPI3_IRQHandler
#undef SPI4_IRQHandler
#undef SPI5_IRQHandler
#undef SPI6_IRQHandler

#undef SWPMI1_IRQHandler

#undef TAMPER_IRQHandler
#undef TAMPER_STAMP_IRQHandler

#undef TAMP_IRQHandler
#undef TAMP_STAMP_IRQHandler
#undef TAMP_STAMP_LSECSS_IRQHandler
#undef TAMP_S_IRQHandler

#undef TEMP_IRQHandler

#undef TIM1_BRK_IRQHandler
#undef TIM1_BRK_TIM9_IRQHandler
#undef TIM1_BRK_TIM15_IRQHandler
#undef TIM1_BRK_UP_TRG_COM_IRQHandler
#undef TIM1_CC_IRQHandler
#undef TIM1_TRG_COM_IRQHandler
#undef TIM1_TRG_COM_TIM11_IRQHandler
#undef TIM1_TRG_COM_TIM17_IRQHandler
#undef TIM1_UP_IRQHandler
#undef TIM1_UP_TIM10_IRQHandler
#undef TIM1_UP_TIM16_IRQHandler
#undef TIM2_IRQHandler
#undef TIM3_IRQHandler
#undef TIM4_IRQHandler
#undef TIM5_IRQHandler
#undef TIM6_DAC_IRQHandler
#undef TIM6_DAC_LPTIM1_IRQHandler
#undef TIM6_DAC1_IRQHandler
#undef TIM6_IRQHandler
#undef TIM7_DAC_IRQHandler
#undef TIM7_DAC2_IRQHandler
#undef TIM7_IRQHandler
#undef TIM7_LPTIM2_IRQHandler
#undef TIM8_BRK_IRQHandler
#undef TIM8_BRK_TIM12_IRQHandler
#undef TIM8_CC_IRQHandler
#undef TIM8_TRG_COM_IRQHandler
#undef TIM8_TRG_COM_TIM14_IRQHandler
#undef TIM8_UP_IRQHandler
#undef TIM8_UP_TIM13_IRQHandler
#undef TIM9_IRQHandler
#undef TIM10_IRQHandler
#undef TIM11_IRQHandler
#undef TIM12_IRQHandler
#undef TIM13_IRQHandler
#undef TIM14_IRQHandler
#undef TIM15_IRQHandler
#undef TIM16_IRQHandler
#undef TIM17_IRQHandler
#undef TIM18_DAC2_IRQHandler
#undef TIM19_IRQHandler
#undef TIM20_BRK_IRQHandler
#undef TIM20_CC_IRQHandler
#undef TIM20_TRG_COM_IRQHandler
#undef TIM20_UP_IRQHandler
#undef TIM21_IRQHandler
#undef TIM22_IRQHandler
#undef TIM23_IRQHandler
#undef TIM24_IRQHandler

#undef TSC_IRQHandler

#undef UART4_IRQHandler
#undef UART5_IRQHandler
#undef UART7_IRQHandler
#undef UART8_IRQHandler
#undef UART9_IRQHandler
#undef UART10_IRQHandler

#undef UCPD1_IRQHandler
#undef UCPD1_2_IRQHandler

#undef USART1_IRQHandler
#undef USART2_IRQHandler
#undef USART3_IRQHandler
#undef USART3_4_IRQHandler
#undef USART3_4_LPUART1_IRQHandler
#undef USART3_6_IRQHandler
#undef USART3_8_IRQHandler
#undef USART4_5_IRQHandler
#undef USART6_IRQHandler
#undef USART10_IRQHandler

#undef USBH_EHCI_IRQHandler
#undef USBH_OHCI_IRQHandler

#undef USBWakeUp_IRQHandler
#undef USBWakeUp_RMP_IRQHandler

#undef USB_FS_IRQHandler
#undef USB_FS_WKUP_IRQHandler
#undef USB_HP_CAN_TX_IRQHandler
#undef USB_HP_CAN1_TX_IRQHandler
#undef USB_HP_IRQHandler
#undef USB_IRQHandler
#undef USB_LP_CAN_RX0_IRQHandler
#undef USB_LP_CAN1_RX0_IRQHandler
#undef USB_LP_IRQHandler

#undef WAKEUP_PIN_IRQHandler

#undef WWDG_IRQHandler
#undef WWDG_RST_IRQHandler
#undef WWDG1_IRQHandler


extern "C" {
extern const char _wiring_irq; /**< @see _wiring_irq_forced_linkage in Arduino.cpp */
const char _wiring_irq = 1;

/* Remapped IRQ routines. One per STM32 HAL instance. Empty fallback implementation is provided here. */
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForADC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForADC1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForADC2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForADC3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForADC4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForADC5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForAES(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForAES1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForAES2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA_CH0(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA_CH1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA_CH2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA_CH3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA_CH4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA_CH5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA_CH6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA_CH7(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA1_CH0(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA1_CH1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA1_CH2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA1_CH3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA1_CH4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA1_CH5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA1_CH6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA1_CH7(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA2_CH0(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA2_CH1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA2_CH2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA2_CH3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA2_CH4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA2_CH5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA2_CH6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA2_CH7(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForBDMA2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCAN(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCAN1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCAN2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCAN3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCEC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCLK_CTRL(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCOMP1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCOMP2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCOMP3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCOMP4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCOMP5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCOMP6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCOMP7(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCORDIC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCRS(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCRYP(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCRYP1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForCRYP2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDAC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDAC1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDAC2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDAC3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDAC4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDCMI(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDFSDM1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDFSDM2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA1_CH0(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA1_CH1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA1_CH2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA1_CH3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA1_CH4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA1_CH5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA1_CH6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA1_CH7(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA1_CH8(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA2D(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA2_CH0(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA2_CH1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA2_CH2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA2_CH3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA2_CH4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA2_CH5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA2_CH6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA2_CH7(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDMA2_CH8(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDSIHOST(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForDTS(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForETH(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForETH1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI0(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI7(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI8(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI9(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI10(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI11(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI12(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI13(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI14(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForEXTI15(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForFDCAN1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForFDCAN2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForFDCAN3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForFLASH(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForFMAC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForFMC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForFMPI2C1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForFSMC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForGFXMMU(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForGPU(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForGTZC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForHASH(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForHASH1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForHASH2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForHDMI_CEC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForHRTIM(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForHRTIM1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForHSEM(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForICACHE(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForIPCC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2C1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2C2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2C3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2C4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2C5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2C6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2S1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2S2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2S3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2S4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2S5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForI2S6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForJPEG(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForLCD(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForLPTIM1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForLPTIM2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForLPTIM3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForLPTIM4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForLPTIM5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForLPUART1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForLPUART2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForLTDC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForMDIOS(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForMDMA(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForOCTOSPI1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForOCTOSPI2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForOTFDEC1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForOTFDEC2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForPKA(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForPSSI(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForPWR(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForQUADSPI(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForQUADSPI1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForRCC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForRNG(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForRNG1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForRNG2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForRTC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSAI1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSAI2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSAI3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSAI4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSDADC1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSDADC2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSDADC3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSDIO(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSDMMC1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSDMMC2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSDMMC3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSPDIFRX(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSPDIFRX1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSPI1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSPI2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSPI3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSPI4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSPI5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSPI6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForSWPMI1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTAMPER(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTAMP(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTEMPSENS(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM7(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM8(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM9(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM10(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM11(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM12(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM13(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM14(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM15(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM16(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM17(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM18(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM19(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM20(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM21(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM22(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM23(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTIM24(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForTSC(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUART4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUART5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUART7(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUART8(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUART9(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUART10(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUCPD1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUCPD2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSART1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSART2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSART3(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSART4(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSART5(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSART6(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSART7(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSART8(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSART10(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSBH_HS1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSBH_HS2(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSB_OTG_FS(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSB_OTG_HS(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForUSB(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForWWDG(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForWWDG1(void) {}
__attribute__((weak)) void STM32CubeDuinoIrqHandlerForWWDG2(void) {}


#ifndef STM32CUBEDUINO_CUSTOM_IRQ_HANDLERS
/* STM32CubeDuinoIrqHandlerForXXX() wrapper implementation of all STM32 HAL IRQ handlers. Unused functions are removed by the linker. */
#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ADC) && defined(ADC_SOFTWARE_START))
/** ADC1, ADC2 and ADC3 global interrupts */
void ADC_IRQHandler(void) {
	clearNvicIrqFor(ADC_IRQn);
	STM32CubeDuinoIrqHandlerForADC1();
	STM32CubeDuinoIrqHandlerForADC2();
	STM32CubeDuinoIrqHandlerForADC3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or ADC_SOFTWARE_START */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ADC) && defined(ADC_SOFTWARE_START))
/** ADC1, COMP1 and COMP2 interrupts (COMP interrupts through EXTI lines 17 and 18) */
void ADC1_COMP_IRQHandler(void) {
	clearNvicIrqFor(ADC1_COMP_IRQn);
	ifIsPendingIrqForADC() STM32CubeDuinoIrqHandlerForADC();
	STM32CubeDuinoIrqHandlerForADC1();
	ifIsPendingIrqForCOMP1() STM32CubeDuinoIrqHandlerForCOMP1();
	ifIsPendingIrqForCOMP2() STM32CubeDuinoIrqHandlerForCOMP2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or ADC_SOFTWARE_START */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ADC) && defined(ADC_SOFTWARE_START))
/** ADC1 global interrupt */
void ADC1_IRQHandler(void) {
	clearNvicIrqFor(ADC1_IRQn);
	ifIsPendingIrqForADC() STM32CubeDuinoIrqHandlerForADC();
	STM32CubeDuinoIrqHandlerForADC1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or ADC_SOFTWARE_START */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ADC) && defined(ADC_SOFTWARE_START))
/** ADC1 and ADC2 global interrupts */
void ADC1_2_IRQHandler(void) {
	clearNvicIrqFor(ADC1_2_IRQn);
	STM32CubeDuinoIrqHandlerForADC1();
	STM32CubeDuinoIrqHandlerForADC2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or ADC_SOFTWARE_START */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ADC) && defined(ADC_SOFTWARE_START))
/** ADC2 global interrupt */
void ADC2_IRQHandler(void) {
	clearNvicIrqFor(ADC2_IRQn);
	STM32CubeDuinoIrqHandlerForADC2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or ADC_SOFTWARE_START */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ADC) && defined(ADC_SOFTWARE_START))
/** ADC3 global interrupt */
void ADC3_IRQHandler(void) {
	clearNvicIrqFor(ADC3_IRQn);
	STM32CubeDuinoIrqHandlerForADC3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or ADC_SOFTWARE_START */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ADC) && defined(ADC_SOFTWARE_START))
/** ADC4 global interrupt */
void ADC4_IRQHandler(void) {
	clearNvicIrqFor(ADC4_IRQn);
	STM32CubeDuinoIrqHandlerForADC4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or ADC_SOFTWARE_START */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ADC) && defined(ADC_SOFTWARE_START))
/** ADC5 global interrupt */
void ADC5_IRQHandler(void) {
	clearNvicIrqFor(ADC5_IRQn);
	STM32CubeDuinoIrqHandlerForADC5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or ADC_SOFTWARE_START */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** AES global interrupt */
void AES_IRQHandler(void) {
	clearNvicIrqFor(AES_IRQn);
	ifIsPendingIrqForAES() STM32CubeDuinoIrqHandlerForAES();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** AES and RNG interrupts */
void AES_RNG_IRQHandler(void) {
	clearNvicIrqFor(AES_RNG_IRQn);
	ifIsPendingIrqForAES() STM32CubeDuinoIrqHandlerForAES();
	ifIsPendingIrqForRNG() STM32CubeDuinoIrqHandlerForRNG();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** AES, RNG and LPUART1 interrupts / LPUART1 wake-up interrupt through EXTI line 28 */
void AES_RNG_LPUART1_IRQHandler(void) {
	clearNvicIrqFor(AES_RNG_LPUART1_IRQn);
	ifIsPendingIrqForAES() STM32CubeDuinoIrqHandlerForAES();
	ifIsPendingIrqForLPUART1() STM32CubeDuinoIrqHandlerForLPUART1();
	ifIsPendingIrqForRNG() STM32CubeDuinoIrqHandlerForRNG();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** AES1 global interrupt */
void AES1_IRQHandler(void) {
	clearNvicIrqFor(AES1_IRQn);
	STM32CubeDuinoIrqHandlerForAES1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** AES2 global interrupt */
void AES2_IRQHandler(void) {
	clearNvicIrqFor(AES2_IRQn);
	STM32CubeDuinoIrqHandlerForAES2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA channel0 global interrupt */
void BDMA_Channel0_IRQHandler(void) {
	clearNvicIrqFor(BDMA_Channel0_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA_CH0();
	STM32CubeDuinoIrqHandlerForBDMA2_CH0();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA channel1 global interrupt */
void BDMA_Channel1_IRQHandler(void) {
	clearNvicIrqFor(BDMA_Channel1_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA_CH1();
	STM32CubeDuinoIrqHandlerForBDMA2_CH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA channel2 global interrupt */
void BDMA_Channel2_IRQHandler(void) {
	clearNvicIrqFor(BDMA_Channel2_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA_CH2();
	STM32CubeDuinoIrqHandlerForBDMA2_CH2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA channel3 global interrupt */
void BDMA_Channel3_IRQHandler(void) {
	clearNvicIrqFor(BDMA_Channel3_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA_CH3();
	STM32CubeDuinoIrqHandlerForBDMA2_CH3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA channel4 global interrupt */
void BDMA_Channel4_IRQHandler(void) {
	clearNvicIrqFor(BDMA_Channel4_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA_CH4();
	STM32CubeDuinoIrqHandlerForBDMA2_CH4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA channel5 global interrupt */
void BDMA_Channel5_IRQHandler(void) {
	clearNvicIrqFor(BDMA_Channel5_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA_CH5();
	STM32CubeDuinoIrqHandlerForBDMA2_CH5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA channel6 global interrupt */
void BDMA_Channel6_IRQHandler(void) {
	clearNvicIrqFor(BDMA_Channel6_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA_CH6();
	STM32CubeDuinoIrqHandlerForBDMA2_CH6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA channel7 global interrupt */
void BDMA_Channel7_IRQHandler(void) {
	clearNvicIrqFor(BDMA_Channel7_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA_CH7();
	STM32CubeDuinoIrqHandlerForBDMA2_CH7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA1 for DFSDM global interrupt */
void BDMA1_IRQHandler(void) {
	clearNvicIrqFor(BDMA1_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA1_CH0();
	STM32CubeDuinoIrqHandlerForBDMA1_CH1();
	STM32CubeDuinoIrqHandlerForBDMA1_CH2();
	STM32CubeDuinoIrqHandlerForBDMA1_CH3();
	STM32CubeDuinoIrqHandlerForBDMA1_CH4();
	STM32CubeDuinoIrqHandlerForBDMA1_CH5();
	STM32CubeDuinoIrqHandlerForBDMA1_CH6();
	STM32CubeDuinoIrqHandlerForBDMA1_CH7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA2 channel0 global interrupt */
void BDMA2_Channel0_IRQHandler(void) {
	clearNvicIrqFor(BDMA2_Channel0_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA2_CH0();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA2 channel1 global interrupt */
void BDMA2_Channel1_IRQHandler(void) {
	clearNvicIrqFor(BDMA2_Channel1_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA2_CH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA2 channel2 global interrupt */
void BDMA2_Channel2_IRQHandler(void) {
	clearNvicIrqFor(BDMA2_Channel2_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA2_CH2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA2 channel3 global interrupt */
void BDMA2_Channel3_IRQHandler(void) {
	clearNvicIrqFor(BDMA2_Channel3_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA2_CH3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA2 channel4 global interrupt */
void BDMA2_Channel4_IRQHandler(void) {
	clearNvicIrqFor(BDMA2_Channel4_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA2_CH4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA2 channel5 global interrupt */
void BDMA2_Channel5_IRQHandler(void) {
	clearNvicIrqFor(BDMA2_Channel5_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA2_CH5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA2 channel6 global interrupt */
void BDMA2_Channel6_IRQHandler(void) {
	clearNvicIrqFor(BDMA2_Channel6_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA2_CH6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** BDMA2 channel7 global interrupt */
void BDMA2_Channel7_IRQHandler(void) {
	clearNvicIrqFor(BDMA2_Channel7_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA2_CH7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN RX0 interrupt */
void CAN_RX0_IRQHandler(void) {
	clearNvicIrqFor(CAN_RX0_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN RX1 interrupt */
void CAN_RX1_IRQHandler(void) {
	clearNvicIrqFor(CAN_RX1_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN SCE interrupt */
void CAN_SCE_IRQHandler(void) {
	clearNvicIrqFor(CAN_SCE_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN TX interrupt */
void CAN_TX_IRQHandler(void) {
	clearNvicIrqFor(CAN_TX_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN1 RX0 interrupts */
void CAN1_RX0_IRQHandler(void) {
	clearNvicIrqFor(CAN1_RX0_IRQn);
	STM32CubeDuinoIrqHandlerForCAN1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN1 RX1 interrupt */
void CAN1_RX1_IRQHandler(void) {
	clearNvicIrqFor(CAN1_RX1_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
	STM32CubeDuinoIrqHandlerForCAN1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN1 SCE interrupt */
void CAN1_SCE_IRQHandler(void) {
	clearNvicIrqFor(CAN1_SCE_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
	STM32CubeDuinoIrqHandlerForCAN1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN1 TX interrupts */
void CAN1_TX_IRQHandler(void) {
	clearNvicIrqFor(CAN1_TX_IRQn);
	STM32CubeDuinoIrqHandlerForCAN1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN2 RX0 interrupts */
void CAN2_RX0_IRQHandler(void) {
	clearNvicIrqFor(CAN2_RX0_IRQn);
	STM32CubeDuinoIrqHandlerForCAN2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN2 RX1 interrupt */
void CAN2_RX1_IRQHandler(void) {
	clearNvicIrqFor(CAN2_RX1_IRQn);
	STM32CubeDuinoIrqHandlerForCAN2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN2 SCE interrupt */
void CAN2_SCE_IRQHandler(void) {
	clearNvicIrqFor(CAN2_SCE_IRQn);
	STM32CubeDuinoIrqHandlerForCAN2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN2 TX interrupts */
void CAN2_TX_IRQHandler(void) {
	clearNvicIrqFor(CAN2_TX_IRQn);
	STM32CubeDuinoIrqHandlerForCAN2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN3 RX0 interrupts */
void CAN3_RX0_IRQHandler(void) {
	clearNvicIrqFor(CAN3_RX0_IRQn);
	STM32CubeDuinoIrqHandlerForCAN3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN3 RX1 interrupt */
void CAN3_RX1_IRQHandler(void) {
	clearNvicIrqFor(CAN3_RX1_IRQn);
	STM32CubeDuinoIrqHandlerForCAN3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN3 SCE interrupt */
void CAN3_SCE_IRQHandler(void) {
	clearNvicIrqFor(CAN3_SCE_IRQn);
	STM32CubeDuinoIrqHandlerForCAN3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CAN3 TX interrupts */
void CAN3_TX_IRQHandler(void) {
	clearNvicIrqFor(CAN3_TX_IRQn);
	STM32CubeDuinoIrqHandlerForCAN3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HDMI-CEC and CAN global interrupts / HDMI-CEC wake-up interrupt through EXTI line 27 */
void CEC_CAN_IRQHandler(void) {
	clearNvicIrqFor(CEC_CAN_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
	STM32CubeDuinoIrqHandlerForHDMI_CEC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HDMI-CEC global interrupt / HDMI-CEC wake-up interrupt through EXTI line 27 */
void CEC_IRQHandler(void) {
	clearNvicIrqFor(CEC_IRQn);
	STM32CubeDuinoIrqHandlerForHDMI_CEC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Comparator channel acquisition global interrupt through EXTI line 23 */
void COMP_ACQ_IRQHandler(void) {
	clearNvicIrqFor(COMP_ACQ_IRQn);
	ifIsPendingIrqForCOMP1() STM32CubeDuinoIrqHandlerForCOMP1();
	ifIsPendingIrqForCOMP2() STM32CubeDuinoIrqHandlerForCOMP2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** COMP1 and COMP2 wake-up interrupts through EXTI lines 21 and 22 */
void COMP_IRQHandler(void) {
	clearNvicIrqFor(COMP_IRQn);
	ifIsPendingIrqForCOMP1() STM32CubeDuinoIrqHandlerForCOMP1();
	ifIsPendingIrqForCOMP2() STM32CubeDuinoIrqHandlerForCOMP2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** COMP1 and COMP2 interrupts through EXTI lines 21 and 22 */
void COMP1_2_IRQHandler(void) {
	clearNvicIrqFor(COMP1_2_IRQn);
	ifIsPendingIrqForCOMP1() STM32CubeDuinoIrqHandlerForCOMP1();
	ifIsPendingIrqForCOMP2() STM32CubeDuinoIrqHandlerForCOMP2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** COMP1, COMP2 and COMP3 interrupts through EXTI lines 21, 22 and 29 */
void COMP1_2_3_IRQHandler(void) {
	clearNvicIrqFor(COMP1_2_3_IRQn);
	ifIsPendingIrqForCOMP1() STM32CubeDuinoIrqHandlerForCOMP1();
	ifIsPendingIrqForCOMP2() STM32CubeDuinoIrqHandlerForCOMP2();
	ifIsPendingIrqForCOMP3() STM32CubeDuinoIrqHandlerForCOMP3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** COMP2 global interrupt through EXTI line 22 */
void COMP2_IRQHandler(void) {
	clearNvicIrqFor(COMP2_IRQn);
	ifIsPendingIrqForCOMP2() STM32CubeDuinoIrqHandlerForCOMP2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** COMP4 interrupt through EXTI line 30 */
void COMP4_IRQHandler(void) {
	clearNvicIrqFor(COMP4_IRQn);
	STM32CubeDuinoIrqHandlerForCOMP4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** COMP4, COMP5 and COMP6 interrupts through EXTI lines 30, 31 and 32 */
void COMP4_5_6_IRQHandler(void) {
	clearNvicIrqFor(COMP4_5_6_IRQn);
	STM32CubeDuinoIrqHandlerForCOMP4();
	STM32CubeDuinoIrqHandlerForCOMP5();
	STM32CubeDuinoIrqHandlerForCOMP6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** COMP4 and COMP6 interrupts through EXTI lines 30 and 32 */
void COMP4_6_IRQHandler(void) {
	clearNvicIrqFor(COMP4_6_IRQn);
	STM32CubeDuinoIrqHandlerForCOMP4();
	STM32CubeDuinoIrqHandlerForCOMP6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** COMP7 interrupt through EXTI line 33 */
void COMP7_IRQHandler(void) {
	clearNvicIrqFor(COMP7_IRQn);
	STM32CubeDuinoIrqHandlerForCOMP7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CORDIC interrupt */
void CORDIC_IRQHandler(void) {
	clearNvicIrqFor(CORDIC_IRQn);
	STM32CubeDuinoIrqHandlerForCORDIC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Clock Recovery System global interrupt */
void CRS_IRQHandler(void) {
	clearNvicIrqFor(CRS_IRQn);
	STM32CubeDuinoIrqHandlerForRCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CRYP global interrupt */
void CRYP_IRQHandler(void) {
	clearNvicIrqFor(CRYP_IRQn);
	STM32CubeDuinoIrqHandlerForCRYP();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CRYP1 global interrupt */
void CRYP1_IRQHandler(void) {
	clearNvicIrqFor(CRYP1_IRQn);
	STM32CubeDuinoIrqHandlerForCRYP1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** CRYP2 global interrupt */
void CRYP2_IRQHandler(void) {
	clearNvicIrqFor(CRYP2_IRQn);
	STM32CubeDuinoIrqHandlerForCRYP2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && defined(DAC_TRIGGER_NONE))
/** DAC_CH1 and DAC_CH2 underrun error interrupt */
void DAC_IRQHandler(void) {
	clearNvicIrqFor(DAC_IRQn);
	ifIsPendingIrqForDAC() STM32CubeDuinoIrqHandlerForDAC();
	STM32CubeDuinoIrqHandlerForDAC1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or DAC_TRIGGER_NONE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && defined(DAC_TRIGGER_NONE))
/** DAC2 global interrupt */
void DAC2_IRQHandler(void) {
	clearNvicIrqFor(DAC2_IRQn);
	STM32CubeDuinoIrqHandlerForDAC2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or DAC_TRIGGER_NONE */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DCMI global interrupt */
void DCMI_IRQHandler(void) {
	clearNvicIrqFor(DCMI_IRQn);
	STM32CubeDuinoIrqHandlerForDCMI();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DCMI and PSSI global interrupt */
void DCMI_PSSI_IRQHandler(void) {
	clearNvicIrqFor(DCMI_PSSI_IRQn);
	STM32CubeDuinoIrqHandlerForDCMI();
	STM32CubeDuinoIrqHandlerForPSSI();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM1 filter0 global interrupt */
void DFSDM1_FLT0_IRQHandler(void) {
	clearNvicIrqFor(DFSDM1_FLT0_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM1 filter1 global interrupt */
void DFSDM1_FLT1_IRQHandler(void) {
	clearNvicIrqFor(DFSDM1_FLT1_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM1 filter2 global interrupt */
void DFSDM1_FLT2_IRQHandler(void) {
	clearNvicIrqFor(DFSDM1_FLT2_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM1 filter3 global interrupt */
void DFSDM1_FLT3_IRQHandler(void) {
	clearNvicIrqFor(DFSDM1_FLT3_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM1 filter4 global interrupt */
void DFSDM1_FLT4_IRQHandler(void) {
	clearNvicIrqFor(DFSDM1_FLT4_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM1 filter5 global interrupt */
void DFSDM1_FLT5_IRQHandler(void) {
	clearNvicIrqFor(DFSDM1_FLT5_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM1 filter6 global interrupt */
void DFSDM1_FLT6_IRQHandler(void) {
	clearNvicIrqFor(DFSDM1_FLT6_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM1 filter7 global interrupt */
void DFSDM1_FLT7_IRQHandler(void) {
	clearNvicIrqFor(DFSDM1_FLT7_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM2 filter0 global interrupt */
void DFSDM2_FLT0_IRQHandler(void) {
	clearNvicIrqFor(DFSDM2_FLT0_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM2 filter1 global interrupt */
void DFSDM2_FLT1_IRQHandler(void) {
	clearNvicIrqFor(DFSDM2_FLT1_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM2 filter2 global interrupt */
void DFSDM2_FLT2_IRQHandler(void) {
	clearNvicIrqFor(DFSDM2_FLT2_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM2 filter3 global interrupt */
void DFSDM2_FLT3_IRQHandler(void) {
	clearNvicIrqFor(DFSDM2_FLT3_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DFSDM2 global interrupt */
void DFSDM2_IRQHandler(void) {
	clearNvicIrqFor(DFSDM2_IRQn);
	STM32CubeDuinoIrqHandlerForDFSDM2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMAMUX overrun interrupt */
void DMAMUX_OVR_IRQHandler(void) {
	clearNvicIrqFor(DMAMUX_OVR_IRQn);
	ifIsPendingIrqForDMAMUX1() STM32CubeDuinoIrqHandlerForDMA();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMAMUX1 non-secure overrun interrupt */
void DMAMUX1_IRQHandler(void) {
	clearNvicIrqFor(DMAMUX1_IRQn);
	ifIsPendingIrqForDMAMUX1() STM32CubeDuinoIrqHandlerForDMA();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMAMUX1 overrun interrupt */
void DMAMUX1_OVR_IRQHandler(void) {
	clearNvicIrqFor(DMAMUX1_OVR_IRQn);
	ifIsPendingIrqForDMAMUX1() STM32CubeDuinoIrqHandlerForDMA();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMAMUX1 secure overrun interrupt */
void DMAMUX1_S_IRQHandler(void) {
	clearNvicIrqFor(DMAMUX1_S_IRQn);
	ifIsPendingIrqForDMAMUX1() STM32CubeDuinoIrqHandlerForDMA();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMAMUX2 overrun interrupt */
void DMAMUX2_OVR_IRQHandler(void) {
	clearNvicIrqFor(DMAMUX2_OVR_IRQn);
	STM32CubeDuinoIrqHandlerForBDMA();
	STM32CubeDuinoIrqHandlerForBDMA2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel 1 global interrupt */
void DMA1_Channel1_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel1_IRQn);
	ifIsPendingIrqForDMA1_CH1() STM32CubeDuinoIrqHandlerForDMA1_CH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel2 global interrupt */
void DMA1_Channel2_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel2_IRQn);
	ifIsPendingIrqForDMA1_CH2() STM32CubeDuinoIrqHandlerForDMA1_CH2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel 2 and channel 3 interrupts */
void DMA1_Channel2_3_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel2_3_IRQn);
	ifIsPendingIrqForDMA1_CH2() STM32CubeDuinoIrqHandlerForDMA1_CH2();
	ifIsPendingIrqForDMA1_CH3() STM32CubeDuinoIrqHandlerForDMA1_CH3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel3 global interrupt */
void DMA1_Channel3_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel3_IRQn);
	ifIsPendingIrqForDMA1_CH3() STM32CubeDuinoIrqHandlerForDMA1_CH3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel4 global interrupt */
void DMA1_Channel4_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel4_IRQn);
	ifIsPendingIrqForDMA1_CH4() STM32CubeDuinoIrqHandlerForDMA1_CH4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel 4 and channel 5 interrupts */
void DMA1_Channel4_5_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel4_5_IRQn);
	ifIsPendingIrqForDMA1_CH4() STM32CubeDuinoIrqHandlerForDMA1_CH4();
	ifIsPendingIrqForDMA1_CH5() STM32CubeDuinoIrqHandlerForDMA1_CH5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel 4, channel 5, channel 6 and channel 7 interrupts */
void DMA1_Channel4_5_6_7_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel4_5_6_7_IRQn);
	ifIsPendingIrqForDMA1_CH4() STM32CubeDuinoIrqHandlerForDMA1_CH4();
	ifIsPendingIrqForDMA1_CH5() STM32CubeDuinoIrqHandlerForDMA1_CH5();
	ifIsPendingIrqForDMA1_CH6() STM32CubeDuinoIrqHandlerForDMA1_CH6();
	ifIsPendingIrqForDMA1_CH7() STM32CubeDuinoIrqHandlerForDMA1_CH7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel5 global interrupt */
void DMA1_Channel5_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel5_IRQn);
	ifIsPendingIrqForDMA1_CH5() STM32CubeDuinoIrqHandlerForDMA1_CH5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel6 global interrupt */
void DMA1_Channel6_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel6_IRQn);
	ifIsPendingIrqForDMA1_CH6() STM32CubeDuinoIrqHandlerForDMA1_CH6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel7 global interrupt */
void DMA1_Channel7_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel7_IRQn);
	ifIsPendingIrqForDMA1_CH7() STM32CubeDuinoIrqHandlerForDMA1_CH7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel8 global interrupt */
void DMA1_Channel8_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Channel8_IRQn);
	STM32CubeDuinoIrqHandlerForDMA1_CH8();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel 1 interrupt */
void DMA1_Ch1_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Ch1_IRQn);
	ifIsPendingIrqForDMA1_CH1() STM32CubeDuinoIrqHandlerForDMA1_CH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel 2 to 3 and DMA2 channel 1 to 2 interrupts */
void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
	ifIsPendingIrqForDMA1_CH2() STM32CubeDuinoIrqHandlerForDMA1_CH2();
	ifIsPendingIrqForDMA1_CH3() STM32CubeDuinoIrqHandlerForDMA1_CH3();
	ifIsPendingIrqForDMA2_CH1() STM32CubeDuinoIrqHandlerForDMA2_CH1();
	ifIsPendingIrqForDMA2_CH2() STM32CubeDuinoIrqHandlerForDMA2_CH2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel 4, channel 5 and DMAMUX1 interrupts */
void DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Ch4_5_DMAMUX1_OVR_IRQn);
	ifIsPendingIrqForDMA1_CH4() STM32CubeDuinoIrqHandlerForDMA1_CH4();
	ifIsPendingIrqForDMA1_CH5() STM32CubeDuinoIrqHandlerForDMA1_CH5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel 4, channel 5, channel 6, channel 7 and DMAMUX1 interrupts */
void DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Ch4_7_DMAMUX1_OVR_IRQn);
	ifIsPendingIrqForDMA1_CH4() STM32CubeDuinoIrqHandlerForDMA1_CH4();
	ifIsPendingIrqForDMA1_CH5() STM32CubeDuinoIrqHandlerForDMA1_CH5();
	ifIsPendingIrqForDMA1_CH6() STM32CubeDuinoIrqHandlerForDMA1_CH6();
	ifIsPendingIrqForDMA1_CH7() STM32CubeDuinoIrqHandlerForDMA1_CH7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 channel 4 to 7 and DMA2 channel 3 to 5 interrupts */
void DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
	ifIsPendingIrqForDMA1_CH4() STM32CubeDuinoIrqHandlerForDMA1_CH4();
	ifIsPendingIrqForDMA1_CH5() STM32CubeDuinoIrqHandlerForDMA1_CH5();
	ifIsPendingIrqForDMA1_CH6() STM32CubeDuinoIrqHandlerForDMA1_CH6();
	ifIsPendingIrqForDMA1_CH7() STM32CubeDuinoIrqHandlerForDMA1_CH7();
	ifIsPendingIrqForDMA2_CH3() STM32CubeDuinoIrqHandlerForDMA2_CH3();
	ifIsPendingIrqForDMA2_CH4() STM32CubeDuinoIrqHandlerForDMA2_CH4();
	ifIsPendingIrqForDMA2_CH5() STM32CubeDuinoIrqHandlerForDMA2_CH5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 stream0 global interrupt */
void DMA1_Stream0_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Stream0_IRQn);
	STM32CubeDuinoIrqHandlerForDMA1_CH0();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 stream1 global interrupt */
void DMA1_Stream1_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Stream1_IRQn);
	ifIsPendingIrqForDMA1_CH1() STM32CubeDuinoIrqHandlerForDMA1_CH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 stream2 global interrupt */
void DMA1_Stream2_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Stream2_IRQn);
	ifIsPendingIrqForDMA1_CH2() STM32CubeDuinoIrqHandlerForDMA1_CH2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 stream3 global interrupt */
void DMA1_Stream3_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Stream3_IRQn);
	ifIsPendingIrqForDMA1_CH3() STM32CubeDuinoIrqHandlerForDMA1_CH3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 stream4 global interrupt */
void DMA1_Stream4_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Stream4_IRQn);
	ifIsPendingIrqForDMA1_CH4() STM32CubeDuinoIrqHandlerForDMA1_CH4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 stream5 global interrupt */
void DMA1_Stream5_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Stream5_IRQn);
	ifIsPendingIrqForDMA1_CH5() STM32CubeDuinoIrqHandlerForDMA1_CH5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 stream6 global interrupt */
void DMA1_Stream6_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Stream6_IRQn);
	ifIsPendingIrqForDMA1_CH6() STM32CubeDuinoIrqHandlerForDMA1_CH6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA1 stream7 global interrupt */
void DMA1_Stream7_IRQHandler(void) {
	clearNvicIrqFor(DMA1_Stream7_IRQn);
	ifIsPendingIrqForDMA1_CH7() STM32CubeDuinoIrqHandlerForDMA1_CH7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2D global interrupt */
void DMA2D_IRQHandler(void) {
	clearNvicIrqFor(DMA2D_IRQn);
	STM32CubeDuinoIrqHandlerForDMA2D();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 channel1 global interrupt */
void DMA2_Channel1_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Channel1_IRQn);
	ifIsPendingIrqForDMA2_CH1() STM32CubeDuinoIrqHandlerForDMA2_CH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 channel2 global interrupt */
void DMA2_Channel2_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Channel2_IRQn);
	ifIsPendingIrqForDMA2_CH2() STM32CubeDuinoIrqHandlerForDMA2_CH2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 channel3 global interrupt */
void DMA2_Channel3_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Channel3_IRQn);
	ifIsPendingIrqForDMA2_CH3() STM32CubeDuinoIrqHandlerForDMA2_CH3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 channel4 global interrupt */
void DMA2_Channel4_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Channel4_IRQn);
	ifIsPendingIrqForDMA2_CH4() STM32CubeDuinoIrqHandlerForDMA2_CH4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 channel4 and channel5 global interrupts */
void DMA2_Channel4_5_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Channel4_5_IRQn);
	ifIsPendingIrqForDMA2_CH4() STM32CubeDuinoIrqHandlerForDMA2_CH4();
	ifIsPendingIrqForDMA2_CH5() STM32CubeDuinoIrqHandlerForDMA2_CH5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 channel5 global interrupt */
void DMA2_Channel5_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Channel5_IRQn);
	ifIsPendingIrqForDMA2_CH5() STM32CubeDuinoIrqHandlerForDMA2_CH5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 channel6 global interrupt */
void DMA2_Channel6_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Channel6_IRQn);
	STM32CubeDuinoIrqHandlerForDMA2_CH6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 channel7 global interrupt */
void DMA2_Channel7_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Channel7_IRQn);
	STM32CubeDuinoIrqHandlerForDMA2_CH7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 channel8 global interrupt */
void DMA2_Channel8_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Channel8_IRQn);
	STM32CubeDuinoIrqHandlerForDMA2_CH8();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 stream0 global interrupt */
void DMA2_Stream0_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Stream0_IRQn);
	STM32CubeDuinoIrqHandlerForDMA2_CH0();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 stream1 global interrupt */
void DMA2_Stream1_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Stream1_IRQn);
	ifIsPendingIrqForDMA2_CH1() STM32CubeDuinoIrqHandlerForDMA2_CH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 stream2 global interrupt */
void DMA2_Stream2_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Stream2_IRQn);
	ifIsPendingIrqForDMA2_CH2() STM32CubeDuinoIrqHandlerForDMA2_CH2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 stream3 global interrupt */
void DMA2_Stream3_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Stream3_IRQn);
	ifIsPendingIrqForDMA2_CH3() STM32CubeDuinoIrqHandlerForDMA2_CH3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 stream4 global interrupt */
void DMA2_Stream4_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Stream4_IRQn);
	ifIsPendingIrqForDMA2_CH4() STM32CubeDuinoIrqHandlerForDMA2_CH4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 stream5 global interrupt */
void DMA2_Stream5_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Stream5_IRQn);
	ifIsPendingIrqForDMA2_CH5() STM32CubeDuinoIrqHandlerForDMA2_CH5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 stream6 global interrupt */
void DMA2_Stream6_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Stream6_IRQn);
	STM32CubeDuinoIrqHandlerForDMA2_CH6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DMA2 stream7 global interrupt */
void DMA2_Stream7_IRQHandler(void) {
	clearNvicIrqFor(DMA2_Stream7_IRQn);
	STM32CubeDuinoIrqHandlerForDMA2_CH7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** DSI global interrupt */
void DSI_IRQHandler(void) {
	clearNvicIrqFor(DSI_IRQn);
	STM32CubeDuinoIrqHandlerForDSIHOST();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Digital Temperature Sensor Global Interrupt */
void DTS_IRQHandler(void) {
	clearNvicIrqFor(DTS_IRQn);
	STM32CubeDuinoIrqHandlerForDTS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Ethernet global interrupt */
void ETH_IRQHandler(void) {
	clearNvicIrqFor(ETH_IRQn);
	STM32CubeDuinoIrqHandlerForETH();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Ethernet wake-up interrupt through EXTI line 19 */
void ETH_WKUP_IRQHandler(void) {
	clearNvicIrqFor(ETH_WKUP_IRQn);
	STM32CubeDuinoIrqHandlerForETH();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Ethernet global interrupt */
void ETH1_IRQHandler(void) {
	clearNvicIrqFor(ETH1_IRQn);
	STM32CubeDuinoIrqHandlerForETH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Ethernet low power idle interrupt */
void ETH1_LPI_IRQHandler(void) {
	clearNvicIrqFor(ETH1_LPI_IRQn);
	STM32CubeDuinoIrqHandlerForETH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Ethernet wake-up interrupt */
void ETH1_WKUP_IRQHandler(void) {
	clearNvicIrqFor(ETH1_WKUP_IRQn);
	STM32CubeDuinoIrqHandlerForETH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line 0 interrupt */
void EXTI0_IRQHandler(void) {
	clearNvicIrqFor(EXTI0_IRQn);
	ifIsPendingIrqForEXTI0() STM32CubeDuinoIrqHandlerForEXTI0();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line 0 and line 1 interrupts */
void EXTI0_1_IRQHandler(void) {
	clearNvicIrqFor(EXTI0_1_IRQn);
	ifIsPendingIrqForEXTI0() STM32CubeDuinoIrqHandlerForEXTI0();
	ifIsPendingIrqForEXTI1() STM32CubeDuinoIrqHandlerForEXTI1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line 1 interrupt */
void EXTI1_IRQHandler(void) {
	clearNvicIrqFor(EXTI1_IRQn);
	ifIsPendingIrqForEXTI1() STM32CubeDuinoIrqHandlerForEXTI1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line 2 interrupt */
void EXTI2_IRQHandler(void) {
	clearNvicIrqFor(EXTI2_IRQn);
	ifIsPendingIrqForEXTI2() STM32CubeDuinoIrqHandlerForEXTI2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line2 interrupt and touch sense controller interrupt */
void EXTI2_TSC_IRQHandler(void) {
	clearNvicIrqFor(EXTI2_TSC_IRQn);
	ifIsPendingIrqForEXTI2() STM32CubeDuinoIrqHandlerForEXTI2();
	ifIsPendingIrqForTSC_EOA() STM32CubeDuinoIrqHandlerForTSC();
	ifIsPendingIrqForTSC_MCE() STM32CubeDuinoIrqHandlerForTSC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line 2 and line 3 interrupts */
void EXTI2_3_IRQHandler(void) {
	clearNvicIrqFor(EXTI2_3_IRQn);
	ifIsPendingIrqForEXTI2() STM32CubeDuinoIrqHandlerForEXTI2();
	ifIsPendingIrqForEXTI3() STM32CubeDuinoIrqHandlerForEXTI3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line 3 interrupt */
void EXTI3_IRQHandler(void) {
	clearNvicIrqFor(EXTI3_IRQn);
	ifIsPendingIrqForEXTI3() STM32CubeDuinoIrqHandlerForEXTI3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line 4 interrupt */
void EXTI4_IRQHandler(void) {
	clearNvicIrqFor(EXTI4_IRQn);
	ifIsPendingIrqForEXTI4() STM32CubeDuinoIrqHandlerForEXTI4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line 4 to 15 interrupts */
void EXTI4_15_IRQHandler(void) {
	clearNvicIrqFor(EXTI4_15_IRQn);
	ifIsPendingIrqForEXTI4() STM32CubeDuinoIrqHandlerForEXTI4();
	ifIsPendingIrqForEXTI5() STM32CubeDuinoIrqHandlerForEXTI5();
	ifIsPendingIrqForEXTI6() STM32CubeDuinoIrqHandlerForEXTI6();
	ifIsPendingIrqForEXTI7() STM32CubeDuinoIrqHandlerForEXTI7();
	ifIsPendingIrqForEXTI8() STM32CubeDuinoIrqHandlerForEXTI8();
	ifIsPendingIrqForEXTI9() STM32CubeDuinoIrqHandlerForEXTI9();
	ifIsPendingIrqForEXTI10() STM32CubeDuinoIrqHandlerForEXTI10();
	ifIsPendingIrqForEXTI11() STM32CubeDuinoIrqHandlerForEXTI11();
	ifIsPendingIrqForEXTI12() STM32CubeDuinoIrqHandlerForEXTI12();
	ifIsPendingIrqForEXTI13() STM32CubeDuinoIrqHandlerForEXTI13();
	ifIsPendingIrqForEXTI14() STM32CubeDuinoIrqHandlerForEXTI14();
	ifIsPendingIrqForEXTI15() STM32CubeDuinoIrqHandlerForEXTI15();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line5 interrupt */
void EXTI5_IRQHandler(void) {
	clearNvicIrqFor(EXTI5_IRQn);
	ifIsPendingIrqForEXTI5() STM32CubeDuinoIrqHandlerForEXTI5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line6 interrupt */
void EXTI6_IRQHandler(void) {
	clearNvicIrqFor(EXTI6_IRQn);
	ifIsPendingIrqForEXTI6() STM32CubeDuinoIrqHandlerForEXTI6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line7 interrupt */
void EXTI7_IRQHandler(void) {
	clearNvicIrqFor(EXTI7_IRQn);
	ifIsPendingIrqForEXTI7() STM32CubeDuinoIrqHandlerForEXTI7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line8 interrupt */
void EXTI8_IRQHandler(void) {
	clearNvicIrqFor(EXTI8_IRQn);
	ifIsPendingIrqForEXTI8() STM32CubeDuinoIrqHandlerForEXTI8();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line9 interrupt */
void EXTI9_IRQHandler(void) {
	clearNvicIrqFor(EXTI9_IRQn);
	ifIsPendingIrqForEXTI9() STM32CubeDuinoIrqHandlerForEXTI9();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line[9:5] interrupts */
void EXTI9_5_IRQHandler(void) {
	clearNvicIrqFor(EXTI9_5_IRQn);
	ifIsPendingIrqForEXTI5() STM32CubeDuinoIrqHandlerForEXTI5();
	ifIsPendingIrqForEXTI6() STM32CubeDuinoIrqHandlerForEXTI6();
	ifIsPendingIrqForEXTI7() STM32CubeDuinoIrqHandlerForEXTI7();
	ifIsPendingIrqForEXTI8() STM32CubeDuinoIrqHandlerForEXTI8();
	ifIsPendingIrqForEXTI9() STM32CubeDuinoIrqHandlerForEXTI9();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line10 interrupt */
void EXTI10_IRQHandler(void) {
	clearNvicIrqFor(EXTI10_IRQn);
	ifIsPendingIrqForEXTI10() STM32CubeDuinoIrqHandlerForEXTI10();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line11 interrupt */
void EXTI11_IRQHandler(void) {
	clearNvicIrqFor(EXTI11_IRQn);
	ifIsPendingIrqForEXTI11() STM32CubeDuinoIrqHandlerForEXTI11();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line12 interrupt */
void EXTI12_IRQHandler(void) {
	clearNvicIrqFor(EXTI12_IRQn);
	ifIsPendingIrqForEXTI12() STM32CubeDuinoIrqHandlerForEXTI12();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line13 interrupt */
void EXTI13_IRQHandler(void) {
	clearNvicIrqFor(EXTI13_IRQn);
	ifIsPendingIrqForEXTI13() STM32CubeDuinoIrqHandlerForEXTI13();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line14 interrupt */
void EXTI14_IRQHandler(void) {
	clearNvicIrqFor(EXTI14_IRQn);
	ifIsPendingIrqForEXTI14() STM32CubeDuinoIrqHandlerForEXTI14();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line15 interrupt */
void EXTI15_IRQHandler(void) {
	clearNvicIrqFor(EXTI15_IRQn);
	ifIsPendingIrqForEXTI15() STM32CubeDuinoIrqHandlerForEXTI15();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_EXTI))
/** EXTI line[15:10] interrupts */
void EXTI15_10_IRQHandler(void) {
	clearNvicIrqFor(EXTI15_10_IRQn);
	ifIsPendingIrqForEXTI10() STM32CubeDuinoIrqHandlerForEXTI10();
	ifIsPendingIrqForEXTI11() STM32CubeDuinoIrqHandlerForEXTI11();
	ifIsPendingIrqForEXTI12() STM32CubeDuinoIrqHandlerForEXTI12();
	ifIsPendingIrqForEXTI13() STM32CubeDuinoIrqHandlerForEXTI13();
	ifIsPendingIrqForEXTI14() STM32CubeDuinoIrqHandlerForEXTI14();
	ifIsPendingIrqForEXTI15() STM32CubeDuinoIrqHandlerForEXTI15();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FDCAN calibration unit interrupt */
void FDCAN_CAL_IRQHandler(void) {
	clearNvicIrqFor(FDCAN_CAL_IRQn);
	ifIsPendingIrqForFDCAN1_IT0() STM32CubeDuinoIrqHandlerForFDCAN1();
	ifIsPendingIrqForFDCAN1_IT1() STM32CubeDuinoIrqHandlerForFDCAN1();
	ifIsPendingIrqForFDCAN2_IT0() STM32CubeDuinoIrqHandlerForFDCAN2();
	ifIsPendingIrqForFDCAN2_IT1() STM32CubeDuinoIrqHandlerForFDCAN2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FDCAN1 interrupt 0 */
void FDCAN1_IT0_IRQHandler(void) {
	clearNvicIrqFor(FDCAN1_IT0_IRQn);
	ifIsPendingIrqForFDCAN1_IT0() STM32CubeDuinoIrqHandlerForFDCAN1();
	ifIsPendingIrqForFDCAN1_IT1() STM32CubeDuinoIrqHandlerForFDCAN1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FDCAN1 interrupt 1 */
void FDCAN1_IT1_IRQHandler(void) {
	clearNvicIrqFor(FDCAN1_IT1_IRQn);
	ifIsPendingIrqForFDCAN1_IT0() STM32CubeDuinoIrqHandlerForFDCAN1();
	ifIsPendingIrqForFDCAN1_IT1() STM32CubeDuinoIrqHandlerForFDCAN1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FDCAN2 interrupt 0 */
void FDCAN2_IT0_IRQHandler(void) {
	clearNvicIrqFor(FDCAN2_IT0_IRQn);
	ifIsPendingIrqForFDCAN2_IT0() STM32CubeDuinoIrqHandlerForFDCAN2();
	ifIsPendingIrqForFDCAN2_IT1() STM32CubeDuinoIrqHandlerForFDCAN2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FDCAN2 interrupt 1 */
void FDCAN2_IT1_IRQHandler(void) {
	clearNvicIrqFor(FDCAN2_IT1_IRQn);
	ifIsPendingIrqForFDCAN2_IT0() STM32CubeDuinoIrqHandlerForFDCAN2();
	ifIsPendingIrqForFDCAN2_IT1() STM32CubeDuinoIrqHandlerForFDCAN2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FDCAN3 interrupt 0 */
void FDCAN3_IT0_IRQHandler(void) {
	clearNvicIrqFor(FDCAN3_IT0_IRQn);
	STM32CubeDuinoIrqHandlerForFDCAN3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FDCAN3 interrupt 1 */
void FDCAN3_IT1_IRQHandler(void) {
	clearNvicIrqFor(FDCAN3_IT1_IRQn);
	STM32CubeDuinoIrqHandlerForFDCAN3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Flash and EEPROM global interrupt */
void FLASH_IRQHandler(void) {
	clearNvicIrqFor(FLASH_IRQn);
	ifIsPendingIrqForFLASH_ECC() STM32CubeDuinoIrqHandlerForFLASH();
	ifIsPendingIrqForFLASH_ITF() STM32CubeDuinoIrqHandlerForFLASH();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Flash secure global interrupt */
void FLASH_S_IRQHandler(void) {
	clearNvicIrqFor(FLASH_S_IRQn);
	ifIsPendingIrqForFLASH_ECC() STM32CubeDuinoIrqHandlerForFLASH();
	ifIsPendingIrqForFLASH_ITF() STM32CubeDuinoIrqHandlerForFLASH();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FMAC interrupt */
void FMAC_IRQHandler(void) {
	clearNvicIrqFor(FMAC_IRQn);
	STM32CubeDuinoIrqHandlerForFMAC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FMC global interrupt */
void FMC_IRQHandler(void) {
	clearNvicIrqFor(FMC_IRQn);
	STM32CubeDuinoIrqHandlerForFMC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FMPI2C1 error interrupt */
void FMPI2C1_ER_IRQHandler(void) {
	clearNvicIrqFor(FMPI2C1_ER_IRQn);
	STM32CubeDuinoIrqHandlerForFMPI2C1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FMPI2C1 event interrupt */
void FMPI2C1_EV_IRQHandler(void) {
	clearNvicIrqFor(FMPI2C1_EV_IRQn);
	STM32CubeDuinoIrqHandlerForFMPI2C1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** FSMC global interrupt */
void FSMC_IRQHandler(void) {
	clearNvicIrqFor(FSMC_IRQn);
	STM32CubeDuinoIrqHandlerForFSMC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** GFXMMU global error interrupt */
void GFXMMU_IRQHandler(void) {
	clearNvicIrqFor(GFXMMU_IRQn);
	STM32CubeDuinoIrqHandlerForGFXMMU();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** GPU global interrupt */
void GPU_IRQHandler(void) {
	clearNvicIrqFor(GPU_IRQn);
	STM32CubeDuinoIrqHandlerForGPU();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Global TrustZone controller global interrupt */
void GTZC_IRQHandler(void) {
	clearNvicIrqFor(GTZC_IRQn);
	STM32CubeDuinoIrqHandlerForGTZC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HASH and CRS global interrupts */
void HASH_CRS_IRQHandler(void) {
	clearNvicIrqFor(HASH_CRS_IRQn);
	STM32CubeDuinoIrqHandlerForHASH();
	STM32CubeDuinoIrqHandlerForRCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HASH global interrupt */
void HASH_IRQHandler(void) {
	clearNvicIrqFor(HASH_IRQn);
	STM32CubeDuinoIrqHandlerForHASH();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HASH and RNG global interrupts */
void HASH_RNG_IRQHandler(void) {
	clearNvicIrqFor(HASH_RNG_IRQn);
	STM32CubeDuinoIrqHandlerForHASH();
	ifIsPendingIrqForRNG() STM32CubeDuinoIrqHandlerForRNG();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HASH1 global interrupt */
void HASH1_IRQHandler(void) {
	clearNvicIrqFor(HASH1_IRQn);
	STM32CubeDuinoIrqHandlerForHASH1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HASH2 global interrupt */
void HASH2_IRQHandler(void) {
	clearNvicIrqFor(HASH2_IRQn);
	STM32CubeDuinoIrqHandlerForHASH2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HRTIM fault global interrupt */
void HRTIM1_FLT_IRQHandler(void) {
	clearNvicIrqFor(HRTIM1_FLT_IRQn);
	STM32CubeDuinoIrqHandlerForHRTIM();
	STM32CubeDuinoIrqHandlerForHRTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HRTIM master timer global interrupt */
void HRTIM1_Master_IRQHandler(void) {
	clearNvicIrqFor(HRTIM1_Master_IRQn);
	STM32CubeDuinoIrqHandlerForHRTIM();
	STM32CubeDuinoIrqHandlerForHRTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HRTIM timer A global interrupt */
void HRTIM1_TIMA_IRQHandler(void) {
	clearNvicIrqFor(HRTIM1_TIMA_IRQn);
	STM32CubeDuinoIrqHandlerForHRTIM();
	STM32CubeDuinoIrqHandlerForHRTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HRTIM timer B global interrupt */
void HRTIM1_TIMB_IRQHandler(void) {
	clearNvicIrqFor(HRTIM1_TIMB_IRQn);
	STM32CubeDuinoIrqHandlerForHRTIM();
	STM32CubeDuinoIrqHandlerForHRTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HRTIM timer C global interrupt */
void HRTIM1_TIMC_IRQHandler(void) {
	clearNvicIrqFor(HRTIM1_TIMC_IRQn);
	STM32CubeDuinoIrqHandlerForHRTIM();
	STM32CubeDuinoIrqHandlerForHRTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HRTIM timer D global interrupt */
void HRTIM1_TIMD_IRQHandler(void) {
	clearNvicIrqFor(HRTIM1_TIMD_IRQn);
	STM32CubeDuinoIrqHandlerForHRTIM();
	STM32CubeDuinoIrqHandlerForHRTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HRTIM timer E global interrupt */
void HRTIM1_TIME_IRQHandler(void) {
	clearNvicIrqFor(HRTIM1_TIME_IRQn);
	STM32CubeDuinoIrqHandlerForHRTIM();
	STM32CubeDuinoIrqHandlerForHRTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HRTIM timer F global interrupt */
void HRTIM1_TIMF_IRQHandler(void) {
	clearNvicIrqFor(HRTIM1_TIMF_IRQn);
	STM32CubeDuinoIrqHandlerForHRTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** HSEM global interrupt */
void HSEM_IRQHandler(void) {
	clearNvicIrqFor(HSEM_IRQn);
	STM32CubeDuinoIrqHandlerForHSEM();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Instruction cache global interrupt */
void ICACHE_IRQHandler(void) {
	clearNvicIrqFor(ICACHE_IRQn);
	STM32CubeDuinoIrqHandlerForICACHE();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** IPCC RX occupied interrupt */
void IPCC_C1_RX_IRQHandler(void) {
	clearNvicIrqFor(IPCC_C1_RX_IRQn);
	STM32CubeDuinoIrqHandlerForIPCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** IPCC TX free interrupt */
void IPCC_C1_TX_IRQHandler(void) {
	clearNvicIrqFor(IPCC_C1_TX_IRQn);
	STM32CubeDuinoIrqHandlerForIPCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** IPCC RX0 occupied interrupt */
void IPCC_RX0_IRQHandler(void) {
	clearNvicIrqFor(IPCC_RX0_IRQn);
	STM32CubeDuinoIrqHandlerForIPCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** IPCC RX1 occupied interrupt */
void IPCC_RX1_IRQHandler(void) {
	clearNvicIrqFor(IPCC_RX1_IRQn);
	STM32CubeDuinoIrqHandlerForIPCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** IPCC TX0 free interrupt */
void IPCC_TX0_IRQHandler(void) {
	clearNvicIrqFor(IPCC_TX0_IRQn);
	STM32CubeDuinoIrqHandlerForIPCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** IPCC TX1 free interrupt */
void IPCC_TX1_IRQHandler(void) {
	clearNvicIrqFor(IPCC_TX1_IRQn);
	STM32CubeDuinoIrqHandlerForIPCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C1 error interrupt */
void I2C1_ER_IRQHandler(void) {
	clearNvicIrqFor(I2C1_ER_IRQn);
	ifIsPendingIrqForI2C1() STM32CubeDuinoIrqHandlerForI2C1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C1 event global interrupt / I2C1 wake-up interrupt through EXTI line 23 */
void I2C1_EV_IRQHandler(void) {
	clearNvicIrqFor(I2C1_EV_IRQn);
	ifIsPendingIrqForI2C1() STM32CubeDuinoIrqHandlerForI2C1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C1 event global interrupt / I2C1 wake-up interrupt through EXTI line 23 */
void I2C1_IRQHandler(void) {
	clearNvicIrqFor(I2C1_IRQn);
	ifIsPendingIrqForI2C1() STM32CubeDuinoIrqHandlerForI2C1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C2 error interrupt */
void I2C2_ER_IRQHandler(void) {
	clearNvicIrqFor(I2C2_ER_IRQn);
	ifIsPendingIrqForI2C2() STM32CubeDuinoIrqHandlerForI2C2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C2 event global interrupt / I2C2 wake-up interrupt through EXTI line 24 */
void I2C2_EV_IRQHandler(void) {
	clearNvicIrqFor(I2C2_EV_IRQn);
	ifIsPendingIrqForI2C2() STM32CubeDuinoIrqHandlerForI2C2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C2 global interrupt */
void I2C2_IRQHandler(void) {
	clearNvicIrqFor(I2C2_IRQn);
	ifIsPendingIrqForI2C2() STM32CubeDuinoIrqHandlerForI2C2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C3 error interrupt */
void I2C3_ER_IRQHandler(void) {
	clearNvicIrqFor(I2C3_ER_IRQn);
	ifIsPendingIrqForI2C3() STM32CubeDuinoIrqHandlerForI2C3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C3 event interrupt / I2C3 wake-up interrupt through EXTI line 25 */
void I2C3_EV_IRQHandler(void) {
	clearNvicIrqFor(I2C3_EV_IRQn);
	ifIsPendingIrqForI2C3() STM32CubeDuinoIrqHandlerForI2C3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C3 global interrupt / I2C3 wake-up interrupt through EXTI line 24 */
void I2C3_IRQHandler(void) {
	clearNvicIrqFor(I2C3_IRQn);
	ifIsPendingIrqForI2C3() STM32CubeDuinoIrqHandlerForI2C3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C4 error interrupt */
void I2C4_ER_IRQHandler(void) {
	clearNvicIrqFor(I2C4_ER_IRQn);
	STM32CubeDuinoIrqHandlerForI2C4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C4 event interrupt / I2C4 wake-up interrupt through EXTI line 40 */
void I2C4_EV_IRQHandler(void) {
	clearNvicIrqFor(I2C4_EV_IRQn);
	STM32CubeDuinoIrqHandlerForI2C4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C5 error interrupt */
void I2C5_ER_IRQHandler(void) {
	clearNvicIrqFor(I2C5_ER_IRQn);
	STM32CubeDuinoIrqHandlerForI2C5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C5 event interrupt */
void I2C5_EV_IRQHandler(void) {
	clearNvicIrqFor(I2C5_EV_IRQn);
	STM32CubeDuinoIrqHandlerForI2C5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C6 error interrupt */
void I2C6_ER_IRQHandler(void) {
	clearNvicIrqFor(I2C6_ER_IRQn);
	STM32CubeDuinoIrqHandlerForI2C6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE))
/** I2C6 event interrupt */
void I2C6_EV_IRQHandler(void) {
	clearNvicIrqFor(I2C6_EV_IRQn);
	STM32CubeDuinoIrqHandlerForI2C6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_I2C_ADDRESSING_MODE */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** JPEG global interrupt */
void JPEG_IRQHandler(void) {
	clearNvicIrqFor(JPEG_IRQn);
	STM32CubeDuinoIrqHandlerForJPEG();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** LCD global interrupt */
void LCD_IRQHandler(void) {
	clearNvicIrqFor(LCD_IRQn);
	STM32CubeDuinoIrqHandlerForLCD();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_ENABLE_TIMER) && defined(LPTIM_PRESCALER_DIV1))
/** LPTIM1 global interrupt / LPTIM1 wake-up interrupt through EXTI line 29 */
void LPTIM1_IRQHandler(void) {
	clearNvicIrqFor(LPTIM1_IRQn);
	ifIsPendingIrqForLPTIM1() STM32CubeDuinoIrqHandlerForLPTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or LPTIM_PRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_ENABLE_TIMER) && defined(LPTIM_PRESCALER_DIV1))
/** LPTIM2 global interrupt through EXTI line 33 */
void LPTIM2_IRQHandler(void) {
	clearNvicIrqFor(LPTIM2_IRQn);
	ifIsPendingIrqForLPTIM2() STM32CubeDuinoIrqHandlerForLPTIM2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or LPTIM_PRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_ENABLE_TIMER) && defined(LPTIM_PRESCALER_DIV1))
/** LPTIM3 global interrupt / LPTIM3 wake-up interrupt through EXTI line 42 */
void LPTIM3_IRQHandler(void) {
	clearNvicIrqFor(LPTIM3_IRQn);
	STM32CubeDuinoIrqHandlerForLPTIM3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or LPTIM_PRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_ENABLE_TIMER) && defined(LPTIM_PRESCALER_DIV1))
/** LPTIM4 global interrupt */
void LPTIM4_IRQHandler(void) {
	clearNvicIrqFor(LPTIM4_IRQn);
	STM32CubeDuinoIrqHandlerForLPTIM4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or LPTIM_PRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_ENABLE_TIMER) && defined(LPTIM_PRESCALER_DIV1))
/** LPTIM5 global interrupt */
void LPTIM5_IRQHandler(void) {
	clearNvicIrqFor(LPTIM5_IRQn);
	STM32CubeDuinoIrqHandlerForLPTIM5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or LPTIM_PRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** LPUART1 global interrupt / LPUART1 wake-up interrupt through EXTI line 28 */
void LPUART1_IRQHandler(void) {
	clearNvicIrqFor(LPUART1_IRQn);
	ifIsPendingIrqForLPUART1() STM32CubeDuinoIrqHandlerForLPUART1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** LTDC Error global Interrupt */
void LTDC_ER_IRQHandler(void) {
	clearNvicIrqFor(LTDC_ER_IRQn);
	STM32CubeDuinoIrqHandlerForLTDC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** LTDC global interrupt */
void LTDC_IRQHandler(void) {
	clearNvicIrqFor(LTDC_IRQn);
	STM32CubeDuinoIrqHandlerForLTDC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** MDIO slave global interrupt */
void MDIOS_IRQHandler(void) {
	clearNvicIrqFor(MDIOS_IRQn);
	STM32CubeDuinoIrqHandlerForMDIOS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** MDIOS wake-up interrupt */
void MDIOS_WKUP_IRQHandler(void) {
	clearNvicIrqFor(MDIOS_WKUP_IRQn);
	STM32CubeDuinoIrqHandlerForMDIOS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** MDMA global interrupt */
void MDMA_IRQHandler(void) {
	clearNvicIrqFor(MDMA_IRQn);
	STM32CubeDuinoIrqHandlerForMDMA();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** OCTOSPI1 global interrupt */
void OCTOSPI1_IRQHandler(void) {
	clearNvicIrqFor(OCTOSPI1_IRQn);
	STM32CubeDuinoIrqHandlerForOCTOSPI1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** OCTOSPI2 global interrupt */
void OCTOSPI2_IRQHandler(void) {
	clearNvicIrqFor(OCTOSPI2_IRQn);
	STM32CubeDuinoIrqHandlerForOCTOSPI2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** OTFDEC1 global interrupt */
void OTFDEC1_IRQHandler(void) {
	clearNvicIrqFor(OTFDEC1_IRQn);
	STM32CubeDuinoIrqHandlerForOTFDEC1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** OTFDEC2 global interrupt */
void OTFDEC2_IRQHandler(void) {
	clearNvicIrqFor(OTFDEC2_IRQn);
	STM32CubeDuinoIrqHandlerForOTFDEC2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB On The Go FS End Point 1 In global interrupt */
void OTG_FS_EP1_IN_IRQHandler(void) {
	clearNvicIrqFor(OTG_FS_EP1_IN_IRQn);
	STM32CubeDuinoIrqHandlerForUSB_OTG_FS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB On The Go FS End Point 1 Out global interrupt */
void OTG_FS_EP1_OUT_IRQHandler(void) {
	clearNvicIrqFor(OTG_FS_EP1_OUT_IRQn);
	STM32CubeDuinoIrqHandlerForUSB_OTG_FS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB On The Go FS global interrupt */
void OTG_FS_IRQHandler(void) {
	clearNvicIrqFor(OTG_FS_IRQn);
	STM32CubeDuinoIrqHandlerForUSB_OTG_FS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB On The Go FS wake-up interrupt through EXTI line 18 */
void OTG_FS_WKUP_IRQHandler(void) {
	clearNvicIrqFor(OTG_FS_WKUP_IRQn);
	STM32CubeDuinoIrqHandlerForUSB_OTG_FS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** USB On The Go HS End Point 1 In global interrupt */
void OTG_HS_EP1_IN_IRQHandler(void) {
	clearNvicIrqFor(OTG_HS_EP1_IN_IRQn);
	STM32CubeDuinoIrqHandlerForUSB_OTG_HS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** USB On The Go HS End Point 1 Out global interrupt */
void OTG_HS_EP1_OUT_IRQHandler(void) {
	clearNvicIrqFor(OTG_HS_EP1_OUT_IRQn);
	STM32CubeDuinoIrqHandlerForUSB_OTG_HS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** USB On The Go HS global interrupt */
void OTG_HS_IRQHandler(void) {
	clearNvicIrqFor(OTG_HS_IRQn);
	STM32CubeDuinoIrqHandlerForUSB_OTG_HS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** USB On The Go HS wake-up interrupt through EXTI line 20 */
void OTG_HS_WKUP_IRQHandler(void) {
	clearNvicIrqFor(OTG_HS_WKUP_IRQn);
	STM32CubeDuinoIrqHandlerForUSB_OTG_HS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** USB On The Go HS global interrupt */
void OTG_IRQHandler(void) {
	clearNvicIrqFor(OTG_IRQn);
	STM32CubeDuinoIrqHandlerForUSB_OTG_HS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** PKA global interrupt */
void PKA_IRQHandler(void) {
	clearNvicIrqFor(PKA_IRQn);
	STM32CubeDuinoIrqHandlerForPKA();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** PVD and AVD interrupt through EXTI line 16 */
void PVD_AVD_IRQHandler(void) {
	clearNvicIrqFor(PVD_AVD_IRQn);
	ifIsPendingIrqForPVDOUT() STM32CubeDuinoIrqHandlerForPWR();
	ifIsPendingIrqForPVMOUT() STM32CubeDuinoIrqHandlerForPWR();
	ifIsPendingIrqForVDDIO2() STM32CubeDuinoIrqHandlerForPWR();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** QUADSPI global interrupt */
void QUADSPI_IRQHandler(void) {
	clearNvicIrqFor(QUADSPI_IRQn);
	STM32CubeDuinoIrqHandlerForQUADSPI();
	STM32CubeDuinoIrqHandlerForQUADSPI1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RCC and CRS global interrupts */
void RCC_CRS_IRQHandler(void) {
	clearNvicIrqFor(RCC_CRS_IRQn);
	STM32CubeDuinoIrqHandlerForRCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RCC non-secure global interrupt */
void RCC_IRQHandler(void) {
	clearNvicIrqFor(RCC_IRQn);
	STM32CubeDuinoIrqHandlerForRCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RCC secure global interrupt */
void RCC_S_IRQHandler(void) {
	clearNvicIrqFor(RCC_S_IRQn);
	STM32CubeDuinoIrqHandlerForRCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RCC wake-up interrupt */
void RCC_WAKEUP_IRQHandler(void) {
	clearNvicIrqFor(RCC_WAKEUP_IRQn);
	STM32CubeDuinoIrqHandlerForRCC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RNG global interrupt */
void RNG_IRQHandler(void) {
	clearNvicIrqFor(RNG_IRQn);
	ifIsPendingIrqForRNG() STM32CubeDuinoIrqHandlerForRNG();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** RNG and LPUART1 Interrupts / LPUART1 wake-up interrupt through EXTI line 28 */
void RNG_LPUART1_IRQHandler(void) {
	clearNvicIrqFor(RNG_LPUART1_IRQn);
	ifIsPendingIrqForLPUART1() STM32CubeDuinoIrqHandlerForLPUART1();
	ifIsPendingIrqForRNG() STM32CubeDuinoIrqHandlerForRNG();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RNG1 global interrupt */
void RNG1_IRQHandler(void) {
	clearNvicIrqFor(RNG1_IRQn);
	STM32CubeDuinoIrqHandlerForRNG1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RNG2 global interrupt */
void RNG2_IRQHandler(void) {
	clearNvicIrqFor(RNG2_IRQn);
	STM32CubeDuinoIrqHandlerForRNG2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC alarms (A and B) interrupt through EXTI line 17 */
void RTC_Alarm_IRQHandler(void) {
	clearNvicIrqFor(RTC_Alarm_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC global interrupt through EXTI lines 17, 19 and 20 and LSE CSS interrupt through EXTI line 19 */
void RTC_IRQHandler(void) {
	clearNvicIrqFor(RTC_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC secure interrupts through EXTI line 18 */
void RTC_S_IRQHandler(void) {
	clearNvicIrqFor(RTC_S_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC and TAMP interrupts through EXTI lines 19 and 21 */
void RTC_TAMP_IRQHandler(void) {
	clearNvicIrqFor(RTC_TAMP_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC Tamper and TimeStamp and RCC LSE CSS interrupts through the EXTI */
void RTC_TAMP_LSECSS_IRQHandler(void) {
	clearNvicIrqFor(RTC_TAMP_LSECSS_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Tamper, timestamp, CSS and LSE interrupts through EXTI line */
void RTC_TAMP_STAMP_CSS_LSE_IRQHandler(void) {
	clearNvicIrqFor(RTC_TAMP_STAMP_CSS_LSE_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC timestamp interrupt */
void RTC_TIMESTAMP_IRQHandler(void) {
	clearNvicIrqFor(RTC_TIMESTAMP_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC wake-up and alarms (A and B) interrupt */
void RTC_WKUP_ALARM_IRQHandler(void) {
	clearNvicIrqFor(RTC_WKUP_ALARM_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC wake-up interrupt through EXTI line 19 */
void RTC_WKUP_IRQHandler(void) {
	clearNvicIrqFor(RTC_WKUP_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SAI1 global interrupt */
void SAI1_IRQHandler(void) {
	clearNvicIrqFor(SAI1_IRQn);
	STM32CubeDuinoIrqHandlerForSAI1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SAI2 global interrupt */
void SAI2_IRQHandler(void) {
	clearNvicIrqFor(SAI2_IRQn);
	STM32CubeDuinoIrqHandlerForSAI2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SAI3 global interrupt */
void SAI3_IRQHandler(void) {
	clearNvicIrqFor(SAI3_IRQn);
	STM32CubeDuinoIrqHandlerForSAI3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SAI4 global interrupt */
void SAI4_IRQHandler(void) {
	clearNvicIrqFor(SAI4_IRQn);
	STM32CubeDuinoIrqHandlerForSAI4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SDADC1 global interrupt */
void SDADC1_IRQHandler(void) {
	clearNvicIrqFor(SDADC1_IRQn);
	STM32CubeDuinoIrqHandlerForSDADC1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SDADC2 global interrupt */
void SDADC2_IRQHandler(void) {
	clearNvicIrqFor(SDADC2_IRQn);
	STM32CubeDuinoIrqHandlerForSDADC2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SDADC3 global interrupt */
void SDADC3_IRQHandler(void) {
	clearNvicIrqFor(SDADC3_IRQn);
	STM32CubeDuinoIrqHandlerForSDADC3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SDIO global interrupt */
void SDIO_IRQHandler(void) {
	clearNvicIrqFor(SDIO_IRQn);
	STM32CubeDuinoIrqHandlerForSDIO();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SDMMC1 global interrupt */
void SDMMC1_IRQHandler(void) {
	clearNvicIrqFor(SDMMC1_IRQn);
	STM32CubeDuinoIrqHandlerForSDMMC1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SDMMC2 global interrupt */
void SDMMC2_IRQHandler(void) {
	clearNvicIrqFor(SDMMC2_IRQn);
	STM32CubeDuinoIrqHandlerForSDMMC2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SDMMC3 global interrupt */
void SDMMC3_IRQHandler(void) {
	clearNvicIrqFor(SDMMC3_IRQn);
	STM32CubeDuinoIrqHandlerForSDMMC3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SPDIF-RX global interrupt */
void SPDIF_RX_IRQHandler(void) {
	clearNvicIrqFor(SPDIF_RX_IRQn);
	STM32CubeDuinoIrqHandlerForSPDIFRX();
	STM32CubeDuinoIrqHandlerForSPDIFRX1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SPI1 global interrupt */
void SPI1_IRQHandler(void) {
	clearNvicIrqFor(SPI1_IRQn);
	STM32CubeDuinoIrqHandlerForI2S1();
	ifIsPendingIrqForSPI1() STM32CubeDuinoIrqHandlerForSPI1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SPI2 global interrupt */
void SPI2_IRQHandler(void) {
	clearNvicIrqFor(SPI2_IRQn);
	STM32CubeDuinoIrqHandlerForI2S2();
	ifIsPendingIrqForSPI2() STM32CubeDuinoIrqHandlerForSPI2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SPI3 global interrupt */
void SPI3_IRQHandler(void) {
	clearNvicIrqFor(SPI3_IRQn);
	STM32CubeDuinoIrqHandlerForI2S3();
	ifIsPendingIrqForSPI3() STM32CubeDuinoIrqHandlerForSPI3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SPI4 global interrupt */
void SPI4_IRQHandler(void) {
	clearNvicIrqFor(SPI4_IRQn);
	STM32CubeDuinoIrqHandlerForI2S4();
	STM32CubeDuinoIrqHandlerForSPI4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SPI5 global interrupt */
void SPI5_IRQHandler(void) {
	clearNvicIrqFor(SPI5_IRQn);
	STM32CubeDuinoIrqHandlerForI2S5();
	STM32CubeDuinoIrqHandlerForSPI5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** SPI6 global interrupt */
void SPI6_IRQHandler(void) {
	clearNvicIrqFor(SPI6_IRQn);
	STM32CubeDuinoIrqHandlerForI2S6();
	STM32CubeDuinoIrqHandlerForSPI6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Serial Wire Interface 1 global interrupt */
void SWPMI1_IRQHandler(void) {
	clearNvicIrqFor(SWPMI1_IRQn);
	STM32CubeDuinoIrqHandlerForSWPMI1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Tamper interrupt */
void TAMPER_IRQHandler(void) {
	clearNvicIrqFor(TAMPER_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC tamper, LSECSS and timestamp interrupts through EXTI line 19 */
void TAMPER_STAMP_IRQHandler(void) {
	clearNvicIrqFor(TAMPER_STAMP_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Tamper non-secure global interrupt through EXTI line 19 */
void TAMP_IRQHandler(void) {
	clearNvicIrqFor(TAMP_IRQn);
	STM32CubeDuinoIrqHandlerForTAMP();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC tamper and time stamp, CSS on LSE interrupts through EXTI line 19 */
void TAMP_STAMP_IRQHandler(void) {
	clearNvicIrqFor(TAMP_STAMP_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** RTC tamper and time stamp, CSS on LSE interrupts through EXTI line 18 */
void TAMP_STAMP_LSECSS_IRQHandler(void) {
	clearNvicIrqFor(TAMP_STAMP_LSECSS_IRQn);
	ifIsPendingIrqForRTC() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_ALRA() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_TSTAMP() STM32CubeDuinoIrqHandlerForRTC();
	ifIsPendingIrqForRTC_WAKEUP() STM32CubeDuinoIrqHandlerForRTC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Tamper secure global interrupt through EXTI line 20 */
void TAMP_S_IRQHandler(void) {
	clearNvicIrqFor(TAMP_S_IRQn);
	STM32CubeDuinoIrqHandlerForTAMP();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Temperature sensor global Interrupt */
void TEMP_IRQHandler(void) {
	clearNvicIrqFor(TEMP_IRQn);
	STM32CubeDuinoIrqHandlerForTEMPSENS();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 break interrupt */
void TIM1_BRK_IRQHandler(void) {
	clearNvicIrqFor(TIM1_BRK_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 break interrupt and TIM9 global interrupt */
void TIM1_BRK_TIM9_IRQHandler(void) {
	clearNvicIrqFor(TIM1_BRK_TIM9_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
	STM32CubeDuinoIrqHandlerForTIM9();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 break interrupt and TIM15 global interrupt */
void TIM1_BRK_TIM15_IRQHandler(void) {
	clearNvicIrqFor(TIM1_BRK_TIM15_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM15() STM32CubeDuinoIrqHandlerForTIM15();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 break, update, trigger and commutation interrupts */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
	clearNvicIrqFor(TIM1_BRK_UP_TRG_COM_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 capture compare interrupt */
void TIM1_CC_IRQHandler(void) {
	clearNvicIrqFor(TIM1_CC_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 trigger and commutation interrupts */
void TIM1_TRG_COM_IRQHandler(void) {
	clearNvicIrqFor(TIM1_TRG_COM_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 trigger and commutation interrupts and TIM11 global interrupt */
void TIM1_TRG_COM_TIM11_IRQHandler(void) {
	clearNvicIrqFor(TIM1_TRG_COM_TIM11_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
	STM32CubeDuinoIrqHandlerForTIM11();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 trigger and commutation interrupts and TIM17 global interrupt */
void TIM1_TRG_COM_TIM17_IRQHandler(void) {
	clearNvicIrqFor(TIM1_TRG_COM_TIM17_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM17() STM32CubeDuinoIrqHandlerForTIM17();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 update interrupt */
void TIM1_UP_IRQHandler(void) {
	clearNvicIrqFor(TIM1_UP_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 update interrupt and TIM10 global interrupt */
void TIM1_UP_TIM10_IRQHandler(void) {
	clearNvicIrqFor(TIM1_UP_TIM10_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
	STM32CubeDuinoIrqHandlerForTIM10();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM1 update interrupt and TIM16 global interrupt */
void TIM1_UP_TIM16_IRQHandler(void) {
	clearNvicIrqFor(TIM1_UP_TIM16_IRQn);
	ifIsPendingIrqForTIM1_BRK() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CC() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_CCU() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_TRG() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM1_UPD() STM32CubeDuinoIrqHandlerForTIM1();
	ifIsPendingIrqForTIM16() STM32CubeDuinoIrqHandlerForTIM16();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM2 global interrupt */
void TIM2_IRQHandler(void) {
	clearNvicIrqFor(TIM2_IRQn);
	ifIsPendingIrqForTIM2() STM32CubeDuinoIrqHandlerForTIM2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM3 global interrupt */
void TIM3_IRQHandler(void) {
	clearNvicIrqFor(TIM3_IRQn);
	ifIsPendingIrqForTIM3() STM32CubeDuinoIrqHandlerForTIM3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM4 global interrupt */
void TIM4_IRQHandler(void) {
	clearNvicIrqFor(TIM4_IRQn);
	ifIsPendingIrqForTIM4() STM32CubeDuinoIrqHandlerForTIM4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM5 global interrupt */
void TIM5_IRQHandler(void) {
	clearNvicIrqFor(TIM5_IRQn);
	STM32CubeDuinoIrqHandlerForTIM5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(DAC_TRIGGER_NONE)) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM6 global interrupt, DAC channel1 and channel2 underrun error interrupts */
void TIM6_DAC_IRQHandler(void) {
	clearNvicIrqFor(TIM6_DAC_IRQn);
	ifIsPendingIrqForDAC() STM32CubeDuinoIrqHandlerForDAC();
	STM32CubeDuinoIrqHandlerForDAC1();
	STM32CubeDuinoIrqHandlerForDAC3();
	ifIsPendingIrqForTIM6() STM32CubeDuinoIrqHandlerForTIM6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or DAC_TRIGGER_NONE or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_ENABLE_TIMER) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(DAC_TRIGGER_NONE)) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_ENABLE_TIMER) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(LPTIM_PRESCALER_DIV1)) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_ENABLE_TIMER) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM6, DAC1 and LPTIM1 interrupts (LPTIM1 interrupt through EXTI line 29) */
void TIM6_DAC_LPTIM1_IRQHandler(void) {
	clearNvicIrqFor(TIM6_DAC_LPTIM1_IRQn);
	STM32CubeDuinoIrqHandlerForDAC1();
	ifIsPendingIrqForLPTIM1() STM32CubeDuinoIrqHandlerForLPTIM1();
	ifIsPendingIrqForTIM6() STM32CubeDuinoIrqHandlerForTIM6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or DAC_TRIGGER_NONE or LPTIM_PRESCALER_DIV1 or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(DAC_TRIGGER_NONE)) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM6 global interrupt and DAC1 underrun error interrupts */
void TIM6_DAC1_IRQHandler(void) {
	clearNvicIrqFor(TIM6_DAC1_IRQn);
	STM32CubeDuinoIrqHandlerForDAC1();
	ifIsPendingIrqForTIM6() STM32CubeDuinoIrqHandlerForTIM6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or DAC_TRIGGER_NONE or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM6 global interrupt */
void TIM6_IRQHandler(void) {
	clearNvicIrqFor(TIM6_IRQn);
	ifIsPendingIrqForTIM6() STM32CubeDuinoIrqHandlerForTIM6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(DAC_TRIGGER_NONE)) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM7 global interrupt, DAC2 and DAC4 channel underrun error interrupts */
void TIM7_DAC_IRQHandler(void) {
	clearNvicIrqFor(TIM7_DAC_IRQn);
	STM32CubeDuinoIrqHandlerForDAC2();
	STM32CubeDuinoIrqHandlerForDAC4();
	ifIsPendingIrqForTIM7() STM32CubeDuinoIrqHandlerForTIM7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or DAC_TRIGGER_NONE or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(DAC_TRIGGER_NONE)) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM7 global and DAC2 underrun error interrupts */
void TIM7_DAC2_IRQHandler(void) {
	clearNvicIrqFor(TIM7_DAC2_IRQn);
	STM32CubeDuinoIrqHandlerForDAC2();
	ifIsPendingIrqForTIM7() STM32CubeDuinoIrqHandlerForTIM7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or DAC_TRIGGER_NONE or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM7 global interrupt */
void TIM7_IRQHandler(void) {
	clearNvicIrqFor(TIM7_IRQn);
	ifIsPendingIrqForTIM7() STM32CubeDuinoIrqHandlerForTIM7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_ENABLE_TIMER) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(LPTIM_PRESCALER_DIV1)) || (!defined(STM32CUBEDUINO_ENABLE_TIMER) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM7 and LPTIM2 interrupts (LPTIM2 interrupt through EXTI line 30) */
void TIM7_LPTIM2_IRQHandler(void) {
	clearNvicIrqFor(TIM7_LPTIM2_IRQn);
	ifIsPendingIrqForLPTIM2() STM32CubeDuinoIrqHandlerForLPTIM2();
	ifIsPendingIrqForTIM7() STM32CubeDuinoIrqHandlerForTIM7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or LPTIM_PRESCALER_DIV1 or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM8 break global interrupt */
void TIM8_BRK_IRQHandler(void) {
	clearNvicIrqFor(TIM8_BRK_IRQn);
	STM32CubeDuinoIrqHandlerForTIM8();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM8 break interrupt and TIM12 global interrupt */
void TIM8_BRK_TIM12_IRQHandler(void) {
	clearNvicIrqFor(TIM8_BRK_TIM12_IRQn);
	STM32CubeDuinoIrqHandlerForTIM8();
	STM32CubeDuinoIrqHandlerForTIM12();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM8 capture compare interrupt */
void TIM8_CC_IRQHandler(void) {
	clearNvicIrqFor(TIM8_CC_IRQn);
	STM32CubeDuinoIrqHandlerForTIM8();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM8 trigger and commutation interrupts */
void TIM8_TRG_COM_IRQHandler(void) {
	clearNvicIrqFor(TIM8_TRG_COM_IRQn);
	STM32CubeDuinoIrqHandlerForTIM8();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM8 trigger and commutation interrupts and TIM14 global interrupt */
void TIM8_TRG_COM_TIM14_IRQHandler(void) {
	clearNvicIrqFor(TIM8_TRG_COM_TIM14_IRQn);
	STM32CubeDuinoIrqHandlerForTIM8();
	ifIsPendingIrqForTIM14() STM32CubeDuinoIrqHandlerForTIM14();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM8 update interrupt */
void TIM8_UP_IRQHandler(void) {
	clearNvicIrqFor(TIM8_UP_IRQn);
	STM32CubeDuinoIrqHandlerForTIM8();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM8 update interrupt and TIM13 global interrupt */
void TIM8_UP_TIM13_IRQHandler(void) {
	clearNvicIrqFor(TIM8_UP_TIM13_IRQn);
	STM32CubeDuinoIrqHandlerForTIM8();
	STM32CubeDuinoIrqHandlerForTIM13();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM9 global interrupt */
void TIM9_IRQHandler(void) {
	clearNvicIrqFor(TIM9_IRQn);
	STM32CubeDuinoIrqHandlerForTIM9();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM10 global interrupt */
void TIM10_IRQHandler(void) {
	clearNvicIrqFor(TIM10_IRQn);
	STM32CubeDuinoIrqHandlerForTIM10();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM11 global interrupt */
void TIM11_IRQHandler(void) {
	clearNvicIrqFor(TIM11_IRQn);
	STM32CubeDuinoIrqHandlerForTIM11();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM12 global interrupt */
void TIM12_IRQHandler(void) {
	clearNvicIrqFor(TIM12_IRQn);
	STM32CubeDuinoIrqHandlerForTIM12();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM13 global interrupt */
void TIM13_IRQHandler(void) {
	clearNvicIrqFor(TIM13_IRQn);
	STM32CubeDuinoIrqHandlerForTIM13();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM14 global interrupt */
void TIM14_IRQHandler(void) {
	clearNvicIrqFor(TIM14_IRQn);
	ifIsPendingIrqForTIM14() STM32CubeDuinoIrqHandlerForTIM14();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM15 global interrupt */
void TIM15_IRQHandler(void) {
	clearNvicIrqFor(TIM15_IRQn);
	ifIsPendingIrqForTIM15() STM32CubeDuinoIrqHandlerForTIM15();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM16 global interrupt */
void TIM16_IRQHandler(void) {
	clearNvicIrqFor(TIM16_IRQn);
	ifIsPendingIrqForTIM16() STM32CubeDuinoIrqHandlerForTIM16();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM17 global interrupt */
void TIM17_IRQHandler(void) {
	clearNvicIrqFor(TIM17_IRQn);
	ifIsPendingIrqForTIM17() STM32CubeDuinoIrqHandlerForTIM17();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(DAC_TRIGGER_NONE)) || (!defined(STM32CUBEDUINO_DISABLE_DAC) && !defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM18 global interrupt and DAC2 underrun error interrupt */
void TIM18_DAC2_IRQHandler(void) {
	clearNvicIrqFor(TIM18_DAC2_IRQn);
	STM32CubeDuinoIrqHandlerForDAC2();
	STM32CubeDuinoIrqHandlerForTIM18();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or DAC_TRIGGER_NONE or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM19 global interrupt */
void TIM19_IRQHandler(void) {
	clearNvicIrqFor(TIM19_IRQn);
	STM32CubeDuinoIrqHandlerForTIM19();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM20 break interrupt */
void TIM20_BRK_IRQHandler(void) {
	clearNvicIrqFor(TIM20_BRK_IRQn);
	STM32CubeDuinoIrqHandlerForTIM20();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM20 capture compare interrupt */
void TIM20_CC_IRQHandler(void) {
	clearNvicIrqFor(TIM20_CC_IRQn);
	STM32CubeDuinoIrqHandlerForTIM20();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM20 trigger and commutation interrupts */
void TIM20_TRG_COM_IRQHandler(void) {
	clearNvicIrqFor(TIM20_TRG_COM_IRQn);
	STM32CubeDuinoIrqHandlerForTIM20();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM20 update interrupt */
void TIM20_UP_IRQHandler(void) {
	clearNvicIrqFor(TIM20_UP_IRQn);
	STM32CubeDuinoIrqHandlerForTIM20();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM21 global interrupt */
void TIM21_IRQHandler(void) {
	clearNvicIrqFor(TIM21_IRQn);
	STM32CubeDuinoIrqHandlerForTIM21();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM22 global interrupt */
void TIM22_IRQHandler(void) {
	clearNvicIrqFor(TIM22_IRQn);
	STM32CubeDuinoIrqHandlerForTIM22();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM23 global interrupt */
void TIM23_IRQHandler(void) {
	clearNvicIrqFor(TIM23_IRQn);
	STM32CubeDuinoIrqHandlerForTIM23();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_ALL_TIMERS) && defined(TIM_CLOCKPRESCALER_DIV1))
/** TIM24 global interrupt */
void TIM24_IRQHandler(void) {
	clearNvicIrqFor(TIM24_IRQn);
	STM32CubeDuinoIrqHandlerForTIM24();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or TIM_CLOCKPRESCALER_DIV1 */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Touch sensing controller interrupt */
void TSC_IRQHandler(void) {
	clearNvicIrqFor(TSC_IRQn);
	ifIsPendingIrqForTSC_EOA() STM32CubeDuinoIrqHandlerForTSC();
	ifIsPendingIrqForTSC_MCE() STM32CubeDuinoIrqHandlerForTSC();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** UART4 global interrupt / UART4 wake-up interrupt through EXTI line 29 */
void UART4_IRQHandler(void) {
	clearNvicIrqFor(UART4_IRQn);
	STM32CubeDuinoIrqHandlerForUART4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** UART5 global interrupt / UART5 wake-up interrupt through EXTI line 30 */
void UART5_IRQHandler(void) {
	clearNvicIrqFor(UART5_IRQn);
	STM32CubeDuinoIrqHandlerForUART5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** UART7 global interrupt */
void UART7_IRQHandler(void) {
	clearNvicIrqFor(UART7_IRQn);
	STM32CubeDuinoIrqHandlerForUART7();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** UART8 global interrupt */
void UART8_IRQHandler(void) {
	clearNvicIrqFor(UART8_IRQn);
	STM32CubeDuinoIrqHandlerForUART8();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** UART9 global interrupt */
void UART9_IRQHandler(void) {
	clearNvicIrqFor(UART9_IRQn);
	STM32CubeDuinoIrqHandlerForUART9();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** UART10 global interrupt */
void UART10_IRQHandler(void) {
	clearNvicIrqFor(UART10_IRQn);
	STM32CubeDuinoIrqHandlerForUART10();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** UCPD1 interrupt / UCPD1 wake-up interrupt through EXTI line 41 */
void UCPD1_IRQHandler(void) {
	clearNvicIrqFor(UCPD1_IRQn);
	ifIsPendingIrqForUCPD1() STM32CubeDuinoIrqHandlerForUCPD1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** UCPD1 and UCPD2 interrupts / UCPD1 and UCPD2 wake-up interrupts through EXTI lines 32 and 33 */
void UCPD1_2_IRQHandler(void) {
	clearNvicIrqFor(UCPD1_2_IRQn);
	ifIsPendingIrqForUCPD1() STM32CubeDuinoIrqHandlerForUCPD1();
	ifIsPendingIrqForUCPD2() STM32CubeDuinoIrqHandlerForUCPD2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25 */
void USART1_IRQHandler(void) {
	clearNvicIrqFor(USART1_IRQn);
	ifIsPendingIrqForUSART1() STM32CubeDuinoIrqHandlerForUSART1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26 */
void USART2_IRQHandler(void) {
	clearNvicIrqFor(USART2_IRQn);
	ifIsPendingIrqForUSART2() STM32CubeDuinoIrqHandlerForUSART2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** USART3 global interrupt / USART3 wake-up interrupt through EXTI line 28 */
void USART3_IRQHandler(void) {
	clearNvicIrqFor(USART3_IRQn);
	ifIsPendingIrqForUSART3() STM32CubeDuinoIrqHandlerForUSART3();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** USART3 and USART4 global interrupts */
void USART3_4_IRQHandler(void) {
	clearNvicIrqFor(USART3_4_IRQn);
	ifIsPendingIrqForUSART3() STM32CubeDuinoIrqHandlerForUSART3();
	ifIsPendingIrqForUSART4() STM32CubeDuinoIrqHandlerForUSART4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** USART3, USART4 and LPUART1 interrupts / LPUART1 wake-up interrupt through EXTI line 28 */
void USART3_4_LPUART1_IRQHandler(void) {
	clearNvicIrqFor(USART3_4_LPUART1_IRQn);
	ifIsPendingIrqForLPUART1() STM32CubeDuinoIrqHandlerForLPUART1();
	ifIsPendingIrqForUSART3() STM32CubeDuinoIrqHandlerForUSART3();
	ifIsPendingIrqForUSART4() STM32CubeDuinoIrqHandlerForUSART4();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** USART3 to USART6 global interrupts */
void USART3_6_IRQHandler(void) {
	clearNvicIrqFor(USART3_6_IRQn);
	ifIsPendingIrqForUSART3() STM32CubeDuinoIrqHandlerForUSART3();
	ifIsPendingIrqForUSART4() STM32CubeDuinoIrqHandlerForUSART4();
	ifIsPendingIrqForUSART5() STM32CubeDuinoIrqHandlerForUSART5();
	ifIsPendingIrqForUSART6() STM32CubeDuinoIrqHandlerForUSART6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** USART3 to USART8 global interrupts / USART3 wake-up interrupt through EXTI line 28 */
void USART3_8_IRQHandler(void) {
	clearNvicIrqFor(USART3_8_IRQn);
	ifIsPendingIrqForUSART3() STM32CubeDuinoIrqHandlerForUSART3();
	ifIsPendingIrqForUSART4() STM32CubeDuinoIrqHandlerForUSART4();
	ifIsPendingIrqForUSART5() STM32CubeDuinoIrqHandlerForUSART5();
	ifIsPendingIrqForUSART6() STM32CubeDuinoIrqHandlerForUSART6();
	ifIsPendingIrqForUSART7() STM32CubeDuinoIrqHandlerForUSART7();
	ifIsPendingIrqForUSART8() STM32CubeDuinoIrqHandlerForUSART8();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** USART4 and USART5 interrupt */
void USART4_5_IRQHandler(void) {
	clearNvicIrqFor(USART4_5_IRQn);
	ifIsPendingIrqForUSART4() STM32CubeDuinoIrqHandlerForUSART4();
	ifIsPendingIrqForUSART5() STM32CubeDuinoIrqHandlerForUSART5();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** USART6 global interrupt */
void USART6_IRQHandler(void) {
	clearNvicIrqFor(USART6_IRQn);
	ifIsPendingIrqForUSART6() STM32CubeDuinoIrqHandlerForUSART6();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE))
/** USART10 global interrupt */
void USART10_IRQHandler(void) {
	clearNvicIrqFor(USART10_IRQn);
	STM32CubeDuinoIrqHandlerForUSART10();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or IS_UART_MODE */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** USB host EHCI global interrupt */
void USBH_EHCI_IRQHandler(void) {
	clearNvicIrqFor(USBH_EHCI_IRQn);
	STM32CubeDuinoIrqHandlerForUSBH_HS2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** USB host OHCI global interrupt */
void USBH_OHCI_IRQHandler(void) {
	clearNvicIrqFor(USBH_OHCI_IRQn);
	STM32CubeDuinoIrqHandlerForUSBH_HS1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB wake-up interrupt through EXTI line 18 */
void USBWakeUp_IRQHandler(void) {
	clearNvicIrqFor(USBWakeUp_IRQn);
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB wake-up interrupt through EXTI line 18 remap */
void USBWakeUp_RMP_IRQHandler(void) {
	clearNvicIrqFor(USBWakeUp_RMP_IRQn);
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB FS global interrupt / USB FS wake-up interrupt through EXTI line 34 */
void USB_FS_IRQHandler(void) {
	clearNvicIrqFor(USB_FS_IRQn);
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB FS wake-up interrupt through EXTI line 18 */
void USB_FS_WKUP_IRQHandler(void) {
	clearNvicIrqFor(USB_FS_WKUP_IRQn);
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** CAN TX and USB high priority interrupts */
void USB_HP_CAN_TX_IRQHandler(void) {
	clearNvicIrqFor(USB_HP_CAN_TX_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB high priority or CAN TX interrupts */
void USB_HP_CAN1_TX_IRQHandler(void) {
	clearNvicIrqFor(USB_HP_CAN1_TX_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB high priority global interrupt */
void USB_HP_IRQHandler(void) {
	clearNvicIrqFor(USB_HP_IRQn);
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB global interrupt / USB wake-up interrupt through EXTI line 18 */
void USB_IRQHandler(void) {
	clearNvicIrqFor(USB_IRQn);
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** CAN RX0 and USB low priority interrupts */
void USB_LP_CAN_RX0_IRQHandler(void) {
	clearNvicIrqFor(USB_LP_CAN_RX0_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB low priority or CAN RX0 interrupts */
void USB_LP_CAN1_RX0_IRQHandler(void) {
	clearNvicIrqFor(USB_LP_CAN1_RX0_IRQn);
	ifIsPendingIrqForCAN() STM32CubeDuinoIrqHandlerForCAN();
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (!defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED))
/** USB low priority interrupt, USB wake-up interrupt through EXTI line 28 */
void USB_LP_IRQHandler(void) {
	clearNvicIrqFor(USB_LP_IRQn);
	ifIsPendingIrqForUSB() STM32CubeDuinoIrqHandlerForUSB();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS or PCD_PHY_EMBEDDED */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Interrupt for all 6 wake-up pins */
void WAKEUP_PIN_IRQHandler(void) {
	clearNvicIrqFor(WAKEUP_PIN_IRQn);
	ifIsPendingIrqForPVDOUT() STM32CubeDuinoIrqHandlerForPWR();
	ifIsPendingIrqForPVMOUT() STM32CubeDuinoIrqHandlerForPWR();
	ifIsPendingIrqForVDDIO2() STM32CubeDuinoIrqHandlerForPWR();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Window watchdog interrupt */
void WWDG_IRQHandler(void) {
	clearNvicIrqFor(WWDG_IRQn);
	ifIsPendingIrqForEWDG() STM32CubeDuinoIrqHandlerForWWDG();
	ifIsPendingIrqForWWDG() STM32CubeDuinoIrqHandlerForWWDG();
	STM32CubeDuinoIrqHandlerForWWDG1();
	STM32CubeDuinoIrqHandlerForWWDG2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Window watchdog reset interrupt */
void WWDG_RST_IRQHandler(void) {
	clearNvicIrqFor(WWDG_RST_IRQn);
	STM32CubeDuinoIrqHandlerForWWDG1();
	STM32CubeDuinoIrqHandlerForWWDG2();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */


#ifdef STM32CUBEDUINO_MAP_ALL_IRQS
/** Window watchdog interrupt */
void WWDG1_IRQHandler(void) {
	clearNvicIrqFor(WWDG1_IRQn);
	STM32CubeDuinoIrqHandlerForWWDG1();
}
#endif /* STM32CUBEDUINO_MAP_ALL_IRQS */
#endif /* STM32CUBEDUINO_CUSTOM_IRQ_HANDLERS */
} /* extern "C" */
