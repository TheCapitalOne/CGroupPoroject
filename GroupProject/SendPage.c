/*
 * SendPage.c
 *
 *  Created on: Mar 4, 2022
 *      Author: djc7941, znb34
 */

// posix library includes
#include <unistd.h>

// standard c library includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

// system status includes
#include <time.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

int SendStatusPage( int clientfd )
{
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

	//init uname
	struct utsname buf;

	//check for uname error
	if (uname(&buf) < 0){
	    perror("uname() error");
	    exit(1);
	}

	//init sysinfo
	struct sysinfo info;
	char sysLong[64];

	//check for sysinfo error
	if (sysinfo(&info) < 0)
	{
		perror("sysinfo() error");
	    exit(1);
	}


	char buffer[65535];
	strcpy( buffer, "HTTP/1.1 200 OK\r\n"
					"Server: DJC_ZNB_WEB v1.0\r\n"
					"Content-Type: text/html\r\n"
					"\r\n"
					"<html>"
					"<head><title>System Status</title></head>"
					"<body><h2>Status Page</h2>"
					"<p>The local time is: "
			);

	//prints system time to page
		strcat( buffer, asctime(localtime(&timer)));

	//prints system name to page
		strcat( buffer, "</p><h3>User info</h3>"
						"<p>System name is: " );
		strcat( buffer, buf.sysname);

	//prints node name to page
		strcat( buffer, "</p><p>Node name is: " );
		strcat( buffer, buf.nodename);

	//prints system release to page
		strcat( buffer, "</p><p>Release is: " );
		strcat( buffer, buf.release);

	//prints system version to page
		strcat( buffer, "</p><p>Version is: " );
		strcat( buffer, buf.version);

	//prints machine number to page
		strcat( buffer, "</p><p>Machine Number is: " );
		strcat( buffer, buf.machine);

#ifdef _GNU_SOURCE
	//prints domain name to page
		strcat( buffer, "</p><p>Domain Name is: " );
		strcat( buffer, buf.domainname);
#endif

	//seconds since boot
		strcat( buffer, "</p><h3>System info</h3>"
						"<p>Seconds since boot: " );
		sprintf( sysLong,"%ld", info.uptime);
		strcat( buffer, sysLong);

	//load averages
		strcat( buffer, "</p><p>Load Averages: " );
		// 1 min
			sprintf( sysLong,"1 min ~ %ld, ", info.loads[2]);
			strcat( buffer, sysLong);
		// 5 min
			sprintf( sysLong,"5 min ~ %ld, ", info.loads[1]);
			strcat( buffer, sysLong);
		// 15 min
			sprintf( sysLong,"15 min ~ %ld", info.loads[0]);
			strcat( buffer, sysLong);

	//total ram
		strcat( buffer, "</p><p>Total usable memory size: " );
		sprintf( sysLong,"%ld", info.totalram);
		strcat( buffer, sysLong);

	//free ram
		strcat( buffer, "</p><p>Available memory size: " );
		sprintf( sysLong,"%ld", info.freeram);
		strcat( buffer, sysLong);

	//shared ram
		strcat( buffer, "</p><p>Amount of shared memory: " );
		sprintf( sysLong,"%ld", info.sharedram);
		strcat( buffer, sysLong);

	//buffer ram
		strcat( buffer, "</p><p>Memory used by buffers: " );
		sprintf( sysLong,"%ld", info.bufferram);
		strcat( buffer, sysLong);

	//total swap
		strcat( buffer, "</p><p>Total swap space size: " );
		sprintf( sysLong,"%ld", info.totalswap);
		strcat( buffer, sysLong);

	//free swap
		strcat( buffer, "</p><p>Swap space still available: " );
		sprintf( sysLong,"%ld", info.freeswap);
		strcat( buffer, sysLong);

	//current procs
		strcat( buffer, "</p><p>Number of current processes: " );
		sprintf( sysLong,"%d", info.procs);
		strcat( buffer, sysLong);

	//total high
		strcat( buffer, "</p><p>Total high memory size: " );
		sprintf( sysLong,"%ld", info.totalhigh);
		strcat( buffer, sysLong);

	//free high
		strcat( buffer, "</p><p>High Memory still available: " );
		sprintf( sysLong,"%ld", info.freehigh);
		strcat( buffer, sysLong);

	//mem_unit
		strcat( buffer, "</p><p>Memory unit size in bytes: " );
		sprintf( sysLong,"%d", info.mem_unit);
		strcat( buffer, sysLong);
		strcat( buffer, "</p><h3>Network info</h3>");

	//Network Addr and system port vars
		char hostbuffer[256];
		char *IPbuffer;
		struct hostent *host_entry;
		int hostname;

	// retrieves hostname
		hostname = gethostname(hostbuffer, sizeof(hostbuffer));

		// Returns host information corresponding to host name
		void checkHostName(int hostname)
		{
		    if (hostname == -1)
			    {
			        perror("gethostname");
			        exit(1);
			    }
			}
		checkHostName(hostname);

	// retrieves host information
		host_entry = gethostbyname(hostbuffer);
		// Returns hostname for the local computer

			void checkHostEntry(struct hostent * hostentry)
				{
			   if (hostentry == NULL)
			    {
			       perror("gethostbyname");
			       exit(1);
			    }
			}
		checkHostEntry(host_entry);

	// print host name to page
		strcat( buffer, "<p>Host Info: " );
		strcat( buffer, hostbuffer);


	// converts IP to string
		IPbuffer = inet_ntoa(*((struct in_addr*)
			host_entry->h_addr_list[0]));

	// prints host IP address
		strcat( buffer, "</p><p>Host IP: " );
		strcat( buffer, IPbuffer);


	//ends html content
	strcat( buffer, "</p>"
					"</body>"
					"</html>"
					"\n"
			);

	//bytes written for the page
	int bytes_written = write( clientfd, buffer, strlen( buffer ) );

	fprintf(LogFile, "%sBytes Written: %d\n", asctime(localtime(&timer)), bytes_written );

	fclose(LogFile);

	return( 0 );
}

