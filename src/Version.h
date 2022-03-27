/**
 * @file Version.h
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-06-07
 * @version 2022-03-21
 * 
 * Inspired from Linux Kernel. Usage example:
 * @code .cpp
 * #if STM32CUBEDUINO_VERSION_CODE >= STM32CUBEDUINO_VERSION(1, 0, 0)
 * 	// include code compatible with STM32CubeDuino version 1.0.0 and newer here
 * #endif
 * @endcode
 */
#ifndef __STM32CUBEDUINO_VERSION_H__
#define __STM32CUBEDUINO_VERSION_H__


/** Derived from `ARDUINO` which is defined in `BuildScript.py` */
#define ARDUINO_API_VERSION ARDUINO

#define STM32CUBEDUINO "1.0.0"
#define STM32CUBEDUINO_MAJOR 1
#define STM32CUBEDUINO_MINOR 0
#define STM32CUBEDUINO_PATCH 0

#define STM32CUBEDUINO_VERSION(x, y, z) ((x << 16) + (y << 8) + z)
#define STM32CUBEDUINO_VERSION_CODE STM32CUBEDUINO_VERSION(STM32CUBEDUINO_MAJOR, STM32CUBEDUINO_MINOR, STM32CUBEDUINO_PATCH)


#endif /* __STM32CUBEDUINO_VERSION_H__ */
