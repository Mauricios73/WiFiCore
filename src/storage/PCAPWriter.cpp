#include <Arduino.h>
#include <SD.h>
#include "config.h"
#include "events.h"

static void put32(File& f,uint32_t v){f.write((uint8_t*)&v,4);}
static void put16(File& f,uint16_t v){f.write((uint8_t*)&v,2);}
void pcapWriteHeader(){
  if(SD.exists(PCAP_FILE))return;
  File f=SD.open(PCAP_FILE,FILE_WRITE); if(!f)return;
  put32(f,0xa1b2c3d4); put16(f,2); put16(f,4);
  put32(f,0); put32(f,0); put32(f,FRAME_MAX_LEN); put32(f,105); f.close();
}
void pcapTask(void*){
  RawFrame r;
  for(;;){
    if(xQueueReceive(pcapQueue,&r,pdMS_TO_TICKS(100))==pdTRUE){
      File f=SD.open(PCAP_FILE,FILE_APPEND); if(!f)continue;
      uint32_t now=millis(), sec=now/1000, usec=(now%1000)*1000, len=r.len;
      put32(f,sec);put32(f,usec);put32(f,len);put32(f,len);f.write(r.data,r.len);f.close();
    }
  }
}
