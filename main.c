
// Compilare	:	g++ main.c -o main
// Eseguire	:	./main

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <math.h>

#define LEN 30 //len max : xxx.xxx.xxx.xxx/22 =18+1
#define TRUE 1
#define FALSE 0
struct net {
	unsigned long ip;		/* ip address */
	unsigned long network;		/* Network address */
	unsigned long mask;		/* subnet mask */
	unsigned long broadcast;	/* Broadcast address */
	unsigned long wildcardmask;
	unsigned long startIP, endIP;
	char ip_str[LEN];
	char network_str[LEN];
	char mask_str[LEN];
	char broadcast_str[LEN];
	char wildcardmask_str[LEN];
	char startIP_str[LEN];
	char endIP_str[LEN];
} net;

unsigned int IP2Int(char *str, unsigned long *ip)
{
	unsigned long ip1, ip2, ip3, ip4;

	sscanf(str, "%lu.%lu.%lu.%lu", &ip1, &ip2, &ip3, &ip4);
	*ip  = ip1<<24 | ip2<<16 | ip3<<8 | ip4;
	/* printf("%lu \n", *ip); */

	return TRUE;
}

unsigned int Int2IP(unsigned long ip, char *str)
{
	unsigned long ip1, ip2, ip3, ip4;

	ip1  = (ip & 0xff000000) >> 24 ;
	ip2  = (ip & 0x00ff0000) >> 16 ;
	ip3  = (ip & 0x0000ff00) >> 8 ;
	ip4  = ip & 0x000000ff ;
	/* printf("%lu %lu %lu %lu \n", ip1,ip2,ip3,ip4); */

	sprintf(str, "%lu.%lu.%lu.%lu", ip1, ip2, ip3, ip4);
	/* printf("%s, len %i \n", str, strlen(str));*/
	return TRUE;
}

int CIDR2Network(char *Net_CIDR)
{
	//char tokenstring[LEN] = "10.39.65.1/22";
	char *tokenstring = Net_CIDR;
	struct net net;
	unsigned int bits;
	char s1[LEN]="";
	char s2[LEN]="";
	int result;
	unsigned long usableIps;

	memset(&net, '\0', sizeof(net));

	//network, bitmask
	result = sscanf(tokenstring, "%[^'/']/%s", s1, s2);
	//printf("%s \t/ %s \n",  s1, s2);
	//printf("len %d \t/ len %d \n",  strlen(s1), strlen(s2));

	IP2Int(s1, &net.ip);
	sscanf(s2, "%i", &bits);
	//printf("%lu / %i \n", net.ip, bits);

	net.mask = ~(0xffffffff >> bits);
	net.wildcardmask = ~net.mask;
	net.network = net.ip & net.mask;
	net.broadcast = net.network + ~net.mask;

	//usableIps = (bits >30)?0:(net.broadcast - net.network - 1);
	usableIps = net.wildcardmask -1; /* tolgo il broacast*/
	if (usableIps <= 0)
		net.startIP = net.endIP = 0;
	else
	{
		net.startIP = net.network + 1;
		net.endIP = net.broadcast - 1;
	}

	Int2IP( net.ip, net.ip_str);
	Int2IP( net.network, net.network_str);
	Int2IP( net.mask, net.mask_str);
	Int2IP( net.wildcardmask, net.wildcardmask_str);
	Int2IP( net.broadcast, net.broadcast_str);
	Int2IP( net.startIP, net.startIP_str);
	Int2IP( net.endIP, net.endIP_str);

	/* WildcardMask= ~mask; */

	printf(" Net data :\n \
	CIDR		= %s \n \
	ip		= %s \n \
	network		= %s \n \
	mask		= %s \n \
	wildcardmask	= %s \n \
	usableIps 	= %lu \n \
	broadcast	= %s \n \
	startIP		= %s \n \
	endIP		= %s \n", \
	Net_CIDR, net.ip_str, net.network_str, net.mask_str, net.wildcardmask_str, \
	usableIps, net.broadcast_str, net.startIP_str, net.endIP_str);

	return TRUE;
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

	return TRUE;
}

/**/
int Calculate_NetBroacastAdd (char *net_str, char *mask_str, char *br_str){

	unsigned long net;
	unsigned long mask;
	unsigned long broadcast_add;
	unsigned int bits;

	IP2Int(net_str, &net);
	IP2Int(mask_str, &mask);

	broadcast_add = (net & mask) + ~mask;
	Int2IP( broadcast_add, br_str);
	printf(" Ip %s in NET with BroacastAdd is %s \n", net_str, br_str );
	return TRUE;
}


/**/
int checkip (char *ip_str, char *net_str, char *mask_str){

	unsigned long ip;
	unsigned long net;
	unsigned long mask;

	IP2Int(ip_str, &ip);
	IP2Int(net_str, &net);
	IP2Int(mask_str, &mask);

	if ( (ip & mask) == (net & mask) ){
		printf(" %s\tappartente alla rete\t\t%s\n", ip_str, net_str);
		return TRUE;
	}
	else {
		printf(" %s\tNON appartente alla rete\t%s\n", ip_str, net_str);
		return FALSE;
	}

}

int main()
{
   char CIDR[LEN]="10.39.65.1/22";  /* CIDR */

   char Net[LEN]="10.39.65.1";	/* std */
   char Mask[LEN]="255.255.252.0";
   char BroadcastAdr[LEN]="";
   char IP1[LEN]="10.39.44.11"; /* NON in maschera*/
   char IP2[LEN]="10.39.63.255"; /* NON in  maschera*/
   char IP3[LEN]="10.39.64.0"; /* NON in  maschera*/
   char IP4[LEN]="10.39.66.11"; /* in maschera*/
   char IP5[LEN]="10.39.67.255"; /* in maschera*/
   char IP6[LEN]="10.39.68.0"; /* NON in maschera*/

   CIDR2Network(CIDR);
   Network2CIDR(Net, Mask);

   Calculate_NetBroacastAdd(Net, Mask, BroadcastAdr);

   checkip (IP1, Net, Mask);
   checkip (IP2, Net, Mask);
   checkip (IP3, Net, Mask);
   checkip (IP4, Net, Mask);
   checkip (IP5, Net, Mask);
   checkip (IP6, Net, Mask);

   return (0);
}

/* End Of File. */
