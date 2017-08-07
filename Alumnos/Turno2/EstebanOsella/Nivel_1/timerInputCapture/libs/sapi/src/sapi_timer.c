/* Copyright 2016, Ian Olivieri
 * Copyright 2017, Eric Pernia
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

/* Date: 2016-02-10 */

/*
 * For more information about the Timer peripheral, refer to the Chapter 32 of
 * the LPC43xx user manual
 */

/*==================[inclusions]=============================================*/

#include "sapi_timer.h"

//#include "chip.h"
//#include "timer_18xx_43xx.h"
/* Specific modules used:
   #include "timer_18xx_43xx.h" for Chip_TIMER functions
   #include "rgu_18xx_43xx.h" for Chip_RGU functions
   #include "core_cm4.h" for NVIC functions
*/

/*==================[macros and definitions]=================================*/

#define LPC4337_MAX_FREC 204000000UL /* Microcontroller frequency */
#define MAX_SYSCALL_INTERRUPT_PRIORITY 5

/*==================[internal data declaration]==============================*/

typedef struct{
   LPC_TIMER_T *name;
   uint32_t RGU; /* Reset Generator Unit */
   uint32_t IRQn;
} timerStaticData_t;

typedef struct{
   voidFunctionPointer_t timerCompareMatchFunctionPointer[4];
} timerDinamicData_t;

/*==================[internal functions declaration]=========================*/

static void errorOcurred(void);
static void doNothing(void);

/*==================[internal data definition]===============================*/

/*Timers Static Data, given by the uC libraries*/
static const timerStaticData_t timer_sd[4] = {
   { LPC_TIMER0, RGU_TIMER0_RST, TIMER0_IRQn },
   { LPC_TIMER1, RGU_TIMER1_RST, TIMER1_IRQn },
   { LPC_TIMER2, RGU_TIMER2_RST, TIMER2_IRQn },
   { LPC_TIMER3, RGU_TIMER3_RST, TIMER3_IRQn }
};

/*Timers dynamic data. Function pointers and Compare match frequencies, which can vary.
 * This is the default initialization*/
static timerDinamicData_t timer_dd[4] = {
   {doNothing,errorOcurred,errorOcurred,errorOcurred},
   {doNothing,errorOcurred,errorOcurred,errorOcurred},
   {doNothing,errorOcurred,errorOcurred,errorOcurred},
   {doNothing,errorOcurred,errorOcurred,errorOcurred}
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/* Causes:
 * User forgot to initialize the functions for the compare match interrupt on Timer_init call
 */
static void errorOcurred(void){
   while(1);
}

static void doNothing(void){
}

/*==================[external functions definition]==========================*/


// NEW TIMER --------------------------------------------------------------------------------------------

volatile sAPI_FuncPtr_t timer0CaptureHookFunction[4] = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };
volatile sAPI_FuncPtr_t timer0MatchHookFunction[4]   = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };

volatile sAPI_FuncPtr_t timer1CaptureHookFunction[4] = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };
volatile sAPI_FuncPtr_t timer1MatchHookFunction[4]   = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };

volatile sAPI_FuncPtr_t timer2CaptureHookFunction[4] = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };
volatile sAPI_FuncPtr_t timer2MatchHookFunction[4]   = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };

volatile sAPI_FuncPtr_t timer3CaptureHookFunction[4] = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };
volatile sAPI_FuncPtr_t timer3MatchHookFunction[4]   = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };
                                                         
uint32_t timerGetClock( timerMap_t timer ){
   return ( LPC4337_MAX_FREC / timerGetPrescale( timer ) );
}

                                                         
uint32_t timerGetPrescale( timerMap_t timer ){
   switch( timer ){
      case TIMER0:
         return LPC_TIMER0->PR;
      break;      
      case TIMER1:
         return LPC_TIMER1->PR;
      break;      
      case TIMER2:      
         return LPC_TIMER2->PR;
      break;      
      case TIMER3:
         return LPC_TIMER3->PR;
      break;
      default:
         return 0;
   }
}


