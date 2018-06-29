
/*
 * c�digo adaptado/alterdo de:
 * https://gist.github.com/austinmarton/1922600
 */

/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include "conv.h"

#define MY_DEST_MAC0	0xFF
#define MY_DEST_MAC1	0xFF
#define MY_DEST_MAC2	0xFF
#define MY_DEST_MAC3	0xFF
#define MY_DEST_MAC4	0xFF
#define MY_DEST_MAC5	0xFF

#define DEFAULT_IF	"lo"
#define BUF_SIZ		2048

typedef struct ip {
  int* senderIp;
  int* targetIp;
} ip ;

typedef struct porta {
    int* sourcePort;
		int* dPort;
} porta;

int payload(char *sendbuf, int tx_len, ip* ips)
{
        // esse � um exemplo de IP...
  			//version IHL
	      sendbuf[tx_len++] = 0x45;
				// TOS
        sendbuf[tx_len++] = 0;
      	// Total leght
        sendbuf[tx_len++] = 0X00;
        sendbuf[tx_len++] = 0X2B;
				// ID
        sendbuf[tx_len++] = 0xBC;
        sendbuf[tx_len++] = 0XE4;
  			// flags frag_offset
        sendbuf[tx_len++] = 0X00;
        sendbuf[tx_len++] = 0X00;
  			// time to live
        sendbuf[tx_len++] = 6;
        //protocol
        sendbuf[tx_len++] = 17;
        // chesum
        sendbuf[tx_len++] = 0x00;
        sendbuf[tx_len++] = 0x00;

	// sender IP
        sendbuf[tx_len++] = ips->senderIp[0];
        sendbuf[tx_len++] = ips->senderIp[1];
        sendbuf[tx_len++] = ips->senderIp[2];
        sendbuf[tx_len++] = ips->senderIp[3];

	// target IP
        sendbuf[tx_len++] = ips->targetIp[0];
        sendbuf[tx_len++] = ips->targetIp[1];
        sendbuf[tx_len++] = ips->targetIp[2];
        sendbuf[tx_len++] = ips->targetIp[3];

        //carga util

  return tx_len;
}

int criarUdp(char *sendbuf, int tx_len, porta* portas){
  // source port
  sendbuf[tx_len++] = portas->sourcePort[0];
  sendbuf[tx_len++] = portas->sourcePort[1];
  // destination port
  sendbuf[tx_len++] = portas->dPort[0];
  sendbuf[tx_len++] = portas->dPort[1];
  // udp length
  sendbuf[tx_len++] = 0x00;
  sendbuf[tx_len++] = 0x17;
  // udp checksum
  sendbuf[tx_len++] = 0x00;
  sendbuf[tx_len++] = 0x01;
  // dados
  sendbuf[tx_len++] = 0X41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;
  sendbuf[tx_len++] = 0x41;

  return tx_len;
}

int main(int argc, char *argv[])
{
  if(argc < 3){
    printf("Argumentos invalidos, utilize: SEU.IP PORTA IP.DESTINO PORTA DESTINO\n");
    //return -1;
  }
  char ipD[20];
  char ipT[20];
	char portaD[10];
	char portaO[10];
  strcpy(ipD, argv[1]);
  strcpy(portaO, argv[2]);
  strcpy(ipT, argv[3]);
	strcpy(portaD, argv[4]);
  ip* dados = (ip*) calloc(1, sizeof(ip));
  dados->senderIp = getIP(ipD);
  dados->targetIp = getIP(ipT);
	porta* portas = (porta*) calloc(1, sizeof(porta));
	portas->sourcePort = getPort(portaO);
	portas->dPort = getPort(portaD);


	int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;
	int tx_len = 0;
	char sendbuf[BUF_SIZ];
	struct ether_header *eh = (struct ether_header *) sendbuf;
	struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];

	/* Get interface name */

		strcpy(ifName, DEFAULT_IF);

	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}

	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");

	/* Construct the Ethernet header */
	memset(sendbuf, 0, BUF_SIZ);
	/* Ethernet header */
	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh->ether_dhost[0] = MY_DEST_MAC0;
	eh->ether_dhost[1] = MY_DEST_MAC1;
	eh->ether_dhost[2] = MY_DEST_MAC2;
	eh->ether_dhost[3] = MY_DEST_MAC3;
	eh->ether_dhost[4] = MY_DEST_MAC4;
	eh->ether_dhost[5] = MY_DEST_MAC5;
	/* Ethertype field */
	//eh->ether_type = htons(ETH_P_IP);
	eh->ether_type = htons(0x0800);
	tx_len += sizeof(struct ether_header);

	/* Packet data */
	//sendbuf[tx_len++] = 0xde;
	//sendbuf[tx_len++] = 0xad;
	//sendbuf[tx_len++] = 0xbe;
	//sendbuf[tx_len++] = 0xef;
	tx_len = payload(sendbuf,tx_len, dados);
  tx_len = criarUdp(sendbuf, tx_len, portas);

	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = MY_DEST_MAC0;
	socket_address.sll_addr[1] = MY_DEST_MAC1;
	socket_address.sll_addr[2] = MY_DEST_MAC2;
	socket_address.sll_addr[3] = MY_DEST_MAC3;
	socket_address.sll_addr[4] = MY_DEST_MAC4;
	socket_address.sll_addr[5] = MY_DEST_MAC5;

	/* Send packet */
	if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
	    printf("Send failed\n");

	return 0;
}
