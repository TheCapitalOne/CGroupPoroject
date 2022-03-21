/*
 * Server.c
 *
 *  Created on: Mar 2, 2022
 *      Author: djc7941, znb34
 */

// application includes
#include "SendPage.h"
#include "Socket.h"

// posix library includes
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// standard c library includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>    // for signal function
#include <time.h>	   // for time

// global variable for handler
int Exit;

//declare global log file
FILE    *LogFile;


// my signal handler
void SigHandler (int signal) {

	//init the timer
			time_t timer;
			timer=time(NULL);


	fprintf(LogFile, "%sIn Signal Handler with signal %d\n", asctime(localtime(&timer)), signal);

	Exit = 1;

	return;
}

//starts running a server with a specific port number
int RunServer(int portnum)
{
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

	// declare sigaction structure
	struct sigaction    sa;

	// clear the sigaction structure
	memset( &sa, 0, sizeof( sa ));

	// set up what to do
	sa.sa_handler = SigHandler;
	sigemptyset( &sa.sa_mask );

	// set the SIGINT handler
	if( sigaction( SIGINT, &sa, NULL ) == -1 )
	{
		fprintf(LogFile, "Error assigning the handler for SIGTERM\n");
		perror( "Error assigning the handler for SIGINT\n");
	    return( 1 );
	}

	// set the SIGTERM handler
	if( sigaction( SIGTERM, &sa, NULL ) == -1 )
	{
		fprintf(stderr, "Error assigning the handler for SIGTERM\n");
		fprintf(LogFile, "Error assigning the handler for SIGTERM\n");
		return( 1 );
	}

	fprintf(LogFile, "%sOpening the socket\n", asctime(localtime(&timer)) );

		//Opens a Socket on the specified port number
		int sockfd = OpenSocket(portnum);

		//checks if the socket was opened successfully
		if ( -1 == sockfd ) {
			perror( "Error opening the socket\n" );
			fprintf(LogFile, "Error opening the socket\n" );
			return( -1 );
		}
	// start handling connections
		while( Exit == 0 )
		{
			// declare a variables for the client address
			struct sockaddr_in      client_addr;
			//      need to initialize the length or
			//      accept may not return all the information
			//      in client_addr. use socklen_t as the datatype
			//      socklen_t is defined to the size needed in the
			//      sys/socket.h header file
			socklen_t               client_addr_len = sizeof( client_addr );
			// initialize the client address
			memset( &client_addr, 0, client_addr_len );

			// call accept to get a connection
			int clientfd = accept( sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len );

			// declare variables to use
			char    buffer[65535];

			// initialize the buffer
			memset( buffer, 0, sizeof( buffer ) );

			// read the bytes
			int bytes_read = read( clientfd, buffer, 65534 );

			fprintf(LogFile, "%sBytes Read: %d\n", asctime(localtime(&timer)), bytes_read);

			fflush( stdout );

			if (( strncmp( buffer, "GET ", 4 ) == 0 ) &&
				( strncmp( buffer + 3, " /status ", 9 ) == 0 ))
				{
				// send back the status page
				int result = SendStatusPage( clientfd );
				fprintf(LogFile, "%sSendStatusPage return: %d\n", asctime(localtime(&timer)), result );
			}
			else {
				// send back the error page
				int result = SendErrorPage( clientfd );
				fprintf(LogFile, "%sSendErrorPage return: %d\n", asctime(localtime(&timer)), result);
			}
		close( clientfd );
	}
	close( sockfd );

	//close log file
	fclose(LogFile);

	// return success
	return( 0 );
}
