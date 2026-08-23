#include <Arduino.h>
#include "config.h"
void audioBegin(){pinMode(BUZZER_PIN,OUTPUT);digitalWrite(BUZZER_PIN,LOW);}
void audioBeep(uint16_t freq,uint16_t ms){tone(BUZZER_PIN,freq,ms);}
