#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include<stdlib.h>
#include "dummy_http.h"

int main(int argc, char *argv[])
{
	int counterm = 0;
	int counterp = 0;
	int counters = 0;
	int sockfd, newsockfd, num_bytes;
	char buffer[BUFFER_SIZE];
	char  *structure = (char*)malloc(structure_size);
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	/* Create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
	{
		fprintf(stderr, "ERROR: Failed to open socket\n");
		return -1;
	}

	/* allow to reuse the socket as soon as it stops being active */
	int so_reuse_enabled = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &so_reuse_enabled, sizeof(int)) < 0)
	{
		fprintf(stderr, "setsockopt(SO_REUSEADDR) failed");
		return -2;
	}

	/* Initialize socket structure (sockarrd_in) */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //or INADDR_ANY
	serv_addr.sin_port = htons(HTTP_PORT);

	/* Bind the host address */
	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		fprintf(stderr, "ERROR: bind() failed\n");
		fprintf(stderr, "%ld\n", sizeof(serv_addr));
		fprintf(stderr, "Error code: %s\n", strerror(errno));
		return -3;
	}

	/* Start listening for the clients (thread blocks) */
	if (listen(sockfd, MAX_CONNECTIONS) < 0)
	{
		fprintf(stderr, "ERROR: listen() failed\n");
		fprintf(stderr, "Error code: %s\n", strerror(errno));
		return -4;
	}

	while (stderr != NULL)
	{

		/* Accept connection from a client */
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd < 0)
		{
			fprintf(stderr, "ERROR: accept() failed or server has been closed\n");
			return -5;
		}

		/* Clear buffer and start communicating */
		memset(buffer, 0, BUFFER_SIZE);
		num_bytes = recv(newsockfd, buffer, BUFFER_SIZE - 1, 0);
		if (num_bytes < 0)
		{
			fprintf(stderr, "ERROR: recv() failed\n");
			return -6;
		}
		printf("Recieved: %s\n", buffer);

		char response[200];
		float num;
		srand(time(NULL));
		int choice = 0;
		if (strcmp(buffer, "moisture") == 0)
		{
			choice = 1;
		}
		else if (strcmp(buffer, "ph") == 0)
		{
			choice = 2;

		}
		else if (strcmp(buffer, "sunlight") == 0)
		{
			choice = 3;

		}
		else if (strcmp(buffer, "reset") == 0)
		{
			choice = 4;
		}
		else if (strcmp(buffer, "stats") == 0)
		{

			choice = 5;

		}

		switch (choice)
		{
		case 1:
			counterm++;
			num = (rand() % 10);
			if (num >= 0 && num <= 3)
			{
				strcpy(response, "the soil is very dry");
			}
			else if (num >= 4 && num <= 7)
			{
				strcpy(response, "the soil is not too moist");
			}
			else
			{
				strcpy(response, "the soil is very wet");
			}
			break;
		case 2:
			counterp++;
			num = (double)rand() / RAND_MAX * 6.5 + 3.5;
			if (num >= 3.5 && num <= 6.9)
			{
				strcpy(response, "acidic");
			}
			else if (num == 7.0)
			{
				strcpy(response, "neutral");
			}
			else if (num >= 7.1 && num <= 10.0)
			{
				strcpy(response, "alkaline");
			}
			break;
		case 3:
			counters++;
			num = rand() % 2000;
			if (num >= 0 && num <= 500)
			{
				strcpy(response, "very dark");
			}
			else if (num >= 501 && num <= 1000)
			{
				strcpy(response, "dark");
			}
			else if (num >= 1001 && num <= 1500)
			{
				strcpy(response, "light");
			}
			else if (num >= 1501 && num <= 2000)
			{
				strcpy(response, "very light");
			}
			break;
		case 4:
			strcpy(response, "server is closed");
			close(sockfd);
			break;
		case 5:
			strcpy(response, "moisture: ");
			char m[12];
			sprintf(m, "%d", counterm);
			strcat(response, m);
			strcat(response, "\nph: ");
			char p[12];
			sprintf(p, "%d", counterp);
			strcat(response, p);
			strcat(response, "\nsunlight: ");
			char s[12];
			sprintf(s, "%d", counters);
			strcat(response, s);
			break;
		default:
			strcpy(response, "UNKNOWN");
		}

			size_t request_size = strlen(buffer);
			size_t struct_size = strlen(structure);
			size_t overall_size = struct_size + request_size;						
			//reallocate only the required memory to the memory block
			structure= (char *)realloc(structure,overall_size);
			strcat(structure, buffer);
			//find the size of the new request
			

		
		/* Write a response to the client */
		printf("Sending: %s \n", structure);
		num_bytes = send(newsockfd, response, strlen(response), 0);
		if (num_bytes < 0)
		{
			fprintf(stderr, "ERROR: send() failed\n");
			return -7;
		}

		close(newsockfd);
	}
	//after the loop free the memory from the mmemory block
	free(structure);
	printf("Because the server has been closed... please reopen the server\n");
	//close(sockfd);

	return 0;
}
