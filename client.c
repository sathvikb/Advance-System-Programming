#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
int main(int argc, char *argv[]){
	char message[1000];
	char exitcmd[255];
	long int n1;
	int server, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;
	if(argc != 3){
		printf("Please enter ip address of server along with port number.\n",argv[0]);
		exit(0);
	}
	if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
		if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
			fprintf(stderr, " inet_pton() has failed\n");
			exit(2);
		}
	if(connect(server,(struct sockaddr *)&servAdd,
				sizeof(servAdd))<0){
		fprintf(stderr, "connect() has failed, exiting\n");
		exit(3);
	}
		while(1){
	
			fprintf(stderr, "Type \"quit\" to quit or a message for the server\n");
			fgets(message, 254, stdin);
		        strcpy(exitcmd,message);	
                        // fprintf(stderr,"message:%s\n",exitcmd);         
	      		if(strcmp(exitcmd,"quit\n")==0){
					close(server);
					exit(0);
			}
			write(server, message, strlen(message)+1);
			if (read(server, message, 10000)<0){
				fprintf(stderr, "read() error\n");
				exit(3);
			}
			// while(read(server, message, 100)>0)
				printf("\nOutput:\n");
				fprintf(stderr, "%s\n", message);
				memset(&message[0], 0, sizeof(message));

		}
}