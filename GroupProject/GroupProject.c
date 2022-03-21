/*
 * GroupProject.c
 *
 *  Created on: Feb 21, 2022
 *      Authors: znb34, djc7941
 */

// application includes
#include "Server.h"

// standard c library includes
#include <stdio.h>		// for printf and file
#include <stdlib.h>		// for atoi
#include <errno.h>		// for errors
#include <time.h>		// for time

int main(int argc, char **argv) {

	FILE    *LogFile;

	// opens a new file
	LogFile = fopen( "LogOutput.txt", "w" );

	//checks for success
	if ( NULL == LogFile )
	{
		perror( "File not opened\n" );
		return(1);
	}
	//init the timer
		time_t timer;
		timer=time(NULL);

	//check if enough arguments were entered
	if (argc < 2){
		perror("WARNING!: Command line argument not given\n");
		exit(1);
	}

	//init the port number from argument
	int PortArg = atoi( argv[1] );
	//prints system time to page

	//prints server information to log file
	fprintf(LogFile, "%sOpening a server on port %d\n", asctime(localtime(&timer)), PortArg);


	fprintf(LogFile, "%sStarting the server\n", asctime(localtime(&timer)));

	// run the server
	int result = RunServer(PortArg);

	fprintf(LogFile, "%sThe server has exited with a %d\n", asctime(localtime(&timer)), result );

	//closes the file
	fclose( LogFile );

	return(0);
}
