/*
=================================================================================
 Name        : pcd8544_rpi.c
 Version     : 0.1

 Copyright (C) 2012 by Andre Wussow, 2012, desk@binerry.de

 Description :
     A simple PCD8544 LCD (Nokia3310/5110) for Raspberry Pi for displaying some system informations.
	 Makes use of WiringPI-library of Gordon Henderson (https://projects.drogon.net/raspberry-pi/wiringpi/)

	 Recommended connection (http://www.raspberrypi.org/archives/384):
	 LCD pins      Raspberry Pi
	 LCD1 - GND    P06  - GND
	 LCD2 - VCC    P01 - 3.3V
	 LCD3 - CLK    P11 - GPIO0
	 LCD4 - Din    P12 - GPIO1
	 LCD5 - D/C    P13 - GPIO2
	 LCD6 - CS     P15 - GPIO3
	 LCD7 - RST    P16 - GPIO4
	 LCD8 - LED    P01 - 3.3V 

================================================================================
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
================================================================================
 */
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "PCD8544.h"

#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>


#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



// pin setup
int _din = 1;
int _sclk = 0;
int _dc = 2;
int _rst = 4;
int _cs = 3;
  
// lcd contrast 
//may be need modify to fit your screen!  normal: 30- 90 ,default is:45 !!!maybe modify this value!
int contrast = 60;  
  
int main (void)
{
int listenfd,connfd,n;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t clilen;
   pid_t     childpid;
   char mesg[1000];
   char *hashInfo; 

// this is where i try to get the mhash from the cgminer

    int sockfd, portno;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    portno = 4028;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
/*
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
*/
    sprintf(buffer,"summary|0");
   
 n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
//    printf("%s\n",buffer);

n=0;

char *pt;
pt = strtok(buffer,",");
while (pt != NULL) {
//    printf("%s\n",pt);

    if ( (pt[0]=='M') && (pt[5]=='v') )
    {printf("%s\n",pt);
    hashInfo=pt;}

//printf(pt);
pt = strtok (NULL, ",");
}

// no more cgminer




   listenfd=socket(AF_INET,SOCK_STREAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(32000);
   bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   listen(listenfd,1024);

// get ip address stuff
/* int fd;
struct ifreq ifr;
fd=socket(AF_INET,SOCK_DGRAM,0);

ifr.ifr_addr.sa_family = AF_INET;

strncpy(ifr.ifr_name,"eth0",IFNAMSIZE-1);
ioctl(fd,SIOCGIFADR,&ifr);
close(fd);

*/


struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
            char ipInfo[16]; 

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }


    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL)
            continue;  

        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if((strcmp(ifa->ifa_name,"eth0")==0)&&(ifa->ifa_addr->sa_family==AF_INET))
        {
            if (s != 0)
            {
                //printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            //printf("\tInterface : <%s>\n",ifa->ifa_name );
            //printf("\t  Address : <%s>\n", host); 
            //printf("\t Port 32000\n");
            sprintf(ipInfo, "%s", host);

        }
    }

    freeifaddrs(ifaddr);







  // print infos
  //printf("Raspberry Pi PCD8544 sysinfo display\n");
  //printf("========================================\n");
  
  // check wiringPi setup
  if (wiringPiSetup() == -1)
  {
//	printf("wiringPi-Error\n");
    exit(1);
  }
  
  // init and clear lcd
  LCDInit(_sclk, _din, _dc, _cs, _rst, contrast);
  LCDclear();
  
  // show logo
  LCDshowLogo();
  
  delay(2000);
  // chad
  //for (;;)
 // {
	  // clear lcd
	  LCDclear();
	  
	  // get system usage / info
	  struct sysinfo sys_info;
	  if(sysinfo(&sys_info) != 0)
	  {
		printf("sysinfo-Error\n");
	  }
	  
	  // uptime
	  char uptimeInfo[15];
	  unsigned long uptime = sys_info.uptime / 60;
	  sprintf(uptimeInfo, "Uptime %ld min.", uptime);
	  
	  // cpu info
	  char cpuInfo[10]; 
	  unsigned long avgCpuLoad = sys_info.loads[0] / 1000;
	  sprintf(cpuInfo, "CPU %ld%%", avgCpuLoad);
	  
	  // ram info
	  char ramInfo[10]; 
	  unsigned long totalRam = sys_info.freeram / 1024 / 1024;
	  sprintf(ramInfo, "RAM %ld MB", totalRam);
	  
// adding stuff here....
//	  LCDdrawstring(0, 0, "CHADS Rpi");
//	  LCDdrawline(0, 10, 83, 10, BLACK);
//	  LCDdrawstring(0, 12, ipInfo);
//	  LCDdrawstring(0, 12, uptimeInfo);
//	  LCDdrawstring(0, 20, cpuInfo);
//	  LCDdrawstring(0, 28, hashInfo);
//	  LCDdrawstring(0, 28, ramInfo);
//	  LCDdisplay();

          
clilen=sizeof(cliaddr);
      connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);

      if ((childpid = fork()) == 0)
      {
         close (listenfd);
//ohshit
         for(;;)
         {
	  LCDdrawstring(0, 0, "CHADS Rpi");
	  LCDdrawline(0, 10, 83, 10, BLACK);
	  LCDdrawstring(0, 12, ipInfo);
//	  LCDdrawstring(0, 12, uptimeInfo);
	  LCDdrawstring(0, 20, cpuInfo);
	  LCDdrawstring(0, 28, hashInfo);
          n = recvfrom(connfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&clilen);
          LCDdrawstring(0,36,mesg);
	  LCDdisplay();
	  delay(1000);

          //  sendto(connfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
          //  printf("-------------------------------------------------------\n");
         //   mesg[n] = 0;
          //  printf("Received the following:\n");
           // printf("%s",mesg);
          //  printf("-------------------------------------------------------\n");
         }
         
      }
      close(connfd);
   



// thats it.

	  // build screen
	  //LCDdrawstring(0, 0, "Raspberry Pi:");
	  
	  delay(1000);
//  }
  
    //for (;;){
  //  printf("LED On\n");
  //  digitalWrite(pin, 1);
  //  delay(250);
  //  printf("LED Off\n");
  //  digitalWrite(pin, 0);
  //  delay(250);
  //}

  return 0;
}
