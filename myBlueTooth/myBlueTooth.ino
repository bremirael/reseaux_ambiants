#include <LBT.h>
#include <LBTServer.h>
#include <LGPS.h>

gpsSentenceInfoStruct info;
char buff[256];


void setup() 
{
  Serial.begin(9600);
  LGPS.powerOn();
  //Serial.println("LGPS Power on, and waiting ..."); 
  
  if(!LBTServer.begin((uint8_t*)"Bluetooth_Gael_B_2"))
  {
    Serial.println("Fail to start BT.");
    return;
  }
  Serial.println("Le serveur Bluetooth est démarré.");
}

void loop() 
{
  uint8_t buf[64];
  //char * buff = ''; 
  int bytesRead;

  if(LBTServer.connected()) 
  {
    // echo back all received data
    LGPS.getData(&info);
    Serial.println((char*)info.GPGGA);

    while(true)
    {
      bytesRead = LBTServer.readBytes(buf, 64);
      if(!bytesRead)
        break;

      Serial.write(buf, bytesRead);
      LBTServer.write(buff, bytesRead);
      //Serial.write(buff, (char*)info.GPGGA);
      LBTServer.write(, (char*)info.GPGGA);
    }
   delay(100);
  }
  else
  {
    LBTServer.accept(5);
    Serial.println("Ne lit pas dans le Buffer");
  }
}
