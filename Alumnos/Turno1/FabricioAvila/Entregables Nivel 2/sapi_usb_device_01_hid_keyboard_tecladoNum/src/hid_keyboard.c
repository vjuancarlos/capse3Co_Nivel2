/*
 * @brief This file contains USB HID Keyboard example using USB ROM Drivers.
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "sapi.h"
#include <stdint.h>
#include <string.h>
#include "keypad4x4.h"
#include "usbd_rom_api.h"
#include "hid_keyboard.h"
uint8_t  state = 0;
uint16_t tecl_pres = 0;
// A�adidas las teclas del teclado matricial - state = 0
// En este modo el led verde est� apagado
uint16_t keypadKeys[16] = {
                               0x1E, 0x1F, 0x20, 0x04,
                               0x21, 0x22, 0x23, 0x05,
                               0x24, 0x25, 0x26, 0x06,
                               0x55, 0x27, 0xCC, 0x07
                          };
//A�adidas las teclas "ENTER" y "BACKSPACE"
//En este modo el led verde est� encendido para indicar state = 1
uint16_t keypadKeysVol[2] = {
							   0x28, 0x2A
};

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/**
 * @brief Structure to hold Keyboard data
 */
typedef struct {
	USBD_HANDLE_T hUsb;	/*!< Handle to USB stack. */
	uint8_t report[KEYBOARD_REPORT_SIZE];	/*!< Last report data  */
	uint8_t tx_busy;	/*!< Flag indicating whether a report is pending in endpoint queue. */
} Keyboard_Ctrl_T;

/** Singleton instance of Keyboard control */
static Keyboard_Ctrl_T g_keyBoard;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

extern const uint8_t Keyboard_ReportDescriptor[];
extern const uint16_t Keyboard_ReportDescSize;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Routine to update keyboard state */
static void Keyboard_UpdateReport(void){

   HID_KEYBOARD_CLEAR_REPORT(&g_keyBoard.report[0]);
 // Selecci�n del state, para definir la funci�n del teclado matricial
   if( !gpioRead( TEC1 ) ){
	   if (state==0)
		   state=1; //Funcionalidad de ENTER y BACKSPACE
	   else
		   state=0; //Funcionalidad normal del teclado matricial
   }

   tecl_pres =leerTecladoMatricial();  // lectura de la tecla presionada
   switch(state){

   case 0:
	   gpioWrite (LEDG, OFF);
	   if(tecl_pres!=88){
	   	   HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report, keypadKeys[tecl_pres]);
	      }
	   break;
   case 1:
	   gpioWrite (LEDG, ON);
	   if(tecl_pres!=88){
	   	   	   HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report, keypadKeysVol[tecl_pres]);
	   	      }
	   break;
   }


/*   if( !gpioRead( TEC1 ) ){
      HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report, 0x06); /* 'c' */
/*   }
   else if( !gpioRead( TEC2 ) ){
      HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report, 0x0C); /* 'i' */
/*   }
   else if( !gpioRead( TEC3 ) ){
      HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report, 0x04); /* 'a' */
/*   }
   else if( !gpioRead( TEC4 ) ){
      HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report, 0x28); /* 'ENTER' */
//   }

}

/* HID Get Report Request Callback. Called automatically on HID Get Report Request */
static ErrorCode_t Keyboard_GetReport( USBD_HANDLE_T hHid,
                                       USB_SETUP_PACKET *pSetup,
                                       uint8_t * *pBuffer,
                                       uint16_t *plength ){

	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {

      case HID_REPORT_INPUT:
         Keyboard_UpdateReport();
         memcpy(*pBuffer, &g_keyBoard.report[0], KEYBOARD_REPORT_SIZE);
         *plength = KEYBOARD_REPORT_SIZE;
      break;

      case HID_REPORT_OUTPUT:				/* Not Supported */
      case HID_REPORT_FEATURE:			/* Not Supported */

         return ERR_USBD_STALL;
	}

	return LPC_OK;
}

