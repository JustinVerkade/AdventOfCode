/*
 * System.c
 *
 *  Created on: 3 aug. 2023
 *      Author: J.Verkade
 */

#include "System/system.h"
#include "usbd_cdc_if.h"

extern USBD_HandleTypeDef hUsbDeviceHS;

// state functions
void System_init(System_t *state)
{
	// system resource states
	state->sram_working = 1;
	state->sdio_working = 1;
	state->wifi_working = 1;

	// external interface status
	state->vcom_open = 0;

	// perform selftest
	System_selfTest(state);
}

void System_selfTest(System_t *state)
{
	// test psram
	uint32_t *ptr = SRAM_ADDRESS + 1000000ul;
	for(uint32_t byte=0; byte<1000000ul; byte++)
		*(ptr + byte) = 0xFFFF & (byte >> 4);
	for(uint32_t byte=0; byte<1000000ul; byte++)
		if(*(ptr + byte) != (0xFFFF & (byte >> 4)))
			state->sram_working = 0;

	// test sdio
	state->sdio.file_result = f_mount(&state->sdio.file_system, (const TCHAR*)SDPath, 0);
	if(state->sdio.file_result != FR_OK)
		state->sdio_working = 0;

	// test wifi
	// todo add wifi check
}

// interface log functions
void System_initLog(System_t *state)
{
	char *logo = "________________________________________________________________________________\n"
			"\n"
			"  ____________        __________      _______        _______\n"
			" |            |      /          \\     \\      \\      /      /\n"
			" |     _______|     /   ______   \\     \\      \\    /      /\n"
			" |    |            /   /      \\   \\     \\      \\  /      /\n"
			" |    |_______    |   |        |   |     \\      \\/      /\n"
			" |            |   |   |        |   |      \\            /\n"
			" |     _______|   |   |        |   |      /            \\\n"
			" |    |           |   |        |   |     /      /\\      \\\n"
			" |    |            \\   \\______/   /     /      /  \\      \\\n"
			" |    |             \\            /     /      /    \\      \\\n"
			" |____|              \\__________/     /______/      \\______\\\n"
			"\n"
			" Device: FOX-PDA-v1\n"
			" Author: Justin Verkade\n"
			" Function: General purpose high performance developement board.\n"
			"________________________________________________________________________________\n";

	while(((USBD_CDC_HandleTypeDef*)hUsbDeviceHS.pClassData)->TxState != 0);
	CDC_Transmit_HS((uint8_t*)logo, strlen(logo));
}

void System_selfTestLog(System_t *state)
{
	char *text = "\n"
			" System test:\n"
			" -> SRAM status: %s\n"
			" -> SDIO status: %s\n"
			" -> WIFI status: %s\n"
			"________________________________________________________________________________\n";

	char sram_str[16];
	if(state->sram_working)
		strcpy(sram_str, "ENABLED");
	else
		strcpy(sram_str, "DISABLED");

	char sdio_str[16];
	if(state->sdio_working)
		strcpy(sdio_str, "ENABLED");
	else
		strcpy(sdio_str, "DISABLED");

	char wifi_str[16];
	if(state->wifi_working)
		strcpy(wifi_str, "NOT IMPLEMENTED"); // todo wifi not functional
	else
		strcpy(wifi_str, "DISABLED");

	char text_buffer[256];
	sprintf(text_buffer, text, sram_str, sdio_str, wifi_str);

	while(((USBD_CDC_HandleTypeDef*)hUsbDeviceHS.pClassData)->TxState != 0);
	CDC_Transmit_HS((uint8_t*)text_buffer, strlen(text_buffer));
}