uint32_t timerReadCounter(timerMap_t timer ){
   switch( timer ){
      case TIMER0:
         return Chip_TIMER_ReadCount( LPC_TIMER0 );
      break;      
      case TIMER1:
         return Chip_TIMER_ReadCount( LPC_TIMER1 );
      break;      
      case TIMER2:      
         return Chip_TIMER_ReadCount( LPC_TIMER2 );   
      break;      
      case TIMER3:
         return Chip_TIMER_ReadCount( LPC_TIMER3 );
      break;
      default:
         return 0;
   }
}

uint32_t timerReadCapture(timerMap_t timer, uint8_t captureNumber ){
   switch( timer ){
      case TIMER0:
         return Chip_TIMER_ReadCapture( LPC_TIMER0, captureNumber);
      break;      
      case TIMER1:
         return Chip_TIMER_ReadCapture( LPC_TIMER1, captureNumber);
      break;      
      case TIMER2:      
         return Chip_TIMER_ReadCapture( LPC_TIMER2, captureNumber);   
      break;      
      case TIMER3:
         return Chip_TIMER_ReadCapture( LPC_TIMER3, captureNumber);
      break;
      default:
         return 0;
   }
}

// TIMER_TICKER

bool_t timerTickerConfig( timerMap_t timer, uint32_t tickRateHz ){
   bool_t retVal = TRUE;
   uint32_t timerFreq = 0;
   switch( timer ){
      case TIMER0:
      break;      
      case TIMER1:
         // Configurar TIMER1 modo comparacion para generar una interrupcion periodica a TICKRATE_HZ
         // Enable timer 1 clock and reset it
         Chip_TIMER_Init( LPC_TIMER1 );
         Chip_RGU_TriggerReset( RGU_TIMER1_RST );
         while( Chip_RGU_InReset( RGU_TIMER1_RST ) );
         // Get timer 1 peripheral clock rate
         timerFreq = Chip_Clock_GetRate( CLK_MX_TIMER1 );
         // Timer setup for match and interrupt at TICKRATE_HZ
         Chip_TIMER_Reset( LPC_TIMER1 );
         Chip_TIMER_MatchEnableInt( LPC_TIMER1, 1 );
         Chip_TIMER_SetMatch( LPC_TIMER1, 1, (timerFreq/tickRateHz) );
         
         // TODO: separar esta config
         Chip_TIMER_ResetOnMatchEnable( LPC_TIMER1, 1 );
      break;      
      case TIMER2:         
      break;      
      case TIMER3:         
      break;      
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}

void timerTickerSetTickEvent( timerMap_t timer, sAPI_FuncPtr_t eventHook ){
   switch( timer ){
      case TIMER0:
         if( eventHook ){
            timer0MatchHookFunction[0] = eventHook;
            // Enable TIMER0 interrupt
            timerInterruptEnable( TIMER0, ON );
         }
      break;
      case TIMER1:
         if( eventHook ){
            timer1MatchHookFunction[0] = eventHook;
            // Enable TIMER1 interrupt
            timerInterruptEnable( TIMER1, ON );
         } 
      break;
      case TIMER2:
         if( eventHook ){
            timer2MatchHookFunction[0] = eventHook;
            // Enable TIMER2 interrupt
            timerInterruptEnable( TIMER2, ON );
         } 
      break;
      case TIMER3:
         if( eventHook ){
            timer3MatchHookFunction[0] = eventHook;
            // Enable TIMER3 interrupt
            timerInterruptEnable( TIMER3, ON );
         } 
      break;
   }
}


// TIMER_OVERFLOW

bool_t timerConfigOverflow( timerMap_t timer ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:         
      break;      
      case TIMER1:         
      break;      
      case TIMER2:         
      break;
      case TIMER3:
      break;
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}


// TIMER_MATCH

bool_t timerConfigMatch( timerMap_t timer ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:         
      break;      
      case TIMER1:         
      break;      
      case TIMER2:         
      break;
      case TIMER3:
      break;
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}


// TIMER_MATCH_OUTPUT

bool_t timerConfigMatchOutput( timerMap_t timer ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:         
      break;      
      case TIMER1:         
      break;      
      case TIMER2:         
      break;
      case TIMER3:
      break;
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}


// TIMER_PWM

bool_t timerConfigPwm( timerMap_t timer ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:         
      break;      
      case TIMER1:         
      break;      
      case TIMER2:         
      break;
      case TIMER3:
      break;
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}


// TIMER_INPUT_CAPTURE

bool_t timerInputCaptureConfig( timerMap_t timer,
                                timerCapture_t capture,
                                uint32_t prescale,
                                bool_t captureRising,
                                bool_t captureFalling ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:
         // Configurar TIMER0 en modo entrada de captura 1 
         // Configurar pin P1.20 como T0_CAP2
         // ENET_TXD1 --> P1.20, FUNC4 --> T0_CAP2
         Chip_SCU_PinMux( 0x1,20, MD_PLN_FAST, SCU_MODE_FUNC4 ); // P1.20 whith SCU_MODE_FUNC4 is T0_CAP2
                      // T0_CAP2
         LPC_GIMA->CAP0_IN[0][capture]|= 0x20;
         // Enable timer 0 clock and reset it
         Chip_TIMER_Init( LPC_TIMER0 );
         Chip_TIMER_Reset( LPC_TIMER0 );
         Chip_TIMER_TIMER_SetCountClockSrc( LPC_TIMER0, TIMER_CAPSRC_RISING_PCLK, capture );
         Chip_TIMER_PrescaleSet( LPC_TIMER0, prescale );
         Chip_TIMER_ClearCapture( LPC_TIMER0, capture );      
         if(captureRising)
            Chip_TIMER_CaptureRisingEdgeEnable( LPC_TIMER0, capture );
         if(captureFalling)
            Chip_TIMER_CaptureFallingEdgeEnable( LPC_TIMER0, capture );
      break;
      case TIMER1:
      break;
      case TIMER2:
      break;
      case TIMER3:
      break;
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}
                                          
void timerInputCaptureSetCaptureEvent( timerMap_t timer,
                                       timerCapture_t capture,
                                       sAPI_FuncPtr_t eventHook ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:
         if( eventHook ){
            timer0CaptureHookFunction[capture] = eventHook;
            // Enable TIMER0 Capture interrupt
            Chip_TIMER_CaptureEnableInt( LPC_TIMER0, capture );
            // Enable TIMER0 interrupt
            timerInterruptEnable( TIMER0, ON );
         }
      break;
      case TIMER1:
         if( eventHook ){
            timer0CaptureHookFunction[capture] = eventHook;
            // Enable TIMER1 Capture interrupt
            Chip_TIMER_CaptureEnableInt( LPC_TIMER1, capture );
            // Enable TIMER1 interrupt
            timerInterruptEnable( TIMER1, ON );
         } 
      break;
      case TIMER2:
         if( eventHook ){
            timer0CaptureHookFunction[capture] = eventHook;
            // Enable TIMER2 Capture interrupt
            Chip_TIMER_CaptureEnableInt( LPC_TIMER2, capture );
            // Enable TIMER2 interrupt
            timerInterruptEnable( TIMER2, ON );
         } 
      break;
      case TIMER3:
         if( eventHook ){
            timer0CaptureHookFunction[capture] = eventHook;
            // Enable TIMER3 Capture interrupt
            Chip_TIMER_CaptureEnableInt( LPC_TIMER3, capture );
            // Enable TIMER3 interrupt
            timerInterruptEnable( TIMER3, ON );
         } 
      break;
      default:
         retVal = FALSE;
      break;
   }
}



