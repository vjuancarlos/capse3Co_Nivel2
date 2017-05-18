/* Copyright (c) 2017, Eric Pernia.
 * All rights reserved.
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
 * ============================================================================
 * Date: 2017-03-15
 * ============================================================================
 */

/*==================[inclusions]=============================================*/

#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library

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
   
   // ---------- REPEAT FOR EVER ------------------------------
   while( TRUE )
   {
      // If TEC1 is pressed, turn on the LEDB
      gpioWrite( LEDB, !(gpioRead(TEC1)) );
      
      // If TEC2 is pressed, turn on the LED1
      gpioWrite( LED1, !(gpioRead(TEC2)) );
      
      // If TEC3 is pressed, turn on the LED2
      gpioWrite( LED2, !(gpioRead(TEC3)) );
      
      // If TEC4 is pressed, turn on the LED3
      gpioWrite( LED3, !(gpioRead(TEC4)) );
   } 

   // IT SHOULD NEVER COME HERE, because this program runs directly on a
   // microcontroller and is not called by any Operating System, as in
   // case of a PC program.
   return 0;
}

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*==================[end of file]============================================*/
