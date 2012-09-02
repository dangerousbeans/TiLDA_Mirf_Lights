/**
 * An Mirf example which flashes the LEDs when activity is seen
 *
 */
#include "lights.h"
#include "pins.h"

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define IRpin_PIN PIND
#define IRpin 2

Lights lights;


void button_press() {
  // Interrupt handler when button state changes
  
  Serial.println("Button!");
  
  unsigned long time = millis();
  
  transmit("test");
  
  while(Mirf.isSending()){
    Serial.print(".");
  }
  
  
}

// sends a string via the nRF24L01
void transmit(const char *string)
{
  byte c; 
  
  for( int i=0 ; string[i]!=0x00 ; i++ )
  { 
    c = string[i];
    Mirf.send(&c);
    while( Mirf.isSending() ) ;
  }
}


void setup(){
  Serial.begin(57600);
  
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  attachInterrupt(INT_BUTTON, button_press, CHANGE);
  
  /*
   * Set the SPI Driver.
   */

  Mirf.spi = &MirfHardwareSpi;
  
  /*
   * Setup pins / SPI.
   */
   
  Mirf.init();
  
  /*
   * Configure reciving address.
   */
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
   
  Mirf.channel = 90;
  Mirf.payload = 1;
  
  
  
  /*
   * Write channel and payload config then power up reciver.
   */
  Mirf.config();

  Mirf.setRADDR((byte *)"face");
  Mirf.setTADDR((byte *)"face");


  lights.set(PIN_LED_BOTH, 0, 0, 0);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  
  Serial.println("Listening..."); 
}

void loop(){
   
  lights.set(PIN_LED_BOTH, 0, 0, 0);
   
  /*
   * If a packet has been recived.
   *
   * isSending also restores listening mode when it 
   * transitions from true to false.
   */
   byte data[Mirf.payload];
   
  if(!Mirf.isSending() && Mirf.dataReady()){
     
    Mirf.getData(data);
    
    lights.set(PIN_LED_BOTH, 0, 0, sizeof(data));
    
    if(data[0] != 255 )
    {
        Serial.print(char(data[0]));
    }
  }
}
