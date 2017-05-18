/*=============================================================================
Ejercicio Entregable 10
Autores:
        Gessi Paniagua
        Guillermo López
        Amid Ale
Implementación de MEF en un ascensor
 =============================================================================*/
 

/*==================[inclusions]=============================================*/

#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library
#include "driveruart.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[main function]==========================================*/

// MAIN FUNCTION, ENTRY PINT TO THE PROGRAM AFTER POR OR RESET.
int main( void ){

   // ---------- SETUP ----------------------------------------
   // Initialize an configurate the board
   boardConfig();
   entrarModoMenu();
   // ---------- REPEAT FOR EVER ------------------------------
   while( TRUE )
   {

 
   }

   // IT SHOULD NEVER COME HERE, because this program runs directly on a
   // microcontroller and is not called by any Operating System, as in
   // case of a PC program.
   return 0;
}

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*==================[end of file]============================================*/
