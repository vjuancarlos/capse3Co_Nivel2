/*============================================================================
 * License: 
 * Author: 
 * Date: 
 *===========================================================================*/

/*==================[inclusions]=============================================*/

#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library


#define TIEMPOBLINKEO 500




/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[main function]==========================================*/

// MAIN FUNCTION, ENTRY PINT TO THE PROGRAM AFTER POR OR RESET.

delay_t blinkTime;
uint8_t led;
 
uint8_t led = 0;
delayConfig(&blinkTime, TIEMPOBLINKEO);


int main( void ){

   // ---------- SETUP ----------------------------------------
   // Initialize an configurate the board
   boardConfig();   
   
   // ---------- REPEAT FOR EVER ------------------------------
   led = 0;
   while( TRUE )
   {
      
      if (gpioRead(TEC1)){led=1;}
      
      if (gpioRead(TEC2)){led=2;}
      
      if (gpioRead(TEC3)){led=3;}
      
      if (gpioRead(TEC4)){led=4;}
    
   if (delayRead(&blinkTime)) {
      if (led == 1)
        {
        	gpioToggle(LEDR);
        } 
      
      if (led == 2)
        {
        	gpioToggle(LED1);
        }
        
      if (led == 3)
        {
        	gpioToggle(LED2);
        } 
     
     if (led == 4)
        {
        	gpioToggle(LED3);
        }  
        
} 
}
   // IT SHOULD NEVER COME HERE, because this program runs directly on a
   // microcontroller and is not called by any Operating System, as in
   // case of a PC program.
   return 0;
}

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*==================[end of file]============================================*/