void timerSetPower( timerMap_t timer, bool_t power ){
   switch( timer ){
      case TIMER0:
         if( power )
            Chip_TIMER_Enable( LPC_TIMER0 );
         else
            Chip_TIMER_Disable( LPC_TIMER0 );
      break;
      case TIMER1:
         if( power )
            Chip_TIMER_Enable( LPC_TIMER1 );
         else
            Chip_TIMER_Disable( LPC_TIMER1 );
      break;
      case TIMER2:
         if( power )
            Chip_TIMER_Enable( LPC_TIMER2 );
         else
            Chip_TIMER_Disable( LPC_TIMER2 );
      break;
      case TIMER3:
         if( power )
            Chip_TIMER_Enable( LPC_TIMER3 );
         else
            Chip_TIMER_Disable( LPC_TIMER3 );
      break;
   }
}

void timerInterruptEnable( timerMap_t timer, bool_t enabeling ){
   switch( timer ){
      case TIMER0:
         if( enabeling ){
            // Enable TIMER0 interrupt
            NVIC_EnableIRQ( TIMER0_IRQn );
            NVIC_ClearPendingIRQ( TIMER0_IRQn );
         }else{
            // Disable TIMER0 interrupt
            NVIC_DisableIRQ( TIMER0_IRQn );
         }
      break;
      case TIMER1:
         if( enabeling ){
            // Enable TIMER1 interrupt
            NVIC_EnableIRQ( TIMER1_IRQn );
            NVIC_ClearPendingIRQ( TIMER1_IRQn );
         }else{
            // Disable TIMER1 interrupt
            NVIC_DisableIRQ( TIMER1_IRQn );
         }
      break;
      case TIMER2:
         if( enabeling ){
            // Enable TIMER2 interrupt
            NVIC_EnableIRQ( TIMER2_IRQn );
            NVIC_ClearPendingIRQ( TIMER2_IRQn );
         }else{
            // Disable TIMER2 interrupt
            NVIC_DisableIRQ( TIMER2_IRQn );
         }
      break;
      case TIMER3:
         if( enabeling ){
            // Enable TIMER3 interrupt
            NVIC_EnableIRQ( TIMER3_IRQn );
            NVIC_ClearPendingIRQ( TIMER3_IRQn );
         }else{
            // Disable TIMER3 interrupt
            NVIC_DisableIRQ( TIMER3_IRQn );
         }
      break;
   }
}

