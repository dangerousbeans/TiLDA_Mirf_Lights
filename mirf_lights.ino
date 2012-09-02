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

void setup(){
//  Serial.begin(57600);
  
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
   
  Mirf.setRADDR((byte *)"face");
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
   
  Mirf.payload = sizeof(unsigned long);
  
  /*
   * Write channel and payload config then power up reciver.
   */
   
  Mirf.config();

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
   
  if(!Mirf.isSending() && Mirf.dataReady()){
    lights.set(PIN_LED_BOTH, 0, 0, Mirf.payload);
  }
}
