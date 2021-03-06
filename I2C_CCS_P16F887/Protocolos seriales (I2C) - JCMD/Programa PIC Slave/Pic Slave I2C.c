////////////////////////////////////////////////////////////////////////////////
//                    COMUNICACIÓN I2C EN TRE DOS PIC                         //
//                                                                            //
//                      Aplicación para PIC ESCLAVO                           //
//                                                                            //
//          El PIC esclavo lee tres botones y estos activan tres led          //
//                   respectivamente provenientes del maestro                 //
//             y envía el dato de tres botones al PIC maestro via I2C         //
//                  que lo mostrará en tres leds respectivamente.             //
//                                                                            //
//                          Desarrollo de JCesarMD                            //
////////////////////////////////////////////////////////////////////////////////

#include <16F887.h>

#device ADC=10     // Preparación de modulo analogico para transmisión de datos
#FUSES HS
#FUSES NOWDT       // Sin temporizador Watch Dog
#FUSES BROWNOUT    // Sin brownout reset
#FUSES NOLVP       // Sin programación de bajo voltaje, B3(PIC16) or ...
                   // ...B5(PIC18) used for I/O
#FUSES PUT


#use delay(clock=4000000)
#use standard_io(c)
#use i2c(SLAVE, SLOW, SDA=PIN_C4, SCL=PIN_C3, force_hw, address=0x46,)
#use rs232(uart1, baud = 9600)   // Initializa modulo UART proyecto explandible

#use FIXED_IO( B_outputs=PIN_B5,PIN_B4,PIN_B3 ) // Configuración de botones

#define Led1   PIN_B3             // Configuración de Leds
#define Led2   PIN_B4
#define Led3   PIN_B5

int8 buffer;

/*********** Interrupciones del modulo SSP de comunicaciones ******************/

#INT_SSP                 // Arquitectura de interrupción del modulo SSP
void SSP_isr()        
{
int8 incoming;
int8 state;

state = i2c_isr_state();    // Asignación del estado del bus I2C

if(state < 0x80)     // El maestro está enviando datos
  {
  
   incoming = i2c_read();   // El esclavo lee los datos recibidos

// Considerando el dato guardado en la variable incoming se activara el led ...
// ...que corresponda

         if (incoming==1){output_high(Led1);}else{output_low(Led1);}
         if (incoming==2){output_high(Led2);}else{output_low(Led2);}
         if (incoming==3){output_high(Led3);}else{output_low(Led3);}
      
  }

   if((state == 0x80))  // El maestro está solicitando datos del esclavo
  {
     i2c_write(buffer); // El esclavo envia datos al maestro por la condición...
                        // ...solicitada del maestro
  }

}
/******************************************************************************/

/*************************** FUNCIÓN PRINCIPAL ********************************/

void main() {

   enable_interrupts(INT_SSP);   // Activación de los modulos de interrupción
   enable_interrupts(GLOBAL);

  setup_oscillator(OSC_4MHZ);
  
  delay_ms(100);                 // Espera 100ms
  
  output_low(Led1);              // Limpieza de los tres led de entrada
  output_low(Led2);
  output_low(Led3);

  while (TRUE) {
 
/** Envio del datos al esclavo mediante la variable buffer a la interrupción **/

    if(input_state(PIN_B0) == 1){buffer=1;} // Asignación de dato para envió...
    if(input_state(PIN_B1) == 1){buffer=2;} // ...al maestro con un botón
    if(input_state(PIN_B2) == 1){buffer=3;}
      
/******************************************************************************/      
   }
}

/******************************************************************************/
