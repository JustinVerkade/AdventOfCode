/*
 * System.h
 *
 *  Created on: 3 aug. 2023
 *      Author: J.Verkade
 */

#ifndef INC_SYSTEM_SYSTEM_H_
#define INC_SYSTEM_SYSTEM_H_

#include "fatfs.h"
#include "stdint.h"
#include "string.h"
#include "main.h"

#define SRAM_ADDRESS ((uint32_t*)0x60000000ul)

typedef enum SystemError_t SystemError_t;
enum SystemError_t
{
	SYS_ERROR_SRAM_FAIL,
	SYS_ERROR_SDIO_FAIL,
	SYS_ERROR_WIFI_FAIL,
};

typedef enum SystemState_t SystemState_t;
enum SystemState_t
{
	SYS_STATE_DISCONNECTED,
	SYS_STATE_CONNECTED,
	SYS_STATE_IDLE,
	SYS_STATE_ERROR,
};

typedef struct System_t System_t;
struct System_t
{
	// virtual comport status
	uint8_t vcom_open;

	// hardware functionality states
	uint8_t sram_working;
	uint8_t sdio_working;
	uint8_t wifi_working;

	// sdio interface
	struct
	{
		FIL loaded_file;
		FATFS file_system;
		FRESULT file_result;
	} sdio;


	// error state
	SystemError_t error;
};

// state functions
void System_init(System_t *state);
void System_selfTest(System_t *state);

// interface log functions
void System_initLog(System_t *state);
void System_selfTestLog(System_t *state);

#endif /* INC_SYSTEM_SYSTEM_H_ */
