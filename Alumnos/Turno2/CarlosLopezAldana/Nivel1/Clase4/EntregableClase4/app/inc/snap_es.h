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
 * =============================================================================
 *
 * Copyright (c) 2017, Eric Pernia.
 * Todos los derechos reservados.
 * 
 * La redistribución y el uso en las formas de código fuente y binario, con o 
 * sin modificaciones, están permitidos siempre que se cumplan las siguientes 
 * condiciones:
 * 
 * 1. Las redistribuciones del código fuente deben conservar el aviso de
 *    copyright anterior, esta lista de condiciones y el siguiente descargo de
 *    responsabilidad.
 * 
 * 2. Las redistribuciones en formato binario deben reproducir el aviso de
 *    copyright anterior, esta lista de condiciones y el siguiente descargo de
 *    responsabilidad en la documentación y/u otros materiales suministrados 
 *    con la distribución.
 * 
 * 3. Ni el nombre de los titulares de derechos de autor ni los nombres de sus
 *    colaboradores pueden usarse para apoyar o promocionar productos 
 *    derivados de este software sin permiso específico previo y por escrito.
 * 
 * ESTE SOFTWARE SE SUMINISTRA POR LOS PROPIETARIOS DEL COPYRIGHT Y
 * COLABORADORES ''COMO ESTÁ'' Y CUALQUIER GARANTÍAS EXPRESA O IMPLÍCITA,
 * INCLUYENDO, PERO NO LIMITADO A, LAS GARANTÍAS IMPLÍCITAS DE COMERCIALIZACIÓN
 * Y APTITUD PARA UN PROPÓSITO PARTICULAR SON RECHAZADAS. EN NINGÚN CASO LOS
 * PROPIETARIOS DEL COPYRIGHT Y COLABORADORES SERÁN RESPONSABLES POR NINGÚN
 * DAÑO DIRECTO, INDIRECTO, INCIDENTAL, ESPECIAL, EJEMPLAR O COSECUENCIAL
 * (INCLUYENDO, PERO NO LIMITADO A, LA ADQUISICIÓN O SUSTITUCIÓN DE BIENES O
 * SERVICIOS; LA PÉRDIDA DE USO, DE DATOS O DE BENEFICIOS; O INTERRUPCIÓN DE LA
 * ACTIVIDAD EMPRESARIAL) O POR CUALQUIER TEORÍA DE RESPONSABILIDAD, YA SEA POR
 * CONTRATO, RESPONSABILIDAD ESTRICTA O AGRAVIO (INCLUYENDO NEGLIGENCIA O
 * CUALQUIER OTRA CAUSA) QUE SURJA DE CUALQUIER MANERA DEL USO DE ESTE SOFTWARE,
 * INCLUSO SI SE HA ADVERTIDO DE LA POSIBILIDAD DE TALES DAÑOS.
 *
 * =============================================================================
 * Fecha de creacion: 2017-03-04
 * =============================================================================
 */

#ifndef _SNAP_I18N_ES_H_
#define _SNAP_I18N_ES_H_

/*==================[inclusiones]============================================*/

#include "sapi_es.h"

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define programa()                                uint32_t myForIterator = 0;\
                                                  int main( void )

#define esperarSegs( tiempo )                     delay( (tick_t)((tiempo) * 1000.0) )
#define esperarHastaQue( condicion )              while( !(condicion) )
#define porSiempre()                              while(1)
#define repetir( veces )                          for( myForIterator=0; myForIterator<(veces); myForIterator++; )
#define repetirHastaQue( condicion )              while( !(condicion) )

#define no( arg )                                 !( arg )

#define _y_                                       &&
#define _o_                                       ||

#define crearVariable                             int

#define fijarServoEnAngulo( servo, angulo )       servoWrite( (servo), (angulo) )
#define fijarPinDigitalEn( pin, valor )           gpioWrite( (pin), (valor) )
#define fijarPinAlValor( pwm, valor )             pwmWrite( (pwm), (valor) )
#define lecturaAnalogica( adc )                   adcRead( (adc) )
#define lecturaDigital( pin )                     gpioRead( (pin) )

/*==================[tipos de datos declarados por el usuario]===============*/

/*==================[declaraciones de datos externos]========================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[c++]====================================================*/

#ifdef __cplusplus
}
#endif

/*==================[fin del archivo]========================================*/
#endif /* _SNAP_I18N_ES_H_ */
