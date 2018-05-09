#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

int serviceClient(int clientsd){
	char message[1000];
    dup2(clientsd,1);
    while(1){
		fprintf(stderr,"Child: Waiting for command from client\n");
		if(!read(clientsd, message, 1000)){
			close(clientsd);
			fprintf(stderr,"Child: Client has requested to quit / has died. Closing socket and exiting.\nNote: Parent is still waiting for new clients.\n\n");
			exit(0);
		}
		fprintf(stderr,"Child: Requested command: %s",message);
		system(message);
		memset(&message[0], 0, sizeof(message));
		// message[]="\0";
		fprintf(stderr, "Child: Command executed.\n\n");
	}
		return 0;
}
int main(int argc, char *argv[]){//E.g., 2: Server
	int sd, client, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;
	if(argc != 2){
		printf("Please enter the port number.\n", argv[0]);
		exit(0);
	}
	if ((sd=socket(AF_INET,SOCK_STREAM,0))<0){
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
	listen(sd, 5);
    
		while(1){
			printf("Parent: Waiting for a new client\n");
			client=accept(sd,(struct sockaddr*)NULL,NULL);
			printf("Parent: Got a client\n");
			if(!fork()){
				fprintf(stderr, "\nChild: I am servicing the Client request.\n");
				serviceClient(client);

			}
			close(client);
		}
}