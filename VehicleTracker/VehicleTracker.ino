#include <LGPRS.h>
#include <LGPRSClient.h>
#include <LGPRSServer.h>

#include <LGPS.h>
#include <HttpClient.h>
#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LDateTime.h>
#define WIFI_AP "sanjith"
#define WIFI_PASSWORD "kanagavel"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define per 50
#define per1 3
#define DEVICEID "D7ck5tM0"
#define DEVICEKEY "bkoDot4bE5zzSKGm"
#define SITE_URL "api.mediatek.com"
#define USE_SERIAL 0

gpsSentenceInfoStruct info;
char buff[256];
double latitude;
double longitude;

char buffer_latitude[8];
char buffer_longitude[8];
LGPRSClient c;
//LWiFiClient c;
unsigned int rtc;
unsigned int lrtc;
unsigned int rtc1;
unsigned int lrtc1;
char port[4]={0};
char connection_info[21]={0};
char ip[15]={0};             
int portnum;
int val = 0;
String tcpdata = String(DEVICEID) + "," + String(DEVICEKEY) + ",0";
String tcpcmd_led_on = String(DEVICEID) + "," + String(DEVICEKEY) + ",0,LED_CONTROL,on";
String tcpcmd_led_off = String(DEVICEID) + "," + String(DEVICEKEY) + ",0,LED_CONTROL,off";
String upload_led;

LGPRSClient c2;
//LWiFiClient c2;
HttpClient http(c2);

void setup()
{
  pinMode(13, OUTPUT);
  LTask.begin();
  LWiFi.begin();
  if(USE_SERIAL != 1) {
    Serial.begin(115200);
  }
  LGPS.powerOn();
  if(USE_SERIAL != 1) {
    while(!Serial);
  }
  
  AP_connect();
  getconnectInfo();
  connectTCP();
  if(USE_SERIAL != 1) {
    printlnData("==============================");
  }
}

void loop()
{
  String tcpcmd="";
  while (c.available())
   {
      int v = c.read();
      if (v != -1)
      {
        Serial.print((char)v);
        tcpcmd += (char)v;
        if (tcpcmd.equals(tcpcmd_led_on)){
          digitalWrite(13, HIGH);
          printData("Switch LED ON ");
          tcpcmd="";
        }else if(tcpcmd.equals(tcpcmd_led_off)){  
          digitalWrite(13, LOW);
          printData("Switch LED OFF");
          tcpcmd="";
        }
      }
   }

  LDateTime.getRtc(&rtc);
  if ((rtc - lrtc) >= per) {
    heartBeat();
    lrtc = rtc;
  }
  //Check for report datapoint status interval
  LDateTime.getRtc(&rtc1);
  if ((rtc1 - lrtc1) >= per1) {
    
    //uploadstatus();
    GPS_receive();
    if(uploadGPS()) {
      lrtc1 = rtc1;
    }
  }
  delay(3000);
}
