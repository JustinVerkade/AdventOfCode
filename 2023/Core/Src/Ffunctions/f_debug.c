/*
 * FatFsTools.c
 *
 *  Created on: 14 jul. 2023
 *      Author: J.Verkade
 */

#include "Ffunctions/f_debug.h"

#include "fatfs.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "string.h"

#define f_debugPrint(error_message) CDC_Transmit_HS((uint8_t*)error_message, strlen(error_message))

void f_debug(uint8_t f_result)
{
	switch(f_result)
	{
	case FR_DISK_ERR:
		f_debugPrint("[FATFS] error: FR_DISK_ERR!\n");
		break;
	case FR_INT_ERR:
		f_debugPrint("[FATFS] error: FR_INT_ERR!\n");
		break;
	case FR_NOT_READY:
		f_debugPrint("[FATFS] error: FR_NOT_READY!\n");
		break;
	case FR_NO_FILE:
		f_debugPrint("[FATFS] error: FR_NO_FILE!\n");
		break;
	case FR_NO_PATH:
		f_debugPrint("[FATFS] error: FR_NO_PATH!\n");
		break;
	case FR_DENIED:
		f_debugPrint("[FATFS] error: FR_DENIED!\n");
		break;
	case FR_EXIST:
		f_debugPrint("[FATFS] error: FR_EXIST!\n");
		break;
	case FR_INVALID_OBJECT:
		f_debugPrint("[FATFS] error: FR_INVALID_OBJECT!\n");
		break;
	case FR_WRITE_PROTECTED:
		f_debugPrint("[FATFS] error: FR_WRITE_PROTECTED!\n");
		break;
	case FR_NOT_ENABLED:
		f_debugPrint("[FATFS] error: FR_NOT_ENABLED!\n");
		break;
	case FR_NO_FILESYSTEM:
		f_debugPrint("[FATFS] error: FR_NO_FILESYSTEM!\n");
		break;
	case FR_MKFS_ABORTED:
		f_debugPrint("[FATFS] error: FR_MKFS_ABORTED!\n");
		break;
	case FR_TIMEOUT:
		f_debugPrint("[FATFS] error: FR_TIMEOUT!\n");
		break;
	case FR_LOCKED:
		f_debugPrint("[FATFS] error: FR_LOCKED!\n");
		break;
	case FR_NOT_ENOUGH_CORE:
		f_debugPrint("[FATFS] error: FR_NOT_ENOUGH_CORE!\n");
		break;
	case FR_TOO_MANY_OPEN_FILES:
		f_debugPrint("[FATFS] error: FR_TOO_MANY_OPEN_FILES!\n");
		break;
	case FR_INVALID_PARAMETER:
		f_debugPrint("[FATFS] error: FR_INVALID_PARAMETER!\n");
		break;
	default:
		break;
	}
}
