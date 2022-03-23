/*
 * Socket.c
 *
 *  Created on: Mar 9, 2022
 *      Author: djc7941, znb34
 */

// posix library includes
#include <sys/socket.h>
#include <netinet/in.h>

// standard c library includes
#include <string.h>
#include <stdio.h>
// include time
#include <time.h>

//Opens a Socket on the specified port number
int OpenSocket(int portnum){

	FILE    *LogFile;

		// opens a new file
		LogFile = fopen( "LogOutput.txt", "a" );

		//checks for success
		if ( NULL == LogFile )
		{
			perror( "File not opened\n" );
			return(1);
		}
		//init the timer
				time_t timer;
				timer=time(NULL);

		// open a socket
			int sockfd = socket( AF_INET, SOCK_STREAM, 0 );

			// declare address variable
			struct sockaddr_in    address;

			// set the address to the computer's
			memset( &address, 0 , sizeof( address ) );
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_port = htons( portnum );

			// do the bind
			int result = bind( sockfd, (struct sockaddr *) &address, sizeof( address ) );
			fprintf(LogFile,"%sResult: %d\n", asctime(localtime(&timer)), result);

			//check if server has been created
			if (result == 0)
			{
				fprintf(LogFile, "%sServer successfully created\n", asctime(localtime(&timer)));
			}
			if (result == -1)
			{
				fprintf(LogFile, "%sServer can not be created\n", asctime(localtime(&timer)));
				fprintf(stderr, "Server can not be created\n", asctime(localtime(&timer)));
				return(-1);
			}


			// make a listening socket
				result = listen( sockfd, 5 );

			//check if listening socket has been created
			if (result == 0)
			{
				fprintf(LogFile, "%sListening Socket successfully created\n", asctime(localtime(&timer)));
			}
			if (result == -1)
			{
				fprintf(LogFile, "%sListening Socket can not be created\n", asctime(localtime(&timer)));
				fprintf(stderr, "%sListening Socket can not be created\n");
				return(-1);
			}

			//close log file
			fclose(LogFile);

		return(sockfd);
}
