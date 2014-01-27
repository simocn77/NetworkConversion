
// Compilare	:	g++ main.c -o main
// Eseguire	:	./main

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <math.h>

#define LEN 30
struct net {
	unsigned long ip;		/* ip address */
	unsigned long network;		/* Network address */
	unsigned long mask;		/* subnet mask */
	unsigned long broadcast;	/* Broadcast address */
	unsigned long usableIps;
	unsigned long startIP, endIP;
	char ip_str[LEN];
	char network_str[LEN];
	char mask_str[LEN];
	char broadcast_str[LEN];
	char startIP_str[LEN];
	char endIP_str[LEN];
} net;

unsigned int IP2Int(char *str, unsigned long *ip)
{
	unsigned long ip1, ip2, ip3, ip4;

	sscanf(str, "%lu.%lu.%lu.%lu", &ip1, &ip2, &ip3, &ip4);
	*ip  = ip1<<24 | ip2<<16 | ip3<<8 | ip4;
	/* printf("%lu \n", *ip); */

	return 0;
}

unsigned int Int2IP(unsigned long ip, char *str)
{
	unsigned long ip1, ip2, ip3, ip4;

	ip1  = (ip & 0xff000000) >> 24 ;
	ip2  = (ip & 0x00ff0000) >> 16 ;
	ip3  = (ip & 0x0000ff00) >> 8 ;
	ip4  = ip & 0x000000ff ;
	/* printf("%lu %lu %lu %lu \n", ip1,ip2,ip3,ip4); */

	memset(str, '\0', sizeof(str));
	sprintf(str, "%lu.%lu.%lu.%lu", ip1, ip2, ip3, ip4);
	/* printf("%s \n", str); */

	return 0;
}

int CIDR2Network(char *Net_CIDR)
{
	//char tokenstring[] = "10.39.65.1/22";
	char *tokenstring = Net_CIDR;
	struct net net;
	unsigned int bits;
	char s1[LEN]="";
	char s2[LEN]="";
	int result;

	memset(&net, '\0', sizeof(net));

	//network, bitmask
	result = sscanf(tokenstring, "%[^'/']/%s", s1, s2);
	//printf("%s \t/ %s \n",  s1, s2);
	//printf("len %d \t/ len %d \n",  strlen(s1), strlen(s2));

	IP2Int(s1, &net.ip);
	sscanf(s2, "%i", &bits);
	//printf("%lu / %i \n", net.ip, bits);

	net.mask = ~(0xffffffff >> bits);
	net.network = net.ip & net.mask;
	net.broadcast = net.network + ~net.mask;

	net.usableIps = (bits >30)?0:(net.broadcast - net.network - 1);
	if (net.usableIps <= 0)
		net.startIP = net.endIP = 0;
	else
	{
		net.startIP = net.network + 1;
		net.endIP = net.broadcast - 1;
	}

	Int2IP( net.ip, net.ip_str);
	Int2IP( net.network, net.network_str);
	Int2IP( net.mask, net.mask_str);
	Int2IP( net.broadcast, net.broadcast_str);
	Int2IP( net.startIP, net.startIP_str);
	Int2IP( net.endIP, net.endIP_str);

	printf(" Net data :\n \
	CIDR		= %s \n \
	ip		= %s \n \
	network		= %s \n \
	mask		= %s \n \
	broadcast	= %s \n \
	usableIps	= %lu \n \
	startIP		= %s \n \
	endIP		= %s \n", \
	Net_CIDR, net.ip_str, net.network_str, net.mask_str, net.broadcast_str, \
	net.usableIps, net.startIP_str, net.endIP_str);

	/*
	char str[]="10.39.44.171";
	unsigned long ip1;

	IP2Int(str, &ip1);
	Int2IP(ip1, str1);
	return (0);
	*/
}

int Network2CIDR(char *ip, char *mask)
{
	char *token1 = ip;
	char *token2 = mask;
	char CIDR[LEN] = "";
	struct net net;
	int result;
	int count=0;
	unsigned long dato=0;

	memset(&net, '\0', sizeof(net));

	IP2Int(token1, &net.ip);
	IP2Int(token2, &net.mask);

	net.network = net.ip & net.mask;
	Int2IP( net.network, net.network_str);

	//printf(" MASK 0x%x", (int)net.mask);

	count=0;
	dato = net.mask;
	/* conta i bit a 1 */
	while( dato != 0 ){
	  dato = dato & (dato - 1);
	  count++;
	}
	/* printf(" Number of 1's in (mask) is %i", count); */
	/*  conta i bit a 1
	while( dato!=0 ) {
		dato = dato >> 1;
		if( dato & 1)
			count++;
	}
	*/

	sprintf(CIDR,"%s/%i", net.network_str, count);

	printf(" Net data :\n \
	ip		= %s \n \
	network		= %s \n \
	mask		= %s \n \
	CIDR		= %s \n", \
	ip, net.network_str, mask, CIDR);

	return 0;
}

int main()
{
   char str[]="10.39.65.1/22";  /* CIDR */

   char str1[]="10.39.65.1";	/* std */
   char str2[]="255.255.252.0";

   CIDR2Network(str);
   Network2CIDR(str1,str2);
   return (0);
}

/* End Of File. */
