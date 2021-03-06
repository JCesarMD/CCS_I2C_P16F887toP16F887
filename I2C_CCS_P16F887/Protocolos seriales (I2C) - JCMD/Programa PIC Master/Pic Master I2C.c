////////////////////////////////////////////////////////////////////////////////
//                    COMUNICACI?N I2C ENTRE DOS PIC                          //
//                                                                            //
//                      Aplicaci?n para PIC MAESTRO                           //
//                                                                            //
//          El PIC maestro lee tres botones y estos activan tres led          //
//                   respectivamente provenientes del esclavo                 //
//             y env?a el dato de tres botones al PIC esclavo via I2C         //
//                  que lo mostrar? en tres leds respectivamente.             //
//                                                                            //
//                          Desarrollo de JCesarMD                            //
////////////////////////////////////////////////////////////////////////////////

#include <16F887.h>

#fuses XT,PUT,NOPROTECT,BROWNOUT,NOLVP,NOWDT
#use delay (clock=4000000)                      

#use i2c(MASTER, SDA=PIN_C4, SLOW, SCL=PIN_C3, NOFORCE_SW)

#use standard_io(b)
#use standard_io(c)
#use FIXED_IO( B_outputs=PIN_B5,PIN_B4,PIN_B3 ) // Configuraci?n de botones

#define Led1   PIN_B3            // Configuraci?n de Leds
#define Led2   PIN_B4
#define Led3   PIN_B5

int tecla;                       // Dato a transmitir
int8 incoming;                   // Dato de recepci?n
   
/******************************************************************************/
/************************* Funci?n de env?o I2C *******************************/
    
void envio_I2C (){
   
      i2c_start();         // Comienzo de la comunicaci?n I2C ...
      i2c_write(0x46);     // ...con la direcci?n del PIC esclavo...
      i2c_write(tecla);    // Envia dato
      i2c_stop();          // Finalizaci?n de la transmisi?n
  }

/******************************************************************************/
/*************************** FUNCI?N PRINCIPAL ********************************/

void main(){
     
   while (true){
  
/******* Envio del dato al esclavo mediante la funci?n envio_I2C() ************/

      if(input_state(PIN_B0) == 1){tecla=1;envio_I2C();}
      if(input_state(PIN_B1) == 1){tecla=2;envio_I2C();}
      if(input_state(PIN_B2) == 1){tecla=3;envio_I2C();}
      
/******************************************************************************/
      i2c_start();               // Inicia modulo I2C
      i2c_write(0x47);           // Direcci?n de esclavo con instrucci?n de ... 
                                 // ...lectura esclavo + 1
   
/***** Lectura de datos proveniente del esclavo con la funci?n i2c_read(0) ****/

      incoming = i2c_read(0);         // Obtiene el estado base I2C
      i2c_stop();                     // Detiene modulo I2C
       
/*************** Despliegue de datos de llegada en Leds ***********************/

       if (incoming==1){output_high(Led1);}else{output_low(Led1);}
       if (incoming==2){output_high(Led2);}else{output_low(Led2);}
       if (incoming==3){output_high(Led3);}else{output_low(Led3);}
       delay_ms(100);
       
/******************************************************************************/
   
   }
}
      


       
   


   
      
   
   