/* HID Set Report Request Callback. Called automatically on HID Set Report Request */
static ErrorCode_t Keyboard_SetReport( USBD_HANDLE_T hHid,
                                       USB_SETUP_PACKET *pSetup,
                                       uint8_t * *pBuffer,
                                       uint16_t length){

   /* we will reuse standard EP0Buf */
   if (length == 0) {
      return LPC_OK;
   }

   /* ReportID = SetupPacket.wValue.WB.L; */
   switch (pSetup->wValue.WB.H){

      case HID_REPORT_OUTPUT:
         /*  If the USB host tells us to turn on the NUM LOCK LED,
          *  then turn on LED#2.
          */
         if (**pBuffer & 0x01) {
            gpioWrite( LEDB, ON );
         }
         else {
            gpioWrite( LEDB, OFF);
         }
      break;

      case HID_REPORT_INPUT:				/* Not Supported */
      case HID_REPORT_FEATURE:			/* Not Supported */

         return ERR_USBD_STALL;
   }

   return LPC_OK;
}

/* HID interrupt IN endpoint handler */
static ErrorCode_t Keyboard_EpIN_Hdlr( USBD_HANDLE_T hUsb,
                                       void *data,
                                       uint32_t event ){
   switch (event) {
      case USB_EVT_IN:
         g_keyBoard.tx_busy = 0;
         break;
   }
   return LPC_OK;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* HID keyboard init routine */
ErrorCode_t Keyboard_init(USBD_HANDLE_T hUsb,
						  USB_INTERFACE_DESCRIPTOR *pIntfDesc,
						  uint32_t *mem_base,
						  uint32_t *mem_size){

   USBD_HID_INIT_PARAM_T hid_param;
   USB_HID_REPORT_T reports_data[1];
   ErrorCode_t ret = LPC_OK;

   /* Do a quick check of if the interface descriptor passed is the right one. */
   if( (pIntfDesc == 0) || (pIntfDesc->bInterfaceClass
       != USB_DEVICE_CLASS_HUMAN_INTERFACE)) {
      return ERR_FAILED;
   }

   /* Init HID params */
   memset((void *) &hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));
   hid_param.max_reports = 1;
   hid_param.mem_base = *mem_base;
   hid_param.mem_size = *mem_size;
   hid_param.intf_desc = (uint8_t *) pIntfDesc;

   /* user defined functions */
   hid_param.HID_GetReport = Keyboard_GetReport;
   hid_param.HID_SetReport = Keyboard_SetReport;
   hid_param.HID_EpIn_Hdlr  = Keyboard_EpIN_Hdlr;

   /* Init reports_data */
   reports_data[0].len = Keyboard_ReportDescSize;
   reports_data[0].idle_time = 0;
   reports_data[0].desc = (uint8_t *) &Keyboard_ReportDescriptor[0];
   hid_param.report_data  = reports_data;

   ret = USBD_API->hid->init(hUsb, &hid_param);

   /* update memory variables */
   *mem_base = hid_param.mem_base;
   *mem_size = hid_param.mem_size;

   /* store stack handle for later use. */
   g_keyBoard.hUsb = hUsb;
   g_keyBoard.tx_busy = 0;

   return ret;
}

/* Keyboard tasks */
void Keyboard_Tasks(void)
{
	/* check device is configured before sending report. */
	if ( USB_IsConfigured(g_keyBoard.hUsb)) {

		/* send report data */
		if (g_keyBoard.tx_busy == 0) {
			g_keyBoard.tx_busy = 1;

			/* update report based on board state */
			Keyboard_UpdateReport();
			USBD_API->hw->WriteEP( g_keyBoard.hUsb,
			                       HID_EP_IN,
			                       &g_keyBoard.report[0],
			                       KEYBOARD_REPORT_SIZE );
		}
	}
	else {
		/* reset busy flag if we get disconnected. */
		g_keyBoard.tx_busy = 0;
	}

}
