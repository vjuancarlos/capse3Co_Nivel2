/*=============================================================================
Ejercicio Entregable 10
Autores:
        Gessi Paniagua    gessi.paniagua@gmail.com
        Guillermo López   lopezg.ing@gmail.com
        Amid Ale          amidale@gmail.com
Implementación de MEF en un ascensor
MODULO driverio Manejo de entradas y salidas
 =============================================================================*/

#include "driveruart.h"

void entrarModoMenu(void)
    {
      nuevodato = FALSE;
      uartConfig(UART_USB,115200); 
      mostrarMenu();
      leerEntradaUsuario();
    } 	

void mostrarMenu(void)
    {
      uint8_t menu[] = "Bienvenido al modo Configuración\r\nIngrese su opción:\r\n  1_ CONFIGURAR VELOCIDAD ENTRE PISO\r\n  2_ CONFIGURAR VELOCIDAD DE APERTURA/CIERRE DE PUERTA\r\n  3_ CONFIGURAR CANTIDAD DE PISOS (1 - 20)\r\n  4_ CONFIGURAR CANTIDAD DE SUBSUELOS (0 - 5)\r\n  5_ SALIR\r\n\r\n\r\n";
    
      uartWriteString(UART_USB,menu);
     }
    
void leerEntradaUsuario()
    {
     out = FALSE;
     while(!out){
     nuevodato = uartReadByte(UART_USB, &dato);
     if (nuevodato)
       {
        
        switch (dato)
              {
                case '1':
         	        nuevodato = FALSE;
         	        uartWriteString(UART_USB,"Ingrese velocidad entre piso:");
         	        while (!nuevodato)
         	             {
         	              nuevodato = uartReadByte(UART_USB, &dato);
       	             }
       	        configuraciones[0] = dato; 
         	      break; 
             
                case '2': 
                  uartWriteString(UART_USB,"Ingrese velocidad de apertura/cierre de puertas :");
                  while (!nuevodato)
         	             {
         	              nuevodato = uartReadByte(UART_USB, &dato);
       	             }
       	         configuraciones[1] = dato; 
 	      break;
             
                case '3':
                  uartWriteString(UART_USB,"Ingrese Cantidad de pisos: (1 - 20) :"); 
	        while (!nuevodato)
         	             {
         	              nuevodato = uartReadByte(UART_USB, &dato);
       	             }
       	        configuraciones[2] = dato; 
 	      
	      break;
           
                case '4':   
                  uartWriteString(UART_USB,"Ingrese Cantidad de subsuelos: (0 - 5) :");	     	
	        while (!nuevodato)
         	             {
         	              nuevodato = uartReadByte(UART_USB, &dato);
       	             }
       	         configuraciones[3] = dato; 
 	       
	      break;
           
                case '5':
                  uartWriteString(UART_USB,"Configurado Hasta Luego!!!!\r\n");
                  out = TRUE;
     	      break;
     	  
     	      default :
     	        uartWriteString(UART_USB,"Opción invalida!!!");
     	        leerEntradaUsuario();
     	         
     	      
            }	
        }
        }
    }