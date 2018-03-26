#include <LGPS.h>
#include <LBT.h>
#include <LBTServer.h>

gpsSentenceInfoStruct info;
char buff[256];

static unsigned char getComma(unsigned char num,const char *str)
{
  unsigned char i,j = 0;
  int len=strlen(str);
  for(i = 0;i < len;i ++)
  {
     if(str[i] == ',')
      j++;
     if(j == num)
      return i + 1; 
  }
  return 0; 
}

static double getDoubleNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atof(buf);
  return rev; 
}

static double getIntNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atoi(buf);
  return rev; 
}

String parseGPGGA(const char* GPGGAstr)
{
  /* Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
   * Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
   * Where:
   *  GGA          Global Positioning System Fix Data
   *  123519       Fix taken at 12:35:19 UTC
   *  4807.038,N   Latitude 48 deg 07.038' N
   *  01131.000,E  Longitude 11 deg 31.000' E
   *  1            Fix quality: 0 = invalid
   *                            1 = GPS fix (SPS)
   *                            2 = DGPS fix
   *                            3 = PPS fix
   *                            4 = Real Time Kinematic
   *                            5 = Float RTK
   *                            6 = estimated (dead reckoning) (2.3 feature)
   *                            7 = Manual input mode
   *                            8 = Simulation mode
   *  08           Number of satellites being tracked
   *  0.9          Horizontal dilution of position
   *  545.4,M      Altitude, Meters, above mean sea level
   *  46.9,M       Height of geoid (mean sea level) above WGS84
   *                   ellipsoid
   *  (empty field) time in seconds since last DGPS update
   *  (empty field) DGPS station ID number
   *  *47          the checksum data, always begins with *
   */
  double latitude;
  double longitude;
  int tmp, hour, minute, second, num ;
  if(GPGGAstr[0] == '$')
  { 
    tmp = getComma(2, GPGGAstr);
    latitude = getDoubleNumber(&GPGGAstr[tmp])/100;
    tmp = getComma(4, GPGGAstr);
    longitude = getDoubleNumber(&GPGGAstr[tmp])/100;
    sprintf(buff, "%10.8f,%10.8f", latitude, longitude);
    return buff;  
  }
  else
  {
    Serial.println("Not get data"); 
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  LGPS.powerOn();
  //Serial.println("LGPS Power on, and waiting ..."); 
  delay(3000);
  
  if(!LBTServer.begin((uint8_t*)"BT_gael_b"))
  {
    Serial.println("Fail to start BT.");
    return;
  }
  Serial.println("BT server is started.\nPour avoir les coordonnées GPS de la carte Linklt, tapez 'GPS'");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("LGPS loop"); 
  LGPS.getData(&info);
  //Serial.println((char*)info.GPGGA); 
  String coord = parseGPGGA((const char*)info.GPGGA);
  String delimiter = ",";
  String Latitude = coord.substring(0,coord.indexOf(delimiter));
  String Longitude = coord.substring(coord.indexOf(delimiter)+1);
  delay(2000);

  uint8_t buf[64];
  String bytesRead;
  if(LBTServer.connected())
  {
  LBTServer.write("Pour avoir les coordonnées GPS de la carte Linklt, tapez 'GPS'");
  while(true)
   {
    bytesRead = LBTServer.readString();
    if(bytesRead == "GPS"){
    Serial.print("\nLatitude : "+Latitude+"\n"+"Longitude : "+Longitude);
    LBTServer.print("\nLatitude : "+Latitude+"\n"+"Longitude : "+Longitude);
    } else if (bytesRead == ""){
      
    } else {
      Serial.write("Commande inconnue");
      LBTServer.write("Commande inconnue");
      delay(1000); 
    }
    
   }
   delay(100);
  }
else
{
 LBTServer.accept(5);
 Serial.println("Aucune personne connectée");
}
}
