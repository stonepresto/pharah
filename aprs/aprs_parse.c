/* 
APRS Parser
Purpose: Built to take packets recieved by a station and
output the different data fields in a CSV format for use
in other parts of the Pharah program.

References:
https://www.tapr.org/pdf/AX25.2.2.pdf
	> Provided information about bit sequence in the frame.

*/

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 500
/* AX.25 UI Frame used for APRS packets */
struct ax25_UI {
	char* head_flag; //bit sequence that separates each frame by 0x7e
	char* dest; //APRS destination callsign
	char* src; //APRS source callsign
	char* digipeater; //zero to 8 digipeater callsigns
	char* ctl_fld; //set to 0x03 for UI frame in ax25 protocol
	char* proto_ID; //set to 0xf0 (no layer 3 protocol)
	char* info_fld; //APRS data, first byte identifies data type
	char* fcs; //sequence of 16 bits for checking integrity
	char* tail_flag; //same as the head_flag (0x7e)
};

void error_handle (char * error_string);
void print_packet(struct ax25_UI packet);

int main (int argc, char * argv[]) 
{
	struct ax25_UI packet; 
	char buffer[MAXSIZE];

	packet.head_flag = (char *) malloc(sizeof(char *));
	packet.dest = (char *) malloc(sizeof(char *));
	packet.src = (char *) malloc(sizeof(char *));
	packet.digipeater = (char *) malloc(sizeof(char *));
	packet.ctl_fld = (char *) malloc(sizeof(char *));
	packet.proto_ID = (char *) malloc(sizeof(char *));
	packet.info_fld = (char *) malloc(sizeof(char *));
	packet.fcs = (char *) malloc(sizeof(char *));
	packet.tail_flag = (char *) malloc(sizeof(char *));
	memset(buffer, 0x00, MAXSIZE);

	if (MAXSIZE < scanf("%s",buffer))
		error_handle("[!!] Invalid packet size\n");

	int i = 0;
	strcpy(packet.head_flag, &buffer[0]); 
	strcpy(packet.dest, &buffer[2]);
	strcpy(packet.src, &buffer[8]);
	strcpy(packet.digipeater, &buffer[15]);
	strcpy(packet.ctl_fld, &buffer[71]);
	strcpy(packet.proto_ID, &buffer[72]);
	strcpy(packet.info_fld, &buffer[73]);
	strcpy(packet.fcs, &buffer[329]);
	strcpy(packet.tail_flag, &buffer[330]);

	print_packet(packet);
	return 0;
}

void error_handle (char * error_string)
{
	perror(error_string);
	exit(1);
}

void print_packet(struct ax25_UI packet)
{
	printf("###PACKET###\nFLAG: %s\nDest: %s\nSrc: %s\nDigipeaters: %s\nDATA: %s\nFCS: %s\nFLAG: %s\n",
		packet.head_flag,packet.dest,packet.src,packet.digipeater,
		packet.info_fld,packet.fcs,packet.tail_flag);

}