int SendErrorPage( int clientfd ){

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

	// declare variables to use
		char    buffer[65535];

		// initialize the buffer
		memset( buffer, 0, sizeof( buffer ) );

		strcpy( buffer, "HTTP/1.1 404 Not Found\r\n"
						"Server: DJC_ZNB_WEB v1.0\r\n"
						"Content-Type: text/html\r\n"
						"\r\n"
						"<html>"
						"<head><title>Error Page</title></head>"
						"<body><h2>ERROR!</h2></body>"
						"<p>Add /status to the URL to reach the status page</p>"
						"</html>"
						"\n"
				);

		int bytes_written = write( clientfd, buffer, strlen( buffer ) );

		//bytes written for the page
		fprintf(LogFile, "%sBytes Written: %d\n", asctime(localtime(&timer)), bytes_written);

		//close log file
		fclose(LogFile);

		return ( 0 );
}

int SendDirPage( int clientfd, char bufPath[] ){

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

	// declare variables to use
		char    buffer[65535];

		// initialize the buffer
		memset( buffer, 0, sizeof( buffer ) );

		//init dir path
		char dirPath[200] = "/data";
		strcat( dirPath, bufPath);

		//directory stuff
		struct dirent **namelist;
		int n = scandir(dirPath, &namelist, NULL, alphasort);

		//begin page contents
		strcpy( buffer, "HTTP/1.1 200 OK\r\n"
						"Server: DJC_ZNB_WEB v1.0\r\n"
						"Content-Type: text/html\r\n"
						"\r\n"
						"<html>"
						"<head><title>System Files</title></head>"
						"<body><h2>Directory Page</h2>"
						"<p>"
				);

		//check if scandir worked
		if (n == -1) {
			perror("scandir");
			strcat( buffer, dirPath);
		} else {
		//loop prints directory contents to page
		while (n--) {
			strcat( buffer, namelist[n]->d_name);
			strcat( buffer, "</p><p>");
			free(namelist[n]);
		}
		free(namelist);
		}

		//ends page
		strcat( buffer, "</p>"
						"</body>"
						"</html>"
						"\n"
				);

		int bytes_written = write( clientfd, buffer, strlen( buffer ) );

		//bytes written for the page
		fprintf(LogFile, "%sBytes Written: %d\n", asctime(localtime(&timer)), bytes_written);

		//close log file
		fclose(LogFile);

		return ( 0 );
}
