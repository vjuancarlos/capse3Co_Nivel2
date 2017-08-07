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

/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2016-11-06
 */

/*==================[inclusions]=============================================*/

#include "sapi.h"         /* <= sAPI header */

#include <stdio.h>
#include <string.h>

#include "app_usbd_cfg.h"

#include "hid_keyboard.h"

#include "keypad4x4.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static USBD_HANDLE_T g_hUsb;

/* Endpoint 0 patch that prevents nested NAK event processing */
static uint32_t g_ep0RxBusy = 0;/* flag indicating whether EP0 OUT/RX buffer is busy. */
static USB_EP_HANDLER_T g_Ep0BaseHdlr; /* variable to store the pointer to base EP0 handler */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
const USBD_API_T *g_pUsbApi;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* EP0_patch part of WORKAROUND for artf45032. */
ErrorCode_t EP0_patch(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
   switch (event) {
   case USB_EVT_OUT_NAK:
      if (g_ep0RxBusy) {
         /* we already queued the buffer so ignore this NAK event. */
         return LPC_OK;
      }
      else {
         /* Mark EP0_RX buffer as busy and allow base handler to queue the buffer. */
         g_ep0RxBusy = 1;
      }
      break;

   case USB_EVT_SETUP:  /* reset the flag when new setup sequence starts */
   case USB_EVT_OUT:
      /* we received the packet so clear the flag. */
      g_ep0RxBusy = 0;
      break;
   }
   return g_Ep0BaseHdlr(hUsb, data, event);
}


/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief   Handle interrupt from USB
 * @return  Nothing
 */
void USB_IRQHandler(void){
   USBD_API->hw->ISR(g_hUsb);
}


/* Find the address of interface descriptor for given class type. */
USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc, uint32_t intfClass)
{
   USB_COMMON_DESCRIPTOR *pD;
   USB_INTERFACE_DESCRIPTOR *pIntfDesc = 0;
   uint32_t next_desc_adr;

   pD = (USB_COMMON_DESCRIPTOR *) pDesc;
   next_desc_adr = (uint32_t) pDesc;

   while (pD->bLength) {
      /* is it interface descriptor */
      if (pD->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE) {

         pIntfDesc = (USB_INTERFACE_DESCRIPTOR *) pD;
         /* did we find the right interface descriptor */
         if (pIntfDesc->bInterfaceClass == intfClass) {
            break;
         }
      }
      pIntfDesc = 0;
      next_desc_adr = (uint32_t) pD + pD->bLength;
      pD = (USB_COMMON_DESCRIPTOR *) next_desc_adr;
   }

   return pIntfDesc;
}


/**
 * @brief   Configuration routine for USBD UCOM example
 * maxNumEP max number of endpoints supported by the USB device
 *          controller instance (specified by \em usb_reg_base field)
 *          to which this instance of stack is attached.
 */
void usbDeviceConfig( uint8_t maxNumEP, int32_t usbPriority ){

   USBD_API_INIT_PARAM_T usb_param;
   USB_CORE_DESCS_T desc;
   ErrorCode_t ret = LPC_OK;
   USB_CORE_CTRL_T *pCtrl;

   /* enable clocks and pinmux */
   USB_init_pin_clk();

   /* Init USB API structure */
   g_pUsbApi = (const USBD_API_T *) LPC_ROM_API->usbdApiBase;

   /* initialize call back structures */
   memset((void *) &usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
   usb_param.usb_reg_base = LPC_USB_BASE;
   usb_param.mem_base = USB_STACK_MEM_BASE;
   usb_param.mem_size = USB_STACK_MEM_SIZE;

   usb_param.max_num_ep = maxNumEP; // @Eric - En keyboard tiene 2

   /* Set the USB descriptors */
   desc.device_desc = (uint8_t *) USB_DeviceDescriptor;
   desc.string_desc = (uint8_t *) USB_StringDescriptor;

   #ifdef USE_USB0
      desc.high_speed_desc = USB_HsConfigDescriptor;
      desc.full_speed_desc = USB_FsConfigDescriptor;
      desc.device_qualifier = (uint8_t *) USB_DeviceQualifier;
   #else
      /* Note, to pass USBCV test full-speed only devices should have both
       * descriptor arrays point to same location and device_qualifier set
       * to 0.
       */
      desc.high_speed_desc = USB_FsConfigDescriptor;
      desc.full_speed_desc = USB_FsConfigDescriptor;
      desc.device_qualifier = 0;
   #endif

   /* USB Initialization */
   ret = USBD_API->hw->Init(&g_hUsb, &desc, &usb_param);

   if (ret == LPC_OK) {

      /* WORKAROUND for artf45032 ROM driver BUG:
          Due to a race condition there is the chance that a second NAK event will
          occur before the default endpoint0 handler has completed its preparation
          of the DMA engine for the first NAK event. This can cause certain fields
          in the DMA descriptors to be in an invalid state when the USB controller
          reads them, thereby causing a hang.
       */
      pCtrl = (USB_CORE_CTRL_T *) g_hUsb; /* convert the handle to control structure */
      g_Ep0BaseHdlr = pCtrl->ep_event_hdlr[0];/* retrieve the default EP0_OUT handler */
      pCtrl->ep_event_hdlr[0] = EP0_patch;/* set our patch routine as EP0_OUT handler */


      ret = Keyboard_init( g_hUsb,
                           (USB_INTERFACE_DESCRIPTOR *) &USB_FsConfigDescriptor[sizeof(USB_CONFIGURATION_DESCRIPTOR)],
                           &usb_param.mem_base, &usb_param.mem_size );

      if (ret == LPC_OK) {
         /* Make sure USB and UART IRQ priorities are same for this example */
         if( usbPriority >=0 ){
            NVIC_SetPriority( LPC_USB_IRQ, (uint32_t)usbPriority ); // @Eric - En el de keyboard no modifica la prioridad de la IRQ
         }
          /*  enable USB interrupts */
         NVIC_EnableIRQ( LPC_USB_IRQ );
         /* now connect */
         USBD_API->hw->Connect( g_hUsb, 1 );
      }
   }

}



/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();
   configurarTecladoMatricial(); // Configurar teclado matricial
   bool_t ledStatus = OFF;
   delay_t ledDelay;

   delayConfig( &ledDelay, 500 );


   // Configuration routine for USBD UCOM example
   usbDeviceConfig( 2, -1 );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      /* Do Keyboard tasks */
      Keyboard_Tasks();

      if( delayRead( &ledDelay ) ){
         if( ledStatus ){
            ledStatus = OFF;
         } else{
            ledStatus = ON;
         }
      }

      /* Sleep until next IRQ happens */
      __WFI();
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