// NEW TIMER --------------------------------------------------------------------------------------------


/*
 * @Brief   Initialize Timer peripheral
 * @param   timerNumber:   Timer number, 0 to 3
 * @param   ticks:   Number of ticks required to finish the cycle.
 * @param   voidFunctionPointer:   function to be executed at the end of the timer cycle
 * @return   nothing
 * @note   For the 'ticks' parameter, see function Timer_microsecondsToTicks
 */
void Timer_Init( uint8_t timerNumber, uint32_t ticks,
                 voidFunctionPointer_t voidFunctionPointer){
   /* Source:
   http://docs.lpcware.com/lpcopen/v1.03/lpc18xx__43xx_2examples_2periph_2periph__blinky_2blinky_8c_source.html */

   /*If timer period = CompareMatch0 Period = 0 => ERROR*/
   if (ticks==0){
      errorOcurred();
   }

   /* Enable timer clock and reset it */
   Chip_TIMER_Init(timer_sd[timerNumber].name);
   Chip_RGU_TriggerReset(timer_sd[timerNumber].RGU);
   while (Chip_RGU_InReset(timer_sd[timerNumber].RGU)) {}
   Chip_TIMER_Reset(timer_sd[timerNumber].name);

   /* Update the defalut function pointer name of the Compare match 0*/
   timer_dd[timerNumber].timerCompareMatchFunctionPointer[TIMERCOMPAREMATCH0] = voidFunctionPointer;

   /* Initialize compare match with the specified ticks (number of counts needed to clear the match counter) */
   Chip_TIMER_MatchEnableInt(timer_sd[timerNumber].name, TIMERCOMPAREMATCH0);
   Chip_TIMER_SetMatch(timer_sd[timerNumber].name, TIMERCOMPAREMATCH0, ticks);

   /* Makes Timer Match 0 period the timer period*/
   Chip_TIMER_ResetOnMatchEnable(timer_sd[timerNumber].name, TIMERCOMPAREMATCH0);

   /*Enable timer*/
   Chip_TIMER_Enable(timer_sd[timerNumber].name);

   /* Enable timer interrupt */
   NVIC_SetPriority(timer_sd[timerNumber].IRQn, MAX_SYSCALL_INTERRUPT_PRIORITY+1);
   NVIC_EnableIRQ(timer_sd[timerNumber].IRQn);
   NVIC_ClearPendingIRQ(timer_sd[timerNumber].IRQn);
}

