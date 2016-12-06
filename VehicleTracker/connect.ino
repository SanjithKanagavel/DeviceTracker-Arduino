void AP_connect(){
  printData("Connecting to AP...");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    printData(".");
    delay(500);
  }
  printlnData("Success!");
  
  printData("Connecting site...");

  while (!c2.connect(SITE_URL, 80))
  {
    printData(".");
    delay(500);
  }
  printlnData("Success!");
  delay(100);
}


void getconnectInfo(){
  //calling RESTful API to get TCP socket connection
  c2.print("GET /mcs/v2/devices/");
  c2.print(DEVICEID);
  c2.println("/connections.csv HTTP/1.1");
  c2.print("Host: ");
  c2.println(SITE_URL);
  c2.print("deviceKey: ");
  c2.println(DEVICEKEY);
  c2.println("Connection: close");
  c2.println();
  
  delay(500);

  int errorcount = 0;
  printData("waiting for HTTP response...");
  while (!c2.available())
  {
    printData(".");
    errorcount += 1;
    delay(150);
  }
  printlnData("");
  int err = http.skipResponseHeaders();

  int bodyLen = http.contentLength();
  char c;
  int ipcount = 0;
  int count = 0;
  int separater = 0;
  while (c2)
  {
    int v = (int)c2.read();
    if (v != -1)
    {
      c = v;
      //Serial.print(c);
      connection_info[ipcount]=c;
      if(c==',')
      separater=ipcount;
      ipcount++;    
    }
    else
    {
      printlnData("no more content, disconnect");
      c2.stop();

    }
    
  }

  //connection_info[ipcount]=NULL;
  int i;
  for(i=0;i<separater;i++)
  {  ip[i]=connection_info[i];
  }
  int j=0;
  separater++;
  
  for(i=separater;i<21 && j<5 && i < ipcount;i++)
  {  port[j]=connection_info[i];
     j++;
  }
  //port[j] = NULL;
  
  portnum = atoi (port);

} //getconnectInfo

void connectTCP(){
  //establish TCP connection with TCP Server with designate IP and Port
  c.stop();
  printData("Connecting to TCP...");
  while (0 == c.connect(ip, portnum))
  {
    printlnData("Re-Connecting to TCP");    
    delay(1000);
  }
  c.println(tcpdata);
  c.println();
  printlnData("Success!");
} //connectTCP


void heartBeat(){
  printlnData("send TCP heartBeat");
  c.println(tcpdata);
  c.println();
    
} //heartBeat
