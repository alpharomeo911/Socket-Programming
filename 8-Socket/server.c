/**
*
* @author Sarvesh Prajapati
*
* Server part of socket programming
*
* Dependencies - a Linux System / winev with GCC
*
* Practical 8
*
*/

// Importing the necessary files.

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>



int main(int argc, char** argv) {
	char ch;
	int socket_descriptor, newsocket_descriptor, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int port_no = 0;
    char* ip_address = "127.0.0.1";
    // Getting Options.
    while((ch = getopt(argc, argv, "p:i:")) != EOF) {
        switch(ch){
            case 'p' : 
                port_no = atoi(optarg);
                break;
            case 'i' :
                ip_address = optarg;
                break;
            default: 
                printf("Usage : ./client.c -i <ip_address> -p <port_number> \
                    \n\t -i : Takes in the ip address where server is to be\n\
                    created. Use your local / loopback address.\
                    \n\t -p : Takes the port number where the server is \n\
                    running on.\n");
                fprintf(stderr, "Unknown option: '%s'\n", optarg);
                return 1;
        }
    } 
    
    argc -= optind;
    argv += optind;

    if (port_no == 0) {
    	fprintf(stderr, "ERROR: Port number should be provided.\n");
   		return 1;
    }
    // Socket Descriptor
	socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_descriptor < 0) {
	error("ERROR opening socket");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port_no);
	if (bind(socket_descriptor, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR on binding\n");
		return 1;
	}
	printf("The server is now listening, to stop, simply press ^C\n");
	// Loop so that our program continues to receive message.
	while(1) {
		listen(socket_descriptor,5);
		clilen = sizeof(cli_addr);
		newsocket_descriptor = accept(socket_descriptor, (struct sockaddr *) &cli_addr, &clilen);
		if (newsocket_descriptor < 0) 
			fprintf(stderr,"an ERROR occured\n");
		bzero(buffer,256);
		int n = read(newsocket_descriptor,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		printf("*** Incomming message: %s\n",buffer);
		n = write(newsocket_descriptor,"Message:",18);
		if (n < 0) {
			fprintf(stderr, "ERROR writing to socket\n");
		}
	}	
		return 0; 
}