/*
 * @Brief   Disables timer peripheral
 * @param   timerNumber:   Timer number, 0 to 3
 * @return   nothing
 */
void Timer_DeInit(uint8_t timerNumber){
   NVIC_DisableIRQ(timer_sd[timerNumber].IRQn);
   Chip_TIMER_Disable(timer_sd[timerNumber].name);
   Chip_TIMER_DeInit(timer_sd[timerNumber].name);
}

/*
 * @Brief   Converts a value in microseconds (uS = 1x10^-6 sec) to ticks
 * @param   uS:   Value in microseconds
 * @return   Equivalent in Ticks for the LPC4337
 * @note   Can be used for the second parameter in the Timer_init
 */
uint32_t Timer_microsecondsToTicks(uint32_t uS){
   return (uS*(LPC4337_MAX_FREC/1000000));
}

/*
 * @Brief   Enables a compare match in a timer
 * @param   timerNumber:   Timer number, 0 to 3
 * @param   compareMatchNumber:   Compare match number, 1 to 3
 * @param   ticks:   Number of ticks required to reach the compare match.
 * @param   voidFunctionPointer: function to be executed when the compare match is reached
 * @return   None
 * @note   For the 'ticks' parameter, see function Timer_microsecondsToTicks
 */
void Timer_EnableCompareMatch( uint8_t timerNumber, uint8_t compareMatchNumber,
                               uint32_t ticks,
                               voidFunctionPointer_t voidFunctionPointer){

   timer_dd[timerNumber].timerCompareMatchFunctionPointer[compareMatchNumber] = voidFunctionPointer;

   Chip_TIMER_MatchEnableInt(timer_sd[timerNumber].name, compareMatchNumber);
   Chip_TIMER_SetMatch(timer_sd[timerNumber].name, compareMatchNumber, ticks);
}

/*
 * @brief   Disables a compare match of a timer
 * @param   timerNumber:   Timer number, 0 to 3
 * @param   compareMatchNumber:   Compare match number, 1 to 3
 * @return   None
 */
void Timer_DisableCompareMatch( uint8_t timerNumber,
                                uint8_t compareMatchNumber ){

   timer_dd[timerNumber].timerCompareMatchFunctionPointer[compareMatchNumber] = errorOcurred;

   Chip_TIMER_ClearMatch(timer_sd[timerNumber].name, compareMatchNumber);
   Chip_TIMER_MatchDisableInt(timer_sd[timerNumber].name, compareMatchNumber);
}

/*
 * @Purpose:   Allows the user to change the compare value nº
 *    'compareMatchNumber' of timer 'timerNumber'.  This is specially useful to
 *    generate square waves if used in the function for the TIMERCOMPAREMATCH0
 *    (because that compare match resets the timer counter), which will be
 *    passed as a parameter when initializing a timer
 * @note:  The selected time (3rd parameter) must be less than
 *    TIMERCOMPAREMATCH0's compareMatchTime_uS for the compare match to make the
 *    interruption
 */
