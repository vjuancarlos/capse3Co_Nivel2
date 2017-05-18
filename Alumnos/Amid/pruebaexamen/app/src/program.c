/*============================================================================
 * License: "THE BEER-WARE LICENSE" (Revision 42):
 * Author: Amid Ale
 * Date: 03-05-17
 *===========================================================================*/

/*==================[inclusions]=============================================*/

#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library

   uint8_t dato;
   
   uint8_t ledsVector[4] = {
   LEDB,    
   LED1,    
   LED2,    
   LED3     
};	//Estructura con nombres de leds utilizados



// MAIN FUNCTION, ENTRY PINT TO THE PROGRAM AFTER POR OR RESET.
int main( void ){

  boardConfig();     	  //Configuro la placa
  uartConfig(UART_USB,115200);  //Configuro la UART del USB	
  
  
   delay_t delayBlink;  //Defino una variable delay_t
   uint8_t estadoLed=0; //Defino variable estado de leds Define si el led está prendido o apagado
   
   uint8_t led=0;       //Defino variable led Permite seleccionar el led a blinkear

  while (1)
    {
     if (!(gpioRead(TEC1)))	//Configuro delay no bloqueante a 50 ms si presiono TEC1
       {
     	delayConfig(&delayBlink,50);
       }
     if (!(gpioRead(TEC2)))   //Configuro delay no bloqueante a 100 ms si presiono TEC2
       {
         delayConfig(&delayBlink,100);
       }
     if (!(gpioRead(TEC3))) 	//Configuro delay no bloqueante a 200 ms si presiono TEC3
       {
        delayConfig(&delayBlink,200);
       }
     if (!(gpioRead(TEC4)))   //Configuro delay no bloqueante a 500 ms si presiono TEC4
       {
        delayConfig(&delayBlink,500);    
       } 
     
     if (uartReadByte(UART_USB, &dato))	//si llegó un nuevo dato
       { 
        switch (dato)
              {
               case 'a':			//Apago led Azul
                 gpioWrite( ledsVector[led], OFF );
                 led=0;
                 break;
               case '1':   			//Apago led 1
                 gpioWrite( ledsVector[led], OFF );
                 led=1;	     	
	       break;
               case '2':			//Apago led 2
                 gpioWrite( ledsVector[led], OFF );
                 led=2;     	
	       break;
               case '3':      		//Apago led 3
                 gpioWrite( ledsVector[led], OFF );
                 led=3;	
	       break;
              }
        }
    if (delayRead(&delayBlink))	//Si se llega al tiempo indicado cambio de estado el led
      {
       gpioWrite( ledsVector[led], estadoLed );
       estadoLed ^= 1;
      }  
    }
     
   return 0;
}
