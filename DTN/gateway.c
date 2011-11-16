#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <time.h>

 int main()
 {
 int fd;
 struct ifreq ifr;
 struct sockaddr_in server_addr;
 char buf[1024]; //string holding commands for the kernel call
 int sock,listen_sock;
 
 char send_data[1024];
 int value;
 int i = 0;
 struct sockaddr_in server1_addr , client_addr;
 int sock1;
 int addr_len, bytes_read;
 char recv_data[1024],bufip[1024];

 fd = socket(AF_INET, SOCK_DGRAM, 0);
 ifr.ifr_addr.sa_family = AF_INET;
 strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
 /*Ioctl command to know the interfaces IP address*/
 ioctl(fd, SIOCGIFADDR, &ifr);
 close(fd);
 printf("Gateway wlan1 IP: %s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
 sprintf(buf, "route add -net 192.168.1.0 netmask 255.255.255.0 gw %s dev wlan1",inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
 system(buf); //added default route path with gateway IP 
 printf("route added\n");
 strcpy(bufip,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
 /*Socket "sock" created to send BROADCAST message*/

 if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
 {
 perror("socket");
 exit(1);
 }

 if(setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char *)&value,sizeof(uint32_t)) == -1)
 return -1;

 server_addr.sin_family = AF_INET;
 server_addr.sin_port = htons(8000);
 server_addr.sin_addr.s_addr =INADDR_BROADCAST ;
 bzero(&(server_addr.sin_zero),8);

listen_sock=sock;
/*
 Socket "sock1" created to receive ACK message

 if ((sock1 = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
 {
  perror("Socket");
  exit(1);
 }

 server1_addr.sin_family = AF_INET;
 server1_addr.sin_port = htons(7001);
 server1_addr.sin_addr.s_addr = INADDR_ANY;
 bzero(&(server1_addr.sin_zero),8);
*/


 if (bind(listen_sock,(struct sockaddr *)&server1_addr,sizeof(struct sockaddr)) == -1)
 {
  perror("Bind");
  exit(1);
 }

 addr_len = sizeof(struct sockaddr);

 while (1) //Keep broadcasting in a loop after a certain time interval to determine the new immedate neighbour nodes
 {
    printf("Gateway Broadcast message");
    sprintf(send_data,"%s","Gateway:Hello");

    sprintf(buf, "route add default gw %s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    system(buf); //default gateway IP for this node on the wireless interface is right now the gateway IP itself : used for broadcast in the mesh network
//This broadcast has to be be only acked by immediate 1 hop neighbours and not to be forwarded into mesh network... 

    sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));//Broadcast message of the gateway

    sprintf(buf, "route del default gw %s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    system(buf); //removed default gateway IP and waiting for neighbour response
    printf("waiting to receive");
    bytes_read = recvfrom(sock1,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);//reception of ACk from a node

    printf("received");
    recv_data[bytes_read] = '\0';
    printf("\n(%s , %d) said : ",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    printf("%s\n", recv_data);

    if(atoi(recv_data) == 1)    //checking the ACK for a '1' reception : Immediate 1 hop guyz would say 1 as send_data
       {
          printf("ACK RECEIVED \t NEW NEIGHBOUR FOUND! %s", inet_ntoa(client_addr.sin_addr));
          sprintf(buf, "route del -net 192.168.1.0 netmask 255.255.255.0 gw %s dev wlan1",bufip);
          system(buf); //deleted the default route path 
          sprintf(buf, "route add -net 192.168.1.0 netmask 255.255.255.0 gw %s dev wlan1", inet_ntoa(client_addr.sin_addr));//adding the next hop for a return route
          system(buf); //adding the new recd IP information
          strcpy(bufip,inet_ntoa(client_addr.sin_addr));//copying the next hop address to 'bufip'.This is used during refresh 

       }

    sleep(10);
    printf("Once again Broadcast\n");

    //sprintf(buf, "route del -net 192.168.3.0 netmask 255.255.255.0 gw %s dev wlan1", inet_ntoa(client_addr.sin_addr));
    //system(buf);

  }

}