void Timer_SetCompareMatch( uint8_t timerNumber,
                            uint8_t compareMatchNumber,
                            uint32_t ticks){
   Chip_TIMER_SetMatch(timer_sd[timerNumber].name, compareMatchNumber,ticks);
}

/*==================[ISR external functions definition]======================*/
/*
 * @Brief:   Executes the functions passed by parameter in the Timer_init,
 *   at the chosen frequencies
 */


// NEW TIMER --------------------------------------------------------------------------------------------


void TIMER0_IRQHandler(void){
   
   uint8_t i = 0;
/*
   // Input Capture
   for( i=0; i<4; i++){
      if( Chip_TIMER_CapturePending( LPC_TIMER0, i ) ){
         // Execute Timer1 Match Hook function
         (* timer0CaptureHookFunction[i] )( 0 );
         // Clear flag
         Chip_TIMER_ClearCapture( LPC_TIMER0, i );
      }
   }
*/
   // Input Capture
   if( Chip_TIMER_CapturePending( LPC_TIMER0, 2 ) ){  
      // Execute Timer1 Match Hook function
      (* timer0CaptureHookFunction[2] )( 0 );
      // Clear flag
      Chip_TIMER_ClearCapture( LPC_TIMER0, 2 );   
   }  

   // Match
   for( i=0; i<4; i++){
      if( Chip_TIMER_MatchPending(LPC_TIMER0, i) ){
         //Run the functions saved in the timer dynamic data structure
         (*timer_dd[TIMER0].timerCompareMatchFunctionPointer[i])();
         // Clear flag
         Chip_TIMER_ClearMatch(LPC_TIMER0, i);
      }
   }

}


void TIMER1_IRQHandler( void ){
   
   uint8_t i = 0;
   
   // Match   
   for( i=0; i<4; i++){
      if( Chip_TIMER_MatchPending( LPC_TIMER1, i ) ){
         // Execute Timer1 Match Hook function
         (* timer1MatchHookFunction[0] )( 0 );
         // Run the functions saved in the timer dynamic data structure
//         (*timer_dd[TIMER1].timerCompareMatchFunctionPointer[i])();
         Chip_TIMER_ClearMatch( LPC_TIMER1, i );
      } 
   }
   
}


// NEW TIMER --------------------------------------------------------------------------------------------

void TIMER2_IRQHandler( void ){
   
   // Match
   
   uint8_t compareMatchNumber = 0;

   for( compareMatchNumber = TIMERCOMPAREMATCH0;
        compareMatchNumber <= TIMERCOMPAREMATCH3;
        compareMatchNumber++ ){
      if( Chip_TIMER_MatchPending(LPC_TIMER2, compareMatchNumber) ){
         /*Run the functions saved in the timer dynamic data structure*/
         (*timer_dd[TIMER2].timerCompareMatchFunctionPointer[compareMatchNumber])();
         Chip_TIMER_ClearMatch(LPC_TIMER2, compareMatchNumber);
      }
   }
}



//fixme __attribute__ ((section(".after_vectors")))
void TIMER3_IRQHandler( void ){
   
   // Match

   uint8_t compareMatchNumber = 0;

   for( compareMatchNumber = TIMERCOMPAREMATCH0;
        compareMatchNumber <= TIMERCOMPAREMATCH3;
        compareMatchNumber++ ){
      if (Chip_TIMER_MatchPending(LPC_TIMER3, compareMatchNumber)){
         /*Run the functions saved in the timer dynamic data structure*/
         (*timer_dd[TIMER3].timerCompareMatchFunctionPointer[compareMatchNumber])();
         Chip_TIMER_ClearMatch(LPC_TIMER3, compareMatchNumber);
      }
   }
}


// NEW TIMER --------------------------------------------------------------------------------------------

/*==================[end of file]============================================*/
