/**
*
* @author Sarvesh Prajapati
*
* Client part of socket programming
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


int main(int argc, char **argv) {

    // Getting command line arguments using getopt()
    // part of <unistd.h>
    char ch;
    int port_no = 0, socket_descriptor;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char* ip_address = "";
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
                    \n\t -i : Takes in the ip address of the machine with \n\
                    with which the connection is to be made. If you are \n\
                    using this program with server, try using 127.0.0.1\n\
                    \n\t -p : Takes the port number where the server is \n\
                    running on.\n");
                fprintf(stderr, "Unknown option: '%s'\n", optarg);
                return 1;
        }
    } 
    argc -= optind;
    argv += optind;

    // Checking if all the hosts and port are passed

    if (port_no == 0 || ip_address == "") {

        fprintf(stderr, "ERROR: Both port number and IP address are required.\n");
        return 1;
    }
    // Setting up the client side socket
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_descriptor < 0) {
        fprintf(stderr, "ERROR: Loading the socket\n");
        return 1;
    }
    server = gethostbyname(ip_address);
    if (server == NULL) {
        fprintf(stderr,"ERROR: No such host\n");
        return 1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port_no);
    if (connect(socket_descriptor,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)  {
        fprintf(stderr, "ERROR: unable to connect\n");
        return 1;
    }
    // Getting the message to send to the server 
    printf("Enter the message: ");
    char buffer[256];
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    int n = write(socket_descriptor,buffer,strlen(buffer));
    if (n < 0) {
    printf("ERROR writing to socket");
    return 1;
    }
    bzero(buffer,256);
    n = read(socket_descriptor,buffer,255);
    if (n < 0) {
    printf("ERROR writing to socket");
    return 1;
    }
    printf("%s\n",buffer);

    return 0;
}

