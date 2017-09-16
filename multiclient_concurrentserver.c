#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
static int dd = 0;

#define response1 "HTTP 200 OK\r\nServer:shiiva\r\nConnection:close\r\n\r\n....................sjsianal........sasopoadpjdjal... "

#define response2 "HTTP 200 OK\r\nServer:shiiva\r\n\r\n....................sjsianal........sasopoadpjdjal...hskbvkxmcvoiueowuofulshdfkhksdfkhskfjhkshdkhfkshdfkdhskfkdsbkfhksh\r\n"

void doprocessing(int sock)
{

    int n;
    char buffer[256];
    static int nr = 0, nres = 0;
    while (1)
    {
        bzero(buffer, 256);

        n = read(sock, buffer, 255);
        nr++;
        if (n < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }

        char *ptr;
        //checking for valid request
        ptr = strstr(buffer, "HTTP/1.");
        if (ptr == NULL)
        {
            printf("not HTTP request \n");
            //		close(sock);
            //	exit(0);
            break;
        }
        else
        {
            *ptr = 0;
            ptr = NULL;

            if (strncmp(buffer, "GET ", 4) == 0)
                ptr = buffer + 4;

            if (strncmp((char *)ptr, "/text.html ", 11) == 0)
            {
                //n = write(newsockfd,"this should be response",23);
                //printf("client_number:%d\n",client_num);
                n = write(sock, response1, strlen(response1));
                nres++;
                if (n < 0)
                    error("ERROR writing to socket");

                //close(newsockfd);
            }

            else if (strncmp((char *)ptr, "/text2.html ", 12) == 0)
            {
                //printf("client_number:%d\n",client_num);
                n = write(sock, response2, strlen(response2));
                nres++;

                if (n < 0)
                    error("ERROR writing to socket");
                //close(newsockfd);
            }
        }
    }

    printf("Number of  requests recieved :%d\n", nr);
    printf("Number of  responses send :%d\n", nres);
    //close(sock);
    exit(0);
}

/*
    printf("Here is the message: %s\n",buffer);
    n = write(sock,"I got your message",18);
    if (n < 0) 
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}
*/
int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("200.6.104.2");
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }
    /* Now start listening for the clients, here 
     * process will go in sleep mode and will wait 
     * for the incoming connection
     */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while (1)
    {
        newsockfd = accept(sockfd,
                           (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }

        char *client_ip;

        //	char client_ip[16];
        client_ip = (char *)(intptr_t)inet_ntoa((cli_addr.sin_addr));
        int client_port = ntohs(cli_addr.sin_port);

        //	char *client_ipp = (char *)malloc(16*sizeof(char));
        //	strcpy(client_ipp,client_ip);
        //	client_ipp[15]='\0';
        printf("client: ip---->%s port--->%d\n", client_ip, client_port);
        //     free(client_ipp);
        /* Create child process */
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("ERROR on fork");
            exit(1);
        }
        if (pid == 0)
        {
            /* This is the client process */
            close(sockfd);
            doprocessing(newsockfd);
            exit(0);
        }
        else
        {
            close(newsockfd);
        }
    } /* end of while */
}
